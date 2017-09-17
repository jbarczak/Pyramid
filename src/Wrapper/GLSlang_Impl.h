


namespace Pyramid{
namespace GLSlang{


    ref class Compiler_Impl : ICompiler
    {
    public:
        Compiler_Impl( Pyramid::IIncludeHandler^ pmInclude );
        ~Compiler_Impl();
        
        virtual IShader^ Compile( System::String^ text, GLSLShaderType eManagedType, IConfig^ cfg, System::String^ filePath );
        virtual IShader^ CompileHLSL( System::String^ text,  IHLSLOptions^ opts, IConfig^ cfg ,System::String^ filePath );
        virtual IConfig^ CreateConfig(System::String^ text);
        virtual IConfig^ CreateDefaultConfig();

    private:
        Pyramid::IIncludeHandler^ m_pmIncluder;
    };


}}