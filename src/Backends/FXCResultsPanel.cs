using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.IO;


namespace Pyramid
{
    public partial class FXCResultsPanel : UserControl
    {
        private IDXShaderBlob m_Shader;
        private IDXBlob m_RootSignature;

        private static void HexBumpBlob( StringBuilder str, byte[] bytes )
        {
            int nBytes = bytes.Length;

            if (nBytes % 4 == 0)
            {
                for (int i = 0; i < nBytes; i += 4)
                {
                    int n = bytes[i] |
                             bytes[i + 1] << 8 |
                             bytes[i + 2] << 16 |
                             bytes[i + 3] << 24;

                    char c0 = Convert.ToChar(bytes[i]);
                    char c1 = Convert.ToChar(bytes[i+1]);
                    char c2 = Convert.ToChar(bytes[i+2]);
                    char c3 = Convert.ToChar(bytes[i+3]);
                    c0 = (c0 < 0x20 || c0 > 127) ? '.' : c0;
                    c1 = (c1 < 0x20 || c1 > 127) ? '.' : c1;
                    c2 = (c2 < 0x20 || c2 > 127) ? '.' : c2;
                    c3 = (c3 < 0x20 || c3 > 127) ? '.' : c3;
                    str.AppendFormat("{0:X8}  '{1} {2} {3} {4}' ", n, c0,c1,c2,c3 );
                    str.AppendLine();
                }
            }
            else
            {
                for( int i=0; i<nBytes; i++ )
                {
                    if (i % 8 == 0)
                        str.AppendLine();
                    str.AppendFormat("{0:X2} ", bytes[i]);
                }
            }
            str.AppendLine();
        }

        public FXCResultsPanel( HLSLShader shader )
        {
            string error = shader.Messages;
            IDXShaderBlob blob = shader.CompiledBlob;
            m_Shader = blob;
            InitializeComponent();
            txtMessages.Text = error;

            if (blob == null)
            {
                tabControl1.TabPages.Remove(hexPage);
                tabControl1.TabPages.Remove(asmPage);               
            }
            else
            {
                txtASM.Text = blob.Disassemble();

                // generate a blob dump
                try
                {
                    byte[] rawBytes = blob.ReadBytes();

                    StringBuilder str = new StringBuilder();
                    str.AppendFormat("Blob size is {0} ({1:X}) bytes", rawBytes.Length, rawBytes.Length);
                    str.AppendLine();
                    HexBumpBlob(str, rawBytes);

                    if (blob is IDXBCShaderBlob)
                    {
                        IDXBCShaderBlob blobDXBC = blob as IDXBCShaderBlob;
                        IDXBCShaderBlob strip = blobDXBC.Strip();
                        IDXBCShaderBlob sig = strip.GetSignatureBlob();

                        byte[] stripBytes = strip.ReadBytes();
                        byte[] sigBytes = sig.ReadBytes();

                        str.AppendFormat("Stripped blob size is {0} ({1:x}) bytes", stripBytes.Length, stripBytes.Length);
                        str.AppendLine();
                        HexBumpBlob(str, stripBytes);

                        str.AppendFormat("Signature blob size is {0} ({1:x}) bytes", sigBytes.Length, sigBytes.Length);
                        str.AppendLine();
                        HexBumpBlob(str, sigBytes);

                    }
                    
                    txtHex.Text = str.ToString();
                    
                }
                catch( Exception ex )
                {
                    txtHex.Text = String.Format("EXCEPTION while generating hex dump: {0}", ex.Message);
                }                
            }

            IDXBlob rsBlob = shader.CompiledRootSig;
            m_RootSignature = rsBlob;
            txtMessages.Text = String.Concat(txtMessages.Text, shader.RootSigMessages);

            if (rsBlob == null)
            {
                tabControl1.TabPages.Remove(rootSigPage);              
            }
            else
            {
                try
                {
                    byte[] rawBytes = rsBlob.ReadBytes();

                    StringBuilder str = new StringBuilder();
                    str.AppendFormat("RootSig blob size is {0} ({1:X}) bytes", rawBytes.Length, rawBytes.Length);
                    str.AppendLine();

                    HexBumpBlob(str, rawBytes);

                    txtRootSig.Text = str.ToString();
                }
                catch (Exception ex)
                {
                    txtRootSig.Text = String.Format("EXCEPTION while generating rootsig dump: {0}", ex.Message);
                }
            }

        }

        private void btnExportBytecode_Click(object sender, EventArgs e)
        {
            if (m_Shader != null)
            {
                SaveFileDialog save = new SaveFileDialog();
                if (save.ShowDialog() != DialogResult.Cancel)
                {
                    File.WriteAllBytes(save.FileName, m_Shader.ReadBytes());
                }
            }
        }

        private void btnExportRootSig_Click(object sender, EventArgs e)
        {
            if (m_RootSignature != null)
            {
                SaveFileDialog save = new SaveFileDialog();
                if (save.ShowDialog() != DialogResult.Cancel)
                {
                    File.WriteAllBytes(save.FileName, m_RootSignature.ReadBytes());
                }
            }
        }


        private static void DeclareByteArray(StringBuilder str, byte[] bytes, string name)
        {
            int nBytes = bytes.Length;
            str.AppendFormat("static const unsigned char {0}[{1}] = {{", name, nBytes );
            str.AppendLine();

            for( int i=0; i<nBytes; i += 32 )
            {
                str.Append("    ");
                for( int j=0; j<32; j++ )
                {
                    if (i + j >= nBytes)
                        break;
                    str.AppendFormat("{0,-3}, ", bytes[i + j]);
                }
                str.AppendLine();
            }
            str.Append("};");
            str.AppendLine();
        }

        private void btnExportCPP_Click(object sender, EventArgs e)
        {
            SaveFileDialog save = new SaveFileDialog();
            if (save.ShowDialog() != DialogResult.Cancel)
            {
                StringBuilder str = new StringBuilder();

                if ( m_Shader != null )
                {
                    str.Append("#if 0");
                    str.AppendLine();
                    str.Append(m_Shader.Disassemble());
                    str.AppendLine();
                    str.Append("#endif");
                    str.AppendLine();

                    byte[] byteCode = m_Shader.ReadBytes();
                    DeclareByteArray(str, byteCode, "Bytecode");
                }

                if( m_RootSignature != null )
                {
                    byte[] byteCode = m_RootSignature.ReadBytes();
                    DeclareByteArray(str, byteCode, "RootSig");
                }

                File.WriteAllText(save.FileName, str.ToString());
            }
        }
    }
}
