using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;

namespace Pyramid
{
    class MysteryToolResultSet : IResultSet
    {
        MysteryToolResultsPanel m_Results = new MysteryToolResultsPanel();
        public string Name { get { return "MysteryTool"; } }
        public Control AnalysisPanel { get { return m_Analysis; } }
        public Control ResultsPanel { get { return m_Results; } }
        private TextBox m_Analysis = new TextBox();

        public MysteryToolResultSet()
        {
            m_Analysis.Dock = DockStyle.Fill;
            m_Analysis.ReadOnly = true;
            m_Analysis.Multiline = true;
            m_Analysis.Font = new System.Drawing.Font("Lucida Console", 8);
        }

        public void Add( string sku, string text )
        {
            m_Results.Add(sku, text);
      
            // Mystery tool doesn't give us an instruction breakdown
            //   so do a crude estimate by counting lines in the output
            //  I could do much MUCH more here if I didn't need to make a secret
            //   of whose tool this is.
            string[] lines = text.Split(new string[] { "\r\n", "\n" }, StringSplitOptions.None);

            for (int i = 0; i < lines.Length; i++)
                lines[i] = lines[i].Trim();

            int ops = 0;
            for( int i=0; i<lines.Length; i++ )
            {
                lines[i] = lines[i].Trim();
                if (lines[i].StartsWith("ASSEMBLY"))
                {
                    for( ; i < lines.Length; i++ )
                    {
                        if (String.IsNullOrEmpty(lines[i]))
                            continue;
                        if (lines[i].StartsWith("label"))
                            continue;
                        ops++;
                    }
                    break;
                }
            }

            m_Analysis.Text = String.Format("ASM instruction count: {0}", ops - 2); // exclude first two lines, which are fluff
        }

    }


    //
    // The mystery tool is a very particular IHV-specific tool which is
    //  available to me under NDA.  As it is NDA, I cannot disclose whose tool it is, but
    //    I don't mind divulging the command line interface
    //
    //  Hey, recalcitrant IHV!   You know who you are!
    //   Why do you make me engage in this ridiculous charade?!?
    //    Please publish this exceedingly useful thing
    //
    public class MysteryToolBackend :  IBackend
    {
        private string m_ToolPath;
        private string m_TempPath;

        private List<String> m_SKUs;

        public MysteryToolBackend( Options PyramidOpts )
        {
            m_ToolPath = PyramidOpts.MysteryToolPath;
            m_TempPath = PyramidOpts.TempPath;
            m_SKUs = new List<String>();

            // run the tool to get an asic list
            ProcessStartInfo pi = new ProcessStartInfo();
            pi.RedirectStandardOutput = true;
            pi.RedirectStandardInput = true;
            pi.RedirectStandardError = true;
            pi.CreateNoWindow = true;
            pi.Arguments = "";
            pi.FileName = m_ToolPath;
            pi.UseShellExecute = false;

            try
            {
                Process p = Process.Start(pi);
                p.WaitForExit();

                while (!p.StandardOutput.EndOfStream)
                {
                    string s = p.StandardOutput.ReadLine();
                    if( s.Equals("supported skus:") )
                    {
                        // each subsequent line is comma-seperated sku list
                        //  SKU list ends with a blank line
                        do
                        {
                            s = p.StandardOutput.ReadLine();
                            if( String.IsNullOrEmpty(s))
                                break;

                            s = s.Trim();
                            foreach( string sku in s.Split(','))
                                if( !String.IsNullOrEmpty(sku))
                                    m_SKUs.Add(sku.Trim());
                            
                        } while( !p.StandardOutput.EndOfStream );

                        break;
                    }
                }

               
                p.Close();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "uh-oh, Couldn't get mystery tool SKU list", MessageBoxButtons.OK, MessageBoxIcon.Error);            
            }

        }

        public string Name { get { return "MysteryTool"; } }
        
        public IResultSet Compile(IShader shader, IBackendOptions options)
        {
            if (shader.Language == Languages.HLSL)
                return CompileHLSL(shader as HLSLShader, options);
            else
                return null;

        }

        private IResultSet CompileHLSL( HLSLShader shader, IBackendOptions opts )
        {
            
            // if we've got a compiled blob from FXC backend, then use that
            if (shader.CompiledBlob != null)
            {
                
                try
                {
                    MysteryToolResultSet rs = new MysteryToolResultSet();
                
                    // send it to a temp file
                    string tmpFile = Path.Combine(m_TempPath, "MysteryToolShader");
                    File.WriteAllBytes(tmpFile, shader.CompiledBlob.ReadBytes());

                    // shell out to the tool
                    // TODO: Mystery tool is kinda slow.... 
                    //   So lets only do the default SKU for now
                    /*
                    foreach (string sku in m_SKUs)
                    {
                        string commandline = String.Format("--shader_binary={0} --show_assembly --show_input_output_info --sku={1}", tmpFile, sku);

                         
                        ProcessStartInfo pi = new ProcessStartInfo();
                        pi.RedirectStandardOutput = true;
                        pi.RedirectStandardInput = false;
                        pi.RedirectStandardError = true;
                        pi.CreateNoWindow = true;
                        pi.Arguments = commandline;
                        pi.FileName = m_ToolPath;
                        pi.UseShellExecute = false;

                        Process p = Process.Start(pi);

                        rs.Add(sku, p.StandardOutput.ReadToEnd());

                        p.WaitForExit();

                        p.Close();
                    }
                    */

                    string commandline = String.Format("--shader_binary={0} --show_assembly --show_input_output_info", tmpFile);


                    ProcessStartInfo pi = new ProcessStartInfo();
                    pi.RedirectStandardOutput = true;
                    pi.RedirectStandardInput = false;
                    pi.RedirectStandardError = true;
                    pi.CreateNoWindow = true;
                    pi.Arguments = commandline;
                    pi.FileName = m_ToolPath;
                    pi.UseShellExecute = false;

                    Process p = Process.Start(pi);

                    // NOTE: Must read stdout before waiting for exit
                    //  If we don't, then the process will hang if some stdout buffer fills up
                    //  lame....
                    rs.Add("default", p.StandardOutput.ReadToEnd());

                    p.WaitForExit();

                    p.Close();

                    // cleanup the temp file
                    File.Delete(tmpFile);
                    return rs;
                }
                catch( Exception ex )
                {
                    MessageBox.Show(ex.Message, "uh-oh, Couldn't run mystery tool", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

            }
            else
            {
                // TODO: Don't have an HLSL shader.  
                // have them compile it for us.  They know how....
            }

            return null;
        }
    };
}
