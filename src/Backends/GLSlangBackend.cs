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
                m_Results.Text = shader.InfoLog.Replace("\n", Environment.NewLine);
            }
        }
        
        public string Name { get { return "GLSlang"; } }

        public GLSlangBackend( IWrapper wrapper )
        {
            m_Compiler = wrapper.CreateGLSlangCompiler();
            m_Config = m_Compiler.CreateDefaultConfig();
        }

        public IResultSet Compile(string shader, ICompileOptions opts)
        {
            if (opts.Language != Languages.GLSL)
                return null;

            IGLSLOptions glOpts = (IGLSLOptions)opts;

            GLSlangOptions slangOpts = new GLSlangOptions();
            slangOpts.ShaderType = glOpts.ShaderType;
            slangOpts.Config = m_Config;
            GLSlang.IShader result = m_Compiler.Compile(shader, slangOpts);
            return new GLSLangResultSet(result);
        }
    }
}
