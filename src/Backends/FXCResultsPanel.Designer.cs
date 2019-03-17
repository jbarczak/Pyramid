namespace Pyramid
{
    partial class FXCResultsPanel
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
            this.asmPage = new System.Windows.Forms.TabPage();
            this.txtASM = new System.Windows.Forms.TextBox();
            this.hexPage = new System.Windows.Forms.TabPage();
            this.txtHex = new System.Windows.Forms.TextBox();
            this.messagesPage = new System.Windows.Forms.TabPage();
            this.txtMessages = new System.Windows.Forms.TextBox();
            this.rootSigPage = new System.Windows.Forms.TabPage();
            this.exportPage = new System.Windows.Forms.TabPage();
            this.txtRootSig = new System.Windows.Forms.TextBox();
            this.btnExportBytecode = new System.Windows.Forms.Button();
            this.btnExportRootSig = new System.Windows.Forms.Button();
            this.btnExportCPP = new System.Windows.Forms.Button();
            this.tabControl1.SuspendLayout();
            this.asmPage.SuspendLayout();
            this.hexPage.SuspendLayout();
            this.messagesPage.SuspendLayout();
            this.rootSigPage.SuspendLayout();
            this.exportPage.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.asmPage);
            this.tabControl1.Controls.Add(this.hexPage);
            this.tabControl1.Controls.Add(this.messagesPage);
            this.tabControl1.Controls.Add(this.rootSigPage);
            this.tabControl1.Controls.Add(this.exportPage);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(200, 185);
            this.tabControl1.TabIndex = 0;
            // 
            // asmPage
            // 
            this.asmPage.Controls.Add(this.txtASM);
            this.asmPage.Location = new System.Drawing.Point(4, 25);
            this.asmPage.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.asmPage.Name = "asmPage";
            this.asmPage.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.asmPage.Size = new System.Drawing.Size(192, 156);
            this.asmPage.TabIndex = 0;
            this.asmPage.Text = "ASM";
            this.asmPage.UseVisualStyleBackColor = true;
            // 
            // txtASM
            // 
            this.txtASM.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtASM.Location = new System.Drawing.Point(4, 4);
            this.txtASM.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txtASM.Multiline = true;
            this.txtASM.Name = "txtASM";
            this.txtASM.ReadOnly = true;
            this.txtASM.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtASM.Size = new System.Drawing.Size(184, 148);
            this.txtASM.TabIndex = 0;
            // 
            // hexPage
            // 
            this.hexPage.Controls.Add(this.txtHex);
            this.hexPage.Location = new System.Drawing.Point(4, 25);
            this.hexPage.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.hexPage.Name = "hexPage";
            this.hexPage.Size = new System.Drawing.Size(192, 156);
            this.hexPage.TabIndex = 2;
            this.hexPage.Text = "Hex";
            this.hexPage.UseVisualStyleBackColor = true;
            // 
            // txtHex
            // 
            this.txtHex.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtHex.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtHex.Location = new System.Drawing.Point(0, 0);
            this.txtHex.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txtHex.Multiline = true;
            this.txtHex.Name = "txtHex";
            this.txtHex.ReadOnly = true;
            this.txtHex.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtHex.Size = new System.Drawing.Size(192, 156);
            this.txtHex.TabIndex = 3;
            // 
            // messagesPage
            // 
            this.messagesPage.Controls.Add(this.txtMessages);
            this.messagesPage.Location = new System.Drawing.Point(4, 25);
            this.messagesPage.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.messagesPage.Name = "messagesPage";
            this.messagesPage.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.messagesPage.Size = new System.Drawing.Size(192, 156);
            this.messagesPage.TabIndex = 1;
            this.messagesPage.Text = "Messages";
            this.messagesPage.UseVisualStyleBackColor = true;
            // 
            // txtMessages
            // 
            this.txtMessages.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtMessages.Location = new System.Drawing.Point(4, 4);
            this.txtMessages.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txtMessages.Multiline = true;
            this.txtMessages.Name = "txtMessages";
            this.txtMessages.ReadOnly = true;
            this.txtMessages.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtMessages.Size = new System.Drawing.Size(184, 148);
            this.txtMessages.TabIndex = 1;
            // 
            // rootSigPage
            // 
            this.rootSigPage.Controls.Add(this.txtRootSig);
            this.rootSigPage.Location = new System.Drawing.Point(4, 25);
            this.rootSigPage.Name = "rootSigPage";
            this.rootSigPage.Size = new System.Drawing.Size(192, 156);
            this.rootSigPage.TabIndex = 3;
            this.rootSigPage.Text = "RootSig";
            this.rootSigPage.UseVisualStyleBackColor = true;
            // 
            // exportPage
            // 
            this.exportPage.Controls.Add(this.btnExportCPP);
            this.exportPage.Controls.Add(this.btnExportRootSig);
            this.exportPage.Controls.Add(this.btnExportBytecode);
            this.exportPage.Location = new System.Drawing.Point(4, 25);
            this.exportPage.Name = "exportPage";
            this.exportPage.Size = new System.Drawing.Size(192, 156);
            this.exportPage.TabIndex = 4;
            this.exportPage.Text = "Export";
            this.exportPage.UseVisualStyleBackColor = true;
            // 
            // txtRootSig
            // 
            this.txtRootSig.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtRootSig.Font = new System.Drawing.Font("Consolas", 8.5F);
            this.txtRootSig.Location = new System.Drawing.Point(0, 0);
            this.txtRootSig.Margin = new System.Windows.Forms.Padding(4);
            this.txtRootSig.Multiline = true;
            this.txtRootSig.Name = "txtRootSig";
            this.txtRootSig.ReadOnly = true;
            this.txtRootSig.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtRootSig.Size = new System.Drawing.Size(192, 156);
            this.txtRootSig.TabIndex = 2;
            // 
            // btnExportBytecode
            // 
            this.btnExportBytecode.Location = new System.Drawing.Point(12, 18);
            this.btnExportBytecode.Name = "btnExportBytecode";
            this.btnExportBytecode.Size = new System.Drawing.Size(134, 35);
            this.btnExportBytecode.TabIndex = 0;
            this.btnExportBytecode.Text = "Save Bytecode";
            this.btnExportBytecode.UseVisualStyleBackColor = true;
            this.btnExportBytecode.Click += new System.EventHandler(this.btnExportBytecode_Click);
            // 
            // btnExportRootSig
            // 
            this.btnExportRootSig.Location = new System.Drawing.Point(12, 59);
            this.btnExportRootSig.Name = "btnExportRootSig";
            this.btnExportRootSig.Size = new System.Drawing.Size(134, 35);
            this.btnExportRootSig.TabIndex = 1;
            this.btnExportRootSig.Text = "Save RootSig";
            this.btnExportRootSig.UseVisualStyleBackColor = true;
            this.btnExportRootSig.Click += new System.EventHandler(this.btnExportRootSig_Click);
            // 
            // btnExportCPP
            // 
            this.btnExportCPP.Location = new System.Drawing.Point(12, 100);
            this.btnExportCPP.Name = "btnExportCPP";
            this.btnExportCPP.Size = new System.Drawing.Size(134, 35);
            this.btnExportCPP.TabIndex = 2;
            this.btnExportCPP.Text = "Create .cpp File";
            this.btnExportCPP.UseVisualStyleBackColor = true;
            this.btnExportCPP.Click += new System.EventHandler(this.btnExportCPP_Click);
            // 
            // FXCResultsPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabControl1);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "FXCResultsPanel";
            this.Size = new System.Drawing.Size(200, 185);
            this.tabControl1.ResumeLayout(false);
            this.asmPage.ResumeLayout(false);
            this.asmPage.PerformLayout();
            this.hexPage.ResumeLayout(false);
            this.hexPage.PerformLayout();
            this.messagesPage.ResumeLayout(false);
            this.messagesPage.PerformLayout();
            this.rootSigPage.ResumeLayout(false);
            this.rootSigPage.PerformLayout();
            this.exportPage.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage messagesPage;
        private System.Windows.Forms.TextBox txtASM;
        private System.Windows.Forms.TextBox txtMessages;
        private System.Windows.Forms.TabPage hexPage;
        private System.Windows.Forms.TextBox txtHex;
        private System.Windows.Forms.TabPage asmPage;
        private System.Windows.Forms.TabPage rootSigPage;
        private System.Windows.Forms.TextBox txtRootSig;
        private System.Windows.Forms.TabPage exportPage;
        private System.Windows.Forms.Button btnExportCPP;
        private System.Windows.Forms.Button btnExportRootSig;
        private System.Windows.Forms.Button btnExportBytecode;
    }
}
