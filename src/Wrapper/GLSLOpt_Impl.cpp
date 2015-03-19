
#pragma unmanaged
#include "glsl-optimizer/src/glsl/glsl_optimizer.h"
#pragma managed

#include "GLSLOpt_Impl.h"

namespace Pyramid{
namespace GLSLOptimizer{

    Shader_Impl::Shader_Impl( glslopt_shader* pShader, Pyramid::GLSLOptimizer::IOptimizer^ owner ) 
    : m_pShader(pShader),
      m_pLog(glslopt_get_log(pShader)),
      m_pOutput(glslopt_get_output(pShader)),
      m_pOutputRaw(glslopt_get_raw_output(pShader)),
      m_bError(!glslopt_get_status(pShader)),
      m_nFlow(0),
      m_nMath(0),
      m_nTexture(0),
      m_Owner(owner)
    {
        int nMath;
        int nTexture;
        int nFlow;
        m_Inputs = gcnew List<System::String^>();
        if( !m_bError )
        {
            glslopt_shader_get_stats( pShader, &nMath, &nTexture, &nFlow );
            m_nMath = nMath;
            m_nTexture = nTexture;
            m_nFlow = nFlow;

            int nInputs = glslopt_shader_get_input_count( m_pShader );
            for( int i=0; i<nInputs; i++ )
            {
                glslopt_basic_type baseType;
                glslopt_precision precision;
                int vecSize, arrSize, matSize, location;
                const char* pName;
                glslopt_shader_get_input_desc(m_pShader,i,&pName,&baseType, &precision, &vecSize, &matSize, &arrSize, &location );
                m_Inputs->Add( MakeString(pName) );
            }
        }
    }

    Shader_Impl::~Shader_Impl()
    {
        glslopt_shader_delete(m_pShader);
    }



    Optimizer_Impl::Optimizer_Impl( Pyramid::GLSLOptimizer::Target eTarget )
    {
        switch( eTarget )
        {
        case Target::OPENGL:     m_pContext = glslopt_initialize( kGlslTargetOpenGL ); break;
        case Target::OPENGL_ES2: m_pContext = glslopt_initialize( kGlslTargetOpenGLES20 ); break;
        case Target::OPENGL_ES3: m_pContext = glslopt_initialize( kGlslTargetOpenGLES30 ); break;
        }   
    }

    Optimizer_Impl::~Optimizer_Impl()
    {
        glslopt_cleanup(m_pContext);
    }

    IShader^ Optimizer_Impl::Optimize(System::String^ text, IOptions^ opts)
    {
        glslopt_shader_type eShaderType;
        switch( opts->ShaderType )
        {
        default: return nullptr;
        case ShaderType::VERTEX:   eShaderType = kGlslOptShaderVertex;   break;
        case ShaderType::FRAGMENT: eShaderType = kGlslOptShaderFragment; break;
        }

        MarshalledString marshalledText(text);
        glslopt_set_max_unroll_iterations( m_pContext, opts->MaxUnrollIterations );
        glslopt_shader* pShader = glslopt_optimize( m_pContext, eShaderType, marshalledText, 0 );
    
        return gcnew Shader_Impl(pShader,this);
    }

}}