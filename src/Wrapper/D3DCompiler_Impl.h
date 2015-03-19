
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
};