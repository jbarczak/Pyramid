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
    }
}
