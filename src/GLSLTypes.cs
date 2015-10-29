using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pyramid
{
    public enum GLSLShaderType
    {
        VERTEX = 0,
        FRAGMENT = 1,
        TESS_CONTROL = 2,
        TESS_EVALUATION = 3,
        GEOMETRY = 4,
        COMPUTE = 5,
    }

    public interface IGLSLOptions : ICompileOptions
    {
        GLSLShaderType ShaderType { get; }
        GLSLOptimizer.Target OptimizerTarget { get; }
        GLSLOptimizer.IOptions OptimizerOptions { get; }
    }


    public class GLSLShader : IShader
    {
        public GLSLShader(string code, IGLSLOptions opts)
        {
            CompileOptions = opts;
            Code = code;
        }

        public Languages Language { get { return Languages.GLSL; } }
        public IGLSLOptions CompileOptions { get; private set; }
        public string Code { get; private set; }

    };
}
