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
    public partial class MaliSCResultsPanel : UserControl
    {
        private List<string> m_Cores = new List<string>();
        private List<string> m_Output = new List<string>();

        public MaliSCResultsPanel()
        {
            InitializeComponent();
        }

        public void AddResult(string core, string output)
        {
            m_Cores.Add(core);
            m_Output.Add(output);
            cmbCore.Items.Add(core);
            if (cmbCore.Items.Count == 1)
                cmbCore.SelectedIndex = 0;
        }

        private void cmbCore_SelectedIndexChanged(object sender, EventArgs e)
        {
            txtOutput.Text = m_Output[m_Cores.IndexOf(cmbCore.Text)];
        }
    }
}
