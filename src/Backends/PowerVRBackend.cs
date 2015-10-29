using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;


namespace Pyramid
{
    class PVRResultSet : IResultSet
    {
        private PVRResultsPanel m_Results = new PVRResultsPanel();

        public string Name { get { return "PowerVR"; } }
        public Control AnalysisPanel { get { return null; } }
        public Control ResultsPanel { get { return m_Results; } }

        public PVRResultsPanel PVRResultsPanel { get { return m_Results; } }
    };

    class PowerVRBackend : IBackend
    {
        private List<string> m_Compilers = new List<String>();
        private string m_TempPath;

        public string Name { get { return "PowerVR"; } }

        public PowerVRBackend( string CompilerDir, string tempDir )
        {
            CompilerDir = Path.GetFullPath(CompilerDir);
            m_TempPath = tempDir;

            if (!Directory.Exists(CompilerDir))
            {
                MessageBox.Show("PowerVR compiler directory doesn't exist", "uh-oh", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            foreach (string s in Directory.EnumerateFiles(CompilerDir))
            {
                string ext = Path.GetExtension(s);
                if( ext.Equals(".exe") )
                {
                    ProcessStartInfo pi = new ProcessStartInfo();
                    pi.RedirectStandardOutput = true;
                    pi.RedirectStandardInput = true;
                    pi.RedirectStandardError = true;
                    pi.CreateNoWindow = true;
                    pi.Arguments = "-?";
                    pi.FileName = s;
                    pi.UseShellExecute = false;

                    try
                    {
                        Process p = Process.Start(pi);
                        p.WaitForExit();

                        string output = p.StandardError.ReadToEnd();
                        if (!output.Contains("PVR GLSL-ES compiler for"))
                            continue;
                        if (!output.Contains("-disasm"))
                            continue;

                        m_Compilers.Add(s);
                    }
                    catch (System.Exception )
                    {
                        continue;
                    }
                }
            }

        }

        public IResultSet Compile( IShader sh )
        {
            if (sh.Language != Languages.GLSL)
                return null;

            GLSLShader glShader = (GLSLShader)sh;
            IGLSLOptions glOpts = glShader.CompileOptions;
            string shader = glShader.Code;

            
            string shaderSwitch = "";
            switch (glOpts.ShaderType)
            {
                case GLSLShaderType.VERTEX:   shaderSwitch = "-v"; break;
                case GLSLShaderType.FRAGMENT: shaderSwitch = "-f"; break;
                case GLSLShaderType.COMPUTE:  shaderSwitch = "-c"; break;
                default:
                    return null;
            }

            string tmpFile         = Path.Combine(m_TempPath, "PYRAMID_pvr");
            string dummyOutputFile = Path.Combine(m_TempPath, "PYRAMID_pvr.out");
            string disasmFile      = Path.Combine(m_TempPath, "PYRAMID_pvr.disasm");
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

            string args = String.Format("{0} {1} {2} -disasm", tmpFile, dummyOutputFile, shaderSwitch);
          

            PVRResultSet rs = new PVRResultSet();

            foreach (string s in m_Compilers)
            {
                ProcessStartInfo pi = new ProcessStartInfo();
                pi.RedirectStandardOutput = true;
                pi.RedirectStandardInput = true;
                pi.RedirectStandardError = true;
                pi.CreateNoWindow = true;
                pi.Arguments = args;
                pi.FileName  = s;
                pi.UseShellExecute = false;

                try
                {
                    Process p= Process.Start(pi);
                    p.WaitForExit();

                    string asm = "No Output";
                    string output = p.StandardError.ReadToEnd();
                    string compiler = Path.GetFileNameWithoutExtension(s);
                    
                    if (File.Exists(disasmFile))
                    {
                        asm = File.ReadAllText(disasmFile);
                        File.Delete(disasmFile);
                    }

                    rs.PVRResultsPanel.AddResult(compiler, output, asm);
                }
                catch (System.Exception)
                {
                    continue;
                }
            }

            File.Delete(tmpFile);

            return rs;
        }

    }
}
