
#define _CRT_SECURE_NO_WARNINGS

#pragma unmanaged
#include "GLSlang/glslang/Public/ShaderLang.h"
#include "GLSlang/glslang/Include/ResourceLimits.h"
#include "GLSlang/SPIRV/GlslangToSpv.h"
#include "GLSlang/SPIRV/disassemble.h"
#include <sstream>
#pragma managed

#include "Utilities.h"
#include "GLSlang_Impl.h"

//
// These are the default resources for TBuiltInResources, used for both
//  - parsing this string for the case where the user didn't supply one
//  - dumping out a template for user construction of a config file
//
const char* DefaultConfig =
    "MaxLights 32\n"
    "MaxClipPlanes 6\n"
    "MaxTextureUnits 32\n"
    "MaxTextureCoords 32\n"
    "MaxVertexAttribs 64\n"
    "MaxVertexUniformComponents 4096\n"
    "MaxVaryingFloats 64\n"
    "MaxVertexTextureImageUnits 32\n"
    "MaxCombinedTextureImageUnits 80\n"
    "MaxTextureImageUnits 32\n"
    "MaxFragmentUniformComponents 4096\n"
    "MaxDrawBuffers 32\n"
    "MaxVertexUniformVectors 128\n"
    "MaxVaryingVectors 8\n"
    "MaxFragmentUniformVectors 16\n"
    "MaxVertexOutputVectors 16\n"
    "MaxFragmentInputVectors 15\n"
    "MinProgramTexelOffset -8\n"
    "MaxProgramTexelOffset 7\n"
    "MaxClipDistances 8\n"
    "MaxComputeWorkGroupCountX 65535\n"
    "MaxComputeWorkGroupCountY 65535\n"
    "MaxComputeWorkGroupCountZ 65535\n"
    "MaxComputeWorkGroupSizeX 1024\n"
    "MaxComputeWorkGroupSizeX 1024\n"
    "MaxComputeWorkGroupSizeZ 64\n"
    "MaxComputeUniformComponents 1024\n"
    "MaxComputeTextureImageUnits 16\n"
    "MaxComputeImageUniforms 8\n"
    "MaxComputeAtomicCounters 8\n"
    "MaxComputeAtomicCounterBuffers 1\n"
    "MaxVaryingComponents 60\n" 
    "MaxVertexOutputComponents 64\n"
    "MaxGeometryInputComponents 64\n"
    "MaxGeometryOutputComponents 128\n"
    "MaxFragmentInputComponents 128\n"
    "MaxImageUnits 8\n"
    "MaxCombinedImageUnitsAndFragmentOutputs 8\n"
    "MaxImageSamples 0\n"
    "MaxVertexImageUniforms 0\n"
    "MaxTessControlImageUniforms 0\n"
    "MaxTessEvaluationImageUniforms 0\n"
    "MaxGeometryImageUniforms 0\n"
    "MaxFragmentImageUniforms 8\n"
    "MaxCombinedImageUniforms 8\n"
    "MaxGeometryTextureImageUnits 16\n"
    "MaxGeometryOutputVertices 256\n"
    "MaxGeometryTotalOutputComponents 1024\n"
    "MaxGeometryUniformComponents 1024\n"
    "MaxGeometryVaryingComponents 64\n"
    "MaxTessControlInputComponents 128\n"
    "MaxTessControlOutputComponents 128\n"
    "MaxTessControlTextureImageUnits 16\n"
    "MaxTessControlUniformComponents 1024\n"
    "MaxTessControlTotalOutputComponents 4096\n"
    "MaxTessEvaluationInputComponents 128\n"
    "MaxTessEvaluationOutputComponents 128\n"
    "MaxTessEvaluationTextureImageUnits 16\n"
    "MaxTessEvaluationUniformComponents 1024\n"
    "MaxTessPatchComponents 120\n"
    "MaxPatchVertices 32\n"
    "MaxTessGenLevel 64\n"
    "MaxViewports 16\n"
    "MaxVertexAtomicCounters 0\n"
    "MaxTessControlAtomicCounters 0\n"
    "MaxTessEvaluationAtomicCounters 0\n"
    "MaxGeometryAtomicCounters 0\n"
    "MaxFragmentAtomicCounters 8\n"
    "MaxCombinedAtomicCounters 8\n"
    "MaxAtomicCounterBindings 1\n"
    "MaxVertexAtomicCounterBuffers 0\n"
    "MaxTessControlAtomicCounterBuffers 0\n"
    "MaxTessEvaluationAtomicCounterBuffers 0\n"
    "MaxGeometryAtomicCounterBuffers 0\n"
    "MaxFragmentAtomicCounterBuffers 1\n"
    "MaxCombinedAtomicCounterBuffers 1\n"
    "MaxAtomicCounterBufferSize 16384\n"
    "MaxTransformFeedbackBuffers 4\n"
    "MaxTransformFeedbackInterleavedComponents 64\n"
    "nonInductiveForLoops 1\n"
    "whileLoops 1\n"
    "doWhileLoops 1\n"
    "generalUniformIndexing 1\n"
    "generalAttributeMatrixVectorIndexing 1\n"
    "generalVaryingIndexing 1\n"
    "generalSamplerIndexing 1\n"
    "generalVariableIndexing 1\n"
    "generalConstantMatrixVectorIndexing 1\n"
    ;



