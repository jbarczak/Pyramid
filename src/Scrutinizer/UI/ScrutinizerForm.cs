using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Pyramid.Scrutinizer.UI
{
    public partial class ScrutinizerForm : Form
    {
        private List<IInstruction> m_FetchShader;
        private List<IInstruction> m_Ops;
        private List<BasicBlock> m_Blocks;
        private List<Loop> m_Loops;
        private IScrutinizer m_Backend;
        private IDXShaderReflection m_Reflection;

        private HashSet<InstructionWidget> m_SelectedOps = new HashSet<InstructionWidget>();
        private InstructionWidget m_LastClicked = null;

        private Dictionary<IInstruction, InstructionWidget> m_InstructionWidgets = new Dictionary<IInstruction, InstructionWidget>();
        private Dictionary<IInstruction, int> m_InstructionIndexMap = new Dictionary<IInstruction, int>();

        private void ClearSelectedInstructions()
        {
            // de-select everything on a right click
            foreach (InstructionWidget i in m_SelectedOps)
            {
                i.Selected = false;
                i.Refresh();
            }
            m_SelectedOps.Clear();
        }

        private void SelectInstruction( IInstruction op )
        {
            InstructionWidget widget = m_InstructionWidgets[op];
            m_SelectedOps.Add(widget);
            widget.Selected = true;
            widget.Refresh();
        }
      
        private void OnInstructionClick( InstructionWidget w, MouseEventArgs e )
        {
            if (e.Button == MouseButtons.Left)
            {
                if (Form.ModifierKeys == Keys.Control)
                {
                    // ctrl+click: toggle state of one widget
                    w.Selected = !w.Selected;
                    w.Refresh();
                    if (w.Selected)
                        m_SelectedOps.Add(w);
                    else
                        m_SelectedOps.Remove(w);
                }
                else if( Form.ModifierKeys == Keys.Shift && m_LastClicked != null )
                {
                    // shift+click: select everything between last clicked widget
                    //  and current widget
                   
                    int i = m_InstructionIndexMap[m_LastClicked.Instruction];
                    int j = m_InstructionIndexMap[w.Instruction];
                    int first = Math.Min(i, j);
                    int last = Math.Max(i, j);
                    for (int x = first; x <= last; x++)
                        SelectInstruction(m_Ops[x]);
                }
                else
                {
                    // unmodified click.  Select only the clicked widget
                    ClearSelectedInstructions();
                    SelectInstruction(w.Instruction);
                }
                
                m_LastClicked = w;
            }
            else if (e.Button == MouseButtons.Right)
            {
                // de-select everything on a right click
                ClearSelectedInstructions();
            }
        }

        public void AddInstructionToPanel( InstructionWidget widget, IInstruction op )
        {
            m_InstructionIndexMap.Add(op, m_InstructionWidgets.Count);
            m_InstructionWidgets.Add(op, widget);
                        
            widget.Click += delegate(object s, EventArgs e)
            {
                this.OnInstructionClick(widget, e as MouseEventArgs);
            };

            // change format on all selected instructions whenever one of them is changed
            widget.TexelFormatChanged += delegate(ITextureInstruction inst )
            {
                if (!widget.Selected)
                    return;
                foreach( InstructionWidget s in m_SelectedOps )
                {
                    ITextureInstruction tx = s.Instruction as ITextureInstruction;
                    if( tx != null && tx != inst )
                        tx.Format = inst.Format;
                    s.RefreshInstruction();
                }
            };

            // change filter on all selected instructions whenever one of them is changed
            widget.FilterChanged += delegate(ISamplingInstruction inst)
            {
                if (!widget.Selected)
                    return;

                foreach (InstructionWidget s in m_SelectedOps)
                {
                    ISamplingInstruction tx = s.Instruction as ISamplingInstruction;
                    if (tx != null && tx != inst)
                        tx.Filter = inst.Filter;
                    s.RefreshInstruction();
                }
            };

            panel1.Controls.Add(widget);
        }


        public ScrutinizerForm( IAMDShader sh, IDXShaderReflection reflection)
        {
            InitializeComponent();
            m_Reflection = reflection;

            switch( reflection.GetShaderType() )
            {
            case HLSLShaderType.VERTEX:
                txtACMR.Enabled = true;
                txtPixels.Enabled = false;
                break;
            case HLSLShaderType.PIXEL:
                txtACMR.Enabled = false;
                txtPixels.Enabled = true;
                break;
            }

            try
            {
                Wrapper w = new Wrapper();
                m_Backend = sh.CreateScrutinizer();

                txtOccupancy.Text = m_Backend.GetDefaultOccupancy().ToString();
                List<IInstruction> Ops = m_Backend.BuildProgram();

                m_Ops = Ops;
                m_Blocks = Algorithms.BuildBasicBlocks(Ops);
                if (!Algorithms.IsCFGReducible(m_Blocks))
                {
                    MessageBox.Show("Non-reducible flow-graph detected.  Can't analyze this.");
                    return;
                }

                Algorithms.FindDominators(m_Blocks);

                m_Loops = Algorithms.FindLoops(m_Blocks);
                Algorithms.ClassifyBranches(m_Ops);


                int Y = 0;
                if( reflection.GetShaderType() == HLSLShaderType.VERTEX )
                {
                    m_FetchShader = m_Backend.BuildDXFetchShader(reflection);
                    Label l0 = new Label();
                    Label l1 = new Label();
                    l0.AutoSize = true;
                    l1.AutoSize = true;
                    l0.Text = "*** INSTRUCTIONS BELOW ARE AN APPROXIMATION TO THE FETCH SHADER***";
                    l1.Text = "******************************************************************";
                    l0.Left = 128;
                    l1.Left = 128;
                    panel1.Controls.Add(l0);
                    Y = l0.Height;

                    foreach( IInstruction op in m_FetchShader )
                    {
                        InstructionWidget widget = new InstructionWidget(op);
                        AddInstructionToPanel(widget, op);
                        widget.Top = Y;
                        widget.Left = 0;
                        Y += widget.Height;
                    }

                    l1.Top = Y;
                    panel1.Controls.Add(l1);
                    Y += l1.Height;
                }

                foreach (BasicBlock b in m_Blocks)
                {
                    foreach (IInstruction op in b.Instructions)
                    {
                        InstructionWidget widget = new InstructionWidget(op);
                        AddInstructionToPanel(widget, op);
                        widget.Top = Y;
                        widget.Left = 0;
                        Y += widget.Height;
                    }
                    Y += 15;
                }

                cfgWidget1.SetProgram(m_Loops, m_Blocks);
            }
            catch( System.Exception ex )
            {
                MessageBox.Show(ex.Message);
            }


        }

        private void cfgWidget1_BlockSelected(object sender, BasicBlock SelectedBlock)
        {
            ClearSelectedInstructions();
            foreach (IInstruction i in SelectedBlock.Instructions)
                SelectInstruction(i);

            panel1.Refresh();
            
            txtLoopCount.Visible = false;
            lblIterations.Visible = false;
        }

        private void cfgWidget1_LoopSelected(object sender, Loop SelectedLoop)
        {
            ClearSelectedInstructions();
            foreach (BasicBlock b in SelectedLoop.Blocks)
                foreach (IInstruction i in b.Instructions)
                    SelectInstruction(i);

            panel1.Refresh();
            
            lblIterations.Visible = true;
            txtLoopCount.Visible = true;
            txtLoopCount.Text = SelectedLoop.DesiredIterations.ToString();
        }

        private void cfgWidget1_SelectionCleared(object sender)
        {
            ClearSelectedInstructions();

            panel1.Refresh(); 
            
            txtLoopCount.Visible = false;
            lblIterations.Visible = false;
        }

        private void panel1_MouseDown(object sender, MouseEventArgs e)
        {
            panel1.Focus();
        }

        private void txtLoopCount_TextChanged(object sender, EventArgs e)
        {
            int n = 0;
            try
            {
                n = Convert.ToInt32(txtLoopCount.Text);
                cfgWidget1.SelectedLoop.DesiredIterations = n;
            }
            catch(System.Exception )
            {
            }

        }

        private void btnSimulate_Click(object sender, EventArgs e)
        {
            if (m_FetchShader != null)
            {
                foreach (IInstruction op in m_FetchShader)
                    op.SimNotes = "";
            }

            foreach (IInstruction op in m_Ops)
                op.SimNotes = "";

            try
            {
                List<IInstruction> trace = new List<IInstruction>();
                if (m_FetchShader != null)
                    trace.AddRange(m_FetchShader);

                trace.AddRange(Algorithms.DoTrace(m_Ops, m_Blocks, m_Loops));

                uint nOccupancy = Convert.ToUInt32(txtOccupancy.Text);
                if (nOccupancy < 0 || nOccupancy > 10)
                    throw new System.Exception("Occupancy must be 1-10");

                uint nCUs = Convert.ToUInt32(txtCUCount.Text);


                uint nWaveIssueRate = 0;
                switch( m_Reflection.GetShaderType() )
                {
                // one VS wave every M clocks
                //   round-robined amongst CUs
                //
                // Assume that the VGT tests 1 vert/clk against TnL cache
                //   If A = cache hit rate (verts/tri)
                //  then we'll collect on average A/3 unshaded verts every clock
                //    So we get a new wave every M=64*A/3 clocks   
                //
                case HLSLShaderType.VERTEX:
                    {
                        double fVertsPerTri   = Convert.ToDouble( txtACMR.Text );
                        double fVertsPerClock = fVertsPerTri / 3;
                        double fClocksPerWave = Math.Ceiling(64.0 / fVertsPerClock);
                        nWaveIssueRate        = (uint) (nCUs*fClocksPerWave);
                    }
                    break;

                // Assume 16 pix/clk raster rate (4 quads/clk)
                //    Assuming we can repack quads from multiple tris into a wave,
                //     we end up with 
                case HLSLShaderType.PIXEL:
                    {
                        double fQuadsPerTri   = Convert.ToDouble( txtPixels.Text )/4;
                        double fQuadsPerClock = Math.Min( 4.0, Math.Ceiling(fQuadsPerTri) );
                        fQuadsPerClock        = Math.Max(1.0,fQuadsPerClock);
                        double fClocksPerWave = Math.Ceiling( 16.0 / fQuadsPerClock );
                        nWaveIssueRate        = (uint) (nCUs*fClocksPerWave);
                    }
                    break;

                default:
                    throw new System.Exception("Bad shader type");
                }

            
                string sim = m_Backend.AnalyzeExecutionTrace(trace, nWaveIssueRate, nOccupancy, nCUs );
                MessageBox.Show(sim);
                panel1.Refresh();
            }
            catch( System.Exception ex )
            {
                MessageBox.Show(ex.Message);
            }
        }


    }
}
