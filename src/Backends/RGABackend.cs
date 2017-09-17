
using System.Diagnostics;
using System.Collections.Generic;
using System.IO;
using System;
using System.Windows.Forms;

namespace Pyramid
{
    class RGAResult
    {
        public string Isa { get; private set; }
        public string Il { get; private set; }
        public string Analysis { get; private set; }
        public string LiveReg { get; private set; }
        public string Asic { get; private set; }

        public RGAResult( string asic, string path )
        {
            Asic = asic;

            string isaFile      = Path.ChangeExtension(path, ".isa");
            string analysisFile = Path.ChangeExtension(path, ".analysis");
            string ilFile       = Path.ChangeExtension(path, ".il");
            string liveRegFile  = Path.ChangeExtension(path, ".livereg");

            if (File.Exists(isaFile))
            {
                this.Isa = File.ReadAllText(isaFile);
                File.Delete(isaFile);
            }
            if (File.Exists(analysisFile))
            {
                this.Analysis = File.ReadAllText(analysisFile);
                File.Delete(analysisFile);
            }

            if (File.Exists(ilFile))
            {
                this.Il = File.ReadAllText(ilFile);
                File.Delete(ilFile);
            }

            if (File.Exists(liveRegFile))
            {
                this.LiveReg = File.ReadAllText(liveRegFile);
                File.Delete(liveRegFile);
            }
        }
    }

    class GenericTextResultSet : IResultSet
    {
        public GenericTextResultSet(string name, string text)
        {
            Name = name;
            RichTextBox results = new RichTextBox();
            results.Dock = DockStyle.Fill;
            results.ReadOnly = true;
            results.Multiline = true;
            results.Font = new System.Drawing.Font("Lucida Console", 8);
            results.Text = text;
            results.ScrollBars = RichTextBoxScrollBars.Both;

            ResultsPanel = results;
        }
        public Control ResultsPanel { get; private set; }
        public Control AnalysisPanel { get { return null; } }
        public string Name { get; private set; }
        
    }

    class RGAResultSet : IResultSet
    {
        private AMDResultsPanel m_ResultsPanel;
        private TextBox m_Analysis = new TextBox();
        private Dictionary<string, RGAResult> m_ResultMap = new Dictionary<string, RGAResult>();

        public RGAResultSet( string name, string toolOutput )
        {
            m_ResultsPanel = new AMDResultsPanel();
            m_ResultsPanel.SetToolOutput(toolOutput);
          
            m_Analysis.Dock = DockStyle.Fill;
            m_Analysis.ReadOnly = true;
            m_Analysis.Multiline = true;
            m_Analysis.Font = new System.Drawing.Font("Lucida Console", 8);
            m_Analysis.ScrollBars = ScrollBars.Both;
            m_ResultsPanel.AsicChanged += delegate(string asic)
            {
                m_Analysis.Text = m_ResultMap[asic].Analysis;
            };

            Name = name;
        }

        public Control ResultsPanel { get { return m_ResultsPanel; } }
        public Control AnalysisPanel { get { return m_Analysis; } }
        public string Name { get; private set; }
        public int ResultCount { get { return m_ResultsPanel.ResultCount; } }

        public void AddCompileResult( RGAResult result )
        {
            m_ResultMap.Add(result.Asic, result);
            m_ResultsPanel.AddResult(result.Asic, result.Il, result.Isa);
        }

        public void DisplayAsic(string asic)
        {
            m_ResultsPanel.SetAsic(asic);
            m_Analysis.Text = m_ResultMap[asic].Analysis;
        }
    }

    class RGABackendOptions : IBackendOptions
    {
        private Options m_Opts;

        public RGABackendOptions( Options opts )
        {
            m_Opts = opts;
                
        }

        public bool ShouldCompileForAsic( string asic)
        {
            return !m_Opts.IsRGAAsicDisabled(asic);
           
        }
    }

    class RGABackend : IBackend
    {
        private List<string> m_SupportedAsics = new List<string>();
        private string m_RGAPath = "";
        private string m_TempPath = "";
        private GLSlang.ICompiler m_GLSLang;
        private GLSlang.IConfig m_GLSLangConfig;

        public string Name { get { return "RGA_Vulkan"; } }
        public IEnumerable<string> Asics { get { return m_SupportedAsics; } }

