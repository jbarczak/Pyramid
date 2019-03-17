#include "DXILCompiler_Impl.h"
#include "Utilities.h"


#pragma unmanaged
#include <vector>
#include <atlbase.h>
#include <d3dcompiler.h>
#include "GenHLSLExtensionIntrinsics.h"
#include "dxc/hlsl/DxilContainer.h"
#pragma managed



///////////////////////////////////////////////////////////////////////////////////////



// Use our own blob class bc we don't have any path for IDxcBlob from raw bytes
//  without somehow guessing the encoding.  Marcello has punted this problem to us
//   and we shall punt it back to him.... :)
#pragma unmanaged
class MyBlob : public IDxcBlob
{
public:

    MyBlob( void* p, size_t l ) : m_cRef( 1 ),m_pPtr( p ),m_nSize( l ) {}
    ~MyBlob()
    {
        free( m_pPtr );
    }
    ULONG STDMETHODCALLTYPE AddRef()
    {
        InterlockedIncrement( &m_cRef );
        return m_cRef;
    }
    ULONG STDMETHODCALLTYPE Release()
    {
        // Decrement the object's internal counter.
        ULONG ulRefCount = InterlockedDecrement( &m_cRef );
        if ( 0 == m_cRef )
        {
            delete this;
        }
        return ulRefCount;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface( REFIID   riid, LPVOID * ppvObj )
    {
        // Always set out parameter to NULL, validating it first.
        if ( !ppvObj )
            return E_INVALIDARG;
        *ppvObj = NULL;
        if ( riid == IID_IUnknown || riid == __uuidof(IDxcBlob) )
        {
            // Increment the reference count and return the pointer.
            *ppvObj = (LPVOID)this;
            AddRef();
            return NOERROR;
        }
        return E_NOINTERFACE;
    }

    virtual LPVOID STDMETHODCALLTYPE GetBufferPointer( void ) { return m_pPtr; }
    virtual SIZE_T STDMETHODCALLTYPE GetBufferSize( void ) { return m_nSize; }

    ULONG m_cRef;
    void* m_pPtr;
    size_t m_nSize;
};
#pragma managed


class PyramidDxilIncludeHandler : public IDxcIncludeHandler
{
public:
    PyramidDxilIncludeHandler( Pyramid::IIncludeHandler^ pmInclude, IDxcLibrary* pDXCLibrary )
        : m_pmInclude( pmInclude ),
          m_pDXCLibrary( pDXCLibrary ), m_cRef(1)
    {
    }

    virtual HRESULT STDMETHODCALLTYPE LoadSource(
        _In_ LPCWSTR pFilename,                                   // Candidate filename.
        _COM_Outptr_result_maybenull_ IDxcBlob **ppIncludeSource  // Resultant source object for included file, nullptr if not found.
    ) override
    {

        Pyramid::IIncludeHandler^ pmInclude = static_cast<Pyramid::IIncludeHandler^>(m_pmInclude);
        if ( pmInclude == nullptr )
            return E_FAIL;

  
        Pyramid::IIncludeResult^ result = pmInclude->OpenInclude( Pyramid::IncludeType::User,MakeString( pFilename ), nullptr );
        if ( result == nullptr )
        {
            *ppIncludeSource = nullptr;
            return S_OK;
        }

        array<unsigned char>^ pFileContents =  result->Contents;
        void* pNative = malloc( pFileContents->Length );

        Marshal::Copy( pFileContents,0,System::IntPtr( pNative ),pFileContents->Length );

        *ppIncludeSource = new MyBlob( pNative,pFileContents->Length );
        return S_OK;
    }