//
// Parse either a .conf file provided by the user or the default string above.
//
void ProcessConfigFile( TBuiltInResource& Resources,  char* config )
{    
    const char* delims = " \t\n\r";
    const char* token = strtok(config, delims);
    while (token) {
        const char* valueStr = strtok(0, delims);
        if (valueStr == 0 || ! (valueStr[0] == '-' || (valueStr[0] >= '0' && valueStr[0] <= '9'))) {
            printf("Error: '%s' bad .conf file.  Each name must be followed by one number.\n", valueStr ? valueStr : "");
            return;
        }
        int value = atoi(valueStr);

        if (strcmp(token, "MaxLights") == 0)
            Resources.maxLights = value;
        else if (strcmp(token, "MaxClipPlanes") == 0)
            Resources.maxClipPlanes = value;
        else if (strcmp(token, "MaxTextureUnits") == 0)
            Resources.maxTextureUnits = value;
        else if (strcmp(token, "MaxTextureCoords") == 0)
            Resources.maxTextureCoords = value;
        else if (strcmp(token, "MaxVertexAttribs") == 0)
            Resources.maxVertexAttribs = value;
        else if (strcmp(token, "MaxVertexUniformComponents") == 0)
            Resources.maxVertexUniformComponents = value;
        else if (strcmp(token, "MaxVaryingFloats") == 0)
            Resources.maxVaryingFloats = value;
        else if (strcmp(token, "MaxVertexTextureImageUnits") == 0)
            Resources.maxVertexTextureImageUnits = value;
        else if (strcmp(token, "MaxCombinedTextureImageUnits") == 0)
            Resources.maxCombinedTextureImageUnits = value;
        else if (strcmp(token, "MaxTextureImageUnits") == 0)
            Resources.maxTextureImageUnits = value;
        else if (strcmp(token, "MaxFragmentUniformComponents") == 0)
            Resources.maxFragmentUniformComponents = value;
        else if (strcmp(token, "MaxDrawBuffers") == 0)
            Resources.maxDrawBuffers = value;
        else if (strcmp(token, "MaxVertexUniformVectors") == 0)
            Resources.maxVertexUniformVectors = value;
        else if (strcmp(token, "MaxVaryingVectors") == 0)
            Resources.maxVaryingVectors = value;
        else if (strcmp(token, "MaxFragmentUniformVectors") == 0)
            Resources.maxFragmentUniformVectors = value;
        else if (strcmp(token, "MaxVertexOutputVectors") == 0)
            Resources.maxVertexOutputVectors = value;
        else if (strcmp(token, "MaxFragmentInputVectors") == 0)
            Resources.maxFragmentInputVectors = value;
        else if (strcmp(token, "MinProgramTexelOffset") == 0)
            Resources.minProgramTexelOffset = value;
        else if (strcmp(token, "MaxProgramTexelOffset") == 0)
            Resources.maxProgramTexelOffset = value;
        else if (strcmp(token, "MaxClipDistances") == 0)
            Resources.maxClipDistances = value;
        else if (strcmp(token, "MaxComputeWorkGroupCountX") == 0)
            Resources.maxComputeWorkGroupCountX = value;
        else if (strcmp(token, "MaxComputeWorkGroupCountY") == 0)
            Resources.maxComputeWorkGroupCountY = value;
        else if (strcmp(token, "MaxComputeWorkGroupCountZ") == 0)
            Resources.maxComputeWorkGroupCountZ = value;
        else if (strcmp(token, "MaxComputeWorkGroupSizeX") == 0)
            Resources.maxComputeWorkGroupSizeX = value;
        else if (strcmp(token, "MaxComputeWorkGroupSizeY") == 0)
            Resources.maxComputeWorkGroupSizeY = value;
        else if (strcmp(token, "MaxComputeWorkGroupSizeZ") == 0)
            Resources.maxComputeWorkGroupSizeZ = value;
        else if (strcmp(token, "MaxComputeUniformComponents") == 0)
            Resources.maxComputeUniformComponents = value;
        else if (strcmp(token, "MaxComputeTextureImageUnits") == 0)
            Resources.maxComputeTextureImageUnits = value;
        else if (strcmp(token, "MaxComputeImageUniforms") == 0)
            Resources.maxComputeImageUniforms = value;
        else if (strcmp(token, "MaxComputeAtomicCounters") == 0)
            Resources.maxComputeAtomicCounters = value;
        else if (strcmp(token, "MaxComputeAtomicCounterBuffers") == 0)
            Resources.maxComputeAtomicCounterBuffers = value;
        else if (strcmp(token, "MaxVaryingComponents") == 0)
            Resources.maxVaryingComponents = value;
        else if (strcmp(token, "MaxVertexOutputComponents") == 0)
            Resources.maxVertexOutputComponents = value;
        else if (strcmp(token, "MaxGeometryInputComponents") == 0)
            Resources.maxGeometryInputComponents = value;
        else if (strcmp(token, "MaxGeometryOutputComponents") == 0)
            Resources.maxGeometryOutputComponents = value;
        else if (strcmp(token, "MaxFragmentInputComponents") == 0)
            Resources.maxFragmentInputComponents = value;
        else if (strcmp(token, "MaxImageUnits") == 0)
            Resources.maxImageUnits = value;
        else if (strcmp(token, "MaxCombinedImageUnitsAndFragmentOutputs") == 0)
            Resources.maxCombinedImageUnitsAndFragmentOutputs = value;
        else if (strcmp(token, "MaxImageSamples") == 0)
            Resources.maxImageSamples = value;
        else if (strcmp(token, "MaxVertexImageUniforms") == 0)
            Resources.maxVertexImageUniforms = value;
        else if (strcmp(token, "MaxTessControlImageUniforms") == 0)
            Resources.maxTessControlImageUniforms = value;
        else if (strcmp(token, "MaxTessEvaluationImageUniforms") == 0)
            Resources.maxTessEvaluationImageUniforms = value;
        else if (strcmp(token, "MaxGeometryImageUniforms") == 0)
            Resources.maxGeometryImageUniforms = value;
        else if (strcmp(token, "MaxFragmentImageUniforms") == 0)
            Resources.maxFragmentImageUniforms = value;
        else if (strcmp(token, "MaxCombinedImageUniforms") == 0)
            Resources.maxCombinedImageUniforms = value;
        else if (strcmp(token, "MaxGeometryTextureImageUnits") == 0)
            Resources.maxGeometryTextureImageUnits = value;
        else if (strcmp(token, "MaxGeometryOutputVertices") == 0)
            Resources.maxGeometryOutputVertices = value;
        else if (strcmp(token, "MaxGeometryTotalOutputComponents") == 0)
            Resources.maxGeometryTotalOutputComponents = value;
        else if (strcmp(token, "MaxGeometryUniformComponents") == 0)
            Resources.maxGeometryUniformComponents = value;
        else if (strcmp(token, "MaxGeometryVaryingComponents") == 0)
            Resources.maxGeometryVaryingComponents = value;
        else if (strcmp(token, "MaxTessControlInputComponents") == 0)
            Resources.maxTessControlInputComponents = value;
        else if (strcmp(token, "MaxTessControlOutputComponents") == 0)
            Resources.maxTessControlOutputComponents = value;
        else if (strcmp(token, "MaxTessControlTextureImageUnits") == 0)
            Resources.maxTessControlTextureImageUnits = value;
        else if (strcmp(token, "MaxTessControlUniformComponents") == 0)
            Resources.maxTessControlUniformComponents = value;
        else if (strcmp(token, "MaxTessControlTotalOutputComponents") == 0)
            Resources.maxTessControlTotalOutputComponents = value;
        else if (strcmp(token, "MaxTessEvaluationInputComponents") == 0)
            Resources.maxTessEvaluationInputComponents = value;
        else if (strcmp(token, "MaxTessEvaluationOutputComponents") == 0)
            Resources.maxTessEvaluationOutputComponents = value;
        else if (strcmp(token, "MaxTessEvaluationTextureImageUnits") == 0)
            Resources.maxTessEvaluationTextureImageUnits = value;
        else if (strcmp(token, "MaxTessEvaluationUniformComponents") == 0)
            Resources.maxTessEvaluationUniformComponents = value;
        else if (strcmp(token, "MaxTessPatchComponents") == 0)
            Resources.maxTessPatchComponents = value;
        else if (strcmp(token, "MaxPatchVertices") == 0)
            Resources.maxPatchVertices = value;
        else if (strcmp(token, "MaxTessGenLevel") == 0)
            Resources.maxTessGenLevel = value;
        else if (strcmp(token, "MaxViewports") == 0)
            Resources.maxViewports = value;
        else if (strcmp(token, "MaxVertexAtomicCounters") == 0)
            Resources.maxVertexAtomicCounters = value;
        else if (strcmp(token, "MaxTessControlAtomicCounters") == 0)
            Resources.maxTessControlAtomicCounters = value;
        else if (strcmp(token, "MaxTessEvaluationAtomicCounters") == 0)
            Resources.maxTessEvaluationAtomicCounters = value;
        else if (strcmp(token, "MaxGeometryAtomicCounters") == 0)
            Resources.maxGeometryAtomicCounters = value;
        else if (strcmp(token, "MaxFragmentAtomicCounters") == 0)
            Resources.maxFragmentAtomicCounters = value;
        else if (strcmp(token, "MaxCombinedAtomicCounters") == 0)
            Resources.maxCombinedAtomicCounters = value;
        else if (strcmp(token, "MaxAtomicCounterBindings") == 0)
            Resources.maxAtomicCounterBindings = value;
        else if (strcmp(token, "MaxVertexAtomicCounterBuffers") == 0)
            Resources.maxVertexAtomicCounterBuffers = value;
        else if (strcmp(token, "MaxTessControlAtomicCounterBuffers") == 0)
            Resources.maxTessControlAtomicCounterBuffers = value;
        else if (strcmp(token, "MaxTessEvaluationAtomicCounterBuffers") == 0)
            Resources.maxTessEvaluationAtomicCounterBuffers = value;
        else if (strcmp(token, "MaxGeometryAtomicCounterBuffers") == 0)
            Resources.maxGeometryAtomicCounterBuffers = value;
        else if (strcmp(token, "MaxFragmentAtomicCounterBuffers") == 0)
            Resources.maxFragmentAtomicCounterBuffers = value;
        else if (strcmp(token, "MaxCombinedAtomicCounterBuffers") == 0)
            Resources.maxCombinedAtomicCounterBuffers = value;
        else if (strcmp(token, "MaxAtomicCounterBufferSize") == 0)
            Resources.maxAtomicCounterBufferSize = value;
        else if (strcmp(token, "MaxTransformFeedbackBuffers") == 0)
            Resources.maxTransformFeedbackBuffers = value;
        else if (strcmp(token, "MaxTransformFeedbackInterleavedComponents") == 0)
            Resources.maxTransformFeedbackInterleavedComponents = value;

        else if (strcmp(token, "nonInductiveForLoops") == 0)
            Resources.limits.nonInductiveForLoops = (value != 0);
        else if (strcmp(token, "whileLoops") == 0)
            Resources.limits.whileLoops = (value != 0);
        else if (strcmp(token, "doWhileLoops") == 0)
            Resources.limits.doWhileLoops = (value != 0);
        else if (strcmp(token, "generalUniformIndexing") == 0)
            Resources.limits.generalUniformIndexing = (value != 0);
        else if (strcmp(token, "generalAttributeMatrixVectorIndexing") == 0)
            Resources.limits.generalAttributeMatrixVectorIndexing = (value != 0);
        else if (strcmp(token, "generalVaryingIndexing") == 0)
            Resources.limits.generalVaryingIndexing = (value != 0);
        else if (strcmp(token, "generalSamplerIndexing") == 0)
            Resources.limits.generalSamplerIndexing = (value != 0);
        else if (strcmp(token, "generalVariableIndexing") == 0)
            Resources.limits.generalVariableIndexing = (value != 0);
        else if (strcmp(token, "generalConstantMatrixVectorIndexing") == 0)
            Resources.limits.generalConstantMatrixVectorIndexing = (value != 0);
        else
            printf("Warning: unrecognized limit (%s) in configuration file.\n", token);

        token = strtok(0, delims);
    }
}


