


namespace Pyramid{
namespace GLSlang{


    ref class Compiler_Impl : ICompiler
    {
    public:
        Compiler_Impl();
        ~Compiler_Impl();
        
        virtual IShader^ Compile( System::String^ text, IOptions^ opts );
        virtual IShader^ CompileHLSL( System::String^ text, IOptions^ opts, System::String^ EntryPoint );
        virtual IConfig^ CreateConfig(System::String^ text);
        virtual IConfig^ CreateDefaultConfig();
    };


}}