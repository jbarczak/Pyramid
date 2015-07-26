#pragma unmanaged
#include "GCNDecoder.h"
#pragma managed

private ref class AMDAsic_Impl : Pyramid::IAMDAsic
{
public:
    AMDAsic_Impl( const char* pName, unsigned int arg0, unsigned int arg1, GCN::IDecoder::GCNVersions eGCNVersion );
    
    ~AMDAsic_Impl();

    property System::String^ Name {
        virtual System::String^ get() { return m_pmName; }
    }
  

internal:
    System::String^ m_pmName;
    GCN::IDecoder* m_pDecoder;
    unsigned int m_nArg0;
    unsigned int m_nArg1;
};