class PyramidIncluder : public glslang::TShader::Includer
{
public:

    PyramidIncluder( Pyramid::IIncludeHandler^ pmIncluder )
        : m_pmInclude(pmIncluder)
    {
    }

    glslang::TShader::Includer::IncludeResult* Do( Pyramid::IncludeType eType, const char* headerName, const char* includerPath )
    {
        Pyramid::IIncludeHandler^ pmInclude = m_pmInclude;
        if( pmInclude == nullptr )
            return nullptr;

        Pyramid::IIncludeResult^ pmResult = m_pmInclude->OpenInclude( eType, MakeString(headerName), MakeString(includerPath) );
        if( pmResult == nullptr )
            return nullptr;

        array<unsigned char>^ bytes = pmResult->Contents;
        MarshalledString str( pmResult->FullPath );

        size_t nLength = bytes->Length;
        void* pData = malloc( nLength );
        Marshal::Copy( bytes,0,  System::IntPtr(pData), bytes->Length );
        return new glslang::TShader::Includer::IncludeResult( std::string((const char*)str), (const char*)pData, nLength, nullptr);
    }

    glslang::TShader::Includer::IncludeResult* DepthLimit()
    {
        static const char* DEPTH_LIMIT = "Include depth limit reached";
        size_t len = strlen(DEPTH_LIMIT);
        char* pAlloc = (char*)malloc(len+1);
        strcpy(pAlloc,DEPTH_LIMIT);
        glslang::TShader::Includer::IncludeResult* pResult;
        return new glslang::TShader::Includer::IncludeResult( "", pAlloc, len, nullptr);
    }