    ULONG STDMETHODCALLTYPE AddRef()
    {
        InterlockedIncrement( &m_cRef );
        return m_cRef;
    }
    ULONG STDMETHODCALLTYPE Release()
    {
        // Decrement the object's internal counter.
        ULONG ulRefCount = InterlockedDecrement( &m_cRef );
        if ( 0 == m_cRef )
        {
            delete this;
        }
        return ulRefCount;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface( REFIID   riid,LPVOID * ppvObj )
    {
        // Always set out parameter to NULL, validating it first.
        if ( !ppvObj )
            return E_INVALIDARG;
        *ppvObj = NULL;
        if ( riid == IID_IUnknown || riid == __uuidof(IDxcIncludeHandler) )
        {
            // Increment the reference count and return the pointer.
            *ppvObj = (LPVOID)this;
            AddRef();
            return NOERROR;
        }
        return E_NOINTERFACE;
    }

private:
    gcroot<Pyramid::IIncludeHandler^> m_pmInclude;
    IDxcLibrary* m_pDXCLibrary = nullptr;

    ULONG m_cRef;

};

///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////

DXILShaderBlob_Impl::DXILShaderBlob_Impl( IDxcBlob* pBlob,DXILCompiler_Impl^ pCompiler ) : m_pBlob( pBlob ),m_pmCompiler( pCompiler )
{
}
DXILShaderBlob_Impl::~DXILShaderBlob_Impl()
{
    if ( m_pBlob )
        m_pBlob->Release();
}

System::String^ DXILShaderBlob_Impl::Disassemble()
{
    if ( m_pBlob )
    {
        IDxcBlobEncoding* pAsm=0;
        HRESULT hr = m_pmCompiler->m_pDXCCompiler->Disassemble( m_pBlob, &pAsm );

        if ( SUCCEEDED( hr ) && pAsm )
        {
            IDxcBlobEncoding* pUTF16 = nullptr;
            hr = m_pmCompiler->m_pDXCLibrary->GetBlobAsUtf16( pAsm, &pUTF16 );
            if ( SUCCEEDED( hr ) )
            {
                System::String^ IR = Marshal::PtrToStringUni( System::IntPtr( pUTF16->GetBufferPointer() ) );
                IR = IR->Replace( gcnew System::String( "\n" ),System::Environment::NewLine );
                pAsm->Release();
                return IR;
            }
        }
    }
    return gcnew System::String( "" );
}

Pyramid::IDXBlob^ DXILShaderBlob_Impl::ExtractRootSignature()
{
    CComPtr<IDxcContainerReflection> pReflector;
    HRESULT hr = m_pmCompiler->m_pfnCreateInstance( CLSID_DxcContainerReflection,__uuidof(IDxcContainerReflection),(void**)&pReflector );
    if ( pReflector == nullptr || FAILED( hr ) )
        return nullptr;

    hr = pReflector->Load( m_pBlob );
    if ( FAILED( hr ) )
        return nullptr;

    UINT32 partIdx;
    hr = pReflector->FindFirstPartKind( hlsl::DFCC_RootSignature,&partIdx );
    if ( FAILED( hr ) )
        return nullptr;

    IDxcBlob* pBlob;
    hr = pReflector->GetPartContent( partIdx,&pBlob );
    if ( FAILED( hr ) )
        return nullptr;

    return gcnew DXILRootSignatureBlob_Impl( pBlob,m_pmCompiler );
}

array<byte>^ DXILShaderBlob_Impl::ReadBytes()
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
///////////////////////////////////////////////////////////////////////////////////////
DXILRootSignatureBlob_Impl::DXILRootSignatureBlob_Impl( IDxcBlob* pBlob,DXILCompiler_Impl^ pCompiler ) : m_pBlob( pBlob ),m_pmCompiler( pCompiler )
{
}

DXILRootSignatureBlob_Impl::~DXILRootSignatureBlob_Impl()
{
    if ( m_pBlob )
        m_pBlob->Release();
}
array<byte>^ DXILRootSignatureBlob_Impl::ReadBytes()
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


///////////////////////////////////////////////////////////////////////////////////////

DXILCompiler_Impl::DXILCompiler_Impl( System::String^ DLLPath, Pyramid::IIncludeHandler^ pmInclude )
{
    m_pDXCCompiler = nullptr;
    m_pDXCLibrary = nullptr;
    
    MarshalledString DLL(DLLPath);
    
    HMODULE hDLL = LoadLibraryA( DLL );
    if( !hDLL )
        throw gcnew System::Exception(System::String::Format( "LoadLibrary failed for {0}", DLLPath ));
       
    DxcCreateInstanceProc pfnDxcCreateInstance = DxcCreateInstanceProc( GetProcAddress( hDLL,"DxcCreateInstance" ) );
    if ( !pfnDxcCreateInstance )
        throw gcnew System::Exception( System::String::Format( "No entrypoint named DxcCreateInstance in {0}",DLLPath ) );

    IDxcCompiler* pCompiler = nullptr;
    IDxcLibrary* pLibrary   = nullptr;

    if ( FAILED( pfnDxcCreateInstance( CLSID_DxcLibrary,__uuidof(IDxcLibrary),(void**)&pLibrary ) ) )
        throw gcnew System::Exception( System::String::Format( "DXCLibrary create failed in {0} ",DLLPath ) );

    if ( FAILED( pfnDxcCreateInstance( CLSID_DxcCompiler,__uuidof(IDxcCompiler),(void**)&pCompiler ) ) )
    {
        pLibrary->Release();
        throw gcnew System::Exception( System::String::Format( "DXCCompiler create failed in {0} ",DLLPath ) );
    }

    CComPtr<IDxcLangExtensions> pILangExtensions;

    if ( SUCCEEDED( pCompiler->QueryInterface<IDxcLangExtensions>( &pILangExtensions ) ) )
    {
        GenIntrinsicTable* pGenIntrinsicTable = new GenIntrinsicTable( IntelIntrinsics );
        if ( FAILED( pILangExtensions->RegisterIntrinsicTable( pGenIntrinsicTable ) ) )
            delete pGenIntrinsicTable;

        m_pGenIntrinsics = pGenIntrinsicTable;
    }

    m_pDXCCompiler = pCompiler;
    m_pDXCLibrary = pLibrary;
    m_pmInclude    = pmInclude;
    m_pfnCreateInstance = pfnDxcCreateInstance;
}


DXILCompiler_Impl::~DXILCompiler_Impl()
{
    if ( m_pDXCCompiler )
        m_pDXCCompiler->Release();
    if ( m_pDXCLibrary )
        m_pDXCLibrary->Release();

    m_pGenIntrinsics->Release();
}


#pragma unmanaged
static HRESULT CompileFromBlob( 
    IDxcCompiler* pCompiler,
    IDxcBlobEncoding *pSource,LPCWSTR pSourceName,
    const D3D_SHADER_MACRO *pDefines,IDxcIncludeHandler *pInclude,
    LPWSTR pEntrypointW, LPWSTR pTargetProfileW,UINT Flags1, 
    IDxcBlob **ppCode,
    IDxcBlobEncoding **ppErrorMsgs )
{
    CComPtr<IDxcOperationResult> operationResult;
    HRESULT hr;


    try {
        std::vector<std::wstring> defineValues;
        std::vector<DxcDefine> defines;

        std::vector<LPCWSTR> arguments;
        // /Gec, /Ges Not implemented:
        //if(Flags1 & D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY) arguments.push_back(L"/Gec");
        //if(Flags1 & D3DCOMPILE_ENABLE_STRICTNESS) arguments.push_back(L"/Ges");
        if ( Flags1 & D3DCOMPILE_IEEE_STRICTNESS ) arguments.push_back( L"/Gis" );
        if ( Flags1 & D3DCOMPILE_OPTIMIZATION_LEVEL2 )
        {
            switch ( Flags1 & D3DCOMPILE_OPTIMIZATION_LEVEL2 )
            {
            case D3DCOMPILE_OPTIMIZATION_LEVEL0: arguments.push_back( L"/O0" ); break;
            case D3DCOMPILE_OPTIMIZATION_LEVEL2: arguments.push_back( L"/O2" ); break;
            case D3DCOMPILE_OPTIMIZATION_LEVEL3: arguments.push_back( L"/O3" ); break;
            }
        }

        // Currently, /Od turns off too many optimization passes, causing incorrect DXIL to be generated.
        // Re-enable once /Od is implemented properly:
        //if(Flags1 & D3DCOMPILE_SKIP_OPTIMIZATION) arguments.push_back(L"/Od");
        if ( Flags1 & D3DCOMPILE_DEBUG ) arguments.push_back( L"/Zi" );
        if ( Flags1 & D3DCOMPILE_PACK_MATRIX_ROW_MAJOR ) arguments.push_back( L"/Zpr" );
        if ( Flags1 & D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR ) arguments.push_back( L"/Zpc" );
        if ( Flags1 & D3DCOMPILE_AVOID_FLOW_CONTROL ) arguments.push_back( L"/Gfa" );
        if ( Flags1 & D3DCOMPILE_PREFER_FLOW_CONTROL ) arguments.push_back( L"/Gfp" );

        // We don't implement this:
        //if(Flags1 & D3DCOMPILE_PARTIAL_PRECISION) arguments.push_back(L"/Gpp");
        if ( Flags1 & D3DCOMPILE_RESOURCES_MAY_ALIAS ) arguments.push_back( L"/res_may_alias" );

        if ( Flags1 & D3DCOMPILE_SKIP_VALIDATION ) arguments.push_back( L"-Vd" );

        if ( wcscmp( pTargetProfileW+3,L"6_2" ) == 0 )
        {
            // if compiling to SM6_2, turn on 16-bit types
            //  (because Microsoft decided that shader model 2 should be "extra-optional")
            //  also need to disable validation because the validator apparently doesn't know about SM_6_2 yet,
            //   at least as of when I built it
            arguments.push_back( L"/enable-16bit-types" );
            arguments.push_back( L"-Vd" );
        }
        
        if ( FAILED( pCompiler->Compile( pSource,pSourceName,pEntrypointW,pTargetProfileW,
            arguments.data(),(UINT)arguments.size(),
            defines.data(),(UINT)defines.size(),pInclude,
            &operationResult ) ) )
        {
            return E_FAIL;
        }
    }
    catch ( const std::bad_alloc & ) {
        return E_OUTOFMEMORY;
    }
    catch ( const CAtlException &err ) {
        return err.m_hr;
    }

    operationResult->GetStatus( &hr );
    if ( SUCCEEDED( hr ) ) {
        return operationResult->GetResult( (IDxcBlob **)ppCode );
    }
    else {
        if ( ppErrorMsgs )
            operationResult->GetErrorBuffer( (IDxcBlobEncoding **)ppErrorMsgs );
        return hr;
    }
}
#pragma managed

bool DXILCompiler_Impl::Compile( System::String^ Shader,
    Pyramid::IHLSLOptions^ opts,
    System::String^ fileName,
    Pyramid::IDXILShaderBlob^% IR,
    System::String^% Messages )
{
    System::String^ targetString = opts->Target.ToString();

    // DXIL path
    if ( !m_pDXCLibrary )
        return false;

    IDxcBlob* pCode = nullptr;
    IDxcBlobEncoding* pMessages = nullptr;

    DWORD flags = opts->GetD3DCompileFlagBits();
    MarshalledString hlsl( Shader );
    MarshalledStringW entry( opts->EntryPoint );
    MarshalledStringW profile( targetString );
    MarshalledStringW file( fileName );

    IDxcBlobEncoding* pBlobEncoding = nullptr;
    HRESULT hr = m_pDXCLibrary->CreateBlobWithEncodingFromPinned( (LPBYTE) hlsl.GetString(), hlsl.Length()+1, CP_UTF8, &pBlobEncoding );
    if ( !SUCCEEDED( hr ) )
        return false;

    PyramidDxilIncludeHandler kHandler( m_pmInclude,m_pDXCLibrary );
    hr = CompileFromBlob( m_pDXCCompiler,
                            pBlobEncoding,
                            file.GetString(),
                            nullptr, &kHandler,
                            entry.GetString(), profile.GetString(), flags, &pCode,&pMessages );

    pBlobEncoding->Release();

    if ( SUCCEEDED( hr ) )
        IR = gcnew DXILShaderBlob_Impl( pCode,this );

    Messages = nullptr;
    if ( pMessages )
    {
        IDxcBlobEncoding* pMessagesUni = nullptr;
        HRESULT hr2 = m_pDXCLibrary->GetBlobAsUtf16( pMessages,&pMessagesUni );
        if ( SUCCEEDED( hr2 ) )
        {
            Messages = Marshal::PtrToStringUni( System::IntPtr( pMessagesUni->GetBufferPointer() ) );
            Messages = Messages->Replace( gcnew System::String( "\n" ),System::Environment::NewLine );
            pMessagesUni->Release();
        }
        pMessages->Release();
    }

    return SUCCEEDED( hr );
}


bool DXILCompiler_Impl::CompileRootSignature( System::String^ Shader,
                                                Pyramid::IHLSLOptions^ opts,
                                                System::String^ fileName,
                                                Pyramid::IDXBlob^% IR,
                                                System::String^% Messages )
{
    
    // DXIL path
    if ( !m_pDXCLibrary )
        return false;

    IDxcBlob* pCode = nullptr;
    IDxcBlobEncoding* pMessages = nullptr;

    DWORD flags = opts->GetD3DCompileFlagBits();
    MarshalledString hlsl( Shader->Replace( System::Environment::NewLine,"\n" ) );
    MarshalledStringW entry( opts->RootSigMacro );
    MarshalledStringW profile( opts->RootSigTarget.ToString() );
    MarshalledStringW file( fileName );

    IDxcBlobEncoding* pBlobEncoding = nullptr;
    HRESULT hr = m_pDXCLibrary->CreateBlobWithEncodingFromPinned( (LPBYTE)hlsl.GetString(),hlsl.Length()+1,CP_UTF8,&pBlobEncoding );
    if ( !SUCCEEDED( hr ) )
        return false;

    PyramidDxilIncludeHandler kHandler( m_pmInclude,m_pDXCLibrary );
    hr = CompileFromBlob( m_pDXCCompiler,
        pBlobEncoding,
        file.GetString(),
        nullptr,&kHandler,
        entry.GetString(),profile.GetString(),flags,&pCode,&pMessages );

    pBlobEncoding->Release();

    if ( SUCCEEDED( hr ) )
        IR = gcnew DXILRootSignatureBlob_Impl( pCode,this );

    Messages = nullptr;
    if ( pMessages )
    {
        IDxcBlobEncoding* pMessagesUni = nullptr;
        HRESULT hr2 = m_pDXCLibrary->GetBlobAsUtf16( pMessages,&pMessagesUni );
        if ( SUCCEEDED( hr2 ) )
        {
            Messages = Marshal::PtrToStringUni( System::IntPtr( pMessagesUni->GetBufferPointer() ) );
            Messages = Messages->Replace( gcnew System::String( "\n" ),System::Environment::NewLine );
            pMessagesUni->Release();
        }
        pMessages->Release();
    }

    return SUCCEEDED( hr );
}