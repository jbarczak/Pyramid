using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Pyramid
{


    public partial class HLSLOptionsPanel : UserControl, ICompileOptionsPanel
    {
        private class HLSLOptions : IHLSLOptions
        {
            public Languages Language { get { return Languages.HLSL; } }
            public HLSLTarget Target { get; set;  }
            public HLSLOptimizationLevel OptimizationLevel { get; set; }
            public string EntryPoint { get; set;  }
            public bool Compatibility { get; set; }

            public uint GetD3DCompileFlagBits()
            {
                uint flags=0;
                switch (OptimizationLevel)
                {
                    case HLSLOptimizationLevel.SKIP: 
                        flags |= (1 << 2); 
                        break; //D3DCOMPILE_SKIP_OPTIMIZATION
                    case HLSLOptimizationLevel.LEVEL0: 
                        flags |= (1 << 14); 
                        break; //D3DCOMPILE_OPTIMIZATION_LEVEL0
                    case HLSLOptimizationLevel.LEVEL1: 
                        flags |= 0; 
                        break;//D3DCOMPILE_OPTIMIZATION_LEVEL1
                    case HLSLOptimizationLevel.LEVEL2: 
                        flags |= (1<<14)|(1<<15) ; 
                        break; //D3DCOMPILE_OPTIMIZATION_LEVEL2
                    case HLSLOptimizationLevel.LEVEL3: 
                        flags |= (1<<15);
                        break; //D3DCOMPILE_OPTIMIZATION_LEVEL3
                }

                if (Compatibility)
                    flags |= (1 << 12);// D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY
                return flags;
            }

            public string GetFXCCommandLine(string shaderFile)
            {
                string switches = "";
                switch (OptimizationLevel)
                {
                    case HLSLOptimizationLevel.SKIP:   switches += "/Od "; break;
                    case HLSLOptimizationLevel.LEVEL0: switches += "/O0 "; break;
                    case HLSLOptimizationLevel.LEVEL1: switches += "/O1 "; break;
                    case HLSLOptimizationLevel.LEVEL2: switches += "/O2 "; break;
                    case HLSLOptimizationLevel.LEVEL3: switches += "/O3 "; break;
                }

                if (Compatibility)
                    switches += "/Gec";

                return String.Format(
                    "/T{0} /E{1} {2} \"{3}\"",
                    Target.ToString(), 
                    EntryPoint, 
                    switches, 
                    shaderFile );
            }
        }

        public HLSLOptionsPanel()
        {
            InitializeComponent();
            
            foreach( object e in Enum.GetValues(typeof(HLSLTarget)))
            {
                string n = Enum.GetName(typeof(HLSLTarget), e);
                cmbTarget.Items.Add(n);
            }
            foreach( object e in Enum.GetValues(typeof(HLSLOptimizationLevel)))
            {
                string n = Enum.GetName(typeof(HLSLOptimizationLevel), e);
                cmbOpt.Items.Add(n);
            }
            cmbTarget.SelectedIndex = cmbTarget.Items.IndexOf("ps_5_0");
            cmbOpt.SelectedIndex = cmbOpt.Items.IndexOf("LEVEL3");
        }

        public Control Panel { get { return this; }}

        public ICompileOptions ReadOptions()
        {
            HLSLOptions opts = new HLSLOptions();
            opts.Compatibility = chkCompat.Checked;
            opts.EntryPoint = txtEntryPoint.Text;
            opts.Target = (HLSLTarget) Enum.Parse(typeof(HLSLTarget), (string) cmbTarget.Items[cmbTarget.SelectedIndex]);
            opts.OptimizationLevel = (HLSLOptimizationLevel)Enum.Parse(typeof(HLSLOptimizationLevel), (string)
                                                               cmbOpt.Items[cmbOpt.SelectedIndex]);
            return opts;
        }
    }
}
