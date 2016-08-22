#include "D3DCompiler_Impl.h"
#include "Utilities.h"

#include <Windows.h>




DXShaderBlob_Impl::DXShaderBlob_Impl( ID3DBlob* pBlob, D3DCompiler_Impl^ pCompiler ) : m_pBlob(pBlob), m_pmCompiler(pCompiler)
{
}
DXShaderBlob_Impl::~DXShaderBlob_Impl()
{
    if( m_pBlob )
        m_pBlob->Release();
}

System::String^ DXShaderBlob_Impl::Disassemble()
{
    if( m_pBlob )
    {
        ID3DBlob* pAsm=0;
        HRESULT hr = m_pmCompiler->m_pDisassemble( m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), 0, 0, &pAsm );
        if( SUCCEEDED(hr) && pAsm )
        {
            System::String^ IR = Marshal::PtrToStringAnsi( System::IntPtr(pAsm->GetBufferPointer()) );
            IR = IR->Replace( gcnew System::String("\n"), System::Environment::NewLine );
            pAsm->Release();
            return IR;
        }
    }
    return gcnew System::String("");
}
array<byte>^ DXShaderBlob_Impl::ReadBytes()
{
    if( !m_pBlob )
        return nullptr;

    size_t size  = m_pBlob->GetBufferSize();
    byte* pBytes = (byte*) m_pBlob->GetBufferPointer();
    array<byte>^ pmBytes = gcnew array<byte>( m_pBlob->GetBufferSize() );
    for( size_t i=0; i<size; i++ )
        pmBytes[i] = pBytes[i];

    return pmBytes;
}

Pyramid::IDXShaderBlob^ DXShaderBlob_Impl::Strip()
{
    if( !m_pBlob )
        return nullptr;

    UINT uFlags = D3DCOMPILER_STRIP_REFLECTION_DATA|
                  D3DCOMPILER_STRIP_DEBUG_INFO|
                  D3DCOMPILER_STRIP_TEST_BLOBS|
                  D3DCOMPILER_STRIP_PRIVATE_DATA;

    ID3DBlob* pStripped=0;
    HRESULT hr = m_pmCompiler->m_pStrip( m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), uFlags, &pStripped );
    if( !SUCCEEDED(hr))
        throw gcnew System::Exception("D3DStripShader failed");

    return gcnew DXShaderBlob_Impl(pStripped,m_pmCompiler);
}


Pyramid::IDXShaderBlob^ DXShaderBlob_Impl::GetSignatureBlob()
{
    if( !m_pBlob )
        return nullptr;

    ID3DBlob* pSignature=0;
    
    HRESULT hr = m_pmCompiler->m_pGetBlob( m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), D3D_BLOB_INPUT_AND_OUTPUT_SIGNATURE_BLOB,0, &pSignature );
    if( !SUCCEEDED(hr))
        throw gcnew System::Exception("D3DGetBlobPart failed");

    return gcnew DXShaderBlob_Impl(pSignature,m_pmCompiler);
}

Pyramid::IDXShaderBlob^ DXShaderBlob_Impl::GetExecutableBlob()
{
    if( !m_pBlob )
        return nullptr;

    // The actual, executable bytecode is signaled by a magic 4cc code
    //   Following the fourcc is the size of the executable code in bytes
    // 
    DWORD nSM4Sentinel = MAKEFOURCC('S','H','D','R');
    DWORD nSM5Sentinel = MAKEFOURCC('S','H','E','X');
    DWORD* pBlob = (DWORD*) m_pBlob->GetBufferPointer();
    for( UINT i=0; i<m_pBlob->GetBufferSize(); i+= 4 )
    {
        if( pBlob[i/4] == nSM5Sentinel || pBlob[i/4] == nSM4Sentinel )
        {
            DWORD nCodeSizeInBytes = pBlob[(i/4) + 1];
            
            ID3DBlob* pResultBlob=0;
            if( FAILED(m_pmCompiler->m_pCreateBlob( nCodeSizeInBytes, &pResultBlob ) ) )
                throw gcnew System::Exception("D3DCreateBlob failed");

            memcpy( pResultBlob->GetBufferPointer(), pBlob + (i/4)+2, nCodeSizeInBytes );
            return gcnew DXShaderBlob_Impl( pResultBlob, m_pmCompiler );
        }
    }
    
    return nullptr; // no code in this blob
}

