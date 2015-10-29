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
        ps_3_0,
        ps_4_0,
        ps_4_1,
        ps_5_0,
        gs_4_0,
        gs_4_1,
        gs_5_0,
        hs_5_0,
        ds_5_0,
        cs_4_0,
        cs_4_1,
        cs_5_0,
    };

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
        HLSLOptimizationLevel OptimizationLevel { get; }
        bool Compatibility { get; }

        string GetFXCCommandLine(string shaderFile);
        uint GetD3DCompileFlagBits();
    }

    public interface IDXShaderReflection
    {
        HLSLShaderType GetShaderType();

    };

    
    public interface IDXShaderBlob
    {
        string Disassemble();
        byte[] ReadBytes();
        IDXShaderBlob Strip();
        IDXShaderBlob GetSignatureBlob();
        IDXShaderBlob GetExecutableBlob();
        IDXShaderReflection Reflect();
    }

    public class HLSLShader : IShader
    {
        public HLSLShader(string code, IHLSLOptions opts)
        {
            CompileOptions = opts;
            Code = code;
            WasCompiled = false;
        }

        public Languages Language { get { return Languages.HLSL; } }
        public IHLSLOptions CompileOptions { get; private set; }
        public string Code { get; private set; }
        public IDXShaderBlob CompiledBlob { get; private set; }
        public string Messages { get; private set; }

        public bool Compile( ID3DCompiler compiler )
        {
            IDXShaderBlob blob;
            string msg;
            HasError    = !compiler.Compile(this.Code, this.CompileOptions, out blob, out msg);
            WasCompiled = true;
            Messages = msg;
            CompiledBlob = blob;
            return HasError;
        }

        public bool WasCompiled { get; private set; }
        public bool HasError { get; private set; }
        public bool WasCompiledWithErrors { get { return WasCompiled && HasError; } }
    };

    public interface ID3DCompiler
    {
        bool Compile(string text,
                     IHLSLOptions opts,
                     out IDXShaderBlob blob,
                     out string Messages);
    };


}
