using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Pyramid.Scrutinizer.UI
{
    public partial class ParameterWidget : UserControl
    {
        const int TEXTBOX_WIDTH = 40;
        const int LAYOUT_PAD = 4;

        private List<IParameterGUI> m_Parameters = new List<IParameterGUI>();

        public ParameterWidget()
        {
            InitializeComponent();
        }

        interface IParameterGUI
        {
            bool Assign();
            IEnumerable<Control> Controls { get; }
        };

        class ParameterGUI_Int : IParameterGUI
        {
            private List<Control> m_Controls = new List<Control>();
            private TextBox m_Box;
            private SimulationParameterInt m_Param;

            public IEnumerable<Control> Controls { get { return m_Controls; } }

            public ParameterGUI_Int(SimulationParameterInt p)
            {
                TextBox box = new TextBox();
                box.Width = TEXTBOX_WIDTH;
                box.Text = Convert.ToString(p.Default);
                box.Top  = 0;
                box.Left = LAYOUT_PAD;

                Label l = new Label();
                l.AutoSize = true;
                l.Text = p.Name;
                l.TextAlign = ContentAlignment.MiddleLeft;
                l.Top  = (box.Height/2) - (l.Height / 2);
                l.Left = box.Right + LAYOUT_PAD;

                m_Controls.Add(l);
                m_Controls.Add(box);
                
                m_Box = box;
                m_Param = p;
            }

            public bool Assign( )
            {
                try
                { 
                    int n = Convert.ToInt32(m_Box.Text);
                    if( n < m_Param.Min || n > m_Param.Max )
                    {
                        MessageBox.Show( 
                            String.Format( "{0} must be between {1} and {2}", m_Param.Name,
                                           m_Param.Min, m_Param.Max ) );
                        m_Box.Focus();
                        return false;
                    }
                    m_Param.Value = n;
                }
                catch( System.Exception e )
                {
                    MessageBox.Show(e.Message);
                    m_Box.Focus();
                    return false;
                }
                return true;
            }
        }


        class ParameterGUI_Double : IParameterGUI
        {

            private List<Control> m_Controls = new List<Control>();
            private TextBox m_Box;
            private SimulationParameterDouble m_Param;

            public IEnumerable<Control> Controls { get { return m_Controls; } }

            public ParameterGUI_Double( SimulationParameterDouble p)
            {
                TextBox box = new TextBox();
                box.Width = TEXTBOX_WIDTH;
                box.Text = Convert.ToString(p.Default);
                box.Top = 0;
                box.Left = LAYOUT_PAD;

                Label l = new Label();
                l.AutoSize = true;
                l.Text = p.Name;
                l.TextAlign = ContentAlignment.MiddleLeft;
                l.Top = (box.Height / 2) - (l.Height / 2);
                l.Left = box.Right + LAYOUT_PAD;
                m_Controls.Add(l);
                m_Controls.Add(box);
                m_Box = box;
                m_Param = p;
            }
            
            public bool Assign()
            {
                try
                {
                    double n = Convert.ToDouble(m_Box.Text);
                    if (n < m_Param.Min || n > m_Param.Max)
                    {
                        MessageBox.Show(
                            String.Format("{0} must be between {1] and {2}", m_Param.Name,
                                           m_Param.Min, m_Param.Max));
                        m_Box.Focus();
                        return false;
                    }
                    m_Param.Value = n;
                }
                catch (System.Exception e)
                {
                    MessageBox.Show(e.Message);
                    m_Box.Focus();
                    return false;
                }
                return true;
            }
        }


        /// <summary>
        ///  Called to transfer values from UI to the parameters
        /// </summary>
        /// <returns></returns>
        public bool UpdateParameters( )
        {
            foreach( IParameterGUI gui in m_Parameters )
            {
                if (!gui.Assign())
                    return false;
            }
            return true;
        }

        public void BuildUI( IEnumerable<Scrutinizer.ISimulationParameter> ParamSet )
        {
            m_Parameters.Clear();

            foreach( Scrutinizer.ISimulationParameter p in ParamSet )
            {
                if( p is Scrutinizer.SimulationParameterInt )
                {
                    m_Parameters.Add(new ParameterGUI_Int(p as SimulationParameterInt));
                }
                if( p is Scrutinizer.SimulationParameterDouble )
                {
                    m_Parameters.Add(new ParameterGUI_Double(p as SimulationParameterDouble));
                }
            }

            // lay out the controls
            int Y = LAYOUT_PAD;
            foreach( IParameterGUI gui in m_Parameters )
            {
                int h = 0;
                foreach (Control c in gui.Controls)
                {
                    this.Controls.Add(c);
                    c.Top = c.Top + Y;
                   
                    h = Math.Max(h, c.Height);
                }
                Y += h + LAYOUT_PAD;
            }

            this.Refresh();
        }
    }
}
