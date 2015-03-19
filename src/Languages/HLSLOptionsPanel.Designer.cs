namespace Pyramid
{
    partial class HLSLOptionsPanel
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
            this.label1 = new System.Windows.Forms.Label();
            this.cmbTarget = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.cmbOpt = new System.Windows.Forms.ComboBox();
            this.txtEntryPoint = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.chkCompat = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(34, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "HLSL";
            // 
            // cmbTarget
            // 
            this.cmbTarget.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbTarget.FormattingEnabled = true;
            this.cmbTarget.Location = new System.Drawing.Point(64, 37);
            this.cmbTarget.Name = "cmbTarget";
            this.cmbTarget.Size = new System.Drawing.Size(121, 21);
            this.cmbTarget.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 40);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Target";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 65);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(24, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "Opt";
            // 
            // cmbOpt
            // 
            this.cmbOpt.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbOpt.FormattingEnabled = true;
            this.cmbOpt.Location = new System.Drawing.Point(64, 65);
            this.cmbOpt.Name = "cmbOpt";
            this.cmbOpt.Size = new System.Drawing.Size(121, 21);
            this.cmbOpt.TabIndex = 4;
            // 
            // txtEntryPoint
            // 
            this.txtEntryPoint.Location = new System.Drawing.Point(64, 91);
            this.txtEntryPoint.Name = "txtEntryPoint";
            this.txtEntryPoint.Size = new System.Drawing.Size(120, 20);
            this.txtEntryPoint.TabIndex = 5;
            this.txtEntryPoint.Text = "main";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(7, 94);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(55, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "EntryPoint";
            // 
            // chkCompat
            // 
            this.chkCompat.AutoSize = true;
            this.chkCompat.Location = new System.Drawing.Point(64, 117);
            this.chkCompat.Name = "chkCompat";
            this.chkCompat.Size = new System.Drawing.Size(62, 17);
            this.chkCompat.TabIndex = 7;
            this.chkCompat.Text = "Compat";
            this.chkCompat.UseVisualStyleBackColor = true;
            // 
            // HLSLOptionsPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.chkCompat);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.txtEntryPoint);
            this.Controls.Add(this.cmbOpt);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cmbTarget);
            this.Controls.Add(this.label1);
            this.Name = "HLSLOptionsPanel";
            this.Size = new System.Drawing.Size(204, 150);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cmbTarget;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox cmbOpt;
        private System.Windows.Forms.TextBox txtEntryPoint;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox chkCompat;
    }
}
