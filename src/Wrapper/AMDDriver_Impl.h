

struct CompileResult;
struct CompileArgs;

typedef unsigned long (__cdecl *COMPILE_SHADER)( CompileArgs*, CompileResult* );
typedef void  (__cdecl *FREE_SHADER)   (void*);


private ref class AMDDriver_Impl : public Pyramid::IAMDDriver
{
public:

    AMDDriver_Impl( System::String^ path );
    

    property System::Collections::Generic::IEnumerable<Pyramid::IAMDAsic^>^ Asics
    {
        virtual System::Collections::Generic::IEnumerable<Pyramid::IAMDAsic^>^ get()
        {
            return m_pmAsics;
        }
    }

    virtual Pyramid::IAMDShader^  CompileDXBlob(Pyramid::IAMDAsic^ asic, array<byte>^ exe, Pyramid::IDXShaderReflection^ reflection );

internal:

    COMPILE_SHADER m_pCompileFunc;
    FREE_SHADER    m_pFreeFunc;
    
private:

    System::Collections::Generic::List<Pyramid::IAMDAsic^>^ m_pmAsics;
};