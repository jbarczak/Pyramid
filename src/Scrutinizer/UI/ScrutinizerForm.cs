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
        private List<IInstruction> m_Ops;
        private List<BasicBlock> m_Blocks;
        private List<Loop> m_Loops;
        private IScrutinizer m_Backend;
       

        private Dictionary<IInstruction, InstructionWidget> m_Instructions = new Dictionary<IInstruction, InstructionWidget>();

        public ScrutinizerForm( IAMDShader sh )
        {
            InitializeComponent();

            try
            {
                Wrapper w = new Wrapper();
                m_Backend = sh.CreateScrutinizer();

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
                foreach (BasicBlock b in m_Blocks)
                {
                    foreach (IInstruction op in b.Instructions)
                    {
                        InstructionWidget widget = new InstructionWidget(op);
                        m_Instructions.Add(op, widget);

                        panel1.Controls.Add(widget);
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
            foreach( InstructionWidget w in m_Instructions.Values )
                w.Brush = Brushes.DarkGray ;

            foreach (IInstruction op in SelectedBlock.Instructions)
                m_Instructions[op].Brush = Brushes.Black;

            panel1.Refresh();
            panel1.Focus();

            txtLoopCount.Visible = false;
            lblIterations.Visible = false;
        }

        private void cfgWidget1_LoopSelected(object sender, Loop SelectedLoop)
        {
            foreach (InstructionWidget w in m_Instructions.Values)
                w.Brush = Brushes.DarkGray;

            foreach( BasicBlock b in SelectedLoop.Blocks )
                foreach (IInstruction op in b.Instructions)
                    m_Instructions[op].Brush = Brushes.Black;

            panel1.Refresh();
            panel1.Focus();

            lblIterations.Visible = true;
            txtLoopCount.Visible = true;
            txtLoopCount.Text = SelectedLoop.DesiredIterations.ToString();
        }

        private void cfgWidget1_SelectionCleared(object sender)
        {
            foreach (InstructionWidget w in m_Instructions.Values)
                w.Brush = Brushes.Black;

            panel1.Refresh(); 
            panel1.Focus();

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
            List<IInstruction> trace = Algorithms.DoTrace(m_Ops, m_Blocks, m_Loops);

            string sim = m_Backend.AnalyzeExecutionTrace(trace);
            MessageBox.Show(sim);
            panel1.Refresh();
        }


    }
}
