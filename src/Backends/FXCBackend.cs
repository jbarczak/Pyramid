using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;

namespace Pyramid
{
    class FXCResultSet : IResultSet
    {
        private FXCResultsPanel m_Panel = null;

        public FXCResultSet(string error, IDXShaderBlob blob)
        {
            m_Panel = new FXCResultsPanel(error, blob);
        }

        public string Name { get { return "D3DCompiler"; } }
        public Control AnalysisPanel { get { return null; } }
        public Control ResultsPanel { get { return m_Panel; } }
    };

    class D3DCompilerBackend : IBackend
    {
        private ID3DCompiler m_Compiler;

        public string Name { get { return "D3DCompiler"; } }

        public D3DCompilerBackend( ID3DCompiler comp )
        {
            m_Compiler = comp;
        }

        public IResultSet Compile(IShader shader, IBackendOptions options)
        {
            if (!(shader is HLSLShader ) )
                return null;

            HLSLShader hlsl       = (HLSLShader)shader;
            IHLSLOptions hlslOpts = hlsl.CompileOptions;
            string text = hlsl.Code;

            if (!hlsl.WasCompiled)
                hlsl.Compile(m_Compiler);
            
            return new FXCResultSet(hlsl.Messages, hlsl.CompiledBlob);
        }
    }

   
}
