using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace Pyramid
{
    public partial class IntelShaderAnalyzerResultsPanel : UserControl
    {
        public IntelShaderAnalyzerResultsPanel( string stdout, List<string> asics )
        {
            InitializeComponent();
            txtMessages.Text = stdout;
            tabPage1.Visible = false;

            foreach( string asic in asics )
                cmbOutputFile.Items.Add(asic);

            cmbOutputFile.SelectedIndex = 0;
        }

        private Dictionary<string, Dictionary<string,string> > m_APISets = new Dictionary<string, Dictionary<string,string>>();
        
        public void AddResults( string api, List<string> files )
        {
            if (files.Count == 0)
                return;

            Dictionary<string, string> asicFiles = new Dictionary<string, string>();
            foreach( string path in files )
            {
                string asic = Path.GetFileNameWithoutExtension(path);
                if (File.Exists(path))
                {
                    string asm = File.ReadAllText(path);
                    asicFiles.Add(asic, asm);
                }
            }
            
            m_APISets.Add(api, asicFiles);
            cmbAPI.Items.Add(api);
            cmbAPI.SelectedIndex = 0;

            RefreshAsm();
        }

        private void RefreshAsm()
        {
            txtAsm.Text = "";
            if( cmbAPI.SelectedItem != null && cmbOutputFile.SelectedItem != null )
            {
                Dictionary<String, String> Files;
                if (m_APISets.TryGetValue((string)cmbAPI.SelectedItem, out Files))
                {
                    string text;
                    if (Files.TryGetValue((string)cmbOutputFile.SelectedItem, out text))
                    {
                        txtAsm.Text = text;
                    }
                }
            }
        }

        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        private void cmbAPI_SelectedIndexChanged(object sender, EventArgs e)
        {
            RefreshAsm();
        }

        private void cmbOutputFile_SelectedIndexChanged(object sender, EventArgs e)
        {
            RefreshAsm();
        }
    }
}
