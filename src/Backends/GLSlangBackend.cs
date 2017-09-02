using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Pyramid
{
    class GLSlangBackend : IBackend
    {
        private GLSlang.ICompiler m_Compiler;
        private GLSlang.IConfig m_Config;

        private class GLSlangOptions : GLSlang.IOptions
        {
            public GLSLShaderType ShaderType { get; set; }
            public GLSlang.IConfig Config { get; set; }
        }

        private class GLSLangResultSet : IResultSet
        {
            private GLSlangResultsPanel m_Results = null;

            public string Name { get { return "GLSlang"; } }
            public Control AnalysisPanel { get { return null; } }
            public Control ResultsPanel { get { return m_Results;  } }
            public GLSLangResultSet(GLSlang.IShader shader)
            {
                m_Results = new GLSlangResultsPanel(shader, shader.CompileSPIRV());

            }
        }
        
        public string Name { get { return "GLSlang"; } }

        public GLSlangBackend( IWrapper wrapper )
        {
            m_Compiler = wrapper.CreateGLSlangCompiler();
            m_Config = m_Compiler.CreateDefaultConfig();
        }

        public IResultSet Compile(IShader shader, IBackendOptions options)
        {
            if (shader is GLSLShader)
            {
                GLSLShader sh = (GLSLShader)shader;
                IGLSLOptions glOpts = sh.CompileOptions;

                GLSlangOptions slangOpts = new GLSlangOptions();
                slangOpts.ShaderType = glOpts.ShaderType;
                slangOpts.Config = m_Config;
                GLSlang.IShader result = m_Compiler.Compile(sh.Code, slangOpts);
                return new GLSLangResultSet(result);
            }
            else if( shader is HLSLShader )
            {
                HLSLShader sh = (HLSLShader)shader;
                IHLSLOptions hlslOpts = sh.CompileOptions;
        
                GLSlang.IShader result = m_Compiler.CompileHLSL(sh.Code, hlslOpts, m_Config );
                return new GLSLangResultSet(result);
            }
            else
            {
                return null;
            }
        }
    }
}
