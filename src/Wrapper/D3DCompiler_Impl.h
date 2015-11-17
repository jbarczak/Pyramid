
#ifndef _D3D_COMPILER_IMPL_H_
#define _D3D_COMPILER_IMPL_H_

#pragma unmanaged
#include <d3dcompiler.h>
#pragma managed

typedef HRESULT (WINAPI *D3DCOMPILE_FUNC)(
                     LPCVOID pSrcData,
                     SIZE_T SrcDataSize,
                     LPCSTR pSourceName,
                     const D3D_SHADER_MACRO *pDefines,
                     ID3DInclude *pInclude,
                     LPCSTR pEntrypoint,
                     LPCSTR pTarget,
                     UINT Flags1,
                     UINT Flags2,
                     ID3DBlob **ppCode,
                     ID3DBlob **ppErrorMsgs);

typedef HRESULT (WINAPI *D3DDISASSEMBLE_FUNC)(
    LPCVOID pSrcData,
    SIZE_T SrcDataSize,
    UINT Flags,
    LPCSTR szComments,
    ID3DBlob **ppDisassembly
);

typedef HRESULT (WINAPI *D3DSTRIP_FUNC)(
       LPCVOID pShaderBytecode,
       SIZE_T BytecodeLength,
       UINT uStripFlags,
       ID3DBlob **ppStrippedBlob
    );

typedef HRESULT (WINAPI *D3DGETBLOBPART_FUNC)(
       LPCVOID pShaderBytecode,
       SIZE_T BytecodeLength,
       UINT uBlobPartEnum,
       UINT uFlags,
       ID3DBlob **ppBlob
    );

typedef HRESULT (WINAPI *D3DCREATEBLOB_FUNC)(
        SIZE_T nBytes,
        ID3DBlob** ppBlob
    );

typedef HRESULT (WINAPI *D3DREFLECT_FUNC)(
   LPCVOID pSrcData,
   SIZE_T SrcDataSize,
   REFIID pInterface,
   void** ppReflector
    );

ref class D3DCompiler_Impl;

private ref class DXShaderBlob_Impl : Pyramid::IDXShaderBlob
{
public:
    DXShaderBlob_Impl( ID3DBlob* pBlob, D3DCompiler_Impl^ pCompiler );
    ~DXShaderBlob_Impl( );

    virtual System::String^ Disassemble();
    virtual array<byte>^ ReadBytes();
    virtual Pyramid::IDXShaderBlob^ Strip();
    virtual Pyramid::IDXShaderBlob^ GetSignatureBlob();
    virtual Pyramid::IDXShaderBlob^ GetExecutableBlob();
    virtual Pyramid::IDXShaderReflection^ Reflect();

private:
    ID3DBlob* m_pBlob;
    D3DCompiler_Impl^ m_pmCompiler;
};


private ref class D3DCompiler_Impl : Pyramid::ID3DCompiler
{
public:

    D3DCompiler_Impl( System::String^ DLLPath );
    
    virtual bool Compile( System::String^ Shader, 
                          Pyramid::IHLSLOptions^ opts, 
                          Pyramid::IDXShaderBlob^% blob,  
                          System::String^% Errors  );

internal:
    
    D3DCOMPILE_FUNC m_pCompile;
    D3DDISASSEMBLE_FUNC m_pDisassemble;
    D3DSTRIP_FUNC m_pStrip;
    D3DGETBLOBPART_FUNC m_pGetBlob;
    D3DCREATEBLOB_FUNC m_pCreateBlob;
    D3DREFLECT_FUNC m_pReflect;
};


private ref class DXShaderReflection_Impl : public Pyramid::IDXShaderReflection
{
public:
    DXShaderReflection_Impl( ID3D11ShaderReflection* pRef )
        : m_pRef(pRef)
    {
    }

    ~DXShaderReflection_Impl()
    {
        m_pRef->Release();
    }

    virtual UINT GetThreadsPerGroup()
    {
        UINT SizeX, SizeY, SizeZ;
        m_pRef->GetThreadGroupSize(&SizeX,&SizeY,&SizeZ);
        return SizeX*SizeY*SizeZ;
    }

    virtual Pyramid::HLSLShaderType GetShaderType()
    {
        D3D11_SHADER_DESC d;
        m_pRef->GetDesc(&d);

        UINT type = D3D11_SHVER_GET_TYPE(d.Version);
        switch(type)
        {
        case D3D11_SHVER_PIXEL_SHADER:      return Pyramid::HLSLShaderType::PIXEL;
        case D3D11_SHVER_VERTEX_SHADER:     return Pyramid::HLSLShaderType::VERTEX;
        case D3D11_SHVER_GEOMETRY_SHADER:   return Pyramid::HLSLShaderType::GEOMETRY;
        case D3D11_SHVER_HULL_SHADER:       return Pyramid::HLSLShaderType::HULL;
        case D3D11_SHVER_DOMAIN_SHADER:     return Pyramid::HLSLShaderType::DOMAIN;
        case D3D11_SHVER_COMPUTE_SHADER:    return Pyramid::HLSLShaderType::COMPUTE;
        }

        throw gcnew System::Exception("Bad DX shader type??");
    }

    ID3D11ShaderReflection* m_pRef;

};

#endif