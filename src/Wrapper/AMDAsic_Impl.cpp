
#include "AMDAsic_Impl.h"
#include "Utilities.h"

#include "amd-codexl-analyzer/Common/Src/DeviceInfo/DeviceInfoUtils.h"
#include "AMDTBackend/Include/Common/asic_reg/atiid.h"
#include "AMDTBackend/Include/Common/asic_reg/si_id.h"
#include "AMDTBackend/Include/Common/asic_reg/ci_id.h"
#include "AMDTBackend/Include/Common/asic_reg/vi_id.h"
#include "AMDTBackend/Include/Common/asic_reg/cz_id.h"
#include "AMDTBackend/Include/Common/asic_reg/kv_id.h"

static void GetDeviceChipFamilyRevision(
    GDT_HW_ASIC_TYPE type,
    unsigned int&          chipFamily,
    unsigned int&          chipRevision) 
{
    chipFamily = (unsigned int) - 1;
    chipRevision = (unsigned int) - 1;

    switch (type)
    {
        default:
            // The 600's EG, and NI are no longer supported by OpenCL.
            // For GSA the earliest DX/GL support is SI.
            break;

        case GDT_TAHITI_PRO:
        case GDT_TAHITI_XT:
            chipFamily = FAMILY_SI;
            chipRevision = SI_TAHITI_P_B1;
            break;

        case GDT_PITCAIRN_PRO:
        case GDT_PITCAIRN_XT:
            chipFamily = FAMILY_SI;
            chipRevision = SI_PITCAIRN_PM_A1;
            break;

        case GDT_CAPEVERDE_PRO:
        case GDT_CAPEVERDE_XT:
            chipFamily = FAMILY_SI;
            chipRevision = SI_CAPEVERDE_M_A1;
            break;

        case GDT_OLAND:
            chipFamily = FAMILY_SI;
            chipRevision = SI_OLAND_M_A0;
            break;

        case GDT_HAINAN:
            chipFamily = FAMILY_SI;
            chipRevision = SI_HAINAN_V_A0;
            break;

        case GDT_BONAIRE:
            chipFamily = FAMILY_CI;
            chipRevision = CI_BONAIRE_M_A0;
            break;

        case GDT_HAWAII:
            chipFamily = FAMILY_CI;
            chipRevision = CI_HAWAII_P_A0;
            break;

        case GDT_KALINDI:
            chipFamily = FAMILY_CI;
            chipRevision = CI_BONAIRE_M_A0;
            break;

        case GDT_SPECTRE:
        case GDT_SPECTRE_SL:
        case GDT_SPECTRE_LITE:
            chipFamily = FAMILY_CI;
            chipRevision = KV_SPECTRE_A0;
            break;

        case GDT_SPOOKY:
            chipFamily = FAMILY_CI;
            chipRevision = KV_SPOOKY_A0;
            break;

        case GDT_ICELAND:
            chipFamily = FAMILY_VI;
            chipRevision = VI_ICELAND_M_A0;
            break;

        case GDT_TONGA:
            chipFamily = FAMILY_VI;
            chipRevision = VI_TONGA_P_A0;
            break;

        case GDT_CARRIZO:
            chipFamily = FAMILY_VI;
            chipRevision = CARRIZO_A0;
            break;

        case GDT_FIJI:
            chipFamily = FAMILY_VI;
            chipRevision = VI_FIJI_P_A0;
            break;
    }

}



AMDAsic_Impl::AMDAsic_Impl( const GDT_GfxCardInfo& rCard )
    : m_pmName( MakeString(rCard.m_szCALName) )
{
    unsigned int nChipFamily;
    unsigned int nChipRevision;
    GetDeviceChipFamilyRevision(rCard.m_asicType, nChipFamily, nChipRevision);

    switch( rCard.m_generation )
    {
    case GDT_HW_GENERATION_SOUTHERNISLAND:
    case GDT_HW_GENERATION_SEAISLAND:
        m_pDecoder =  GCN::IDecoder::Create(GCN::IDecoder::GCN1);
        break;
    case GDT_HW_GENERATION_VOLCANICISLAND:
        m_pDecoder =  GCN::IDecoder::Create(GCN::IDecoder::GCN3);
        break;
    }

    m_nChipFamily   = nChipFamily;
    m_nChipRevision = nChipRevision;
}

AMDAsic_Impl::~AMDAsic_Impl()
{
    GCN::IDecoder::Destroy(m_pDecoder);
    m_pDecoder=0;
}

 