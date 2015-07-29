namespace Pyramid.Scrutinizer.UI
{
    partial class ScrutinizerForm
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.panel1 = new System.Windows.Forms.Panel();
            this.lblIterations = new System.Windows.Forms.Label();
            this.txtLoopCount = new System.Windows.Forms.TextBox();
            this.cfgWidget1 = new Pyramid.Scrutinizer.UI.CFGWidget();
            this.btnSimulate = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.AutoScroll = true;
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel1.Location = new System.Drawing.Point(13, 13);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(622, 460);
            this.panel1.TabIndex = 2;
            this.panel1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.panel1_MouseDown);
            // 
            // lblIterations
            // 
            this.lblIterations.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblIterations.AutoSize = true;
            this.lblIterations.Location = new System.Drawing.Point(676, 191);
            this.lblIterations.Name = "lblIterations";
            this.lblIterations.Size = new System.Drawing.Size(50, 13);
            this.lblIterations.TabIndex = 4;
            this.lblIterations.Text = "Iterations";
            // 
            // txtLoopCount
            // 
            this.txtLoopCount.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.txtLoopCount.Location = new System.Drawing.Point(728, 188);
            this.txtLoopCount.Name = "txtLoopCount";
            this.txtLoopCount.Size = new System.Drawing.Size(100, 20);
            this.txtLoopCount.TabIndex = 3;
            this.txtLoopCount.WordWrap = false;
            this.txtLoopCount.TextChanged += new System.EventHandler(this.txtLoopCount_TextChanged);
            // 
            // cfgWidget1
            // 
            this.cfgWidget1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cfgWidget1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.cfgWidget1.Location = new System.Drawing.Point(650, 12);
            this.cfgWidget1.Name = "cfgWidget1";
            this.cfgWidget1.Size = new System.Drawing.Size(178, 170);
            this.cfgWidget1.TabIndex = 1;
            this.cfgWidget1.BlockSelected += new Pyramid.Scrutinizer.UI.CFGWidget.BlockSelectionHandler(this.cfgWidget1_BlockSelected);
            this.cfgWidget1.LoopSelected += new Pyramid.Scrutinizer.UI.CFGWidget.LoopSelectionHandler(this.cfgWidget1_LoopSelected);
            this.cfgWidget1.SelectionCleared += new Pyramid.Scrutinizer.UI.CFGWidget.SelectionClearedHandler(this.cfgWidget1_SelectionCleared);
            // 
            // btnSimulate
            // 
            this.btnSimulate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSimulate.Location = new System.Drawing.Point(663, 217);
            this.btnSimulate.Name = "btnSimulate";
            this.btnSimulate.Size = new System.Drawing.Size(62, 21);
            this.btnSimulate.TabIndex = 5;
            this.btnSimulate.Text = "Simulate";
            this.btnSimulate.UseVisualStyleBackColor = true;
            this.btnSimulate.Click += new System.EventHandler(this.btnSimulate_Click);
            // 
            // ScrutinizerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(840, 494);
            this.Controls.Add(this.btnSimulate);
            this.Controls.Add(this.lblIterations);
            this.Controls.Add(this.txtLoopCount);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.cfgWidget1);
            this.Name = "ScrutinizerForm";
            this.Text = "WS";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private CFGWidget cfgWidget1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label lblIterations;
        private System.Windows.Forms.TextBox txtLoopCount;
        private System.Windows.Forms.Button btnSimulate;
    }
}