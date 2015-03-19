using System;
using System.IO;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Pyramid
{
    public class Options
    {
        private List<string> m_DisabledBackends = new List<string>();

        public string D3DCompilerPath { get; set; }
        public string CodeXLPath { get; set; }
        public string TempPath { get; set; }
        public string PowerVRCompilerPath { get; set; }
        public string DXXDriverPath { get; set; }
        public IEnumerable<string> DisabledBackends { get { return m_DisabledBackends; } }

        public void DisableBackend(string name)
        {
            m_DisabledBackends.Add(name);
        }
        public bool IsBackendDisabled(string name)
        {
            return m_DisabledBackends.Contains(name);
        }

        public static Options GetDefaults()
        {
            Options opts = new Options();
            opts.D3DCompilerPath     = "d3dcompiler_47.dll";
            opts.CodeXLPath          = "CodeXLAnalyzer.exe";
            opts.PowerVRCompilerPath = "PowerVR";
            opts.DXXDriverPath = "atidxx32.dll";
            opts.TempPath = Path.Combine(
                Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
                "Pyramid" );

            return opts;
        }

        public static Options Get()
        {
            string OptionsFile = Path.Combine(
                Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
                "Pyramid\\options" );

            Options defaults = GetDefaults();
            if (!System.IO.File.Exists(OptionsFile))
                return defaults;

            Options opts = new Options();
            try
            {
                string[] lines = File.ReadAllLines(OptionsFile);
                Dictionary<string,string> map = new Dictionary<string,string>();
                foreach( string s in lines )
                {
                    string[] keyval = s.Split('=');
                    string key = keyval[0];
                    string value = keyval[1];
                    map.Add(key, value);
                }

                
                string d3dCompiler;
                if (!map.TryGetValue("D3DCompiler", out d3dCompiler))
                    d3dCompiler = defaults.D3DCompilerPath;

                string codeXL;
                if (!map.TryGetValue("CodeXL", out codeXL))
                    codeXL = defaults.CodeXLPath;

                string temp;
                if (!map.TryGetValue("temp", out temp))
                    temp = defaults.TempPath;

                string pvr;
                if (!map.TryGetValue("PowerVR", out pvr))
                    pvr = defaults.PowerVRCompilerPath;

                string dxx;
                if (!map.TryGetValue("DXX", out dxx))
                    dxx = defaults.DXXDriverPath;

                string disabledBackends;
                if( map.TryGetValue("DisabledBackends", out disabledBackends))
                    opts.m_DisabledBackends.AddRange(disabledBackends.Split(','));
                
                opts.D3DCompilerPath = d3dCompiler;
                opts.CodeXLPath = codeXL;
                opts.TempPath = temp;
                opts.PowerVRCompilerPath = pvr;
                opts.DXXDriverPath = dxx;
                return opts;
            }
            catch( Exception e)
            {
                // not found
                MessageBox.Show(e.Message, "uh-oh, couldn't read options file", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return GetDefaults();
            }
        }

        public void Write()
        {
            string OptionsFile = Path.Combine(
               Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
               "Pyramid\\options");

            try
            {
                string DisabledBackends = String.Join(",",m_DisabledBackends.ToArray());

                File.WriteAllText(OptionsFile,
                                   String.Format("D3DCompiler={0}\nCodeXL={1}\ntemp={2}\nPowerVR={3}\nDisabledBackends={4}\n",
                                                             D3DCompilerPath,
                                                             CodeXLPath,
                                                             TempPath,
                                                             PowerVRCompilerPath,
                                                             DisabledBackends));
            }
            catch(Exception e)
            {
                MessageBox.Show(e.Message, "uh-oh, couldn't write options file", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
