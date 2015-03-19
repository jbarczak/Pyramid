using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace Pyramid
{
    
    public partial class GLSLOptionsPanel : UserControl, ICompileOptionsPanel
    {
        class GLSLOptimizerOptions : GLSLOptimizer.IOptions
        {
            public GLSLOptimizer.ShaderType ShaderType { get; set; }
            public uint MaxUnrollIterations { get; set; }
        }

        class GLSLOptions : IGLSLOptions
        {
            public Languages Language { get { return Languages.GLSL; } }
            public GLSLOptimizer.IOptions OptimizerOptions { get; set; }
            public GLSLShaderType ShaderType { get; set; }
            public GLSLOptimizer.Target OptimizerTarget { get; set; }
        }

        public GLSLOptionsPanel()
        {
            InitializeComponent();

            foreach (object e in Enum.GetValues(typeof(GLSLShaderType)))
                cmbShaderType.Items.Add(e);
            foreach (object e in Enum.GetValues(typeof(GLSLOptimizer.Target)))
                cmbGLSLOptTarget.Items.Add(e);

            cmbGLSLOptTarget.SelectedIndex = 0;
            cmbShaderType.SelectedIndex = 0;
        }

        public Control Panel { get { return this; } }

        public ICompileOptions ReadOptions()
        {
            GLSLOptions opts = new GLSLOptions();
            opts.ShaderType = (GLSLShaderType)cmbShaderType.SelectedItem;
            opts.OptimizerTarget = (GLSLOptimizer.Target)cmbGLSLOptTarget.SelectedItem;
            
            GLSLOptimizer.ShaderType eType;
            switch (opts.ShaderType)
            {
                default:    return opts;
                case GLSLShaderType.VERTEX:   eType = GLSLOptimizer.ShaderType.VERTEX;    break;
                case GLSLShaderType.FRAGMENT: eType = GLSLOptimizer.ShaderType.FRAGMENT;  break;
            }

            GLSLOptimizerOptions optOptions = new GLSLOptimizerOptions();
            optOptions.ShaderType = eType;
            optOptions.MaxUnrollIterations = (uint)upUnrollIter.Value;
            opts.OptimizerOptions = optOptions;
            return opts;
        }
    }
}
