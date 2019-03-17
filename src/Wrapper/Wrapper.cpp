
#pragma unmanaged
#include <d3dcompiler.h>
#include <string.h>
#pragma managed

#include "D3DCompiler_Impl.h"
#include "DXILCompiler_Impl.h"
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
        virtual Pyramid::ID3DCompiler^ CreateD3DCompiler( System::String^ DLLPath, IIncludeHandler^ handler )
        {
            return gcnew D3DCompiler_Impl(DLLPath,handler);
        }

        virtual Pyramid::IDXILCompiler^ CreateDXILCompiler( System::String^ DLLPath,IIncludeHandler^ handler )
        {
            return gcnew DXILCompiler_Impl( DLLPath,handler );
        }
       
        virtual Pyramid::GLSLOptimizer::IOptimizer^ CreateGLSLOptimizer( GLSLOptimizer::Target eTarget )
        {
            return gcnew GLSLOptimizer::Optimizer_Impl(eTarget);
        }

        virtual Pyramid::GLSlang::ICompiler^ CreateGLSlangCompiler( IIncludeHandler^ handler )
        {
            return gcnew GLSlang::Compiler_Impl(handler);
        }

        virtual Pyramid::IAMDDriver^ CreateAMDDriver( System::String^ DLLPath)
        {
            return gcnew AMDDriver_Impl(DLLPath);
        }

        
    };


}