using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Pyramid
{
    public partial class AMDDriverResultsPanel : UserControl
    {
        private List<IAMDShader> m_Shaders = new List<IAMDShader>();
        
        IDXShaderReflection m_DXReflection;

        public delegate void AsicChangedDelegate(IAMDShader shader);
        public event AsicChangedDelegate AsicChanged;
        
        public AMDDriverResultsPanel( IDXShaderReflection shader )
        {
            InitializeComponent();
            m_DXReflection = shader;
            btnScrutinize.Enabled =  (shader.GetShaderType() == HLSLShaderType.VERTEX ||
                                      shader.GetShaderType() == HLSLShaderType.PIXEL) ;
        }

        public void AddResult(IAMDShader shader)
        {
            m_Shaders.Add(shader);
            cmbAsic.Items.Add(shader.Asic.Name);
            if (cmbAsic.Items.Count == 1)
                cmbAsic.SelectedIndex = 0;
        }

        private string HexDump( IAMDShader sh )
        {
            byte[] bytes = sh.ReadISABytes();
            int nBytes = bytes.Length;
            StringBuilder str = new StringBuilder();

            if (nBytes % 4 == 0)
            {
                for (int i = 0; i < nBytes; i += 4)
                {
                    int n = bytes[i] |
                             bytes[i + 1] << 8 |
                             bytes[i + 2] << 16 |
                             bytes[i + 3] << 24;
                    str.AppendFormat("{0:X8} ", n);
                    str.AppendLine();
                }
            }
            else
            {
                for (int i = 0; i < nBytes; i++)
                {
                    if (i % 8 == 0)
                        str.AppendLine();
                    str.AppendFormat("{0:X2} ", bytes[i]);
                }
            }
            str.AppendLine();
            return str.ToString();
        }

        private void cmbAsic_SelectedIndexChanged(object sender, EventArgs e)
        {
            int i = cmbAsic.SelectedIndex;
            if (i >= 0 && i < m_Shaders.Count)
            {
                txtHex.Text = HexDump(m_Shaders[i]);
                txtEncodings.Text = m_Shaders[i].ListEncodings();
                txtISA.Text = m_Shaders[i].Disassemble();
            }
            else
            {
                txtHex.Text = "";
                txtISA.Text = "";
            }
            AsicChanged(m_Shaders[i]);
        }

        private void btnScrutinize_Click(object sender, EventArgs e)
        {
            int i = cmbAsic.SelectedIndex;
            if (i >= 0 && i < m_Shaders.Count)
            {
                IAMDShader sh = m_Shaders[cmbAsic.SelectedIndex];

                Scrutinizer.IScrutinizer backend     = sh.CreateScrutinizer();
                List<Scrutinizer.IInstruction> Ops   = backend.BuildProgram();
                List<Scrutinizer.IInstruction> Fetch = backend.BuildDXFetchShader(m_DXReflection);

                Scrutinizer.UI.ScrutinizerForm f = new Scrutinizer.UI.ScrutinizerForm(Fetch, Ops, backend);
                f.ShowDialog();
            }
          
        }

    }
}
