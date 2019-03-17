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
            this.txtRootSig = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.cmbRootSignature = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 14);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(43, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "HLSL";
            // 
            // cmbTarget
            // 
            this.cmbTarget.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbTarget.FormattingEnabled = true;
            this.cmbTarget.Location = new System.Drawing.Point(120, 46);
            this.cmbTarget.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.cmbTarget.Name = "cmbTarget";
            this.cmbTarget.Size = new System.Drawing.Size(160, 24);
            this.cmbTarget.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(17, 49);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(50, 17);
            this.label2.TabIndex = 2;
            this.label2.Text = "Target";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(17, 80);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(31, 17);
            this.label3.TabIndex = 3;
            this.label3.Text = "Opt";
            // 
            // cmbOpt
            // 
            this.cmbOpt.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbOpt.FormattingEnabled = true;
            this.cmbOpt.Location = new System.Drawing.Point(120, 80);
            this.cmbOpt.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.cmbOpt.Name = "cmbOpt";
            this.cmbOpt.Size = new System.Drawing.Size(160, 24);
            this.cmbOpt.TabIndex = 4;
            // 
            // txtEntryPoint
            // 
            this.txtEntryPoint.Location = new System.Drawing.Point(120, 112);
            this.txtEntryPoint.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txtEntryPoint.Name = "txtEntryPoint";
            this.txtEntryPoint.Size = new System.Drawing.Size(159, 22);
            this.txtEntryPoint.TabIndex = 5;
            this.txtEntryPoint.Text = "main";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 116);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(73, 17);
            this.label4.TabIndex = 6;
            this.label4.Text = "EntryPoint";
            // 
            // chkCompat
            // 
            this.chkCompat.AutoSize = true;
            this.chkCompat.Location = new System.Drawing.Point(120, 142);
            this.chkCompat.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.chkCompat.Name = "chkCompat";
            this.chkCompat.Size = new System.Drawing.Size(78, 21);
            this.chkCompat.TabIndex = 7;
            this.chkCompat.Text = "Compat";
            this.chkCompat.UseVisualStyleBackColor = true;
            // 
            // txtRootSig
            // 
            this.txtRootSig.Location = new System.Drawing.Point(120, 191);
            this.txtRootSig.Margin = new System.Windows.Forms.Padding(4);
            this.txtRootSig.Name = "txtRootSig";
            this.txtRootSig.Size = new System.Drawing.Size(159, 22);
            this.txtRootSig.TabIndex = 8;
            this.txtRootSig.Text = "RootSignature";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(9, 194);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(90, 17);
            this.label5.TabIndex = 9;
            this.label5.Text = "RootSig #def";
            // 
            // cmbRootSignature
            // 
            this.cmbRootSignature.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbRootSignature.FormattingEnabled = true;
            this.cmbRootSignature.Location = new System.Drawing.Point(119, 221);
            this.cmbRootSignature.Margin = new System.Windows.Forms.Padding(4);
            this.cmbRootSignature.Name = "cmbRootSignature";
            this.cmbRootSignature.Size = new System.Drawing.Size(160, 24);
            this.cmbRootSignature.TabIndex = 10;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(9, 221);
            this.label6.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(104, 17);
            this.label6.TabIndex = 11;
            this.label6.Text = "RootSig Target";
            // 
            // HLSLOptionsPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.label6);
            this.Controls.Add(this.cmbRootSignature);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.txtRootSig);
            this.Controls.Add(this.chkCompat);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.txtEntryPoint);
            this.Controls.Add(this.cmbOpt);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cmbTarget);
            this.Controls.Add(this.label1);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "HLSLOptionsPanel";
            this.Size = new System.Drawing.Size(310, 267);
            this.Load += new System.EventHandler(this.HLSLOptionsPanel_Load);
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
        private System.Windows.Forms.TextBox txtRootSig;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox cmbRootSignature;
        private System.Windows.Forms.Label label6;
    }
}
