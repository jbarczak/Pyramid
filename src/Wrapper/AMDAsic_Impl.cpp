
#include "AMDAsic_Impl.h"
#include "Utilities.h"



AMDAsic_Impl::AMDAsic_Impl( const char* pName, unsigned int arg0, unsigned int arg1, GCN::IDecoder::GCNVersions eGCNVersion )
    : m_pmName( MakeString(pName) ),
        m_nArg0(arg0),
        m_nArg1(arg1),
        m_pDecoder( GCN::IDecoder::Create( eGCNVersion ) )
{
}

AMDAsic_Impl::~AMDAsic_Impl()
{
    GCN::IDecoder::Destroy(m_pDecoder);
    m_pDecoder=0;
}

 