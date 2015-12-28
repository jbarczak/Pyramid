

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

    enum class ShaderType
    {
        ST_VERTEX,
        ST_PIXEL,
        ST_GEOMETRY,
        ST_HULL,
        ST_DOMAIN,
        ST_COMPUTE
    };

    AMDShader_Impl( AMDDriver_Impl^ pOwner, AMDAsic_Impl^ asic, Elf32_Ehdr* pElf, 
                    DWORD nElfSize, DWORD nThreadsPerThreadGroup,
                    ShaderType eShaderType );
      

    ~AMDShader_Impl();

    virtual array<byte>^ ReadISABytes();
   
    virtual System::String^ Disassemble();
  

    
    virtual System::String^ ListEncodings();

    virtual System::String^ PrintStats( );

    property Pyramid::IAMDAsic^ Asic
    {
        virtual Pyramid::IAMDAsic^ get();
    }

    size_t GetWaveOccupancy();
    size_t GetGroupOccupancy();
    size_t GetThreadsPerThreadGroup()  { return m_nThreadsPerThreadGroup; }
    size_t GetWavesPerThreadGroup() { return (m_nThreadsPerThreadGroup+63)/64;}

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

    DWORD m_nThreadsPerThreadGroup;
    ShaderType m_eShaderType;
};
