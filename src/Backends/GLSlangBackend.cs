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
            private TextBox m_Results = new TextBox();

            public string Name { get { return "GLSlang"; } }
            public Control AnalysisPanel { get { return null; } }
            public Control ResultsPanel { get { return m_Results;  } }
            public GLSLangResultSet(GLSlang.IShader shader)
            {
                m_Results.ReadOnly = true;
                m_Results.Dock = DockStyle.Fill;
                m_Results.Multiline = true;
                m_Results.ScrollBars = ScrollBars.Both;
                m_Results.WordWrap = false;

                SPIRV.IProgram spirv = shader.CompileSPIRV();
                if (spirv != null)
                    m_Results.Text = spirv.Disassemble();
                else
                    m_Results.Text = shader.InfoLog.Replace("\n", Environment.NewLine);
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

                // turn HLSL shader profile into GLSL shader type
                GLSLShaderType eShaderType;
                string profile = hlslOpts.Target.ToString();
                if (profile.StartsWith("vs"))
                    eShaderType = GLSLShaderType.VERTEX;
                else if (profile.StartsWith("ps"))
                    eShaderType = GLSLShaderType.FRAGMENT;
                else if (profile.StartsWith("gs"))
                    eShaderType = GLSLShaderType.GEOMETRY;
                else if (profile.StartsWith("hs"))
                    eShaderType = GLSLShaderType.TESS_CONTROL;
                else if (profile.StartsWith("ds"))
                    eShaderType = GLSLShaderType.TESS_EVALUATION;
                else if (profile.StartsWith("cs"))
                    eShaderType = GLSLShaderType.COMPUTE;
                else
                    throw new System.Exception("Don't know what this shader profile is");

                string EntryPoint = hlslOpts.EntryPoint;
                
                GLSlangOptions slangOpts = new GLSlangOptions();
                slangOpts.ShaderType = eShaderType;
                slangOpts.Config = m_Config;
                GLSlang.IShader result = m_Compiler.CompileHLSL(sh.Code, slangOpts, EntryPoint);
                return new GLSLangResultSet(result);
            }
            else
            {
                return null;
            }
        }
    }
}
