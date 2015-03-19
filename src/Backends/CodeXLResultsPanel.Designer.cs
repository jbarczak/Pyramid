namespace Pyramid
{
    partial class AMDResultsPanel
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
            this.cmbAsic = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.txtIL = new System.Windows.Forms.TextBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.txtISA = new System.Windows.Forms.TextBox();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // cmbAsic
            // 
            this.cmbAsic.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmbAsic.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbAsic.FormattingEnabled = true;
            this.cmbAsic.Location = new System.Drawing.Point(55, 8);
            this.cmbAsic.Name = "cmbAsic";
            this.cmbAsic.Size = new System.Drawing.Size(166, 21);
            this.cmbAsic.TabIndex = 0;
            this.cmbAsic.SelectedIndexChanged += new System.EventHandler(this.cmbAsic_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(27, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Asic";
            // 
            // tabControl1
            // 
            this.tabControl1.Alignment = System.Windows.Forms.TabAlignment.Bottom;
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Location = new System.Drawing.Point(3, 35);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(232, 186);
            this.tabControl1.TabIndex = 2;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.txtIL);
            this.tabPage1.Location = new System.Drawing.Point(4, 4);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(224, 160);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "IL";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // txtIL
            // 
            this.txtIL.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtIL.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtIL.Location = new System.Drawing.Point(3, 3);
            this.txtIL.Multiline = true;
            this.txtIL.Name = "txtIL";
            this.txtIL.ReadOnly = true;
            this.txtIL.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtIL.Size = new System.Drawing.Size(218, 154);
            this.txtIL.TabIndex = 0;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.txtISA);
            this.tabPage2.Location = new System.Drawing.Point(4, 4);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(224, 160);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "ISA";
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
            this.txtISA.Size = new System.Drawing.Size(218, 154);
            this.txtISA.TabIndex = 0;
            // 
            // AMDResultsPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cmbAsic);
            this.Name = "AMDResultsPanel";
            this.Size = new System.Drawing.Size(238, 236);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cmbAsic;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TextBox txtIL;
        private System.Windows.Forms.TextBox txtISA;
    }
}