    // For the "system" or <>-style includes; search the "system" paths.
    virtual IncludeResult* includeSystem(const char* headerName,
                                            const char* includerName,
                                            size_t inclusionDepth) 
    { 
        if( inclusionDepth > 1024 )
            return DepthLimit();
        return Do( Pyramid::IncludeType::System, headerName, includerName );
    }

    // For the "local"-only aspect of a "" include. Should not search in the
    // "system" paths, because on returning a failure, the parser will
    // call includeSystem() to look in the "system" locations.
    virtual IncludeResult* includeLocal(const char* headerName,
                                        const char* includerName,
                                        size_t inclusionDepth) 
    { 
        if( inclusionDepth > 1024 )
            return DepthLimit();
        return Do( Pyramid::IncludeType::System, headerName, includerName );
    }

    // Signals that the parser will no longer use the contents of the
    // specified IncludeResult.
    virtual void releaseInclude(IncludeResult* result) 
    {
        if( !result )
            return;

        if( result->headerData )
            free((void*) result->headerData);
        delete result;
    }

private:

    gcroot<Pyramid::IIncludeHandler^> m_pmInclude;
};


namespace Pyramid{
namespace GLSlang{

    // The authors of glslang have not seen fit to make
    //  the TIntermediate of a TShader directly accessible
    //  But we need it if we're going to generate SPIRV from it
    //  Lucky for us, they made it public
    #pragma unmanaged
    class StubShader : public glslang::TShader
    {
    public:
        StubShader( EShLanguage eType ) : TShader(eType){}
        glslang::TIntermediate* GetIntermediate() { return this->intermediate; }
    };

