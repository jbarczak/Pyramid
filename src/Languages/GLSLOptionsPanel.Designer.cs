namespace Pyramid
{
    partial class GLSLOptionsPanel
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
            this.cmbShaderType = new System.Windows.Forms.ComboBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label3 = new System.Windows.Forms.Label();
            this.upUnrollIter = new System.Windows.Forms.NumericUpDown();
            this.cmbGLSLOptTarget = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.upUnrollIter)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(5, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Type";
            // 
            // cmbShaderType
            // 
            this.cmbShaderType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbShaderType.FormattingEnabled = true;
            this.cmbShaderType.Location = new System.Drawing.Point(42, 9);
            this.cmbShaderType.Name = "cmbShaderType";
            this.cmbShaderType.Size = new System.Drawing.Size(135, 21);
            this.cmbShaderType.TabIndex = 1;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.upUnrollIter);
            this.groupBox1.Controls.Add(this.cmbGLSLOptTarget);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Location = new System.Drawing.Point(3, 36);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(180, 81);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "GLSL Optimizer";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 52);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(57, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Max Unroll";
            // 
            // upUnrollIter
            // 
            this.upUnrollIter.Location = new System.Drawing.Point(69, 48);
            this.upUnrollIter.Name = "upUnrollIter";
            this.upUnrollIter.Size = new System.Drawing.Size(53, 20);
            this.upUnrollIter.TabIndex = 6;
            this.upUnrollIter.Value = new decimal(new int[] {
            16,
            0,
            0,
            0});
            // 
            // cmbGLSLOptTarget
            // 
            this.cmbGLSLOptTarget.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbGLSLOptTarget.FormattingEnabled = true;
            this.cmbGLSLOptTarget.Location = new System.Drawing.Point(61, 20);
            this.cmbGLSLOptTarget.Name = "cmbGLSLOptTarget";
            this.cmbGLSLOptTarget.Size = new System.Drawing.Size(113, 21);
            this.cmbGLSLOptTarget.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 23);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Target";
            // 
            // GLSLOptionsPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.cmbShaderType);
            this.Controls.Add(this.label1);
            this.Name = "GLSLOptionsPanel";
            this.Size = new System.Drawing.Size(198, 132);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.upUnrollIter)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cmbShaderType;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown upUnrollIter;
        private System.Windows.Forms.ComboBox cmbGLSLOptTarget;
        private System.Windows.Forms.Label label2;

    }
}
