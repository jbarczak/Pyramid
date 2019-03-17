#include "D3DCompiler_Impl.h"
#include "Utilities.h"

#include <Windows.h>

#pragma unmanaged
#include <vector>
#include <atlbase.h>
#include "dxc/dxcapi.h"
#pragma managed

///////////////////////////////////////////////////////////////////////////////////////

class PyramidD3DIncludeHandler : public ID3DInclude
{
public:
    PyramidD3DIncludeHandler( Pyramid::IIncludeHandler^ pmInclude, System::String^ root )
        : m_pmInclude(pmInclude)
    {
        // D3D compiler doesn't pass the source file path to you
        //  They expect you to track it yourself.
        m_Stack.push_back(root);
    }

    virtual  HRESULT __stdcall Open(
         D3D_INCLUDE_TYPE IncludeType,
         LPCSTR           pFileName,
         LPCVOID          pParentData,
          LPCVOID          *ppData,
         UINT             *pBytes) override
    {
        *ppData=0;
        *pBytes=0;

        Pyramid::IIncludeHandler^ pmInclude = static_cast<Pyramid::IIncludeHandler^>(m_pmInclude);
        if( pmInclude == nullptr )
            return E_FAIL;
        
        Pyramid::IncludeType eType;
        switch( IncludeType )
        {
        case D3D_INCLUDE_SYSTEM: eType = Pyramid::IncludeType::System; break;
        case D3D_INCLUDE_LOCAL:  eType = Pyramid::IncludeType::User; break;
        default: return E_FAIL;
        }

        
        Pyramid::IIncludeResult^ result = pmInclude->OpenInclude(eType,MakeString(pFileName),m_Stack.back() );
        if( result == nullptr )
            return E_FAIL;
        
        array<unsigned char>^ pFileContents =  result->Contents;
        void* pNative = malloc(pFileContents->Length);

        Marshal::Copy( pFileContents, 0, System::IntPtr(pNative), pFileContents->Length);
        
        *ppData = pNative;
        *pBytes = pFileContents->Length;
        m_Stack.push_back( result->FullPath );
        return S_OK;
    }

    virtual HRESULT __stdcall Close(LPCVOID pData) override
    {
        if( !pData )
            return S_OK;

        free((void*)pData);
        m_Stack.pop_back();
        return S_OK;
    }

private:
    gcroot<Pyramid::IIncludeHandler^> m_pmInclude;
    std::vector< gcroot<System::String^> > m_Stack;

};

///////////////////////////////////////////////////////////////////////////////////////
DXBCRootSignatureBlob_Impl::DXBCRootSignatureBlob_Impl( ID3DBlob* pBlob,D3DCompiler_Impl^ pCompiler ) : m_pBlob( pBlob ),m_pmCompiler( pCompiler )
{
}

DXBCRootSignatureBlob_Impl::~DXBCRootSignatureBlob_Impl()
{
    if ( m_pBlob )
        m_pBlob->Release();
}
array<byte>^ DXBCRootSignatureBlob_Impl::ReadBytes()
{
    if ( !m_pBlob )
        return nullptr;

    size_t size  = m_pBlob->GetBufferSize();
    byte* pBytes = (byte*)m_pBlob->GetBufferPointer();
    array<byte>^ pmBytes = gcnew array<byte>( m_pBlob->GetBufferSize() );
    for ( size_t i=0; i<size; i++ )
        pmBytes[i] = pBytes[i];

    return pmBytes;
}
///////////////////////////////////////////////////////////////////////////////////////

DXBCShaderBlob_Impl::DXBCShaderBlob_Impl( ID3DBlob* pBlob, D3DCompiler_Impl^ pCompiler ) : m_pBlob(pBlob), m_pmCompiler(pCompiler)
{
}
DXBCShaderBlob_Impl::~DXBCShaderBlob_Impl()
{
    if( m_pBlob )
        m_pBlob->Release();
}

Pyramid::IDXBlob^ DXBCShaderBlob_Impl::ExtractRootSignature()
{
    if ( m_pBlob )
    {
        ID3DBlob* pRSBlob;
        HRESULT hr = m_pmCompiler->m_pGetBlob( m_pBlob->GetBufferPointer(),m_pBlob->GetBufferSize(),D3D_BLOB_ROOT_SIGNATURE,0,&pRSBlob );
        if ( SUCCEEDED( hr ) && pRSBlob != nullptr )
        {
            return gcnew DXBCRootSignatureBlob_Impl( pRSBlob,m_pmCompiler );
        }
    }
    return nullptr;
}

System::String^ DXBCShaderBlob_Impl::Disassemble()
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
array<byte>^ DXBCShaderBlob_Impl::ReadBytes()
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

