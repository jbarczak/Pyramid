using System;

namespace Pyramid
{
  
   
    public interface IWrapper
    {
        /// <summary>
        ///  Construct a D3DCompiler by dynamically loading a given D3DCompiler DLL
        ///   Throws if the DLL could not be loaded properly
        /// </summary>
        /// <param name="DLLPath"></param>
        /// <returns></returns>
        ID3DCompiler CreateD3DCompiler(string DLLPath);

        /// <summary>
        ///  Construct an instance of the GLSL Optimizer
        /// </summary>
        /// <returns></returns>
        GLSLOptimizer.IOptimizer CreateGLSLOptimizer( GLSLOptimizer.Target eTarget );

        GLSlang.ICompiler CreateGLSlangCompiler();

        IAMDDriver CreateAMDDriver( string DLLPath );
    }
}
