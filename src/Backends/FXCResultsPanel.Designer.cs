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
            this.messagesPage = new System.Windows.Forms.TabPage();
            this.hexPage = new System.Windows.Forms.TabPage();
            this.txtASM = new System.Windows.Forms.TextBox();
            this.txtMessages = new System.Windows.Forms.TextBox();
            this.txtHex = new System.Windows.Forms.TextBox();
            this.tabControl1.SuspendLayout();
            this.asmPage.SuspendLayout();
            this.messagesPage.SuspendLayout();
            this.hexPage.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.asmPage);
            this.tabControl1.Controls.Add(this.hexPage);
            this.tabControl1.Controls.Add(this.messagesPage);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(150, 150);
            this.tabControl1.TabIndex = 0;
            // 
            // asmPage
            // 
            this.asmPage.Controls.Add(this.txtASM);
            this.asmPage.Location = new System.Drawing.Point(4, 22);
            this.asmPage.Name = "asmPage";
            this.asmPage.Padding = new System.Windows.Forms.Padding(3);
            this.asmPage.Size = new System.Drawing.Size(142, 124);
            this.asmPage.TabIndex = 0;
            this.asmPage.Text = "ASM";
            this.asmPage.UseVisualStyleBackColor = true;
            // 
            // messagesPage
            // 
            this.messagesPage.Controls.Add(this.txtMessages);
            this.messagesPage.Location = new System.Drawing.Point(4, 22);
            this.messagesPage.Name = "messagesPage";
            this.messagesPage.Padding = new System.Windows.Forms.Padding(3);
            this.messagesPage.Size = new System.Drawing.Size(142, 124);
            this.messagesPage.TabIndex = 1;
            this.messagesPage.Text = "Messages";
            this.messagesPage.UseVisualStyleBackColor = true;
            // 
            // hexPage
            // 
            this.hexPage.Controls.Add(this.txtHex);
            this.hexPage.Location = new System.Drawing.Point(4, 22);
            this.hexPage.Name = "hexPage";
            this.hexPage.Size = new System.Drawing.Size(142, 124);
            this.hexPage.TabIndex = 2;
            this.hexPage.Text = "Hex";
            this.hexPage.UseVisualStyleBackColor = true;
            // 
            // txtASM
            // 
            this.txtASM.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtASM.Location = new System.Drawing.Point(3, 3);
            this.txtASM.Multiline = true;
            this.txtASM.Name = "txtASM";
            this.txtASM.ReadOnly = true;
            this.txtASM.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtASM.Size = new System.Drawing.Size(136, 118);
            this.txtASM.TabIndex = 0;
            // 
            // txtMessages
            // 
            this.txtMessages.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtMessages.Location = new System.Drawing.Point(3, 3);
            this.txtMessages.Multiline = true;
            this.txtMessages.Name = "txtMessages";
            this.txtMessages.ReadOnly = true;
            this.txtMessages.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtMessages.Size = new System.Drawing.Size(136, 118);
            this.txtMessages.TabIndex = 1;
            // 
            // txtHex
            // 
            this.txtHex.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtHex.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtHex.Location = new System.Drawing.Point(0, 0);
            this.txtHex.Multiline = true;
            this.txtHex.Name = "txtHex";
            this.txtHex.ReadOnly = true;
            this.txtHex.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtHex.Size = new System.Drawing.Size(142, 124);
            this.txtHex.TabIndex = 3;
            // 
            // FXCResultsPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabControl1);
            this.Name = "FXCResultsPanel";
            this.tabControl1.ResumeLayout(false);
            this.asmPage.ResumeLayout(false);
            this.asmPage.PerformLayout();
            this.messagesPage.ResumeLayout(false);
            this.messagesPage.PerformLayout();
            this.hexPage.ResumeLayout(false);
            this.hexPage.PerformLayout();
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
    }
}