    #pragma managed

    ref class ConfigImpl : IConfig
    {
    public:
        ConfigImpl( const char* pText )
        {
            m_Config = new TBuiltInResource(); // can't use unmanaged struct type in managed class.  Doh
            char* pCopy = new char[strlen(pText)+1];
            strcpy(pCopy,pText);
            ProcessConfigFile( *m_Config, pCopy );
            delete[] pCopy;
        }
        ~ConfigImpl()
        {
            delete m_Config;
        }

        TBuiltInResource* m_Config;
    };



    struct SPIRVBlob_Unmanaged
    {
        std::vector<unsigned int> m_SPIRV;
    };

    ref class SPIRVBlob : Pyramid::SPIRV::IProgram
    {
    public:

        SPIRVBlob( SPIRVBlob_Unmanaged* pBlob ) : m_pBlob(pBlob)
        {
        }
        ~SPIRVBlob()
        {
            delete m_pBlob;
        }
        
        virtual System::String^ Disassemble()
        {
            std::stringstream stream;
            spv::Disassemble( stream, m_pBlob->m_SPIRV );

            System::String^ str = MakeString( stream.str().c_str() );
            return str->Replace( "\n", System::Environment::NewLine );
        }

        virtual array<unsigned int>^ GetTokens()
        {
            array<unsigned int>^ pmArray = gcnew array<unsigned int>(m_pBlob->m_SPIRV.size());
            for( size_t i=0; i<m_pBlob->m_SPIRV.size(); i++ )
                pmArray[i] = m_pBlob->m_SPIRV[i];
            return pmArray;
        }
        
        virtual array<unsigned char>^ GetBytes()
        {
            size_t nSize = m_pBlob->m_SPIRV.size()*sizeof(unsigned int);
            array<unsigned char>^ pmArray = gcnew array<unsigned char>(nSize);
            Marshal::Copy( System::IntPtr((void*)m_pBlob->m_SPIRV.data()), pmArray, 0, nSize );
            return pmArray;
        }

        SPIRVBlob_Unmanaged* m_pBlob; 
    };

