
#pragma unmanaged
#include <windows.h>
#include <algorithm>
#include "elf.h"
#include "GCNDisassembler.h"
#include "GCNDecoder.h"
#include "GCNBufferedPrinter.h"
#pragma managed

#include "AMDDriver_Impl.h"
#include "AMDAsic_Impl.h"
#include "AMDShader_Impl.h"
#include "Utilities.h"

#undef DOMAIN // seriously Microsoft?!?!?

#include "amd-codexl-analyzer/Common/Src/DeviceInfo/DeviceInfoUtils.h"


static bool GfxCardInfoSortPredicate(const GDT_GfxCardInfo& a, const GDT_GfxCardInfo& b)
{
	// Generation is the primary key.
	if (a.m_generation < b.m_generation) { return true; }

	if (a.m_generation > b.m_generation) { return false; }

	// CAL name is next.
	int ret = ::strcmp(a.m_szCALName, b.m_szCALName);

	if (ret < 0) { return true; }

	if (ret > 0) { return false; }

	// Marketing name next.
	ret = ::strcmp(a.m_szMarketingName, b.m_szMarketingName);

	if (ret < 0) { return true; }

	if (ret > 0) { return false; }

	// DeviceID last.
	return a.m_deviceID < b.m_deviceID;
}



AMDDriver_Impl::AMDDriver_Impl( System::String^ path )
{
    MarshalledString sPath(path);
    HMODULE hAMDDriver = LoadLibraryA( sPath );
    if( !hAMDDriver )
        throw gcnew System::Exception( System::String::Format( "AMD driver dll not found at {0}", path ));

    FARPROC pCompile = GetProcAddress( hAMDDriver, "AmdDxGsaCompileShader" );
    FARPROC pFree    = GetProcAddress( hAMDDriver, "AmdDxGsaFreeCompiledShader" );
    if( !pCompile || !pFree )
    {
        FreeLibrary(hAMDDriver);
        throw gcnew System::Exception( System::String::Format( "GSA entrypoints not found in {0}", path ));
    }

    m_pCompileFunc = (COMPILE_SHADER) pCompile;
    m_pFreeFunc    = (FREE_SHADER) pFree;

    m_pmAsics = gcnew System::Collections::Generic::List<Pyramid::IAMDAsic^>();

    // Populate the sorted device (card) info table.
    std::vector<GDT_GfxCardInfo> cardList;
    std::vector<GDT_GfxCardInfo> DXDeviceTable;

    // DX support now only SI, CI and VI
    if (AMDTDeviceInfoUtils::Instance()->GetAllCardsInHardwareGeneration(GDT_HW_GENERATION_SOUTHERNISLAND, cardList))
    {
        DXDeviceTable.insert(DXDeviceTable.end(), cardList.begin(), cardList.end());
    }

    if (AMDTDeviceInfoUtils::Instance()->GetAllCardsInHardwareGeneration(GDT_HW_GENERATION_SEAISLAND, cardList))
    {
        DXDeviceTable.insert(DXDeviceTable.end(), cardList.begin(), cardList.end());
    }

    if (AMDTDeviceInfoUtils::Instance()->GetAllCardsInHardwareGeneration(GDT_HW_GENERATION_VOLCANICISLAND, cardList))
    {
        DXDeviceTable.insert(DXDeviceTable.end(), cardList.begin(), cardList.end());
    }

    std::sort(DXDeviceTable.begin(), DXDeviceTable.end(), GfxCardInfoSortPredicate);
    
    // de-duplicate cards based on cal name
    //  Some of these things are pre-release or otherwise unsupported by the driver (at least the current one)
    //  there are also a few hundred entries in the device table, but only about a dozen or so "cal names"
    size_t i=0;
    while( i < DXDeviceTable.size() )
    {
        m_pmAsics->Add( gcnew AMDAsic_Impl(DXDeviceTable[i] ) );

        const char* pName = DXDeviceTable[i].m_szCALName;
        do
        {
            i++;
        } while( i < DXDeviceTable.size() && strcmp( pName, DXDeviceTable[i].m_szCALName ) == 0 );
    }

}


Pyramid::IAMDShader^  AMDDriver_Impl::CompileDXBlob(Pyramid::IAMDAsic^ asic, array<byte>^ blob, Pyramid::IDXShaderReflection^ reflection )
{
    AMDAsic_Impl^ a = dynamic_cast<AMDAsic_Impl^>(asic);
    
    MarshalledBlob^ bl = gcnew MarshalledBlob(blob);

    AmdDxGsaCompileShaderInput args;
    args.byteCodeLength = bl->GetLength();
    args.pShaderByteCode = bl->GetBlob();
    args.pCompileOptions = 0;
    args.numCompileOptions = 0;
    args.chipRevision   = a->m_nChipRevision;
    args.chipFamily     = a->m_nChipFamily;
    memset( args.reserved, 0, sizeof(args.reserved) );

    AmdDxGsaCompileShaderOutput result;
    result.size = sizeof(result);
    result.pShaderBinary = 0;
    result.shaderBinarySize = 0;
    DWORD hResult = m_pCompileFunc( &args, &result );

    if( hResult != 0 )
        throw gcnew System::Exception( "Error returned from AMD driver compile" );

    AMDShader_Impl::ShaderType eType;
    switch( reflection->GetShaderType() )
    {
    case Pyramid::HLSLShaderType::VERTEX:   eType = AMDShader_Impl::ShaderType::ST_VERTEX;   break;
    case Pyramid::HLSLShaderType::PIXEL:    eType = AMDShader_Impl::ShaderType::ST_PIXEL;    break;
    case Pyramid::HLSLShaderType::GEOMETRY: eType = AMDShader_Impl::ShaderType::ST_GEOMETRY; break;
    case Pyramid::HLSLShaderType::DOMAIN:   eType = AMDShader_Impl::ShaderType::ST_DOMAIN;   break;
    case Pyramid::HLSLShaderType::HULL:     eType = AMDShader_Impl::ShaderType::ST_HULL;     break;
    case Pyramid::HLSLShaderType::COMPUTE:  eType = AMDShader_Impl::ShaderType::ST_COMPUTE;  break;
    default:
        throw gcnew System::Exception( "Bad shader type enum???");
    }

    DWORD nThreadsPerGroup = reflection->GetThreadsPerGroup();

    return gcnew AMDShader_Impl( this, a, (Elf32_Ehdr*)result.pShaderBinary, result.shaderBinarySize, nThreadsPerGroup, eType );
}
