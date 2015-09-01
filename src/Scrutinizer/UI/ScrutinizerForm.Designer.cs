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
            this.btnSimulate = new System.Windows.Forms.Button();
            this.lblACMR = new System.Windows.Forms.Label();
            this.txtACMR = new System.Windows.Forms.TextBox();
            this.txtPixels = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.txtCUCount = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.txtOccupancy = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.chkTaken = new System.Windows.Forms.CheckBox();
            this.cfgWidget1 = new Pyramid.Scrutinizer.UI.CFGWidget();
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
            this.panel1.Size = new System.Drawing.Size(561, 460);
            this.panel1.TabIndex = 2;
            this.panel1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.panel1_MouseDown);
            // 
            // lblIterations
            // 
            this.lblIterations.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblIterations.AutoSize = true;
            this.lblIterations.Location = new System.Drawing.Point(676, 268);
            this.lblIterations.Name = "lblIterations";
            this.lblIterations.Size = new System.Drawing.Size(50, 13);
            this.lblIterations.TabIndex = 4;
            this.lblIterations.Text = "Iterations";
            // 
            // txtLoopCount
            // 
            this.txtLoopCount.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.txtLoopCount.Location = new System.Drawing.Point(728, 265);
            this.txtLoopCount.Name = "txtLoopCount";
            this.txtLoopCount.Size = new System.Drawing.Size(100, 20);
            this.txtLoopCount.TabIndex = 3;
            this.txtLoopCount.WordWrap = false;
            this.txtLoopCount.TextChanged += new System.EventHandler(this.txtLoopCount_TextChanged);
            // 
            // btnSimulate
            // 
            this.btnSimulate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSimulate.Location = new System.Drawing.Point(664, 326);
            this.btnSimulate.Name = "btnSimulate";
            this.btnSimulate.Size = new System.Drawing.Size(62, 21);
            this.btnSimulate.TabIndex = 5;
            this.btnSimulate.Text = "Simulate";
            this.btnSimulate.UseVisualStyleBackColor = true;
            this.btnSimulate.Click += new System.EventHandler(this.btnSimulate_Click);
            // 
            // lblACMR
            // 
            this.lblACMR.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblACMR.AutoSize = true;
            this.lblACMR.Location = new System.Drawing.Point(678, 357);
            this.lblACMR.Name = "lblACMR";
            this.lblACMR.Size = new System.Drawing.Size(48, 13);
            this.lblACMR.TabIndex = 6;
            this.lblACMR.Text = "Verts/Tri";
            // 
            // txtACMR
            // 
            this.txtACMR.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.txtACMR.Location = new System.Drawing.Point(728, 354);
            this.txtACMR.Name = "txtACMR";
            this.txtACMR.Size = new System.Drawing.Size(41, 20);
            this.txtACMR.TabIndex = 7;
            this.txtACMR.Text = "1";
            // 
            // txtPixels
            // 
            this.txtPixels.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.txtPixels.Location = new System.Drawing.Point(728, 380);
            this.txtPixels.Name = "txtPixels";
            this.txtPixels.Size = new System.Drawing.Size(41, 20);
            this.txtPixels.TabIndex = 9;
            this.txtPixels.Text = "64";
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(678, 383);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(51, 13);
            this.label1.TabIndex = 8;
            this.label1.Text = "Pixels/Tri";
            // 
            // txtCUCount
            // 
            this.txtCUCount.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.txtCUCount.Location = new System.Drawing.Point(728, 422);
            this.txtCUCount.Name = "txtCUCount";
            this.txtCUCount.Size = new System.Drawing.Size(41, 20);
            this.txtCUCount.TabIndex = 10;
            this.txtCUCount.Text = "11";
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(691, 425);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(27, 13);
            this.label2.TabIndex = 11;
            this.label2.Text = "CUs";
            // 
            // txtOccupancy
            // 
            this.txtOccupancy.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.txtOccupancy.Location = new System.Drawing.Point(728, 448);
            this.txtOccupancy.Name = "txtOccupancy";
            this.txtOccupancy.Size = new System.Drawing.Size(41, 20);
            this.txtOccupancy.TabIndex = 12;
            this.txtOccupancy.Text = "0";
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(653, 451);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(73, 13);
            this.label3.TabIndex = 13;
            this.label3.Text = "Waves/SIMD";
            // 
            // chkTaken
            // 
            this.chkTaken.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.chkTaken.AutoSize = true;
            this.chkTaken.Location = new System.Drawing.Point(590, 266);
            this.chkTaken.Name = "chkTaken";
            this.chkTaken.Size = new System.Drawing.Size(57, 17);
            this.chkTaken.TabIndex = 14;
            this.chkTaken.Text = "Taken";
            this.chkTaken.UseVisualStyleBackColor = true;
            this.chkTaken.Visible = false;
            this.chkTaken.CheckedChanged += new System.EventHandler(this.chkTaken_CheckedChanged);
            // 
            // cfgWidget1
            // 
            this.cfgWidget1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cfgWidget1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.cfgWidget1.Location = new System.Drawing.Point(580, 12);
            this.cfgWidget1.Name = "cfgWidget1";
            this.cfgWidget1.Size = new System.Drawing.Size(248, 247);
            this.cfgWidget1.TabIndex = 1;
            this.cfgWidget1.BlockSelected += new Pyramid.Scrutinizer.UI.CFGWidget.BlockSelectedHandler(this.cfgWidget1_BlockSelected);
            this.cfgWidget1.LoopSelected += new Pyramid.Scrutinizer.UI.CFGWidget.LoopSelectionHandler(this.cfgWidget1_LoopSelected);
            this.cfgWidget1.BranchSelected += new Pyramid.Scrutinizer.UI.CFGWidget.BranchSelectedHandler(this.cfgWidget1_BranchSelected);
            this.cfgWidget1.BranchTargetSelected += new Pyramid.Scrutinizer.UI.CFGWidget.BranchTargetSelectedHandler(this.cfgWidget1_BranchTargetSelected);
            this.cfgWidget1.SelectionCleared += new Pyramid.Scrutinizer.UI.CFGWidget.SelectionClearedHandler(this.cfgWidget1_SelectionCleared);
            // 
            // ScrutinizerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(840, 494);
            this.Controls.Add(this.chkTaken);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.txtOccupancy);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.txtCUCount);
            this.Controls.Add(this.txtPixels);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.txtACMR);
            this.Controls.Add(this.lblACMR);
            this.Controls.Add(this.btnSimulate);
            this.Controls.Add(this.lblIterations);
            this.Controls.Add(this.txtLoopCount);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.cfgWidget1);
            this.Name = "ScrutinizerForm";
            this.Text = "Scrutinizer";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private CFGWidget cfgWidget1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label lblIterations;
        private System.Windows.Forms.TextBox txtLoopCount;
        private System.Windows.Forms.Button btnSimulate;
        private System.Windows.Forms.Label lblACMR;
        private System.Windows.Forms.TextBox txtACMR;
        private System.Windows.Forms.TextBox txtPixels;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtCUCount;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtOccupancy;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.CheckBox chkTaken;
    }
}