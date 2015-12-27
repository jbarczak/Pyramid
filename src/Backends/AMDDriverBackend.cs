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
    };

    class AMDDriverBackend : IBackend
    {
        private ID3DCompiler m_FXC  = null;
        private IAMDDriver m_Driver = null;

        public string Name { get { return "AMDDXX"; } }

        public AMDDriverBackend( IAMDDriver driver, ID3DCompiler fxc  )
        {
            m_FXC = fxc;
            m_Driver = driver;
        }

        public IResultSet Compile( IShader shaderObj )
        {
            if ( !(shaderObj is HLSLShader ) )
                return null;

            HLSLShader shaderHLSL = shaderObj as HLSLShader;
            IHLSLOptions hlslOpts = shaderHLSL.CompileOptions;
            string shader = shaderObj.Code;

            if (shaderHLSL.WasCompiledWithErrors)
                return null;

            try
            {
                // compile here if we must.  Recycle existing blob if we can
                IDXShaderBlob blob = shaderHLSL.CompiledBlob;
                if ( blob == null )
                {
                    if (!shaderHLSL.Compile(m_FXC))
                        return null;
                    blob = shaderHLSL.CompiledBlob;
                }

                IDXShaderReflection reflect = blob.Reflect();

                IDXShaderBlob exe = blob.GetExecutableBlob();
                if (exe == null)
                    return null;

                byte[] bytes = exe.ReadBytes();

                AMDDriverResultSet rs = new AMDDriverResultSet(reflect );

                foreach (IAMDAsic a in m_Driver.Asics)
                {
                    IAMDShader sh = m_Driver.CompileDXBlob(a, bytes, reflect);
                    rs.Add(sh);
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
