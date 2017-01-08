using System;
using System.Collections.Generic;


namespace Pyramid
{
    namespace GLSlang
    {
        public interface IOptions
        {
            GLSLShaderType ShaderType { get; }
            IConfig Config { get; }
        }

        public interface IShader
        {
            bool HasErrors { get; }
            string InfoLog { get; }
            string InfoDebugLog { get; }
            SPIRV.IProgram CompileSPIRV();
        }

        public interface IConfig
        {
        }

        public interface ICompiler
        {
            IConfig CreateConfig(string text);
            IConfig CreateDefaultConfig();
            IShader Compile(string text, IOptions opts);
        }
    }
}
