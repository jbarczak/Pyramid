
#include "Utilities.h"

struct glslopt_shader;
struct glslopt_ctx;

using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

namespace Pyramid{
namespace GLSLOptimizer{

    ref class Optimizer_Impl : IOptimizer
    {
    public:
           
        Optimizer_Impl( Target eTarget );
        ~Optimizer_Impl();
        virtual IShader^ Optimize(System::String^ text, IOptions^ opts);
 
    private:
        glslopt_ctx* m_pContext;
    };


    ref class Shader_Impl : IShader
    {
    public:
        Shader_Impl( glslopt_shader* pShader, IOptimizer^ owner );
        ~Shader_Impl();

        virtual property bool HadError { bool get() { return m_bError; }};
        virtual property System::String^ Output { System::String^ get() { return MakeString(m_pOutput); }}
        virtual property System::String^ RawOutput { System::String^ get() { return MakeString(m_pOutputRaw); }}
        virtual property System::String^ Log { System::String^ get() { return MakeString(m_pLog); }}
        virtual property IEnumerable<System::String^>^ Inputs { IEnumerable<System::String^>^ get(){ return m_Inputs; }}
        virtual property int MathOps { int get() { return m_nMath; }}
        virtual property int TextureOps { int get() { return m_nTexture; } }
        virtual property int ControlFlowOps { int get() { return m_nFlow; } }

    private:
        glslopt_shader* m_pShader;
        int m_nMath;
        int m_nTexture;
        int m_nFlow;
        bool m_bError;
        const char* m_pLog;
        const char* m_pOutput;
        const char* m_pOutputRaw;
        List<System::String^>^ m_Inputs;
        IOptimizer^ m_Owner; ///< Hold a reference in case .NET tries to destroy shader/optimizer out of order
    };

}}