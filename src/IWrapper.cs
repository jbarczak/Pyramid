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
        ID3DCompiler CreateD3DCompiler(string DLLPath, IIncludeHandler handler );

        /// <summary>
        ///  Construct an instance of the GLSL Optimizer
        /// </summary>
        /// <returns></returns>
        GLSLOptimizer.IOptimizer CreateGLSLOptimizer( GLSLOptimizer.Target eTarget );

        GLSlang.ICompiler CreateGLSlangCompiler( IIncludeHandler handler );

        IAMDDriver CreateAMDDriver( string DLLPath );

    }
}
