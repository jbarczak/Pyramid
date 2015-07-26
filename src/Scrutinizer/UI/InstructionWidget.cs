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


      

        private ComboBox CreateFilterBox( IInstruction op )
        {
            ComboBox cb = new ComboBox();
            foreach (string s in Enum.GetNames(typeof(Scrutinizer.TextureFilter)))
                cb.Items.Add(s);
            cb.Width = 96;
            cb.SelectedIndex = (int)Scrutinizer.TextureFilter.TRILINEAR;
            cb.DropDownStyle = ComboBoxStyle.DropDownList;
            cb.SelectedIndexChanged += delegate(object s, EventArgs e)
            {
                if (op is ISamplingInstruction)
                {
                    ISamplingInstruction samp = op as ISamplingInstruction;
                    samp.Filter = (TextureFilter)cb.SelectedIndex;
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
            cb.SelectedIndex = (int)Scrutinizer.TexelFormat.RGBA8;
            cb.DropDownStyle = ComboBoxStyle.DropDownList;

            cb.SelectedIndexChanged += delegate(object s, EventArgs e)
            {
                if (op is ISamplingInstruction)
                {
                    ISamplingInstruction samp = op as ISamplingInstruction;
                    samp.Format = (TexelFormat)cb.SelectedIndex;
                }
                else if( op is ITexelLoadInstruction )
                {
                    ITexelLoadInstruction ld = op as ITexelLoadInstruction;
                    ld.Format = (TexelFormat)cb.SelectedIndex;
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

            if (op is Scrutinizer.ITexelLoadInstruction)
            {
                ComboBox cb = CreateFormatBox(op);
                cb.Anchor = AnchorStyles.Right;
                this.Controls.Add(cb);
            }
            if (op is Scrutinizer.ISamplingInstruction)
            {
                Panel panel = new Panel();
                ComboBox cbFilter = CreateFilterBox(op);
                ComboBox cbFormat = CreateFormatBox(op);
                panel.Anchor = AnchorStyles.Right;
                panel.Width = cbFilter.Width + cbFormat.Width;
                cbFilter.Left = cbFormat.Width;
                panel.Controls.Add(cbFilter);
                panel.Controls.Add(cbFormat);
                this.Controls.Add(panel);
            }
            
        }

        public Brush Brush { get; set; }

        private void InstructionWidget_Paint(object sender, PaintEventArgs e)
        {
            if (m_Op == null)
                return;

            e.Graphics.DrawString(m_Op.Disassemble(), this.Font, this.Brush, new PointF(80+96, 0));
        }

        private void InstructionWidget_MouseDown(object sender, MouseEventArgs e)
        {
            this.Parent.Focus();
        }
    }
}
