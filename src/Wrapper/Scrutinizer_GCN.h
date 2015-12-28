
using namespace System::Collections::Generic;
using namespace Pyramid::Scrutinizer;

ref class AMDAsic_Impl;
ref class AMDShader_Impl;

ref class Scrutinizer_GCN_Base abstract : public Pyramid::Scrutinizer::IScrutinizer
{
public:

    Scrutinizer_GCN_Base( AMDAsic_Impl^ asic, AMDShader_Impl^ shader )
        : m_pmAsic(asic), m_pmShader(shader),
          m_pmParams(gcnew List<ISimulationParameter^>())
         
    {
    }
 
    virtual List<IInstruction^>^ BuildProgram( );

    virtual List<IInstruction^>^ BuildDXFetchShader( Pyramid::IDXShaderReflection^ refl );

    property IEnumerable<ISimulationParameter^>^ SimulationParameters
    {
        virtual IEnumerable<ISimulationParameter^>^ get()
        {
            return m_pmParams;
        }
    }

    virtual System::String^ AnalyzeExecutionTrace( List<IInstruction^>^ ops ) = 0;


protected:

    System::String^ AnalyzeTrace( List<IInstruction^>^ ops, 
                                  unsigned int nWaveIssueRate, 
                                  unsigned int nWaveOccupancy,
                                  unsigned int nGroupOccupancy,
                                  unsigned int nCUCount );

    AMDAsic_Impl^ m_pmAsic;
    AMDShader_Impl^ m_pmShader;
    List<ISimulationParameter^>^ m_pmParams;
};


ref class Scrutinizer_GCN_VS : public Scrutinizer_GCN_Base
{
public:
     
    Scrutinizer_GCN_VS( AMDAsic_Impl^ asic, AMDShader_Impl^ shader );
  
    virtual System::String^ AnalyzeExecutionTrace( List<IInstruction^>^ ops ) override;


private:

    SimulationParameterInt^ m_pmOccupancy;
    SimulationParameterInt^ m_pmCUCount;
    SimulationParameterDouble^ m_pmACMR;

};


ref class Scrutinizer_GCN_PS : public Scrutinizer_GCN_Base
{
public:

    Scrutinizer_GCN_PS( AMDAsic_Impl^ asic, AMDShader_Impl^ shader );

    virtual System::String^ AnalyzeExecutionTrace( List<IInstruction^>^ ops ) override;

private:
    
    SimulationParameterInt^ m_pmOccupancy;
    SimulationParameterInt^ m_pmCUCount;
    SimulationParameterDouble^ m_pmPixelsPerTri;

};

ref class Scrutinizer_GCN_CS : public Scrutinizer_GCN_Base
{
public:

    Scrutinizer_GCN_CS( AMDAsic_Impl^ asic, AMDShader_Impl^ shader );

    virtual System::String^ AnalyzeExecutionTrace( List<IInstruction^>^ ops ) override;

private:
    
    SimulationParameterInt^ m_pmWaveOccupancy;
    SimulationParameterInt^ m_pmGroupOccupancy;
    SimulationParameterInt^ m_pmCUCount;
    unsigned int m_nWavesPerGroup;
};