using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Pyramid
{
    class AMDDriverResultSet : IResultSet
    {
        private AMDDriverResultsPanel m_Results;
        private TextBox m_Analysis = new TextBox();

        public string Name { get { return "AMDDXX"; } }
        public Control AnalysisPanel { get { return m_Analysis; } }
        public Control ResultsPanel { get { return m_Results; } }

        public AMDDriverResultSet( IDXShaderReflection reflection )
        {
            m_Results = new AMDDriverResultsPanel(reflection);
            m_Analysis.Dock      = DockStyle.Fill;
            m_Analysis.ReadOnly  = true;
            m_Analysis.Multiline = true;
            m_Analysis.Font = new System.Drawing.Font("Lucida Console", 8);

            m_Results.AsicChanged += delegate(IAMDShader sh)
            {
                m_Analysis.Text = sh.PrintStats();
            };
        }

        public void Add( IAMDShader sh )
        {
            m_Results.AddResult(sh);
        }
    }

    class AMDDriverBackendOptions : IBackendOptions
    {
        private List<string> m_RequestedAsics;

        public AMDDriverBackendOptions(List<string> requestedAsics)
        {
            m_RequestedAsics = (requestedAsics != null) ? requestedAsics : new List<string>();
        }

        public List<string> Asics { get { return m_RequestedAsics; } }
    }

    class AMDDriverBackend : IBackend
    {
        private List<string> m_SupportedAsics = new List<string>();
        private ID3DCompiler m_FXC = null;
        private IAMDDriver m_Driver = null;

        public string Name { get { return "AMDDXX"; } }
        public IEnumerable<string> Asics { get { return m_SupportedAsics; } }

        public AMDDriverBackend( IAMDDriver driver, ID3DCompiler fxc  )
        {
            m_FXC = fxc;
            m_Driver = driver;

            if (m_Driver != null)
            {
                foreach (IAMDAsic asic in m_Driver.Asics)
                {
                    m_SupportedAsics.Add(asic.Name);
                }
            }
        }

        private bool CompileForAsic(List<string> asics, string asic)
        {
            if (asics == null || asics.Count == 0)
                return true;

            return asics.Contains(asic);
        }

        public IResultSet Compile(IShader shaderObj, IBackendOptions options)
        {
            if ( !(shaderObj is HLSLShader ) )
                return null;

            HLSLShader shaderHLSL = shaderObj as HLSLShader;
            IHLSLOptions hlslOpts = shaderHLSL.CompileOptions;
            AMDDriverBackendOptions backendOptions = options as AMDDriverBackendOptions;
            string shader = shaderObj.Code;

            if (shaderHLSL.WasCompiledWithErrors)
                return null;

            try
            {
                // compile here if we must.  Recycle existing blob if we can
                IDXShaderBlob blob = shaderHLSL.CompiledBlob;
                if ( blob == null )
                {
                    if (!shaderHLSL.Compile(m_FXC, null))
                        return null;
                    blob = shaderHLSL.CompiledBlob;
                }
                if (!(blob is IDXBCShaderBlob))
                    return null;

                IDXBCShaderBlob dxcBlob = blob as IDXBCShaderBlob;

                IDXShaderReflection reflect = dxcBlob.Reflect();

                IDXShaderBlob exe = dxcBlob.GetExecutableBlob();
                if (exe == null)
                    return null;

                byte[] bytes = exe.ReadBytes();

                AMDDriverResultSet rs = new AMDDriverResultSet(reflect );

                foreach (IAMDAsic a in m_Driver.Asics)
                {
                    if (CompileForAsic(backendOptions.Asics, a.Name))
                    {
                        IAMDShader sh = m_Driver.CompileDXBlob(a, bytes, reflect);
                        rs.Add(sh);
                    }
                }

                return rs;
            }
            catch( System.Exception ex )
            {
                MessageBox.Show(ex.Message);
                return null;
            }
        }
    }
}
