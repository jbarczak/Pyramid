namespace Pyramid
{
    partial class MysteryToolResultsPanel
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
            this.txtOutput = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.cmbSKU = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // txtOutput
            // 
            this.txtOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtOutput.Location = new System.Drawing.Point(3, 42);
            this.txtOutput.Multiline = true;
            this.txtOutput.Name = "txtOutput";
            this.txtOutput.ReadOnly = true;
            this.txtOutput.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtOutput.Size = new System.Drawing.Size(437, 311);
            this.txtOutput.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 18);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "SKU";
            // 
            // cmbSKU
            // 
            this.cmbSKU.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbSKU.FormattingEnabled = true;
            this.cmbSKU.Location = new System.Drawing.Point(53, 9);
            this.cmbSKU.Name = "cmbSKU";
            this.cmbSKU.Size = new System.Drawing.Size(144, 21);
            this.cmbSKU.TabIndex = 3;
            this.cmbSKU.SelectedIndexChanged += new System.EventHandler(this.cmbSKU_SelectedIndexChanged);
            // 
            // MysteryToolResultsPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.cmbSKU);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.txtOutput);
            this.Name = "MysteryToolResultsPanel";
            this.Size = new System.Drawing.Size(443, 356);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cmbSKU;
        private System.Windows.Forms.TextBox txtOutput;
    }
}
