#pragma unmanaged
#include "AMDTBackend/Include/Common/AmdDxGsaCompile.h"
typedef PfnAmdDxGsaCompileShader COMPILE_SHADER;
typedef PfnAmdDxGsaFreeCompiledShader FREE_SHADER;
#pragma managed

private ref class AMDDriver_Impl : public Pyramid::IAMDDriver
{
public:

    AMDDriver_Impl( System::String^ path );
    

    property System::Collections::Generic::IEnumerable<Pyramid::IAMDAsic^>^ Asics
    {
        virtual System::Collections::Generic::IEnumerable<Pyramid::IAMDAsic^>^ get()
        {
            return m_pmAsics;
        }
    }

    virtual Pyramid::IAMDShader^  CompileDXBlob(Pyramid::IAMDAsic^ asic, array<byte>^ exe, Pyramid::IDXShaderReflection^ reflection );

internal:

    COMPILE_SHADER  m_pCompileFunc;
    FREE_SHADER     m_pFreeFunc;
    
private:

    System::Collections::Generic::List<Pyramid::IAMDAsic^>^ m_pmAsics;
};