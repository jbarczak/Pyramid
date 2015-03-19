


namespace Pyramid{
namespace GLSlang{


    ref class Compiler_Impl : ICompiler
    {
    public:
        Compiler_Impl();
        ~Compiler_Impl();
        
        virtual IShader^ Compile( System::String^ text, IOptions^ opts );
        virtual IConfig^ CreateConfig(System::String^ text);
        virtual IConfig^ CreateDefaultConfig();
    };


}}