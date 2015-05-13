using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;
using System.Text.RegularExpressions;

namespace Pyramid
{
    public class MaliSCResultSet : IResultSet
    {
        public string Name { get { return "MaliSC"; } }
        public Control AnalysisPanel { get; private set; }
        public Control ResultsPanel { get; private set; }

        public MaliSCResultSet()
        {
            AnalysisPanel = null;
            ResultsPanel  = new MaliSCResultsPanel();
        }
        public void Add(string core, string output)
        {
            MaliSCResultsPanel rp = ResultsPanel as MaliSCResultsPanel;
            rp.AddResult(core, output);
        }
    }

    public class MaliSCBackend : IBackend
    {
        private string m_MaliRoot = "";
        private string m_TempPath = "";
        private List<string> m_Asics = new List<string>();

        public MaliSCBackend(string MaliRoot, string TempPath)
        {
            m_MaliRoot = MaliRoot;
            m_TempPath = TempPath;

            // get asic list
            string CommandLine = string.Format("--help");

            ProcessStartInfo pi = new ProcessStartInfo();
            pi.RedirectStandardOutput = true;
            pi.RedirectStandardInput = true;
            pi.RedirectStandardError = true;
            pi.EnvironmentVariables.Add("MALICM_LOCATION", m_MaliRoot);
            pi.CreateNoWindow = true;
            pi.Arguments = CommandLine;
            pi.FileName = Path.Combine(MaliRoot, "malisc.exe");
            pi.UseShellExecute = false;

            try
            {
                Process p = Process.Start(pi);
                p.WaitForExit();

                // malisc frontend prints a giant help-text string.
                //  amidst the noise, we'll find a list of supported cores
                //
                //  We'll let them use defaults for driver revision and HW revision
                //   because permuting over all that stuff is probably fruitless
                //
                string output = p.StandardOutput.ReadToEnd();
                int idx = output.IndexOf("[-c <");
                string trimmed = output.Substring(output.IndexOf("[-c"));
                trimmed = trimmed.Substring(0, trimmed.IndexOf("]"));
                trimmed = trimmed.Substring(trimmed.IndexOf("<"));
                trimmed = trimmed.Substring(0, trimmed.IndexOf(">"));
                trimmed = trimmed.Substring(1);

                string[] asics = trimmed.Split('|');
                foreach (string s in asics)
                    m_Asics.Add(s.Trim());
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "uh-oh, couldn't get MaliSC asic list", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        public string Name { get { return "MaliSC"; } }
        
        public IResultSet Compile(string shader, ICompileOptions opts)
        {
            if (opts.Language != Languages.GLSL)
                return null;

            IGLSLOptions glOpts = opts as IGLSLOptions;
            string shaderType = "";
            switch( glOpts.ShaderType )
            {
            case GLSLShaderType.VERTEX:   shaderType = "--vertex";   break;
            case GLSLShaderType.FRAGMENT: shaderType = "--fragment"; break;
            case GLSLShaderType.COMPUTE:  shaderType = "--compute";  break;
            default:
                return null;
            }

            string tmpFile         = Path.Combine(m_TempPath, "PYRAMID_mali");
            try
            {
                StreamWriter stream = File.CreateText(tmpFile);
                stream.Write(shader);
                stream.Close();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "uh-oh, couldn't create temp file", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return null;
            }

            MaliSCResultSet rs = new MaliSCResultSet();
            try
            {
                foreach (string asic in m_Asics)
                {
                    string commandline = String.Format("-V {0} -c {1} {2}", shaderType, asic, tmpFile);
                    ProcessStartInfo pi = new ProcessStartInfo();
                    pi.RedirectStandardOutput = true;
                    pi.RedirectStandardInput = true;
                    pi.RedirectStandardError = true;
                    pi.EnvironmentVariables.Add("MALICM_LOCATION", m_MaliRoot);
                    pi.CreateNoWindow = true;
                    pi.Arguments = commandline;
                    pi.FileName = Path.Combine(m_MaliRoot, "malisc.exe");
                    pi.UseShellExecute = false;

                    Process p = Process.Start(pi);
                    p.WaitForExit();

                    string output = p.StandardOutput.ReadToEnd();
                    rs.Add(asic, output);
                }
            }
            catch (System.Exception e)
            {
                MessageBox.Show(e.Message, "uh-oh, couldn't run MaliSC", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            File.Delete(tmpFile);

            return rs;
        }
    }
}
