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

        public IResultSet Compile(string text, ICompileOptions opts)
        {
            if (opts.Language != Languages.HLSL)
                return null;

            IHLSLOptions hlslOpts = (IHLSLOptions)opts;
            
            string error;
            IDXShaderBlob blob;
            m_Compiler.Compile(text, hlslOpts, out blob, out error);
            
            return new FXCResultSet(error, blob);
        }
    }

   
}
