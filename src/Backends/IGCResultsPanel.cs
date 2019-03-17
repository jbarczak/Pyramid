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
    public partial class IGCResultsPanel : UserControl
    {
        private List<string> m_Files = new List<string>();

        public IGCResultsPanel()
        {
            InitializeComponent();
        }
        public void Add( string file, string text )
        {
            m_Files.Add(text);
            cmbOutputFile.Items.Add(file);
            if (cmbOutputFile.Items.Count == 1)
            {
                cmbOutputFile.SelectedIndex = cmbOutputFile.Items.Count - 1;
                txtOutput.Text = text;
            }
        }

        private void cmbOutputFile_SelectedIndexChanged(object sender, EventArgs e)
        {
            int idx = cmbOutputFile.SelectedIndex;
            if( idx >= 0 && idx < m_Files.Count )
                txtOutput.Text = m_Files[idx];            
        }
    }
}
