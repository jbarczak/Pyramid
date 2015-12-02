
#pragma unmanaged
#include <windows.h>
#include "elf.h"
#include "GCNDisassembler.h"
#include "GCNDecoder.h"
#include "GCNBufferedPrinter.h"
#pragma managed

#include "AMDDriver_Impl.h"
#include "AMDAsic_Impl.h"
#include "AMDShader_Impl.h"
#include "Utilities.h"





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

    // SI chips seen during CodeXL snooping
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Capeverde", 110, 41, GCN::IDecoder::GCN1 ) );
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Hainan",    110, 70, GCN::IDecoder::GCN1 ) );
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Oland",     110, 60, GCN::IDecoder::GCN1 ) );
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Pitcairn",  110, 21, GCN::IDecoder::GCN1 ) );
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Tahiti",    110, 6 , GCN::IDecoder::GCN1 ) );

    // CI chips seen during CodeXL snopping
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Bonaire",   120, 20, GCN::IDecoder::GCN1  ) );
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Kalindi",   120, 20, GCN::IDecoder::GCN1  ) );
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Spectre",   120, 65, GCN::IDecoder::GCN1  ) );
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Spooky",    120, 65, GCN::IDecoder::GCN1  ) );
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Hawaii",    120, 40, GCN::IDecoder::GCN1  ) );
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Mullins",   120, 20, GCN::IDecoder::GCN1  ) );
    
    // VI chips seen during CodeXL snooping
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Carrizo",   130, 1,  GCN::IDecoder::GCN3 )) ;
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Fiji",      130, 60, GCN::IDecoder::GCN3 )) ;
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Iceland",   130, 1,  GCN::IDecoder::GCN3 )) ;
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Tonga",     130, 20, GCN::IDecoder::GCN3 )) ;

    // Other stuff the driver accepted but I haven't identified
    m_pmAsics->Add( gcnew AMDAsic_Impl( "asic-125",  125, 1, GCN::IDecoder::GCN1 )) ;
    m_pmAsics->Add( gcnew AMDAsic_Impl( "asic-135",  135, 1, GCN::IDecoder::GCN3 )) ;
}


#pragma unmanaged
struct CompileArgs
{
    DWORD asic;             /// Identifies the asic family
    DWORD arg1;
    void* pByteCode;
    DWORD nBytecodeSize;
    void* pGarbage;
    DWORD zero;
};
struct CompileResult
{
    DWORD       StructSize;
    Elf32_Ehdr* pElf;
    DWORD       nElfSize;
};
#pragma managed


Pyramid::IAMDShader^  AMDDriver_Impl::CompileDXBlob(Pyramid::IAMDAsic^ asic, array<byte>^ blob )
{
    AMDAsic_Impl^ a = dynamic_cast<AMDAsic_Impl^>(asic);
    
    MarshalledBlob^ bl = gcnew MarshalledBlob(blob);
    
    
    CompileArgs args;
    args.asic                   = a->m_nArg0;
    args.arg1                   = a->m_nArg1;
    args.pByteCode              = bl->GetBlob();
    args.nBytecodeSize          = bl->GetLength();
    args.pGarbage               = 0;
    args.zero                   = 0;

    CompileResult result;
    result.StructSize = sizeof(CompileResult);
    result.pElf = 0;
    result.nElfSize = 0;
    DWORD hResult = m_pCompileFunc( &args, &result );

    if( hResult != 0 )
        throw gcnew System::Exception( "Error returned from AMD driver compile" );

    return gcnew AMDShader_Impl( this, a, result.pElf, result.nElfSize );
}
