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
            public HLSLTarget Target { get; set; }
            public RootSignatureTarget RootSigTarget { get; set; }
            public HLSLOptimizationLevel OptimizationLevel { get; set; }
            public string EntryPoint { get; set; }
            public string RootSigMacro { get; set; }
            public bool Compatibility { get; set; }

            public HLSLShaderType ShaderType
            {
                get
                {
                    switch (this.Target)
                    {
                        case HLSLTarget.vs_3_0:
                        case HLSLTarget.vs_4_0:
                        case HLSLTarget.vs_4_1:
                        case HLSLTarget.vs_5_0:
                        case HLSLTarget.vs_5_1:
                        case HLSLTarget.vs_6_0:
                        case HLSLTarget.vs_6_1:
                        case HLSLTarget.vs_6_2:
                            return HLSLShaderType.VERTEX;                       
                        case HLSLTarget.ps_3_0:
                        case HLSLTarget.ps_4_0:
                        case HLSLTarget.ps_4_1:
                        case HLSLTarget.ps_5_0:
                        case HLSLTarget.ps_5_1:
                        case HLSLTarget.ps_6_0:
                        case HLSLTarget.ps_6_1:
                        case HLSLTarget.ps_6_2:
                            return HLSLShaderType.PIXEL;
                        case HLSLTarget.gs_4_0:
                        case HLSLTarget.gs_4_1:
                        case HLSLTarget.gs_5_0:
                        case HLSLTarget.gs_5_1:
                        case HLSLTarget.gs_6_0:
                        case HLSLTarget.gs_6_1:
                        case HLSLTarget.gs_6_2:
                            return HLSLShaderType.GEOMETRY;
                        case HLSLTarget.hs_5_0:
                        case HLSLTarget.hs_5_1:
                        case HLSLTarget.hs_6_0:
                        case HLSLTarget.hs_6_1:
                        case HLSLTarget.hs_6_2:
                            return HLSLShaderType.HULL;
                        case HLSLTarget.ds_5_0:
                        case HLSLTarget.ds_5_1:
                        case HLSLTarget.ds_6_0:
                        case HLSLTarget.ds_6_1:
                        case HLSLTarget.ds_6_2:
                            return HLSLShaderType.DOMAIN;
                        case HLSLTarget.cs_4_0:
                        case HLSLTarget.cs_4_1:
                        case HLSLTarget.cs_5_0:
                        case HLSLTarget.cs_5_1:
                        case HLSLTarget.cs_6_0:
                        case HLSLTarget.cs_6_1:
                        case HLSLTarget.cs_6_2:
                            return HLSLShaderType.COMPUTE;
                    }
                    throw new System.Exception("What is this?");
                }
            }

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
            foreach( object e in Enum.GetValues(typeof(RootSignatureTarget) ) )
            {
                string n = Enum.GetName(typeof(RootSignatureTarget), e);
                cmbRootSignature.Items.Add(n);
            }
            cmbTarget.SelectedIndex = cmbTarget.Items.IndexOf("ps_5_0");
            cmbOpt.SelectedIndex = cmbOpt.Items.IndexOf("LEVEL3");
            cmbRootSignature.SelectedIndex = cmbRootSignature.Items.IndexOf("rootsig_1_0");
        }
        
        public Control Panel { get { return this; } }

        public ICompileOptions ReadOptions()
        {
            HLSLOptions opts = new HLSLOptions();
            opts.Compatibility = chkCompat.Checked;
            opts.EntryPoint = txtEntryPoint.Text;
            opts.RootSigMacro = txtRootSig.Text;
            opts.RootSigTarget = (RootSignatureTarget)Enum.Parse(typeof(RootSignatureTarget), (string) cmbRootSignature.Items[cmbRootSignature.SelectedIndex]);
            opts.Target = (HLSLTarget) Enum.Parse(typeof(HLSLTarget), (string) cmbTarget.Items[cmbTarget.SelectedIndex]);
            opts.OptimizationLevel = (HLSLOptimizationLevel)Enum.Parse(typeof(HLSLOptimizationLevel), (string)
                                                               cmbOpt.Items[cmbOpt.SelectedIndex]);
            return opts;
        }

        private void HLSLOptionsPanel_Load(object sender, EventArgs e)
        {

        }
    }
}
