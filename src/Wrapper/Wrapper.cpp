
#pragma unmanaged
#include <d3dcompiler.h>
#include <string.h>
#pragma managed

#include "D3DCompiler_Impl.h"
#include "GLSLOpt_Impl.h"
#include "GLSlang_Impl.h"
#include "AMDDriver_Impl.h"
#include "Utilities.h"

namespace Pyramid
{

    public ref class Wrapper : Pyramid::IWrapper
    {
    public:
        Wrapper() {};
        virtual Pyramid::ID3DCompiler^ CreateD3DCompiler( System::String^ DLLPath )
        {
            return gcnew D3DCompiler_Impl(DLLPath);
        }
       
        virtual Pyramid::GLSLOptimizer::IOptimizer^ CreateGLSLOptimizer( GLSLOptimizer::Target eTarget )
        {
            return gcnew GLSLOptimizer::Optimizer_Impl(eTarget);
        }

        virtual Pyramid::GLSlang::ICompiler^ CreateGLSlangCompiler( )
        {
            return gcnew GLSlang::Compiler_Impl();
        }

        virtual Pyramid::IAMDDriver^ CreateAMDDriver( System::String^ DLLPath )
        {
            return gcnew AMDDriver_Impl(DLLPath);
        }

        
    };


}