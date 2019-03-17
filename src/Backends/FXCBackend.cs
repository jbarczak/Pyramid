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

        public FXCResultSet(HLSLShader shader)
        {
            m_Panel = new FXCResultsPanel(shader);
        }

        public string Name { get { return "D3DCompiler"; } }
        public Control AnalysisPanel { get { return null; } }
        public Control ResultsPanel { get { return m_Panel; } }
    };

    class D3DCompilerBackend : IBackend
    {
        private ID3DCompiler m_Compiler;
        private IDXILCompiler m_DXIL;

        public string Name { get { return "D3DCompiler"; } }

        public D3DCompilerBackend( ID3DCompiler comp, IDXILCompiler dxil )
        {
            m_Compiler = comp;
            m_DXIL = dxil;
        }

        public IResultSet Compile(IShader shader, IBackendOptions options)
        {
            if (!(shader is HLSLShader ) )
                return null;

            HLSLShader hlsl       = (HLSLShader)shader;
            IHLSLOptions hlslOpts = hlsl.CompileOptions;
            string text = hlsl.Code;

            if (!hlsl.WasCompiled)
                hlsl.Compile(m_Compiler, m_DXIL);

            if (!hlsl.RootSigWasCompiled)
                hlsl.CompileRootSignature(m_DXIL);

            return new FXCResultSet(hlsl);
        }
    }

   
}
