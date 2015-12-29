using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace Pyramid
{
    public partial class AMDResultsPanel : UserControl
    {
        private List<String> m_IL    = new List<string>();
        private List<String> m_ISA   = new List<string>();
        public AMDResultsPanel()
        {
            InitializeComponent();
        }

        public int ResultCount { get { return m_IL.Count;  } }

        public delegate void AsicChangedDelegate( string asic );
        public event AsicChangedDelegate AsicChanged;

        public void AddResult(string Asic, string IL, string ISA)
        {
            m_IL.Add(IL);
            m_ISA.Add(ISA);
            cmbAsic.Items.Add(Asic);
            if (cmbAsic.Items.Count == 1)
                cmbAsic.SelectedIndex = 0;
        }

        public void SetAsic(string name)
        {
            int index = cmbAsic.Items.IndexOf(name);

            if (index == -1)
            {
                txtIL.Text = "";
                txtISA.Text = "";
                return;
            }

            // invokes handler below indirectly
            cmbAsic.SelectedIndex = index;
        }

        private void cmbAsic_SelectedIndexChanged(object sender, EventArgs e)
        {
            txtIL.Text  = m_IL[cmbAsic.SelectedIndex];
            txtISA.Text = m_ISA[cmbAsic.SelectedIndex];
            AsicChanged((string)cmbAsic.SelectedItem);
        }
    }
}
