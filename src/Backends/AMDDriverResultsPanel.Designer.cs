namespace Pyramid
{
    partial class AMDDriverResultsPanel
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.txtISA = new System.Windows.Forms.TextBox();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.txtHex = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.cmbAsic = new System.Windows.Forms.ComboBox();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.txtEncodings = new System.Windows.Forms.TextBox();
            this.tabControl1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Alignment = System.Windows.Forms.TabAlignment.Bottom;
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Location = new System.Drawing.Point(3, 38);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(262, 185);
            this.tabControl1.TabIndex = 5;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.txtISA);
            this.tabPage2.Location = new System.Drawing.Point(4, 4);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(254, 159);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Asm";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // txtISA
            // 
            this.txtISA.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtISA.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtISA.Location = new System.Drawing.Point(3, 3);
            this.txtISA.Multiline = true;
            this.txtISA.Name = "txtISA";
            this.txtISA.ReadOnly = true;
            this.txtISA.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtISA.Size = new System.Drawing.Size(248, 153);
            this.txtISA.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.txtHex);
            this.tabPage1.Location = new System.Drawing.Point(4, 4);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(254, 159);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Hex";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // txtHex
            // 
            this.txtHex.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtHex.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtHex.Location = new System.Drawing.Point(3, 3);
            this.txtHex.Multiline = true;
            this.txtHex.Name = "txtHex";
            this.txtHex.ReadOnly = true;
            this.txtHex.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtHex.Size = new System.Drawing.Size(248, 153);
            this.txtHex.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(31, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(27, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Asic";
            // 
            // cmbAsic
            // 
            this.cmbAsic.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmbAsic.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbAsic.FormattingEnabled = true;
            this.cmbAsic.Location = new System.Drawing.Point(70, 11);
            this.cmbAsic.Name = "cmbAsic";
            this.cmbAsic.Size = new System.Drawing.Size(166, 21);
            this.cmbAsic.TabIndex = 3;
            this.cmbAsic.SelectedIndexChanged += new System.EventHandler(this.cmbAsic_SelectedIndexChanged);
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.txtEncodings);
            this.tabPage3.Location = new System.Drawing.Point(4, 4);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(254, 159);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Encodings";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // txtEncodings
            // 
            this.txtEncodings.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtEncodings.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtEncodings.Location = new System.Drawing.Point(3, 3);
            this.txtEncodings.Multiline = true;
            this.txtEncodings.Name = "txtEncodings";
            this.txtEncodings.ReadOnly = true;
            this.txtEncodings.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtEncodings.Size = new System.Drawing.Size(248, 153);
            this.txtEncodings.TabIndex = 1;
            // 
            // AMDDriverResultsPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cmbAsic);
            this.Name = "AMDDriverResultsPanel";
            this.Size = new System.Drawing.Size(268, 235);
            this.tabControl1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TextBox txtISA;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TextBox txtHex;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cmbAsic;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TextBox txtEncodings;

    }
}
