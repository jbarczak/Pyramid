using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Pyramid
{
    public partial class OptionsScreen : Form
    {
        private Options m_InitialOpts;
        private string m_IncludeBrowseStart = "";

        public Options SelectedOptions { get; private set; }

        public OptionsScreen( Options opts, List<IBackend> backends )
        {
            InitializeComponent();
            m_InitialOpts       = opts;
            PopulateGUIFromOptions(opts);

            foreach (IBackend backend in backends)
            {
                int index = lstBackends.Items.Add(backend.Name);
                lstBackends.SetItemChecked(index,!opts.IsBackendDisabled(backend.Name));
            }

            // #mivance refactor
            IBackend amdBackend = backends.Find(backend => backend.Name == "AMDDXX");

            if (amdBackend is AMDDriverBackend)
            {
                AMDDriverBackend driver = amdBackend as AMDDriverBackend;

                foreach (string asic in driver.Asics)
                {
                    int index = lstAMDAsics.Items.Add(asic);
                    lstAMDAsics.SetItemChecked(index, !opts.IsAMDAsicDisabled(asic));
                }
            }

            IBackend codeXLBackend = backends.Find(backend => backend.Name == "CodeXL");

            if (codeXLBackend is CodeXLBackend)
            {
                CodeXLBackend driver = codeXLBackend as CodeXLBackend;
                foreach (string asic in driver.Asics)
                {
                    int index = lstCodeXLAsics.Items.Add(asic);
                    lstCodeXLAsics.SetItemChecked(index, !opts.IsCodeXLAsicDisabled(asic));
                }
            }

            if (System.IO.File.Exists(opts.RGAPath))
            {
                List<string> rgaAsics = RGABackend.GetAsicList(opts.RGAPath);
                foreach (string asic in rgaAsics)
                {
                    int index = lstRGAAsics.Items.Add(asic);
                    lstRGAAsics.SetItemChecked(index, !opts.IsRGAAsicDisabled(asic));
                }
            }

        }

        private void PopulateGUIFromOptions(Options opts)
        {
            txtD3DCompiler.Text = opts.D3DCompilerPath;
            txtCodeXL.Text = opts.CodeXLPath;
            txtTemp.Text = opts.TempPath;
            txtPowerVR.Text = opts.PowerVRCompilerPath;
            txtDXX.Text = opts.DXXDriverPath;
            txtMali.Text = opts.MaliSCRoot;
            txtMystery.Text = opts.MysteryToolPath;
            txtRGA.Text = opts.RGAPath;

            foreach (string s in opts.IncludePaths)
                lstIncludes.Items.Add(s);

            // start include browsing at last include directory
            if (lstIncludes.Items.Count > 0)
                m_IncludeBrowseStart = (string) lstIncludes.Items[lstIncludes.Items.Count - 1];
        }

        private string BrowseFile( string initial )
        {
            OpenFileDialog fd = new OpenFileDialog();
            fd.FileName = initial;
            if (fd.ShowDialog() != DialogResult.Cancel)
                return fd.FileName;
            else
                return initial;
        }

        private string BrowseFolder(string initial)
        {
            FolderBrowserDialog fd = new FolderBrowserDialog();
            if( !string.IsNullOrEmpty(initial) && System.IO.Directory.Exists(initial))
                fd.SelectedPath = initial;

            if (fd.ShowDialog() != DialogResult.Cancel)
                return fd.SelectedPath;
            else
                return initial;
        }

        private void btnD3DCompiler_Click(object sender, EventArgs e)
        {
            txtD3DCompiler.Text = BrowseFile(txtD3DCompiler.Text);
        }

        private void btnCodeXL_Click(object sender, EventArgs e)
        {
            txtCodeXL.Text = BrowseFile(txtCodeXL.Text);
        }

        private void btnTemp_Click(object sender, EventArgs e)
        {
            txtTemp.Text = BrowseFolder(txtTemp.Text);
        }

        private void OptionsScreen_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (this.DialogResult == DialogResult.Cancel)
            {
                SelectedOptions = m_InitialOpts;
            }
            else
            {
                SelectedOptions = new Options();
                SelectedOptions.CodeXLPath = txtCodeXL.Text;
                SelectedOptions.D3DCompilerPath = txtD3DCompiler.Text;
                SelectedOptions.TempPath = txtTemp.Text;
                SelectedOptions.PowerVRCompilerPath = txtPowerVR.Text;
                SelectedOptions.DXXDriverPath = txtDXX.Text;
                SelectedOptions.MaliSCRoot = txtMali.Text ;
                SelectedOptions.MysteryToolPath = txtMystery.Text;
                SelectedOptions.RGAPath = txtRGA.Text;

                for (int backendIndex = 0; backendIndex < lstBackends.Items.Count; backendIndex++)
                {
                    if (!lstBackends.GetItemChecked(backendIndex))
                        SelectedOptions.DisableBackend(lstBackends.Items[backendIndex].ToString());
                }

                for (int amdAsicIndex = 0; amdAsicIndex < lstAMDAsics.Items.Count; amdAsicIndex++)
                {
                    if (!lstAMDAsics.GetItemChecked(amdAsicIndex))
                    {
                        SelectedOptions.DisableAMDAsic(lstAMDAsics.Items[amdAsicIndex].ToString());
                    }
                }

                for (int codeXLAsicIndex = 0; codeXLAsicIndex < lstCodeXLAsics.Items.Count; codeXLAsicIndex++)
                {
                    if (!lstCodeXLAsics.GetItemChecked(codeXLAsicIndex))
                    {
                        SelectedOptions.DisableCodeXLAsic(lstCodeXLAsics.Items[codeXLAsicIndex].ToString());
                    }
                }

                for (int i = 0; i < lstRGAAsics.Items.Count; i++)
                {
                    if (!lstRGAAsics.GetItemChecked(i))
                    {
                        SelectedOptions.DisableRGAAsic(lstRGAAsics.Items[i].ToString());
                    }
                }

                foreach( string s in lstIncludes.Items )
                    SelectedOptions.AddInclude(s);
            }
        }

        private void btnPowerVR_Click(object sender, EventArgs e)
        {
            txtPowerVR.Text = BrowseFolder(txtPowerVR.Text);
        }

        private void btnDefaults_Click(object sender, EventArgs e)
        {
            Options opts = Options.GetDefaults();
            PopulateGUIFromOptions(opts);

            for (int i = 0; i < lstBackends.Items.Count; i++)
                lstBackends.SetItemChecked(i, true);
        }

        private void btnDXX_Click(object sender, EventArgs e)
        {
            txtDXX.Text = BrowseFile(txtDXX.Text);
        }

        private void btnMali_Click(object sender, EventArgs e)
        {
            txtMali.Text = BrowseFolder(txtMali.Text);
        }

        private void btnMystery_Click(object sender, EventArgs e)
        {
            txtMystery.Text = BrowseFile(txtMystery.Text);
        }

        private void btnRGA_Click(object sender, EventArgs e)
        {
            txtRGA.Text = BrowseFile(txtRGA.Text);
        }

        private void btnAddInclude_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog fd = new FolderBrowserDialog();
            if( System.IO.Directory.Exists(m_IncludeBrowseStart) )
                fd.SelectedPath = m_IncludeBrowseStart;

            if (fd.ShowDialog() != DialogResult.Cancel)
            {
                string path = fd.SelectedPath;
                if (!lstIncludes.Items.Contains(path))
                {
                    lstIncludes.Items.Add(path);
                    m_IncludeBrowseStart = path; // remember last place user browsed to
                }
            }
        }

        private void btnDeleteInclude_Click(object sender, EventArgs e)
        {
            if( lstIncludes.SelectedItem != null )
            {
                lstIncludes.Items.Remove(lstIncludes.SelectedItem);
            }
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            if (lstIncludes.SelectedItem != null)
            {
                // figure out which direction user wamts it moved in
                int direction = (int)numericUpDown1.Value;
                int oldPlace = lstIncludes.SelectedIndex;
                int newPlace = lstIncludes.SelectedIndex;
                if (direction == 1)
                {
                    // user clicked the up arrow, move it up
                    if( lstIncludes.SelectedIndex > 0 )
                        newPlace = oldPlace - 1;
                }
                else if (direction == -1)
                {
                    // user clicked the down arrow, move it down
                    if (lstIncludes.SelectedIndex < lstIncludes.Items.Count - 1)
                        newPlace = oldPlace + 1;                    
                }

                if( oldPlace != newPlace )
                {
                    string a = (string) lstIncludes.Items[oldPlace];
                    string b = (string) lstIncludes.Items[newPlace];
                    lstIncludes.Items[oldPlace] = b;
                    lstIncludes.Items[newPlace] = a;
                    lstIncludes.SelectedIndex = newPlace;
                }
            }

            // clear the updown for next time
            numericUpDown1.Value = 0;
        }
    }
}
