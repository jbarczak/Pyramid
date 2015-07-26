

#pragma unmanaged
#include "elf.h"
typedef unsigned long DWORD;
typedef unsigned char byte;
#pragma managed

ref class AMDAsic_Impl;
ref class AMDDriver_Impl;

private ref class AMDShader_Impl : public Pyramid::IAMDShader
{
public:

    AMDShader_Impl( AMDDriver_Impl^ pOwner, AMDAsic_Impl^ asic, Elf32_Ehdr* pElf, DWORD nElfSize );
      

    ~AMDShader_Impl();

    virtual array<byte>^ ReadISABytes();
   
    virtual System::String^ Disassemble();
  

    
    virtual System::String^ ListEncodings();

    virtual System::String^ PrintStats();

    property Pyramid::IAMDAsic^ Asic
    {
        virtual Pyramid::IAMDAsic^ get();
    }

    size_t GetOccupancy();
    
    virtual Pyramid::Scrutinizer::IScrutinizer^ CreateScrutinizer();

internal:
    AMDDriver_Impl^ m_pmDriver;
    AMDAsic_Impl^ m_pmAsic; 
    Elf32_Ehdr* m_pElf;
    DWORD m_nElfSize;

    DWORD* m_pStats;
    DWORD  m_nStatsSize;

    byte* m_pISA;
    DWORD m_nISASize;
    GCN::IDecoder* m_pDecoder;
};
