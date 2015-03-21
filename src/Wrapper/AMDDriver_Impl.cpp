
#pragma unmanaged
#include <windows.h>
#include "elf.h"
#include "GCNDisassembler.h"
#include "GCNDecoder.h"
#pragma managed

#include "AMDDriver_Impl.h"
#include "Utilities.h"

#include <vector>

class Printer : public GCN::Disassembler::IPrinter
{
public:
    virtual void Push( const char* p )
    {
        m_Bytes.insert( m_Bytes.end(), p, p + strlen(p) );
    }

    std::vector<char> m_Bytes;
};

private ref class AMDAsic_Impl : Pyramid::IAMDAsic
{
public:
    AMDAsic_Impl( const char* pName, DWORD arg0, DWORD arg1, GCN::IDecoder::GCNVersions eGCNVersion )
        : m_pmName( MakeString(pName) ),
          m_nArg0(arg0),
          m_nArg1(arg1),
          m_pDecoder( GCN::IDecoder::Create( eGCNVersion ) )
    {
    }

    ~AMDAsic_Impl()
    {
        GCN::IDecoder::Destroy(m_pDecoder);
        m_pDecoder=0;
    }

    property System::String^ Name {
        virtual System::String^ get() { return m_pmName; }
    }
  

internal:
    System::String^ m_pmName;
    GCN::IDecoder* m_pDecoder;
    DWORD m_nArg0;
    DWORD m_nArg1;
};


private ref class AMDShader_Impl : public Pyramid::IAMDShader
{
public:

    AMDShader_Impl( AMDDriver_Impl^ pOwner, AMDAsic_Impl^ asic, Elf32_Ehdr* pElf, DWORD nElfSize )
        : m_pmDriver(pOwner), 
          m_pmAsic(asic), 
          m_pElf(pElf),
          m_nElfSize(nElfSize),
          m_pISA(0),
          m_nISASize(0)
    {
        // make sure they're using ELF
        if( pElf->e_ident[0] != 0x7f || 
            pElf->e_ident[1] != 'E' || 
            pElf->e_ident[2] != 'L' ||
            pElf->e_ident[3] != 'F' )
            throw gcnew System::Exception("Blob received from AMD driver is not an elf binary");

        // find the '.text' section, which contains the code
        byte* pElfBytes = (byte*)pElf;
        Elf32_Shdr* pSections = (Elf32_Shdr*) (pElfBytes + pElf->e_shoff);

        Elf32_Shdr* pStringTable = pSections + pElf->e_shstrndx;
        byte* pElfStringTable    = pElfBytes + pStringTable->sh_offset;
        for( DWORD i=0; i<pElf->e_shnum; i++ )
        {
            DWORD nString = pSections[i].sh_name;
            byte* pSectionName = pElfStringTable + pSections[i].sh_name;
            if( strcmp( (const char*) pSectionName, ".text" ) == 0 )
            {
                m_pISA     = pElfBytes + pSections[i].sh_offset;
                m_nISASize = pSections[i].sh_size;
                break;
            }
        }

        // didn't find it.  Kick and scream
        if( !m_pISA )
            throw gcnew System::Exception( "Blob received from AMD driver does not contain a '.text' section");
    }

    ~AMDShader_Impl()
    {
        GCN::IDecoder::Destroy( m_pDecoder );
        m_pmDriver->m_pFreeFunc(m_pElf);
        m_pElf=0;
        m_nElfSize=0;
    }

    virtual array<byte>^ ReadISABytes()
    {
        array<byte>^ bytes = gcnew array<byte>(m_nISASize);
        for( DWORD i=0; i<m_nISASize; i++ )
            bytes[i] = m_pISA[i];
        return bytes;
    }

    virtual System::String^ Disassemble()
    {
        Printer printer;

        if( !GCN::Disassembler::DisassembleProgram( *m_pmAsic->m_pDecoder, printer, m_pISA, m_nISASize ) )
            printer.Push("Disassembly terminated due to encoding error\n");

        printer.m_Bytes.push_back(0);

        System::String^ str = MakeString( printer.m_Bytes.data() );
        return str->Replace( "\n", System::Environment::NewLine );
    }

    
    virtual System::String^ ListEncodings()
    {
        Printer printer;

        if( !GCN::Disassembler::ListEncodings( *m_pmAsic->m_pDecoder, printer, m_pISA, m_nISASize ) )
            printer.Push("Disassembly terminated due to encoding error\n");

        printer.m_Bytes.push_back(0);

        System::String^ str = MakeString( printer.m_Bytes.data() );
        return str->Replace( "\n", System::Environment::NewLine );
    }

    property Pyramid::IAMDAsic^ Asic
    {
        virtual Pyramid::IAMDAsic^ get() { return m_pmAsic; }
    }

internal:
    AMDDriver_Impl^ m_pmDriver;
    AMDAsic_Impl^ m_pmAsic; 
    Elf32_Ehdr* m_pElf;
    DWORD m_nElfSize;

    byte* m_pISA;
    DWORD m_nISASize;
    GCN::IDecoder* m_pDecoder;
};




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
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Kalindi",   120, 20, GCN::IDecoder::GCN1  ) );
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Spectre",   120, 1 , GCN::IDecoder::GCN1  ) );
    m_pmAsics->Add( gcnew AMDAsic_Impl( "Spooky",    120, 65, GCN::IDecoder::GCN1  ) );

    // Other stuff the driver accepted but I haven't identified
    m_pmAsics->Add( gcnew AMDAsic_Impl( "asic-125",  125, 1, GCN::IDecoder::GCN1 )) ;
    m_pmAsics->Add( gcnew AMDAsic_Impl( "asic-130",  130, 1, GCN::IDecoder::GCN3 )) ;
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
    DWORD       Twelve;
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
    result.Twelve = 12;
    result.pElf = 0;
    result.nElfSize = 0;
    DWORD hResult = m_pCompileFunc( &args, &result );

    if( hResult != 0 )
        throw gcnew System::Exception( "Error returned from AMD driver compile" );

    return gcnew AMDShader_Impl( this, a, result.pElf, result.nElfSize );
}