    ref class ShaderImpl : IShader
    {
    public:
        ShaderImpl( StubShader* pShader, System::String^ Info, System::String^ InfoDebug, GLSLShaderType eType)
            : m_pShader(pShader),
              m_InfoLog( Info ),
              m_InfoDebugLog( InfoDebug ),
              m_eType(eType)
        {
        }
        ~ShaderImpl()
        {
            if(m_pShader)
                delete m_pShader;
            m_pShader=nullptr;
        }
        virtual property GLSLShaderType ShaderType { GLSLShaderType get() { return m_eType; } }
        virtual property bool HasErrors { bool get() { return m_pShader == nullptr; } };
        virtual property System::String^ InfoLog { System::String^ get() { return m_InfoLog; } }
        virtual property System::String^ InfoDebugLog { System::String^ get() { return m_InfoDebugLog; } }

        virtual Pyramid::SPIRV::IProgram^ CompileSPIRV()
        {
            if( !m_pShader )
                return nullptr;

            SPIRVBlob_Unmanaged* pBlob = new SPIRVBlob_Unmanaged();
            glslang::GlslangToSpv( *m_pShader->GetIntermediate(), pBlob->m_SPIRV );

            return gcnew SPIRVBlob(pBlob);
        }

    private:
        System::String^ m_InfoLog;
        System::String^ m_InfoDebugLog;
        StubShader* m_pShader;
        GLSLShaderType m_eType;
    };


    static volatile long m_nLock=0;
    static long m_nInstanceCount=0;

    Compiler_Impl::Compiler_Impl( Pyramid::IIncludeHandler^ pmIncluder )
    {
        while( _InterlockedExchange( &m_nLock, 1 ) != 0 )
            ;

        long n = ++m_nInstanceCount;
        if( n == 1 )
            glslang::InitializeProcess();

        m_nLock = 0;
        m_pmIncluder = pmIncluder;
    }
    Compiler_Impl::~Compiler_Impl()
    {
        while( _InterlockedExchange( &m_nLock, 1 ) != 0 )
            ;

        long n = --m_nInstanceCount;
        if( n == 0 )
            glslang::FinalizeProcess();

        m_nLock=0;
    }


    IConfig^ Compiler_Impl::CreateConfig(System::String^ text)
    {
        MarshalledString str(text);
        return gcnew ConfigImpl(str);
    }
     
    IConfig^ Compiler_Impl::CreateDefaultConfig()
    {
        return gcnew ConfigImpl(DefaultConfig);
    }

