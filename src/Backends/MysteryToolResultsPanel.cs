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


    public partial class MysteryToolResultsPanel : UserControl
    {
        private List<string> m_Results = new List<string>();

        public MysteryToolResultsPanel( )
        {
            InitializeComponent();
        }
        public void Add(string sku,string result)
        {
            cmbSKU.Items.Add(sku);
            m_Results.Add(result);
            if (cmbSKU.Items.Count == 1)
                cmbSKU.SelectedIndex = 0;
        }

        private void cmbSKU_SelectedIndexChanged(object sender, EventArgs e)
        {
            if( cmbSKU.SelectedItem != null )
                txtOutput.Text = m_Results[cmbSKU.SelectedIndex];
        }
    }
}
