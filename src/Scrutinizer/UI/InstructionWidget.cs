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
    public partial class InstructionWidget : UserControl
    {
        private IInstruction m_Op;
        private ComboBox m_FilterBox;
        private ComboBox m_FormatBox;

        public delegate void TexelFormatChangedHandler( ITextureInstruction op );
        public delegate void FilterChangedHandler( ISamplingInstruction op );

        public event TexelFormatChangedHandler TexelFormatChanged;
        public event FilterChangedHandler      FilterChanged;

        private ComboBox CreateFilterBox( IInstruction op )
        {
            ComboBox cb = new ComboBox();
            foreach (string s in Enum.GetNames(typeof(Scrutinizer.TextureFilter)))
                cb.Items.Add(s);
            cb.Width = 96;
            cb.SelectedIndex = (int)(op as ISamplingInstruction).Filter;
            cb.DropDownStyle = ComboBoxStyle.DropDownList;

            cb.SelectedIndexChanged += delegate(object s, EventArgs e)
            {
                if (op is ISamplingInstruction)
                {
                    ISamplingInstruction samp = op as ISamplingInstruction;
                    samp.Filter = (TextureFilter)cb.SelectedIndex;
                  
                    if (this.FilterChanged != null )
                        this.FilterChanged(samp);
                }
            };

            return cb;
        }
        private ComboBox CreateFormatBox( IInstruction op )
        {
            ComboBox cb = new ComboBox();
            foreach (string s in Enum.GetNames(typeof(Scrutinizer.TexelFormat)))
                cb.Items.Add(s);
            cb.Width = 80;
            cb.SelectedIndex = (int)(op as ITextureInstruction).Format;
            cb.DropDownStyle = ComboBoxStyle.DropDownList;
            
            cb.SelectedIndexChanged += delegate(object s, EventArgs e)
            {
                if (op is ITextureInstruction)
                {
                    ITextureInstruction samp = op as ITextureInstruction;
                    samp.Format = (TexelFormat)cb.SelectedIndex;

                    if( this.TexelFormatChanged != null )
                        this.TexelFormatChanged(samp);
                }
               
            };
            return cb;
        }

        public InstructionWidget( IInstruction op )
        {
            InitializeComponent();
            this.Font   = new Font("Lucida Console", 8.25f);
            this.Height = 15;
            m_Op = op;
            this.Brush = Brushes.Black;
            Selected = false;


            if (op is Scrutinizer.ISamplingInstruction)
            {
                Panel panel = new Panel();
                m_FilterBox = CreateFilterBox(op);
                m_FormatBox = CreateFormatBox(op);
                panel.Anchor = AnchorStyles.Right;
                panel.Width = m_FilterBox.Width + m_FormatBox.Width;
                m_FilterBox.Left = m_FormatBox.Width;
                panel.Controls.Add(m_FilterBox);
                panel.Controls.Add(m_FormatBox);
                this.Controls.Add(panel);
            }
            else if (op is Scrutinizer.ITextureInstruction)
            {
                m_FormatBox = CreateFormatBox(op);
                m_FormatBox.Anchor = AnchorStyles.Right;
                this.Controls.Add(m_FormatBox);
            }
        }

        public IInstruction Instruction { get { return m_Op; } }
        public Brush Brush { get; set; }
        public bool Selected { get; set; }

        public void RefreshInstruction()
        {
            if( m_FilterBox != null )
                m_FilterBox.SelectedIndex = (int) (m_Op as ISamplingInstruction).Filter;
            if (m_FormatBox != null)
                m_FormatBox.SelectedIndex = (int) (m_Op as ITextureInstruction).Format;
        }

        private void InstructionWidget_Paint(object sender, PaintEventArgs e)
        {
            if (m_Op == null)
                return;

            if( Selected )
            {
                e.Graphics.FillRectangle(Brushes.White, new Rectangle(0, 0, this.Width, this.Height));
            }

            e.Graphics.DrawString(m_Op.Disassemble(), this.Font, this.Brush, new PointF(80+96, 0));

            if( !String.IsNullOrEmpty(m_Op.SimNotes)  )
            {
                e.Graphics.DrawString(m_Op.SimNotes, this.Font, this.Brush, new PointF(this.Width - 100, 0));
            }

           
        }


        private void InstructionWidget_MouseDown(object sender, MouseEventArgs e)
        {
            this.Parent.Focus();
        }

       
    }
}
