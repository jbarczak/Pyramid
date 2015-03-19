using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Windows.Forms;

namespace Pyramid
{
    public partial class PVRResultsPanel : UserControl
    {
        private List<string> m_ASM = new List<string>();
        private List<string> m_Output = new List<string>();

        public PVRResultsPanel()
        {
            InitializeComponent();
        }

        public void AddResult( string compiler, string output, string asm )
        {
            m_ASM.Add(asm);
            m_Output.Add( output );
            cmbCompiler.Items.Add(compiler);
            if( cmbCompiler.Items.Count == 1 )
                cmbCompiler.SelectedIndex = 0;
        }

        private void cmbCompiler_SelectedIndexChanged(object sender, EventArgs e)
        {
            txtASM.Text    = m_ASM[cmbCompiler.SelectedIndex];
            txtOutput.Text = m_Output[cmbCompiler.SelectedIndex];
        }
    }
}
