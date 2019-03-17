
using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;

namespace Pyramid
{
    class IGCResultSet : IResultSet
    {
        IGCResultsPanel m_Results = new IGCResultsPanel();
        public string Name { get { return "IGCStandalone"; } }
        public Control AnalysisPanel { get { return m_Analysis; } }
        public Control ResultsPanel { get { return m_Results; } }
        private TextBox m_Analysis = new TextBox();

        public IGCResultSet()
        {
            m_Analysis.Dock = DockStyle.Fill;
            m_Analysis.ReadOnly = true;
            m_Analysis.Multiline = true;
            m_Analysis.Font = new System.Drawing.Font("Lucida Console", 8);
        }


        public void Add( string name, string output)
        {
            m_Results.Add(name, output);
        }
        
    }
        
    public class IGCStandaloneBackend : IBackend
    {
        private string m_ToolPath;
        private string m_TempPath;
            
        public IGCStandaloneBackend(Options PyramidOpts)
        {
            m_ToolPath = PyramidOpts.IGCPath;
            m_TempPath = PyramidOpts.TempPath;
               
        }

        public string Name { get { return "IGCStandalone"; } }

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
                IGCResultSet rs = new IGCResultSet();
                
                string shaderType = "";
                switch (shader.CompileOptions.ShaderType)
                {
                    case HLSLShaderType.HULL: shaderType = "hs"; break;
                    case HLSLShaderType.DOMAIN: shaderType = "ds"; break;
                    case HLSLShaderType.VERTEX: shaderType = "vs"; break;
                    case HLSLShaderType.GEOMETRY: shaderType = "gs"; break;
                    case HLSLShaderType.PIXEL: shaderType = "ps"; break;
                    case HLSLShaderType.COMPUTE: shaderType = "cs"; break;
                }

                List<string> tempFilesToDelete = new List<string>();

                string commandline = "";
                if (shader.CompiledBlob is IDXBCShaderBlob)
                {
                    // send it to a temp file
                    string tmpFile = Path.Combine(m_TempPath, "IGCShader");

                    IDXBCShaderBlob blobDXBC = shader.CompiledBlob as IDXBCShaderBlob;
                    File.WriteAllBytes(tmpFile, blobDXBC.GetExecutableBlob().ReadBytes());
                    commandline = String.Format("-option ShaderDumpEnable -api D3D10 -step E0 -prod skl -input{0} {1}", shaderType, tmpFile);

                    tempFilesToDelete.Add(tmpFile);
                }
                else
                {
                    // send it to a temp file
                    string tmpFile = Path.Combine(m_TempPath, "IGCShader");
                    string tmpRS = Path.Combine(m_TempPath, "IGCRS");

                    IDXILShaderBlob blobDXIL = shader.CompiledBlob as IDXILShaderBlob;
                    File.WriteAllBytes(tmpFile, blobDXIL.ReadBytes());
                    File.WriteAllBytes(tmpRS, shader.CompiledRootSig.ReadBytes());

                    tempFilesToDelete.Add(tmpFile);
                    tempFilesToDelete.Add(tmpRS);

                    commandline = String.Format("-option ShaderDumpEnable -api D3D12 -step E0 -prod skl -RTLayout {0} -input{1} {2}", tmpRS, shaderType, tmpFile);
                }

                ProcessStartInfo pi = new ProcessStartInfo();
                pi.RedirectStandardOutput = true;
                pi.RedirectStandardInput = false;
                pi.RedirectStandardError = true;
                pi.CreateNoWindow = true;
                pi.Arguments = commandline;
                pi.FileName = m_ToolPath;
                pi.UseShellExecute = false;

                Process p = Process.Start(pi);
                int pid = p.Id;

                // NOTE: Must read stdout before waiting for exit
                //  If we don't, then the process will hang if some stdout buffer fills up
                //  lame....
                rs.Add( "stdout", p.StandardOutput.ReadToEnd());

                p.WaitForExit();

                p.Close();

                // cleanup the temp file
                foreach( string file in tempFilesToDelete )
                    File.Delete(file);

                // IGC has just run the driver compile path and produced shader dumps.  Go find them
                string igcFileName = Path.GetFileName(m_ToolPath);
                string dumpLocation = String.Format("C:\\Intel\\IGC\\{0}_{1}", igcFileName, pid);

                if (Directory.Exists(dumpLocation))
                {
                    foreach (string fullPath in Directory.EnumerateFiles(dumpLocation))
                    {
                        string file = Path.GetFileName(fullPath);
                        rs.Add(file, File.ReadAllText(fullPath));
                        File.Delete(fullPath);
                    }

                    Directory.Delete(dumpLocation);
                }
                    
                return rs;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "uh-oh, Couldn't run IGC", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return null;
            }
        }
    };
}


