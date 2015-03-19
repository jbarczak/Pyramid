using System;
using System.Collections.Generic;

namespace Pyramid
{
    namespace GLSLOptimizer
    {        
        public enum Target
        {
            OPENGL      = 0,
            OPENGL_ES2  = 1,
            OPENGL_ES3  = 2
        };

        public enum ShaderType
        {
            VERTEX   = 0,
            FRAGMENT = 1,
        };

        public interface IOptions
        {
            ShaderType ShaderType { get; }
            uint MaxUnrollIterations { get; }
        }

   
        public interface IShader
        {
            bool HadError { get; }
            string Output { get; }
            string RawOutput { get; }
            string Log { get; }
            IEnumerable<string> Inputs { get;}
            int MathOps { get; }
            int TextureOps { get; }
            int ControlFlowOps { get; }
        };

        public interface IOptimizer
        {
            IShader Optimize(string text, IOptions opts);
        };

    }
}