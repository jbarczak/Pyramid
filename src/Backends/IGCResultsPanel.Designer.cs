namespace Pyramid
{
    partial class IGCResultsPanel
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
            this.cmbOutputFile = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // txtOutput
            // 
            this.txtOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtOutput.Font = new System.Drawing.Font("Consolas", 9F);
            this.txtOutput.Location = new System.Drawing.Point(4, 44);
            this.txtOutput.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txtOutput.Multiline = true;
            this.txtOutput.Name = "txtOutput";
            this.txtOutput.ReadOnly = true;
            this.txtOutput.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtOutput.Size = new System.Drawing.Size(672, 406);
            this.txtOutput.TabIndex = 2;
            // 
            // cmbOutputFile
            // 
            this.cmbOutputFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmbOutputFile.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbOutputFile.FormattingEnabled = true;
            this.cmbOutputFile.Location = new System.Drawing.Point(60, 11);
            this.cmbOutputFile.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.cmbOutputFile.Name = "cmbOutputFile";
            this.cmbOutputFile.Size = new System.Drawing.Size(554, 24);
            this.cmbOutputFile.TabIndex = 4;
            this.cmbOutputFile.SelectedIndexChanged += new System.EventHandler(this.cmbOutputFile_SelectedIndexChanged);
            // 
            // IGCResultsPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.cmbOutputFile);
            this.Controls.Add(this.txtOutput);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "IGCResultsPanel";
            this.Size = new System.Drawing.Size(681, 455);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txtOutput;
        private System.Windows.Forms.ComboBox cmbOutputFile;
    }
}