    IShader^ Compiler_Impl::Compile( System::String^ text, GLSLShaderType eManagedShaderType, IConfig^ config, System::String^ path )
    {
        EShLanguage eShaderType; 
        switch( eManagedShaderType )
        {
        case GLSLShaderType::VERTEX:            eShaderType = EShLangVertex; break;
        case GLSLShaderType::FRAGMENT:          eShaderType = EShLangFragment; break;
        case GLSLShaderType::GEOMETRY:          eShaderType = EShLangGeometry; break;
        case GLSLShaderType::TESS_CONTROL:      eShaderType = EShLangTessControl; break;
        case GLSLShaderType::TESS_EVALUATION:   eShaderType = EShLangTessEvaluation; break;
        case GLSLShaderType::COMPUTE:           eShaderType = EShLangCompute; break;
        default: return nullptr;
        }

        ConfigImpl^ cfg = (ConfigImpl^)config;

        MarshalledString marshalledText(text);
        const char* p = marshalledText.GetString();
        int len = strlen(p);
        
        MarshalledString marshalledPath(path);
        const char* pnames = marshalledPath.GetString();

        StubShader* shader = new StubShader(eShaderType);
        shader->setStringsWithLengthsAndNames( &p, &len, &pnames, 1 );


        PyramidIncluder includer(m_pmIncluder);

        bool bResult = shader->parse( cfg->m_Config, 100, false, EShMsgDefault, includer );

        System::String^ rInfoLog = MakeString( shader->getInfoLog() );
        System::String^ rInfoDebugLog = MakeString( shader->getInfoDebugLog() );
        if( !bResult )
        {
            delete shader;
            shader = nullptr;
        }

        return gcnew ShaderImpl(shader, rInfoLog, rInfoDebugLog, eManagedShaderType );
    }

    IShader^ Compiler_Impl::CompileHLSL( System::String^ text, IHLSLOptions^ opts, IConfig^ config, System::String^ path )
    {
        System::String^ entrypoint = opts->EntryPoint;

        GLSLShaderType eManagedType;
        
        System::String^ profile = opts->Target.ToString();
        if (profile->StartsWith("vs"))
            eManagedType = GLSLShaderType::VERTEX;
        else if (profile->StartsWith("ps"))
            eManagedType = GLSLShaderType::FRAGMENT;
        else if (profile->StartsWith("gs"))
            eManagedType = GLSLShaderType::GEOMETRY;
        else if (profile->StartsWith("hs"))
            eManagedType = GLSLShaderType::TESS_CONTROL;
        else if (profile->StartsWith("ds"))
            eManagedType = GLSLShaderType::TESS_EVALUATION;
        else if (profile->StartsWith("cs"))
            eManagedType = GLSLShaderType::COMPUTE;
        else
            return nullptr;
            

        EShLanguage eShaderType; 
        switch( eManagedType )
        {
        case GLSLShaderType::VERTEX:            eShaderType = EShLangVertex; break;
        case GLSLShaderType::FRAGMENT:          eShaderType = EShLangFragment; break;
        case GLSLShaderType::GEOMETRY:          eShaderType = EShLangGeometry; break;
        case GLSLShaderType::TESS_CONTROL:      eShaderType = EShLangTessControl; break;
        case GLSLShaderType::TESS_EVALUATION:   eShaderType = EShLangTessEvaluation; break;
        case GLSLShaderType::COMPUTE:           eShaderType = EShLangCompute; break;
        default: return nullptr;
        }

        ConfigImpl^ cfg = (ConfigImpl^)config;

        MarshalledString marshalledText(text);
        MarshalledString marshalledEntryPoint(entrypoint);
        const char* p = marshalledText.GetString();
        int len = strlen(p);

        MarshalledString marshalledPath(path);
        const char* pPath = marshalledPath.GetString();
       
        StubShader* shader = new StubShader(eShaderType);
        shader->setStringsWithLengthsAndNames( &p, &len, &pPath, 1 );
        shader->setEntryPoint( marshalledEntryPoint.GetString() );
        shader->setHlslIoMapping(true);
        shader->setAutoMapBindings(true);

        PyramidIncluder includer(m_pmIncluder);

        bool bResult = shader->parse( cfg->m_Config, 100, false, (EShMessages)(EShMsgDefault|EShMsgReadHlsl|EShMsgVulkanRules|EShMsgSpvRules), includer );

        System::String^ rInfoLog = MakeString( shader->getInfoLog() );
        System::String^ rInfoDebugLog = MakeString( shader->getInfoDebugLog() );
        if( !bResult )
        {
            delete shader;
            shader = nullptr;
        }

        return gcnew ShaderImpl(shader, rInfoLog, rInfoDebugLog, eManagedType );
       
    }

}}
