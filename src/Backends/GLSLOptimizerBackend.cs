using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Pyramid
{
    class GLSLOptimizerResultSet : IResultSet
    {
        private TextBox m_AnalysisPanel = new TextBox();
        private TabControl m_ResultsPanel = new TabControl();
        
        public GLSLOptimizerResultSet(GLSLOptimizer.IShader shader)
        {
            m_ResultsPanel.Dock = DockStyle.Fill;
            TabPage page;

            string AnalysisText="";
            if( !shader.HadError )
            {
                AnalysisText = String.Format("GLSLOptimizer stats:\r\nMathOps:\t{0}\r\nTextureOps:\t{1}\r\nFlowOps:\t{2}\r\n",
                                             shader.MathOps,
                                             shader.TextureOps,
                                             shader.ControlFlowOps);
                
                m_ResultsPanel.TabPages.Add("Optimized Output");
                page = m_ResultsPanel.TabPages[m_ResultsPanel.TabPages.Count - 1];
                TextBox txtOutput = new TextBox();
                txtOutput.ReadOnly = true;
                txtOutput.Dock = DockStyle.Fill;
                txtOutput.Multiline = true;
                txtOutput.ScrollBars = ScrollBars.Both;
                txtOutput.Text = shader.Output.Replace("\n", Environment.NewLine) ;
                txtOutput.WordWrap = false;
                page.Controls.Add(txtOutput);

                m_ResultsPanel.TabPages.Add("Raw Output");
                page = m_ResultsPanel.TabPages[m_ResultsPanel.TabPages.Count - 1];
                TextBox txtRawOutput = new TextBox();
                txtRawOutput.ReadOnly = true;
                txtRawOutput.Dock = DockStyle.Fill;
                txtRawOutput.Multiline = true;
                txtRawOutput.ScrollBars = ScrollBars.Both;
                txtRawOutput.Text = shader.RawOutput.Replace("\n",Environment.NewLine);
                txtRawOutput.WordWrap = false;
                page.Controls.Add(txtRawOutput);
            }

            m_ResultsPanel.TabPages.Add("Log");
            page = m_ResultsPanel.TabPages[m_ResultsPanel.TabPages.Count - 1];
            TextBox txtLog = new TextBox();
            txtLog.ReadOnly = true;
            txtLog.Dock = DockStyle.Fill;
            txtLog.Multiline = true;
            txtLog.ScrollBars = ScrollBars.Both;
            txtLog.WordWrap = false;
            txtLog.Text = shader.Log.Replace("\n", Environment.NewLine);
           
            page.Controls.Add(txtLog);

            m_AnalysisPanel.ReadOnly = true;
            m_AnalysisPanel.Dock = DockStyle.Fill;
            m_AnalysisPanel.Text = AnalysisText;
            m_AnalysisPanel.Multiline = true;
            m_AnalysisPanel.Font = new System.Drawing.Font("Lucida Console", 8);
        }

        public string Name { get { return "GLSLOptimizer"; } }
        public Control AnalysisPanel { get { return m_AnalysisPanel; } }
        public Control ResultsPanel { get { return m_ResultsPanel; } }
    };

    class GLSLOptimizerBackend : IBackend
    {
        private Dictionary<GLSLOptimizer.Target, GLSLOptimizer.IOptimizer> m_Optimizers =
            new Dictionary<GLSLOptimizer.Target, GLSLOptimizer.IOptimizer>();

        public string Name { get { return "GLSLOptimizer"; } }

        public GLSLOptimizerBackend(IWrapper wrapper)
        {
            foreach( GLSLOptimizer.Target t in Enum.GetValues(typeof(GLSLOptimizer.Target)))
                m_Optimizers.Add( t, wrapper.CreateGLSLOptimizer(t));
        }

        public IResultSet Compile(string shader, ICompileOptions opts)
        {
            if (opts.Language != Languages.GLSL)
                return null;
            IGLSLOptions glOpts = (IGLSLOptions)opts;
            if (glOpts.OptimizerOptions == null)
                return null;

            GLSLOptimizer.IOptimizer optimizer = m_Optimizers[glOpts.OptimizerTarget];
            return new GLSLOptimizerResultSet( optimizer.Optimize(shader, glOpts.OptimizerOptions) );
        }
    }
}
