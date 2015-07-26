
using namespace System::Collections::Generic;
using namespace Pyramid::Scrutinizer;

ref class AMDAsic_Impl;
ref class AMDShader_Impl;

ref class Scrutinizer_GCN : public Pyramid::Scrutinizer::IScrutinizer
{
public:

    Scrutinizer_GCN( AMDAsic_Impl^ asic, AMDShader_Impl^ shader )
        : m_pmAsic(asic), m_pmShader(shader)
    {
    }

    virtual List<IInstruction^>^ BuildProgram( );

    virtual System::String^ AnalyzeExecutionTrace( List<IInstruction^>^ ops );

private:
    AMDAsic_Impl^ m_pmAsic;
    AMDShader_Impl^ m_pmShader;
};

