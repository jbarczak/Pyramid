using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Forms;
using System.Diagnostics;

namespace Pyramid
{
    class IntelShaderAnalyzerResultSet : IResultSet
    {
        public string Name { get { return "IntelShaderAnalyzer"; } }
        public Control AnalysisPanel { get { return m_Analysis; } }
        public Control ResultsPanel { get { return m_Results; } }
        private TextBox m_Analysis = new TextBox();
        private IntelShaderAnalyzerResultsPanel m_Results = null;

        public IntelShaderAnalyzerResultSet(IntelShaderAnalyzerResultsPanel panel )
        {
            m_Results = panel;
            m_Analysis.Dock = DockStyle.Fill;
            m_Analysis.ReadOnly = true;
            m_Analysis.Multiline = true;
            m_Analysis.Font = new System.Drawing.Font("Lucida Console", 8);
        }
       
    }

    public class IntelShaderAnalyzerBackend : IBackend
    {
        private string m_ToolPath;
        private string m_TempPath;

        public IntelShaderAnalyzerBackend(Options PyramidOpts)
        {
            m_ToolPath = PyramidOpts.IntelShaderAnalyzerPath;
            m_TempPath = PyramidOpts.TempPath;
        }

        public string Name { get { return "IGC"; } }

        public List<string> GetAsicList()
        {
            try
            {
                ProcessStartInfo pi = new ProcessStartInfo();
                pi.RedirectStandardOutput = true;
                pi.RedirectStandardInput = false;
                pi.RedirectStandardError = true;
                pi.CreateNoWindow = true;
                pi.Arguments = "--list-asics";
                pi.FileName = m_ToolPath;
                pi.UseShellExecute = false;

                Process p = Process.Start(pi);
                int pid = p.Id;

                // NOTE: Must read stdout before waiting for exit
                //  If we don't, then the process will hang if some stdout buffer fills up
                //  lame....
                string stdout = p.StandardOutput.ReadToEnd();

                p.WaitForExit();

                p.Close();

                string[] lines = stdout.Split(
                    new[] { "\r\n", "\r", "\n" },
                    StringSplitOptions.None
                );

                List<String> asics = new List<string>();
                foreach( string s in lines )
                {
                    string asic = s.Trim();
                    if (!string.IsNullOrEmpty(asic))
                        asics.Add(asic);
                }

                return asics;
            }
            catch( System.Exception ex )
            {
                MessageBox.Show("Failed to get asic list from IntelShaderAnalyzer");
                return new List<string>();
            }
        }

        public IResultSet Compile(IShader shader, IBackendOptions options)
        {
            if (shader.Language == Languages.HLSL)
                return CompileHLSL(shader as HLSLShader, options);
            else
                return null;
        }

