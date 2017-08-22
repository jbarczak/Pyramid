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
    public partial class GLSlangResultsPanel : UserControl
    {
        private GLSlang.IShader m_Shader;
        private SPIRV.IProgram m_SPIRV;
        public GLSlangResultsPanel(GLSlang.IShader shader, SPIRV.IProgram spirv)
        {
            InitializeComponent();

            m_Shader = shader;
            m_SPIRV = spirv;
            if (spirv != null)
            {
                textBox1.Text = spirv.Disassemble();
                button1.Enabled = true;
            }
            else
            {
                textBox1.Text = shader.InfoLog.Replace("\n", Environment.NewLine);
                button1.Enabled = false;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            SaveFileDialog sd = new SaveFileDialog();
            sd.Title = "Export SPIRV Binary";
            if( sd.ShowDialog() != DialogResult.Cancel )
            {
                try
                {
                    System.IO.File.WriteAllBytes(sd.FileName, m_SPIRV.GetBytes());
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }

        }
    }
}