Pyramid::IDXBCShaderBlob^ DXBCShaderBlob_Impl::Strip()
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

    return gcnew DXBCShaderBlob_Impl(pStripped,m_pmCompiler);
}


Pyramid::IDXBCShaderBlob^ DXBCShaderBlob_Impl::GetSignatureBlob()
{
    if( !m_pBlob )
        return nullptr;

    ID3DBlob* pSignature=0;
    
    HRESULT hr = m_pmCompiler->m_pGetBlob( m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), D3D_BLOB_INPUT_AND_OUTPUT_SIGNATURE_BLOB,0, &pSignature );
    if( !SUCCEEDED(hr))
        throw gcnew System::Exception("D3DGetBlobPart failed");

    return gcnew DXBCShaderBlob_Impl(pSignature,m_pmCompiler);
}

Pyramid::IDXBCShaderBlob^ DXBCShaderBlob_Impl::GetExecutableBlob()
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
            return gcnew DXBCShaderBlob_Impl( pResultBlob, m_pmCompiler );
        }
    }
    
    return nullptr; // no code in this blob
}

Pyramid::IDXShaderReflection^ DXBCShaderBlob_Impl::Reflect()
{
    ID3D11ShaderReflection* pReflect=0;
    HRESULT hr = m_pmCompiler->m_pReflect( m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), (void**) &pReflect );
    if( FAILED(hr) )
        throw gcnew System::Exception("Shader reflection failed");

    return gcnew DXShaderReflection_Impl(pReflect);
}


///////////////////////////////////////////////////////////////////////////////////////

D3DCompiler_Impl::D3DCompiler_Impl( System::String^ DLLPath,  Pyramid::IIncludeHandler^ pmInclude )
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
    m_pmInclude    = pmInclude;
}



bool D3DCompiler_Impl::Compile( System::String^ Shader,
    Pyramid::IHLSLOptions^ opts,
    System::String^ fileName,
    Pyramid::IDXBCShaderBlob^% IR,
    System::String^% Messages )
{
   
     
    ID3DBlob* pCode=0;
    ID3DBlob* pMessages=0;
    HRESULT hr;
    MarshalledString hlsl( Shader );
    MarshalledString entry( opts->EntryPoint );
    MarshalledString profile( opts->Target.ToString() );

    PyramidD3DIncludeHandler include( m_pmInclude,fileName );


    if ( m_pCompile2 )
    {
        hr = m_pCompile2( hlsl.GetString(),
            hlsl.Length()+1,
            "",
            0,
            &include,
            entry,profile,opts->GetD3DCompileFlagBits(),0,
            0,0,0,
            &pCode,&pMessages );
    }
    else
    {
        hr = m_pCompile( hlsl.GetString(),
            hlsl.Length()+1,
            "",
            0,
            &include,
            entry,profile,opts->GetD3DCompileFlagBits(),0,&pCode,&pMessages );
    }
   
    if ( SUCCEEDED( hr ) )
        IR = gcnew DXBCShaderBlob_Impl( pCode,this );

    Messages = nullptr;
    if ( pMessages )
    {
        Messages = Marshal::PtrToStringAnsi( System::IntPtr( pMessages->GetBufferPointer() ) );
        Messages = Messages->Replace( gcnew System::String( "\n" ),System::Environment::NewLine );
        pMessages->Release();
    }

    return SUCCEEDED( hr );
}

bool D3DCompiler_Impl::CompileRootSignature(
        System::String^ Shader,
        Pyramid::IHLSLOptions^ opts,
        System::String^ Path,
        Pyramid::IDXBlob^% blob,
        System::String^% Messages )
{


    ID3DBlob* pCode=0;
    ID3DBlob* pMessages=0;
    HRESULT hr;
    MarshalledString hlsl( Shader );
    MarshalledString entry( opts->RootSigMacro );
    MarshalledString profile( opts->RootSigTarget.ToString() );

    PyramidD3DIncludeHandler include( m_pmInclude,Path );


    {
        hr = m_pCompile( hlsl.GetString(),
            hlsl.Length()+1,
            "",
            0,
            &include,
            entry,profile,opts->GetD3DCompileFlagBits(),0,&pCode,&pMessages );
    }

    if ( SUCCEEDED( hr ) )
        blob = gcnew DXBCRootSignatureBlob_Impl( pCode,this );

    Messages = nullptr;
    if ( pMessages )
    {
        Messages = Marshal::PtrToStringAnsi( System::IntPtr( pMessages->GetBufferPointer() ) );
        Messages = Messages->Replace( gcnew System::String( "\n" ),System::Environment::NewLine );
        pMessages->Release();
    }

    return SUCCEEDED( hr );

}