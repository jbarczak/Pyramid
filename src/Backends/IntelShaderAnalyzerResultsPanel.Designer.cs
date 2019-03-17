namespace Pyramid
{
    partial class IntelShaderAnalyzerResultsPanel
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
            this.cmbOutputFile = new System.Windows.Forms.ComboBox();
            this.cmbAPI = new System.Windows.Forms.ComboBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.txtAsm = new System.Windows.Forms.TextBox();
            this.txtMessages = new System.Windows.Forms.TextBox();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // cmbOutputFile
            // 
            this.cmbOutputFile.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbOutputFile.FormattingEnabled = true;
            this.cmbOutputFile.Location = new System.Drawing.Point(161, 19);
            this.cmbOutputFile.Margin = new System.Windows.Forms.Padding(4);
            this.cmbOutputFile.Name = "cmbOutputFile";
            this.cmbOutputFile.Size = new System.Drawing.Size(250, 24);
            this.cmbOutputFile.TabIndex = 5;
            this.cmbOutputFile.SelectedIndexChanged += new System.EventHandler(this.cmbOutputFile_SelectedIndexChanged);
            // 
            // cmbAPI
            // 
            this.cmbAPI.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbAPI.FormattingEnabled = true;
            this.cmbAPI.Location = new System.Drawing.Point(35, 19);
            this.cmbAPI.Margin = new System.Windows.Forms.Padding(4);
            this.cmbAPI.Name = "cmbAPI";
            this.cmbAPI.Size = new System.Drawing.Size(104, 24);
            this.cmbAPI.TabIndex = 6;
            this.cmbAPI.SelectedIndexChanged += new System.EventHandler(this.cmbAPI_SelectedIndexChanged);
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(3, 50);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(620, 409);
            this.tabControl1.TabIndex = 7;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.txtAsm);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(612, 380);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Asm";
            this.tabPage1.UseVisualStyleBackColor = true;
            this.tabPage1.Click += new System.EventHandler(this.tabPage1_Click);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.txtMessages);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(612, 380);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Messages";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // txtAsm
            // 
            this.txtAsm.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtAsm.Font = new System.Drawing.Font("Consolas", 9F);
            this.txtAsm.Location = new System.Drawing.Point(3, 3);
            this.txtAsm.Margin = new System.Windows.Forms.Padding(4);
            this.txtAsm.Multiline = true;
            this.txtAsm.Name = "txtAsm";
            this.txtAsm.ReadOnly = true;
            this.txtAsm.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtAsm.Size = new System.Drawing.Size(606, 374);
            this.txtAsm.TabIndex = 5;
            this.txtAsm.WordWrap = false;
            // 
            // txtMessages
            // 
            this.txtMessages.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtMessages.Font = new System.Drawing.Font("Consolas", 9F);
            this.txtMessages.Location = new System.Drawing.Point(3, 3);
            this.txtMessages.Margin = new System.Windows.Forms.Padding(4);
            this.txtMessages.Multiline = true;
            this.txtMessages.Name = "txtMessages";
            this.txtMessages.ReadOnly = true;
            this.txtMessages.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtMessages.Size = new System.Drawing.Size(606, 374);
            this.txtMessages.TabIndex = 6;
            // 
            // IntelShaderAnalyzerResultsPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.cmbAPI);
            this.Controls.Add(this.cmbOutputFile);
            this.Name = "IntelShaderAnalyzerResultsPanel";
            this.Size = new System.Drawing.Size(623, 462);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.ComboBox cmbOutputFile;
        private System.Windows.Forms.ComboBox cmbAPI;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TextBox txtAsm;
        private System.Windows.Forms.TextBox txtMessages;
    }
}
