
#pragma unmanaged
#include <windows.h>
#include "elf.h"
#include "GCNDisassembler.h"
#include "GCNDecoder.h"
#include "GCNBufferedPrinter.h"
#pragma managed


#include "AMDShader_Impl.h"
#include "AMDDriver_Impl.h"
#include "AMDAsic_Impl.h"
#include "Utilities.h"

#include "Scrutinizer_GCN.h"

AMDShader_Impl::AMDShader_Impl( AMDDriver_Impl^ pOwner, AMDAsic_Impl^ asic, Elf32_Ehdr* pElf, DWORD nElfSize )
    : m_pmDriver(pOwner), 
        m_pmAsic(asic), 
        m_pElf(pElf),
        m_nElfSize(nElfSize),
        m_pISA(0),
        m_nISASize(0),
        m_pStats(0),
        m_nStatsSize(0)
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
        }
        if( strcmp( (const char*) pSectionName, ".stats" ) == 0 )
        {
            m_pStats = (DWORD*) (pElfBytes + pSections[i].sh_offset);
            m_nStatsSize = pSections[i].sh_size;
        }
    }

    // didn't find it.  Kick and scream
    if( !m_pISA )
        throw gcnew System::Exception( "Blob received from AMD driver does not contain a '.text' section");
    if( !m_pStats || (m_nStatsSize % 4) )
        throw gcnew System::Exception( "'.stats' section from driver blob is missing or misaligned");
}

AMDShader_Impl::~AMDShader_Impl()
{
    GCN::IDecoder::Destroy( m_pDecoder );
    m_pmDriver->m_pFreeFunc(m_pElf);
    m_pElf=0;
    m_nElfSize=0;
    m_pStats=0;
    m_nStatsSize=0;
    m_pISA=0;
    m_nISASize=0;
}


array<byte>^ AMDShader_Impl::ReadISABytes()
{
    array<byte>^ bytes = gcnew array<byte>(m_nISASize);
    for( DWORD i=0; i<m_nISASize; i++ )
        bytes[i] = m_pISA[i];
    return bytes;
}

System::String^ AMDShader_Impl::Disassemble()
{
    GCN::Disassembler::BufferedPrinter printer;

    if( !GCN::Disassembler::DisassembleProgram( *m_pmAsic->m_pDecoder, printer, m_pISA, m_nISASize ) )
        printer.Push("Disassembly terminated due to encoding error\n");

    printer.m_Bytes.push_back(0);

    System::String^ str = MakeString( printer.m_Bytes.data() );
    return str->Replace( "\n", System::Environment::NewLine );
}

System::String^ AMDShader_Impl::ListEncodings()
{
    GCN::Disassembler::BufferedPrinter  printer;

    if( !GCN::Disassembler::ListEncodings( *m_pmAsic->m_pDecoder, printer, m_pISA, m_nISASize ) )
        printer.Push("Disassembly terminated due to encoding error\n");

    printer.m_Bytes.push_back(0);

    System::String^ str = MakeString( printer.m_Bytes.data() );
    return str->Replace( "\n", System::Environment::NewLine );
}


System::String^ AMDShader_Impl::PrintStats()
{
    DWORD nSGPRs        = m_pStats[0];
    DWORD nMaxSGPRs     = m_pStats[1];
    DWORD nVGPRs        = m_pStats[2];
    DWORD nMaxVGPRs     = m_pStats[3];
    DWORD nUsedLDS      = m_pStats[4];
    DWORD nMaxLDS       = m_pStats[5];
    DWORD nScratchBytes = m_pStats[6];
    DWORD nALUOps       = m_pStats[7];
    DWORD nScalarOps    = m_pStats[8];
    DWORD nMemoryOps    = m_pStats[9];

    // NOTE:
    //   SI and CI had to have GPRs allocated in multiples of 4
    //     According to docs this is true for VI as well.
    //
    //  However, for VI chips, driver is reporting VGPR counts that are not multiples of 4
    //     I do not know if this is a driver bug or a doc bug, but
    //     I'm going to assume the latter
    //
    DWORD nVGPROccupancy=10;
    DWORD nSGPROccupancy=10;
    if( nVGPRs )
        nVGPROccupancy = nMaxVGPRs/nVGPRs;
    if( nSGPRs )
        nSGPROccupancy = 512/nSGPRs;

    if( nVGPROccupancy > 10 )
        nVGPROccupancy = 10;
    if( nSGPROccupancy > 10 )
        nSGPROccupancy = 10;

    DWORD nLDSOccupancy = 0;
    if( nUsedLDS )
        nLDSOccupancy = nMaxLDS / nUsedLDS;

    char buffer[4096];
    sprintf( buffer,
        "SGPRs:          %3u / %u\n"
        "VGPRs:          %3u / %u\n"
        "LDS bytes/tg %6u / %u\n"
        "Occupancy:\n"
        "   S: %2u waves\n"
        "   V: %2u waves\n"
        "   L: %2u groups\n"
        "Ops:\n"
        "   VALU: %u\n"
        "   S:    %u\n"
        "   VMEM: %u\n",
        nSGPRs,nMaxSGPRs,
        nVGPRs,nMaxVGPRs,
        nUsedLDS,nMaxLDS,
        nSGPROccupancy,
        nVGPROccupancy,
        nLDSOccupancy,
        nALUOps,
        nScalarOps,
        nMemoryOps
        );

    System::String^ str = gcnew System::String(buffer);
    return str->Replace("\n", System::Environment::NewLine );
}

size_t AMDShader_Impl::GetOccupancy()
{
    DWORD nSGPRs        = m_pStats[0];
    DWORD nMaxSGPRs     = m_pStats[1];
    DWORD nVGPRs        = m_pStats[2];
    DWORD nMaxVGPRs     = m_pStats[3];
    DWORD nUsedLDS      = m_pStats[4];
    DWORD nMaxLDS       = m_pStats[5];
    DWORD nScratchBytes = m_pStats[6];
    DWORD nALUOps       = m_pStats[7];
    DWORD nScalarOps    = m_pStats[8];
    DWORD nMemoryOps    = m_pStats[9];

    // NOTE:
    //   SI and CI had to have GPRs allocated in multiples of 4
    //     According to docs this is true for VI as well.
    //
    //  However, for VI chips, driver is reporting VGPR counts that are not multiples of 4
    //     I do not know if this is a driver bug or a doc bug, but
    //     I'm going to assume the latter
    //
    DWORD nVGPROccupancy=10;
    DWORD nSGPROccupancy=10;
    if( nVGPRs )
        nVGPROccupancy = nMaxVGPRs/nVGPRs;
    if( nSGPRs )
        nSGPROccupancy = 512/nSGPRs;

    if( nVGPROccupancy > 10 )
        nVGPROccupancy = 10;
    if( nSGPROccupancy > 10 )
        nSGPROccupancy = 10;

    return (nVGPROccupancy < nSGPROccupancy) ? nVGPROccupancy : nSGPROccupancy;
}

Pyramid::Scrutinizer::IScrutinizer^ AMDShader_Impl::CreateScrutinizer()
{
    return gcnew Scrutinizer_GCN( m_pmAsic, this );
}


Pyramid::IAMDAsic^ AMDShader_Impl::Asic::get()
{
    return m_pmAsic;
}