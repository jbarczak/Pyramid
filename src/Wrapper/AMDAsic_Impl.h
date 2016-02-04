#pragma unmanaged
#include "GCNDecoder.h"
#pragma managed

struct GDT_GfxCardInfo;

private ref class AMDAsic_Impl : Pyramid::IAMDAsic
{
public:
    AMDAsic_Impl( const GDT_GfxCardInfo& rCardInfo );
    
    ~AMDAsic_Impl();

    property System::String^ Name {
        virtual System::String^ get() { return m_pmName; }
    }
  

internal:
    System::String^ m_pmName;
    GCN::IDecoder* m_pDecoder;
    unsigned int m_nChipFamily;
    unsigned int m_nChipRevision;
};