        private IResultSet CompileHLSL(HLSLShader shader, IBackendOptions opts)
        {
            if (shader.CompiledBlob == null)
                return null;

            if (shader.CompiledBlob is IDXILShaderBlob &&
                shader.CompiledRootSig == null)
                return null;

            try
            {          
                List<string> tempFilesToDelete = new List<string>();

                string byteCodeFile = Path.Combine(m_TempPath, "IGCShader");
                string rootSigFile  = Path.Combine(m_TempPath, "IGCRS");
                File.WriteAllBytes( byteCodeFile, shader.CompiledBlob.ReadBytes()   );
                tempFilesToDelete.Add(byteCodeFile);
                
                if( shader.CompiledRootSig != null )
                {
                    File.WriteAllBytes(rootSigFile, shader.CompiledRootSig.ReadBytes());
                    tempFilesToDelete.Add(rootSigFile);
                }

                string commandline11 = "";
                string commandline12 = "";
                if (shader.CompiledBlob is IDXBCShaderBlob)
                {
                    commandline11 = String.Format("--api dx11 \"{0}\"", byteCodeFile);
                    commandline12 = String.Format("--api dx12 \"{0}\" --rootsig_file \"{1}\"", byteCodeFile, rootSigFile);
                }
                else
                {
                    commandline12 = String.Format("--api dx12 \"{0}\" --rootsig_file \"{1}\"", byteCodeFile, rootSigFile);
                }

                List<string> asics = GetAsicList();
                
                foreach (string asic in asics)
                {
                    commandline11 = String.Concat(commandline11, String.Format(" --asic {0} ", asic));
                    commandline12 = String.Concat(commandline12, String.Format(" --asic {0} ", asic));
                }

                string isaDir11 = Path.Combine(m_TempPath, "IntelISA_11\\");
                string isaDir12 = Path.Combine(m_TempPath, "IntelISA_12\\");

                Directory.CreateDirectory(isaDir11);
                Directory.CreateDirectory(isaDir12);

                // NOTE: Extra \ is required because DOS command interpretter parses \" as a double quote
                commandline11 = String.Concat(commandline11, String.Format("--isa \"{0}\\\"", isaDir11));
                commandline12 = String.Concat(commandline12, String.Format("--isa \"{0}\\\"", isaDir12));

                string stdout = "";
                List<string> dx11Files = new List<string>();
                List<string> dx12Files = new List<string>();

                // DX11 requires DXBC
                if( shader.CompiledBlob is IDXBCShaderBlob)
                {
                    ProcessStartInfo pi = new ProcessStartInfo();
                    pi.RedirectStandardOutput = true;
                    pi.RedirectStandardInput = false;
                    pi.RedirectStandardError = true;
                    pi.CreateNoWindow = true;
                    pi.Arguments = commandline11;
                    pi.FileName = m_ToolPath;
                    pi.UseShellExecute = false;

                    Process p = Process.Start(pi);
                    int pid = p.Id;

                    // NOTE: Must read stdout before waiting for exit
                    //  If we don't, then the process will hang if some stdout buffer fills up
                    //  lame....
                    stdout = String.Concat("DX11 COMMAND LINE:", commandline11, Environment.NewLine);
                    stdout = String.Concat(stdout, p.StandardOutput.ReadToEnd());

                    p.WaitForExit();
                    p.Close();

                    foreach (string asic in asics)
                        dx11Files.Add(String.Concat(isaDir11, asic, ".asm"));
                }

                // DX12 requires a root signature
                if( shader.CompiledRootSig != null )
                {
                    ProcessStartInfo pi = new ProcessStartInfo();
                    pi.RedirectStandardOutput = true;
                    pi.RedirectStandardInput = false;
                    pi.RedirectStandardError = true;
                    pi.CreateNoWindow = true;
                    pi.Arguments = commandline12;
                    pi.FileName = m_ToolPath;
                    pi.UseShellExecute = false;

                    Process p = Process.Start(pi);
                    int pid = p.Id;

                    // NOTE: Must read stdout before waiting for exit
                    //  If we don't, then the process will hang if some stdout buffer fills up
                    //  lame....

                    stdout = String.Concat(stdout, string.Concat("DX12 COMMAND LINE: ", commandline12, Environment.NewLine));
                    stdout = String.Concat(stdout, Environment.NewLine);
                    stdout = String.Concat(stdout, p.StandardOutput.ReadToEnd());

                    p.WaitForExit();
                    p.Close();
                    foreach (string asic in asics)
                        dx12Files.Add(String.Concat(isaDir12, asic, ".asm"));

                }

                tempFilesToDelete.AddRange(dx11Files);
                tempFilesToDelete.AddRange(dx12Files);

                IntelShaderAnalyzerResultsPanel panel = new IntelShaderAnalyzerResultsPanel(stdout,asics);
                panel.AddResults("DX11", dx11Files);
                panel.AddResults("DX12", dx12Files);

                // cleanup the temp files
                foreach (string file in tempFilesToDelete)
                    File.Delete(file);
                
                Directory.Delete(Path.Combine(m_TempPath, "IntelISA_11"));
                Directory.Delete(Path.Combine(m_TempPath, "IntelISA_12"));

                return new IntelShaderAnalyzerResultSet(panel);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "uh-oh, Couldn't run IntelShaderAnalyzer", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return null;
            }
        }
    };
}
