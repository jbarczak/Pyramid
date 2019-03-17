using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Pyramid
{
    public enum HLSLTarget
    {
        vs_3_0,
        vs_4_0,
        vs_4_1,
        vs_5_0,
        vs_5_1,
        vs_6_0,
        vs_6_1,
        vs_6_2,

        ps_3_0,
        ps_4_0,
        ps_4_1,
        ps_5_0,
        ps_5_1,
        ps_6_0,
        ps_6_1,
        ps_6_2,

        gs_4_0,
        gs_4_1,
        gs_5_0,
        gs_5_1,
        gs_6_0,
        gs_6_1,
        gs_6_2,

        hs_5_0,
        hs_5_1,
        hs_6_0,
        hs_6_1,
        hs_6_2,

        ds_5_0,
        ds_5_1,
        ds_6_0,
        ds_6_1,
        ds_6_2,

        cs_4_0,
        cs_4_1,
        cs_5_0,
        cs_5_1,
        cs_6_0,
        cs_6_1,
        cs_6_2,
    };

    public enum RootSignatureTarget
    {
        rootsig_1_0,
        rootsig_1_1,
    }

    public enum HLSLOptimizationLevel
    {
        SKIP,
        LEVEL0,
        LEVEL1,
        LEVEL2,
        LEVEL3
    };

    public enum HLSLShaderType
    {
        VERTEX,
        PIXEL,
        GEOMETRY,
        DOMAIN,
        HULL,
        COMPUTE
    };

    public interface IHLSLOptions : ICompileOptions
    {
        string EntryPoint { get; }
        HLSLTarget Target { get; }
        RootSignatureTarget RootSigTarget { get; }
        HLSLShaderType ShaderType { get; }
        HLSLOptimizationLevel OptimizationLevel { get; }
        bool Compatibility { get; }
        string RootSigMacro { get; }

        string GetFXCCommandLine(string shaderFile);
        uint GetD3DCompileFlagBits();
    }

    public interface IDXShaderReflection
    {
        HLSLShaderType GetShaderType();
        uint GetThreadsPerGroup();
    };

    public interface IDXBlob
    {
        byte[] ReadBytes();
    }

    public interface IDXShaderBlob : IDXBlob
    {
        string Disassemble();
        IDXBlob ExtractRootSignature();
    }

    public interface IDXILShaderBlob : IDXShaderBlob
    {
    }

    public interface IDXBCShaderBlob : IDXShaderBlob
    {
        IDXBCShaderBlob GetSignatureBlob();
        IDXBCShaderBlob GetExecutableBlob();
        IDXBCShaderBlob Strip();
        IDXShaderReflection Reflect();
    }


    public class HLSLShader : IShader
    {
        public HLSLShader(string code, IHLSLOptions opts ,string path)
        {
            CompileOptions = opts;
            Code = code;
            WasCompiled = false;
            RootSigWasCompiled = false;
            RootSigHasError = false;
            SourceFilePath = path;
        }

        public Languages Language { get { return Languages.HLSL; } }
        public IHLSLOptions CompileOptions { get; private set; }
        public string Code { get; private set; }
        public IDXShaderBlob CompiledBlob { get; private set; }
        public string Messages { get; private set; }
        public string SourceFilePath { get; private set; }


        public bool CompileRootSignature( IDXILCompiler compiler )
        {
            if( compiler != null )
            {
                IDXBlob blob;
                string msg;
                RootSigHasError = !compiler.CompileRootSignature(this.Code, this.CompileOptions, this.SourceFilePath, out blob, out msg);
                RootSigMessages = msg;
                CompiledRootSig = blob;
                RootSigWasCompiled = true;
                return !RootSigHasError;
            }
            else
            {
                return false;
            }
        }
        
        public bool Compile( ID3DCompiler compiler, IDXILCompiler dxil )
        {
            if( CompileOptions.Target.ToString()[3] == '6' )
            {
                if (dxil == null)
                    return false;

                IDXILShaderBlob blob;
                string msg;
                HasError = !dxil.Compile(this.Code, this.CompileOptions, this.SourceFilePath, out blob, out msg);
                WasCompiled = true;
                Messages = msg;
                CompiledBlob = blob;
            }
            else
            {
                if( compiler == null )
                    return false;

                IDXBCShaderBlob blob;
                string msg;
                HasError = !compiler.Compile(this.Code, this.CompileOptions, this.SourceFilePath, out blob, out msg);
                WasCompiled = true;
                Messages = msg;
                CompiledBlob = blob;
            }

            // check for embedded root signature and pull it out if there is one
            if( CompiledBlob != null )
            {
                IDXBlob rs = CompiledBlob.ExtractRootSignature();
                if (rs != null)
                {
                    RootSigWasCompiled = true;
                    RootSigHasError = false;
                    CompiledRootSig = rs;
                }
            }

            return !HasError;

        }

        public IDXBlob CompiledRootSig { get; private set; }
        public bool RootSigWasCompiled { get; private set; }
        public bool RootSigHasError { get; private set; }
        public string RootSigMessages { get; private set; }

        public bool WasCompiled { get; private set; }
        public bool HasError { get; private set; }
        public bool WasCompiledWithErrors { get { return WasCompiled && HasError; } }
    };

    public interface ID3DCompiler
    {
        bool CompileRootSignature(string text,
                                 IHLSLOptions opts,
                                 string fileName,
                                 out IDXBlob blob,
                                 out string Messages);

        bool Compile(string text,
                     IHLSLOptions opts,
                     string fileName,
                     out IDXBCShaderBlob blob,
                     out string Messages);
    };

    public interface IDXILCompiler 
    {
        bool CompileRootSignature(string text,
                             IHLSLOptions opts,
                             string fileName,
                             out IDXBlob blob,
                             out string Messages);

        bool Compile(string text,
                     IHLSLOptions opts,
                     string fileName,
                     out IDXILShaderBlob blob,
                     out string Messages);
    };

}