Pyramid::IDXShaderReflection^ DXShaderBlob_Impl::Reflect()
{
    ID3D11ShaderReflection* pReflect=0;
    HRESULT hr = m_pmCompiler->m_pReflect( m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), (void**) &pReflect );
    if( FAILED(hr) )
        throw gcnew System::Exception("Shader reflection failed");

    return gcnew DXShaderReflection_Impl(pReflect);
}


D3DCompiler_Impl::D3DCompiler_Impl( System::String^ DLLPath )
{
    MarshalledString DLL(DLLPath);
    
    HMODULE hDLL = LoadLibraryA( DLL );
    if( !hDLL )
        throw gcnew System::Exception(System::String::Format( "LoadLibrary failed for {0}", DLLPath ));
    
    void* pCompile2Func = GetProcAddress( hDLL, "D3DCompile2" );
    
    void* pCompileFunc = GetProcAddress( hDLL, "D3DCompile" );
    if( !pCompileFunc )
        throw gcnew System::Exception(System::String::Format( "GetProcAddress failed for 'D3DCompile'"));
    
    void* pDisassembleFunc = GetProcAddress( hDLL, "D3DDisassemble" );
    if( !pDisassembleFunc )
        throw gcnew System::Exception(System::String::Format("GetProcAddress failed for 'D3DDisassemble'"));
   
    void* pStripFunc = GetProcAddress(hDLL,"D3DStripShader");
    if( !pStripFunc )
        throw gcnew System::Exception(System::String::Format("GetProcAddress failed for 'D3DStripShader'"));

    void* pGetBlobPartFunc = GetProcAddress(hDLL,"D3DGetBlobPart");
    if( !pGetBlobPartFunc )
        throw gcnew System::Exception(System::String::Format("GetProcAddress failed for 'D3DGetBlobPart'"));

    void* pCreateBlobFunc = GetProcAddress(hDLL, "D3DCreateBlob" );
    if( !pCreateBlobFunc )
        throw gcnew System::Exception(System::String::Format("GetProcAddress failed for 'D3DCreateBlob'"));

    void* pReflectFunc = GetProcAddress(hDLL, "D3DReflect");
    if( !pReflectFunc )
        throw gcnew System::Exception(System::String::Format("GetProcAddress failed for 'D3DReflect'"));


    m_pCompile2    = (D3DCOMPILE2_FUNC) pCompile2Func;
    m_pCompile     = (D3DCOMPILE_FUNC) pCompileFunc;
    m_pDisassemble = (D3DDISASSEMBLE_FUNC) pDisassembleFunc;
    m_pStrip       = (D3DSTRIP_FUNC)pStripFunc;
    m_pGetBlob     = (D3DGETBLOBPART_FUNC)pGetBlobPartFunc;
    m_pCreateBlob  = (D3DCREATEBLOB_FUNC) pCreateBlobFunc;
    m_pReflect     = (D3DREFLECT_FUNC) pReflectFunc;
}

bool D3DCompiler_Impl::Compile( System::String^ Shader, 
                                Pyramid::IHLSLOptions^ opts, 
                                Pyramid::IDXShaderBlob^% IR,  
                                System::String^% Messages  )
{
    MarshalledString hlsl( Shader );
    MarshalledString entry( opts->EntryPoint );
    MarshalledString profile( opts->Target.ToString() );

    ID3DBlob* pCode=0;
    ID3DBlob* pMessages=0;
    ID3DBlob* pAsm=0;
    HRESULT hr;
    if( m_pCompile2 )
    {
        hr = m_pCompile2( hlsl.GetString(), 
                    hlsl.Length()+1, 
                    "", 
                    0,
                    0,
                    entry, profile, opts->GetD3DCompileFlagBits(), 0, 
                    0,0,0,
                    &pCode, &pMessages );
    }
    else
    {
        hr = m_pCompile( hlsl.GetString(), 
                    hlsl.Length()+1, 
                    "", 
                    0,
                    0,
                    entry, profile, opts->GetD3DCompileFlagBits(), 0, &pCode, &pMessages );
    }

    Messages = nullptr;
    if( pMessages )
    {
        Messages = Marshal::PtrToStringAnsi( System::IntPtr(pMessages->GetBufferPointer()) );
        Messages = Messages->Replace( gcnew System::String("\n"), System::Environment::NewLine );   
    }

    if( SUCCEEDED(hr) )
        IR = gcnew DXShaderBlob_Impl( pCode, this );
    
    return SUCCEEDED(hr);
}