        public static List<string> GetAsicList( string RGAPath )
        {
            string CommandLine = string.Format("-s vulkan -l");
            List<string> asics = new List<string>();

            ProcessStartInfo pi = new ProcessStartInfo();
            pi.RedirectStandardOutput = true;
            pi.RedirectStandardInput = true;
            pi.RedirectStandardError = true;
            pi.CreateNoWindow = true;
            pi.Arguments = CommandLine;
            pi.FileName = RGAPath;
            pi.UseShellExecute = false;
            try
            {
                Process p = Process.Start(pi);
                
                while (!p.StandardOutput.EndOfStream)
                {
                    // Output is of the form:
                    // 
                    //name (graphics ip)
                    //\tproduct_name
                    //\tproduct_name
                    // ....
                    //
                    string s = p.StandardOutput.ReadLine();
                    if (s.Length == 0)
                        continue; // blank line
                    if (s[0] == '\t')
                        continue; // indented product name

                    // grab the asic name, ignore everything after
                    int len = s.IndexOf(' ');
                    if (len > 0)
                        s = s.Substring(0, len);

                    asics.Add(s);
                }

                p.WaitForExit();
                p.Close();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "uh-oh, couldn't get RGA asic list", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            return asics;
        }

        public RGABackend(string RGAPath, string TempPath, IWrapper wrapper, IIncludeHandler handler )
        {
            m_SupportedAsics = GetAsicList(RGAPath);
            m_RGAPath = RGAPath;
            m_TempPath = TempPath;

            m_GLSLang = wrapper.CreateGLSlangCompiler(handler);
            m_GLSLangConfig = m_GLSLang.CreateDefaultConfig();
        }

        private string GetRGAShaderType(GLSLShaderType eShaderType)
        {
            string sType = "";
            switch (eShaderType)
            {
                default:
                    return "";
                case GLSLShaderType.VERTEX: sType = "vert"; break;
                case GLSLShaderType.FRAGMENT: sType = "frag"; break;
                case GLSLShaderType.GEOMETRY: sType = "geom"; break;
                case GLSLShaderType.TESS_CONTROL: sType = "tesc"; break;
                case GLSLShaderType.TESS_EVALUATION: sType = "tese"; break;
                case GLSLShaderType.COMPUTE: sType = "comp"; break;
            }
            return sType;
        }

        public IResultSet Compile(IShader shader, IBackendOptions options)
        {
            if( !(options is RGABackendOptions))
                return null;

            RGABackendOptions backendOptions = options as RGABackendOptions;
                
            string CommandLine = "";
            string tmpFile = Path.Combine(m_TempPath, "PYRAMID_amdrga");
            string sType = "";
            if (shader is GLSLShader)
            {
                GLSLShader shaderHLSL = shader as GLSLShader;
                IGLSLOptions glslOpts = shaderHLSL.CompileOptions;
                string text = shader.Code;

                try
                {
                    StreamWriter stream = File.CreateText(tmpFile);
                    stream.Write(text);
                    stream.Close();
                }
                catch (Exception e)
                {
                    MessageBox.Show(e.Message, "uh-oh, couldn't create temp file", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return null;
                }

                sType = GetRGAShaderType(glslOpts.ShaderType);
                CommandLine = String.Format(" -s vulkan --{0} {1} ", sType, tmpFile);
                
            }
            else if( shader is HLSLShader)
            {
                // pass the shader through GLSLang's hlsl front end
                HLSLShader hlsl = shader as HLSLShader;
                GLSlang.IShader glShader = m_GLSLang.CompileHLSL(shader.Code, hlsl.CompileOptions, m_GLSLangConfig, shader.SourceFilePath); 
                if( glShader.HasErrors )
                    return new GenericTextResultSet(this.Name, glShader.InfoLog);

                sType = GetRGAShaderType(glShader.ShaderType);
            
                // get the SPIR-V
                SPIRV.IProgram spirv = glShader.CompileSPIRV();
                if( spirv == null )
                    return new GenericTextResultSet(this.Name, "Error generating SPIR-V");
                
                // dump the SPIR-V to disk
                try
                {
                    File.WriteAllBytes(tmpFile,spirv.GetBytes());
                }
                catch (Exception e)
                {
                    MessageBox.Show(e.Message, "uh-oh, couldn't create temp file", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return null;
                }

                // send the SPIR-V to RGA
                CommandLine = String.Format(" -s vulkan-spv {0}", tmpFile);
            }

            

            string isaPath = Path.Combine(m_TempPath, "pyramid.isa");
            string analysisPath = Path.Combine(m_TempPath, "pyramid.analysis");
            string ilPath = Path.Combine(m_TempPath, "pyramid.il");
            string liveRegPath = Path.Combine(m_TempPath, "pyramid.livereg");
            CommandLine = String.Concat(CommandLine, String.Format(" --isa \"{0}\" ", isaPath));
            CommandLine = String.Concat(CommandLine, String.Format(" -a \"{0}\" ", analysisPath));
            CommandLine = String.Concat(CommandLine, String.Format(" --il \"{0}\" ", ilPath));
            CommandLine = String.Concat(CommandLine, String.Format(" --livereg \"{0}\" ", liveRegPath));
            
            List<string> asicsToCompile = new List<String>();
            foreach (string asic in m_SupportedAsics)
            {
                if ( backendOptions.ShouldCompileForAsic(asic))
                {
                    asicsToCompile.Add(asic);
                    CommandLine = String.Concat(CommandLine, " -c ", asic, " ");
                }
            }

            if (asicsToCompile.Count == 0)
                return null;

            string defaultAsic = asicsToCompile[0];


            ProcessStartInfo pi = new ProcessStartInfo();
            pi.RedirectStandardOutput = true;
            pi.RedirectStandardInput = true;
            pi.RedirectStandardError = false;
            pi.CreateNoWindow = true;
            pi.Arguments = CommandLine;
            pi.FileName = m_RGAPath;
            pi.UseShellExecute = false;

            string output;
            try
            {
                Process p = Process.Start(pi);
                output = p.StandardOutput.ReadToEnd();
                
                int TIMEOUT = 60000;
                p.WaitForExit(TIMEOUT);
                
                p.Close();
                File.Delete(tmpFile);
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "uh-oh, couldn't run CodeXL", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return null;
            }

            output = String.Format(@"Arguments:
                                    ----------
                                    {0}
                                    Output:
                                    -------
                                    {1}", CommandLine, output);
           
            // Compile results are emitted in one set of files per asic
            RGAResultSet results = new RGAResultSet(this.Name, output);
           
            foreach (string asic in asicsToCompile)
            {
                string path = Path.Combine( m_TempPath, String.Format("{0}_{1}_pyramid", asic, sType) );

                try
                {
                    RGAResult result = new RGAResult(asic, path);
                    results.AddCompileResult(result);
                }
                catch (Exception ex)
                {
                    // may occur on compile error
                }
            }


            if (results.ResultCount > 0)
            {
                results.DisplayAsic(defaultAsic);
                return results;
            }
            else
                return null;
            
        }
    }


}

