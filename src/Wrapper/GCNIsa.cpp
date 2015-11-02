#pragma unmanaged
#include "GCNIsa.h"

namespace GCN
{
    uint GetSourceCountForInstruction( VectorInstructions eOp )
    {
        switch( eOp )
        {
        default: return 0;
        
        case V_MAD_F16              :
        case V_MAD_U16              :
        case V_MAD_I16              :
        case V_PERM_B32             :
        case V_FMA_F16              :
        case V_DIV_FIXUP_F16        :
            return 3;

        case V_CVT_F16_U16        :      
        case V_CVT_F16_I16        :   
        case V_CVT_U16_F16        :   
        case V_CVT_I16_F16        :   
        case V_RCP_F16            :   
        case V_SQRT_F16           :   
        case V_RSQ_F16            :   
        case V_LOG_F16            :   
        case V_EXP_F16            :   
        case V_FREXP_MANT_F16     :   
        case V_FREXP_EXP_I16_F16  :   
        case V_FLOOR_F16          :   
        case V_CEIL_F16           :   
        case V_TRUNC_F16          :   
        case V_RNDNE_F16          :   
        case V_FRACT_F16          :   
        case V_SIN_F16            :   
        case V_COS_F16            :   
            return 1;

        case V_ADD_U32:
        case V_SUB_U32:
        case V_SUBREV_U32:
        case V_LSHLREV_B64:
        case V_LSHRREV_B64:
        case V_ASHRREV_I64:
        case V_ADD_F16            : 
        case V_SUB_F16            : 
        case V_SUBREV_F16         : 
        case V_MUL_F16            : 
        case V_MAC_F16            : 
        case V_MADMK_F16          : 
        case V_MADAK_F16          : 
        case V_ADD_U16            : 
        case V_SUB_U16            : 
        case V_SUBREV_U16         : 
        case V_MUL_LO_U16         : 
        case V_LSHLREV_B16        : 
        case V_LSHRREV_B16        : 
        case V_ASHRREV_I16        : 
        case V_MAX_F16            : 
        case V_MIN_F16            : 
        case V_MAX_U16            : 
        case V_MAX_I16            : 
        case V_MIN_U16            : 
        case V_MIN_I16            : 
        case V_LDEXP_F16          : 
        case V_CNDMASK_B32        :     
        case V_READLANE_B32             :                          
        case V_WRITELANE_B32            :
        case V_ADD_F32                  :
        case V_SUB_F32                  :
        case V_SUBREV_F32               :
        case V_MAC_LEGACY_F32           :
        case V_MUL_LEGACY_F32           :
        case V_MUL_F32                  :
        case V_MUL_I32_I24              :
        case V_MUL_HI_I32_I24           :
        case V_MUL_U32_U24              :
        case V_MUL_HI_U32_U24           :
        case V_MIN_LEGACY_F32           :
        case V_MAX_LEGACY_F32           :
        case V_MIN_F32                  :
        case V_MAX_F32                  :
        case V_MIN_I32                  :
        case V_MAX_I32                  :
        case V_MIN_U32                  :
        case V_MAX_U32                  :
        case V_LSHR_B32                 :
        case V_LSHRREV_B32              :
        case V_ASHR_I32                 :
        case V_ASHRREV_I32              :
        case V_LSHL_B32                 :
        case V_LSHLREV_B32              :
        case V_AND_B32                  :
        case V_OR_B32                   :
        case V_XOR_B32                  :
        case V_BFM_B32                  :
        case V_MAC_F32                  :
        case V_MADMK_F32                :
        case V_MADAK_F32                :
        case V_BCNT_U32_B32             :
        case V_MBCNT_LO_U32_B32         :                                                          
        case V_MBCNT_HI_U32_B32         :                    
        case V_ADD_I32                  :
        case V_SUB_I32                  :
        case V_SUBREV_I32               :
        case V_ADDC_U32                 :
        case V_SUBB_U32                 :
        case V_SUBBREV_U32              :
        case V_LDEXP_F32                :
        case V_CVT_PKACCUM_U8_F32       :
        case V_CVT_PKNORM_I16_F32       :
        case V_CVT_PKNORM_U16_F32       :
        case V_CVT_PKRTZ_F16_F32        :
        case V_CVT_PK_U16_U32           :
        case V_CVT_PK_I16_I32           :
        case V_CMP_F_F32            :
        case V_CMP_LT_F32           :
        case V_CMP_EQ_F32           :
        case V_CMP_LE_F32           :
        case V_CMP_GT_F32           :
        case V_CMP_LG_F32           :
        case V_CMP_GE_F32           :
        case V_CMP_O_F32            :
        case V_CMP_U_F32            :
        case V_CMP_NGE_F32          :
        case V_CMP_NLG_F32          :
        case V_CMP_NGT_F32          :
        case V_CMP_NLE_F32          :
        case V_CMP_NEQ_F32          :
        case V_CMP_NLT_F32          :
        case V_CMP_TRU_F32          :
        case V_CMPX_F_F32           :
        case V_CMPX_LT_F32          :
        case V_CMPX_EQ_F32          :
        case V_CMPX_LE_F32          :
        case V_CMPX_GT_F32          :
        case V_CMPX_LG_F32          :
        case V_CMPX_GE_F32          :
        case V_CMPX_O_F32           :
        case V_CMPX_U_F32           :
        case V_CMPX_NGE_F32         :
        case V_CMPX_NLG_F32         :
        case V_CMPX_NGT_F32         :
        case V_CMPX_NLE_F32         :
        case V_CMPX_NEQ_F32         :
        case V_CMPX_NLT_F32         :
        case V_CMPX_TRU_F32         :
        case V_CMP_F_F64            :
        case V_CMP_LT_F64           :
        case V_CMP_EQ_F64           :
        case V_CMP_LE_F64           :
        case V_CMP_GT_F64           :
        case V_CMP_LG_F64           :
        case V_CMP_GE_F64           :
        case V_CMP_O_F64            :
        case V_CMP_U_F64            :
        case V_CMP_NGE_F64          :
        case V_CMP_NLG_F64          :
        case V_CMP_NGT_F64          :
        case V_CMP_NLE_F64          :
        case V_CMP_NEQ_F64          :
        case V_CMP_NLT_F64          :
        case V_CMP_TRU_F64          :
        case V_CMPX_F_F64           :
        case V_CMPX_LT_F64          :
        case V_CMPX_EQ_F64          :
        case V_CMPX_LE_F64          :
        case V_CMPX_GT_F64          :
        case V_CMPX_LG_F64          :
        case V_CMPX_GE_F64          :
        case V_CMPX_O_F64           :
        case V_CMPX_U_F64           :
        case V_CMPX_NGE_F64         :
        case V_CMPX_NLG_F64         :
        case V_CMPX_NGT_F64         :
        case V_CMPX_NLE_F64         :
        case V_CMPX_NEQ_F64         :
        case V_CMPX_NLT_F64         :
        case V_CMPX_TRU_F64         :
        case V_CMPS_F_F32           :
        case V_CMPS_LT_F32          :
        case V_CMPS_EQ_F32          :
        case V_CMPS_LE_F32          :
        case V_CMPS_GT_F32          :
        case V_CMPS_LG_F32          :
        case V_CMPS_GE_F32          :
        case V_CMPS_O_F32           :
        case V_CMPS_U_F32           :
        case V_CMPS_NGE_F32         :
        case V_CMPS_NLG_F32         :
        case V_CMPS_NGT_F32         :
        case V_CMPS_NLE_F32         :
        case V_CMPS_NEQ_F32         :
        case V_CMPS_NLT_F32         :
        case V_CMPS_TRU_F32         :
        case V_CMPSX_F_F32          :
        case V_CMPSX_LT_F32         :
        case V_CMPSX_EQ_F32         :
        case V_CMPSX_LE_F32         :
        case V_CMPSX_GT_F32         :
        case V_CMPSX_LG_F32         :
        case V_CMPSX_GE_F32         :
        case V_CMPSX_O_F32          :
        case V_CMPSX_U_F32          :
        case V_CMPSX_NGE_F32        :
        case V_CMPSX_NLG_F32        :
        case V_CMPSX_NGT_F32        :
        case V_CMPSX_NLE_F32        :
        case V_CMPSX_NEQ_F32        :
        case V_CMPSX_NLT_F32        :
        case V_CMPSX_TRU_F32        :
        case V_CMPS_F_F64           :
        case V_CMPS_LT_F64          :
        case V_CMPS_EQ_F64          :
        case V_CMPS_LE_F64          :
        case V_CMPS_GT_F64          :
        case V_CMPS_LG_F64          :
        case V_CMPS_GE_F64          :
        case V_CMPS_O_F64           :
        case V_CMPS_U_F64           :
        case V_CMPS_NGE_F64         :
        case V_CMPS_NLG_F64         :
        case V_CMPS_NGT_F64         :
        case V_CMPS_NLE_F64         :
        case V_CMPS_NEQ_F64         :
        case V_CMPS_NLT_F64         :
        case V_CMPS_TRU_F64         :
        case V_CMPSX_F_F64          :
        case V_CMPSX_LT_F64         :
        case V_CMPSX_EQ_F64         :
        case V_CMPSX_LE_F64         :
        case V_CMPSX_GT_F64         :
        case V_CMPSX_LG_F64         :
        case V_CMPSX_GE_F64         :
        case V_CMPSX_O_F64          :
        case V_CMPSX_U_F64          :
        case V_CMPSX_NGE_F64        :
        case V_CMPSX_NLG_F64        :
        case V_CMPSX_NGT_F64        :
        case V_CMPSX_NLE_F64        :
        case V_CMPSX_NEQ_F64        :
        case V_CMPSX_NLT_F64        :
        case V_CMPSX_TRU_F64        :
        case V_CMP_F_I32            :
        case V_CMP_LT_I32           :
        case V_CMP_EQ_I32           :
        case V_CMP_LE_I32           :
        case V_CMP_GT_I32           :
        case V_CMP_LG_I32           :
        case V_CMP_GE_I32           :
        case V_CMP_TRU_I32          :
        case V_CMPX_F_I32           :
        case V_CMPX_LT_I32          :
        case V_CMPX_EQ_I32          :
        case V_CMPX_LE_I32          :
        case V_CMPX_GT_I32          :
        case V_CMPX_LG_I32          :
        case V_CMPX_GE_I32          :
        case V_CMPX_TRU_I32         :
        case V_CMP_F_I64            :
        case V_CMP_LT_I64           :
        case V_CMP_EQ_I64           :
        case V_CMP_LE_I64           :
        case V_CMP_GT_I64           :
        case V_CMP_LG_I64           :
        case V_CMP_GE_I64           :
        case V_CMP_TRU_I64          :
        case V_CMPX_F_I64           :
        case V_CMPX_LT_I64          :
        case V_CMPX_EQ_I64          :
        case V_CMPX_LE_I64          :
        case V_CMPX_GT_I64          :
        case V_CMPX_LG_I64          :
        case V_CMPX_GE_I64          :
        case V_CMPX_TRU_I64         :
        case V_CMP_F_U32            :
        case V_CMP_LT_U32           :
        case V_CMP_EQ_U32           :
        case V_CMP_LE_U32           :
        case V_CMP_GT_U32           :
        case V_CMP_LG_U32           :
        case V_CMP_GE_U32           :
        case V_CMP_TRU_U32          :
        case V_CMPX_F_U32           :
        case V_CMPX_LT_U32          :
        case V_CMPX_EQ_U32          :
        case V_CMPX_LE_U32          :
        case V_CMPX_GT_U32          :
        case V_CMPX_LG_U32          :
        case V_CMPX_GE_U32          :
        case V_CMPX_TRU_U32         :
        case V_CMP_F_U64            :
        case V_CMP_LT_U64           :
        case V_CMP_EQ_U64           :
        case V_CMP_LE_U64           :
        case V_CMP_GT_U64           :
        case V_CMP_LG_U64           :
        case V_CMP_GE_U64           :
        case V_CMP_TRU_U64          :
        case V_CMPx_F_U64           :
        case V_CMPx_LT_U64          :
        case V_CMPx_EQ_U64          :
        case V_CMPx_LE_U64          :
        case V_CMPx_GT_U64          :
        case V_CMPx_LG_U64          :
        case V_CMPx_GE_U64          :
        case V_CMPx_TRU_U64         :
        case V_CMP_CLASS_F32        :
        case V_CMPX_CLASS_F32       :
        case V_CMP_CLASS_F64        :
        case V_CMPX_CLASS_F64       :

        case V_CMP_F_F16         :
        case V_CMP_LT_F16        :
        case V_CMP_EQ_F16        :
        case V_CMP_LE_F16        :
        case V_CMP_GT_F16        :
        case V_CMP_LG_F16        :
        case V_CMP_GE_F16        :
        case V_CMP_O_F16         :
        case V_CMP_U_F16         :
        case V_CMP_NGE_F16       :
        case V_CMP_NLG_F16       :
        case V_CMP_NGT_F16       :
        case V_CMP_NLE_F16       :
        case V_CMP_NEQ_F16       :
        case V_CMP_NLT_F16       :
        case V_CMP_TRU_F16       :
        case V_CMPX_F_F16        :
        case V_CMPX_LT_F16       :
        case V_CMPX_EQ_F16       :
        case V_CMPX_LE_F16       :
        case V_CMPX_GT_F16       :
        case V_CMPX_LG_F16       :
        case V_CMPX_GE_F16       :
        case V_CMPX_O_F16        :
        case V_CMPX_U_F16        :
        case V_CMPX_NGE_F16      :
        case V_CMPX_NLG_F16      :
        case V_CMPX_NGT_F16      :
        case V_CMPX_NLE_F16      :
        case V_CMPX_NEQ_F16      :
        case V_CMPX_NLT_F16      :
        case V_CMPX_TRU_F16      :
        case V_CMP_F_U16         :
        case V_CMP_LT_U16        :
        case V_CMP_EQ_U16        :
        case V_CMP_LE_U16        :
        case V_CMP_GT_U16        :
        case V_CMP_LG_U16        :
        case V_CMP_GE_U16        :
        case V_CMP_TRU_U16       :
        case V_CMPx_F_U16        :
        case V_CMPx_LT_U16       :
        case V_CMPx_EQ_U16       :
        case V_CMPx_LE_U16       :
        case V_CMPx_GT_U16       :
        case V_CMPx_LG_U16       :
        case V_CMPx_GE_U16       :
        case V_CMPx_TRU_U16      :
        case V_CMP_F_I16         :
        case V_CMP_LT_I16        :
        case V_CMP_EQ_I16        :
        case V_CMP_LE_I16        :
        case V_CMP_GT_I16        :
        case V_CMP_LG_I16        :
        case V_CMP_GE_I16        :
        case V_CMP_TRU_I16       :
        case V_CMPx_F_I16        :
        case V_CMPx_LT_I16       :
        case V_CMPx_EQ_I16       :
        case V_CMPx_LE_I16       :
        case V_CMPx_GT_I16       :
        case V_CMPx_LG_I16       :
        case V_CMPx_GE_I16       :
        case V_CMPx_TRU_I16      :
        case V_CMP_CLASS_F16:
        case V_CMPX_CLASS_F16:
            return 2;

        case V_MOV_B32                 :
        case V_READFIRSTLANE_B32       :                                        
        case V_CVT_I32_F64             :
        case V_CVT_F64_I32             :
        case V_CVT_F32_I32             :
        case V_CVT_F32_U32             :
        case V_CVT_U32_F32             :
        case V_CVT_I32_F32             :
        case V_MOV_FED_B32             :
        case V_CVT_F16_F32             :
        case V_CVT_F32_F16             :
        case V_CVT_RPI_I32_F32         :
        case V_CVT_FLR_I32_F32         :
        case V_CVT_OFF_F32_I4          :
        case V_CVT_F32_F64             :
        case V_CVT_F64_F32             :
        case V_CVT_F32_UBYTE0          :
        case V_CVT_F32_UBYTE1          :
        case V_CVT_F32_UBYTE2          :
        case V_CVT_F32_UBYTE3          :
        case V_CVT_U32_F64             :
        case V_CVT_F64_U32             :
        case V_TRUNC_F64               :
        case V_CEIL_F64                :
        case V_RNDNE_F64               :
        case V_FLOOR_F64               :
        case V_FRACT_F32               :
        case V_TRUNC_F32               :
        case V_CEIL_F32                :
        case V_RNDNE_F32               :
        case V_FLOOR_F32               :
        case V_EXP_F32                 :
        case V_LOG_CLAMP_F32           :
        case V_LOG_F32                 :
        case V_RCP_CLAMP_F32           :
        case V_RCP_LEGACY_F32          :
        case V_RCP_F32                 :
        case V_RCP_IFLAG_F32           :
        case V_RSQ_CLAMP_F32           :
        case V_RSQ_LEGACY_F32          :
        case V_RSQ_F32                 :
        case V_RCP_F64                 :
        case V_RCP_CLAMP_F64           :
        case V_RSQ_F64                 :
        case V_RSQ_CLAMP_F64           :
        case V_SQRT_F32                :
        case V_SQRT_F64                :
        case V_SIN_F32                 :
        case V_COS_F32                 :
        case V_NOT_B32                 :
        case V_BFREV_B32               :
        case V_FFBH_U32                :
        case V_FFBL_B32                :
        case V_FFBH_I32                :
        case V_FREXP_EXP_I32_F64       :
        case V_FREXP_MANT_F64          :
        case V_FRACT_F64               :
        case V_FREXP_EXP_I32_F32       :                 
        case V_FREXP_MANT_F32          :
        case V_MOVRELD_B32             :
        case V_MOVRELS_B32             :
        case V_MOVRELSD_B32            :
        case V_LOG_LEGACY_F32          :
        case V_EXP_LEGACY_F32          :
            return 1;

        case V_MULLIT_F32       :   //  = D.f = S0.f * S1.f, replicate result into 4 components (0.0 * x = 0.0; special INF, NaN, overflow rules).
        case V_LSHL_B64         :   //  = D = S0.u << S1.u[4:0].
        case V_LSHR_B64         :   //  = D = S0.u >> S1.u[4:0].
        case V_ASHR_I64         :   //  = D = S0.u >> S1.u[4:0].
        case V_ADD_F64          :   //  = D.d = S0.d + S1.d.
        case V_MUL_F64          :   //  = D.d = S0.d * S1.d.
        case V_MIN_F64          :   //  = D.d = min(S0.d, S1.d).
        case V_MAX_F64          :   //  = D.d = max(S0.d, S1.d).
        case V_LDEXP_F64        :   //  = D.d = pow(S0.d, S1.i[31:0]).
        case V_MUL_LO_U32       :   //  = D.u = S0.u * S1.u.
        case V_MUL_HI_U32       :   //  = D.u = (S0.u * S1.u)>>32.
        case V_MUL_LO_I32       :   //  = D.i = S0.i * S1.i.
        case V_MUL_HI_I32       :   //  = D.i = (S0.i * S1.i)>>32.
        case V_TRIG_PREOP_F64  : // = D.d = Look Up 2/PI (S0.d) with segment select  S1.u[4:0].
            return 2;

        case V_DIV_SCALE_F32    :    //  = D.f = Special case divide preop and flags(s0.f = Quotient,s1.f = Denominator, s2.f = Numerator) s0 must equal s1 or s2.
        case V_DIV_SCALE_F64    :    //  = D.d = Special case divide preop and flags(s0.d = Quotient,s1.d = Denominator, s2.d = Numerator) s0 must equal s1 or s2.
        case V_MAD_LEGACY_F32   :   //   = D.f = S0.f * S1.f + S2.f (DX9 rules, 0.0*x = 0.0).
        case V_MAD_F32          :   //   = D.f = S0.f * S1.f + S2.f.
        case V_MAD_I32_I24      :   //   = D.i = S0.i * S1.i + S2.iD.i = S0.i * S1.i + S2.i.
        case V_MAD_U32_U24      :   //   = D.u = S0.u * S1.u + S2.u.
        case V_CUBEID_F32       :   //   = Rm.w <- Rn,x, Rn,y, Rn.z.
        case V_CUBESC_F32       :   //   = Rm.y <- Rn,x, Rn,y, Rn.z.
        case V_CUBETC_F32       :   //   = Rm.x <- Rn,x, Rn,y, Rn.z.
        case V_CUBEMA_F32       :   //   = Rm.z <- Rn,x, Rn,y, Rn.z
        case V_BFE_U32          :   //   = D.u = (S0.u>>S1.u[4:0]) & ((1<<S2.u[4:0])-1); bitfield extract,S0=data, S1=field_offset, S2=field_width.
        case V_BFE_I32          :   //   = D.i = (S0.i>>S1.u[4:0]) & ((1<<S2.u[4:0])-1); bitfield extract, S0=data, S1=field_offset, S2=field_width.
        case V_BFI_B32          :   //   = D.u = (S0.u & S1.u) | (~S0.u & S2.u); bitfield insert.
        case V_FMA_F32          :   //   = D.f = S0.f * S1.f + S2.f
        case V_FMA_F64          :   //   = D.d = S0.d * S1.d + S2.d.
        case V_LERP_U8          :   //  = D.u = ((S0.u[31:24] + S1.u[31:24] + S2.u[24]) >> 1) << 24 +
        case V_ALIGNBIT_B32     :   //  = D.u = ({S0,S1} >> S2.u[4:0]) & 0xFFFFFFFF.
        case V_ALIGNBYTE_B32    :   //  = D.u = ({S0,S1} >> (8*S2.u[4:0])) & 0xFFFFFFFF.
        case V_MIN3_F32         :   //  = D.f = min(S0.f, S1.f, S2.f).
        case V_MIN3_I32         :   //  = D.i = min(S0.i, S1.i, S2.i).
        case V_MIN3_U32         :   //  = 0x153 D.u = min(S0.u, S1.u, S2.u).
        case V_MAX3_F32         :   //  = D.f = max(S0.f, S1.f, S2.f).
        case V_MAX3_I32         :   //  = D.i = max(S0.i, S1.i, S2.i).
        case V_MAX3_U32         :   //  = D.u = max(S0.u, S1.u, S2.u).
        case V_MED3_F32         :   //  = D.f = median(S0.f, S1.f, S2.f).
        case V_MED3_I32         :   //  = D.i = median(S0.i, S1.i, S2.i).
        case V_MED3_U32         :   //  = D.u = median(S0.u, S1.u, S2.u).
        case V_SAD_U8           :   //  = D.u = Byte SAD with accum_lo(S0.u, S1.u, S2.u).
        case V_SAD_HI_U8        :   //  = D.u = Byte SAD with accum_hi(S0.u, S1.u, S2.u).
        case V_SAD_U16          :   //  = D.u = Word SAD with accum(S0.u, S1.u, S2.u).
        case V_SAD_U32          :   //  = D.u = Dword SAD with accum(S0.u, S1.u, S2.u).
        case V_CVT_PK_U8_F32    :   //  = f32->u8(s0.f), pack into byte(s1.u), of dword(s2).
        case V_DIV_FIXUP_F32    :   //  = D.f = Special case divide fixup and flags(s0.f = Quotient,   s1.f = Denominator, s2.f = Numerator).
        case V_DIV_FIXUP_F64    :   //  = D.d = Special case divide fixup and flags(s0.d = Quotient,  s1.d = Denominator, s2.d = Numerator).
        case V_DIV_FMAS_F32     : // = D.f = Special case divide FMA with scale and flags(s0.f = Quotient, s1.f = Denominator, s2.f = Numerator).
        case V_DIV_FMAS_F64     : // = D.d = Special case divide FMA with scale and flags(s0.d= Quotient, s1.d = Denominator, s2.d = Numerator).
        case V_MSAD_U8          : // = D.u = Masked Byte SAD with accum_lo(S0.u, S1.u, S2.u).
        case V_QSAD_U8          : // = D.u = Quad-Byte SAD with accum_lo/hiu(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        case V_MQSAD_U8         : // = D.u = Masked Quad-Byte SAD with accum_lo/hi(S0.u[63:0],S1.u[31:0], S2.u[63:0]).       
        case V_QSAD_PK_U16_U8   : // : D.u = Quad-Byte SAD with accum_lo/hiu(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        case V_MQSAD_PK_U16_U8  : // = D.u = Masked Quad-Byte SAD with accum_lo/hi(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        case V_MQSAD_U32_U8     :  // : D.u128 = Masked Quad-Byte SAD with 32-bit accum_lo/hi(S0.u[63:0], S1.u[31:0], S2.u[127:0]).
        case V_MAD_U64_U32      :  // : {vcc_out,D.u64} = S0.u32 * S1.u32 + S2.u64.
        case V_MAD_I64_I32      :  // : {vcc_out,D.i64} = S0.i32 * S1.i32 + S2.i64.
            return 3;

        case V_INTERP_P1_F32  :
        case V_INTERP_P2_F32  :
        case V_INTERP_MOV_F32 :
        case V_INTERP_P1LL_F16:
        case V_INTERP_P1LV_F16:
        case V_INTERP_P2_F16  :
            return 1;
        }
    }




    uint GetResultWidthInDWORDs( VectorInstructions e )
    {
        switch( e )
        {
        default: return 0;
        case V_ADD_U32:
        case V_SUB_U32:
        case V_SUBREV_U32:
            return 1;

        case V_LSHLREV_B64:
        case V_LSHRREV_B64:
        case V_ASHRREV_I64:
            return 2;
                
        case V_MAD_F16           : 
        case V_MAD_U16           : 
        case V_MAD_I16           : 
        case V_PERM_B32          : 
        case V_FMA_F16           : 
        case V_DIV_FIXUP_F16     : 
        case V_ADD_F16          :   
        case V_SUB_F16          :   
        case V_SUBREV_F16       :   
        case V_MUL_F16          :   
        case V_MAC_F16          :   
        case V_MADMK_F16        :   
        case V_MADAK_F16        :   
        case V_ADD_U16          :   
        case V_SUB_U16          :   
        case V_SUBREV_U16       :   
        case V_MUL_LO_U16       :   
        case V_LSHLREV_B16      :   
        case V_LSHRREV_B16      :   
        case V_ASHRREV_I16      :   
        case V_MAX_F16          :   
        case V_MIN_F16          :   
        case V_MAX_U16          :   
        case V_MAX_I16          :   
        case V_MIN_U16          :   
        case V_MIN_I16          :   
        case V_LDEXP_F16        :   
        case V_CVT_F16_U16            :
        case V_CVT_F16_I16            :
        case V_CVT_U16_F16            :
        case V_CVT_I16_F16            :
        case V_RCP_F16                :
        case V_SQRT_F16               :
        case V_RSQ_F16                :
        case V_LOG_F16                :
        case V_EXP_F16                :
        case V_FREXP_MANT_F16         :
        case V_FREXP_EXP_I16_F16      :
        case V_FLOOR_F16              :
        case V_CEIL_F16               :
        case V_TRUNC_F16              :
        case V_RNDNE_F16              :
        case V_FRACT_F16              :
        case V_SIN_F16                :
        case V_COS_F16                :
        case V_CNDMASK_B32              :
        case V_READLANE_B32             :                          
        case V_WRITELANE_B32            :
        case V_ADD_F32                  :
        case V_SUB_F32                  :
        case V_SUBREV_F32               :
        case V_MAC_LEGACY_F32           :
        case V_MUL_LEGACY_F32           :
        case V_MUL_F32                  :
        case V_MUL_I32_I24              :
        case V_MUL_HI_I32_I24           :
        case V_MUL_U32_U24              :
        case V_MUL_HI_U32_U24           :
        case V_MIN_LEGACY_F32           :
        case V_MAX_LEGACY_F32           :
        case V_MIN_F32                  :
        case V_MAX_F32                  :
        case V_MIN_I32                  :
        case V_MAX_I32                  :
        case V_MIN_U32                  :
        case V_MAX_U32                  :
        case V_LSHR_B32                 :
        case V_LSHRREV_B32              :
        case V_ASHR_I32                 :
        case V_ASHRREV_I32              :
        case V_LSHL_B32                 :
        case V_LSHLREV_B32              :
        case V_AND_B32                  :
        case V_OR_B32                   :
        case V_XOR_B32                  :
        case V_BFM_B32                  :
        case V_MAC_F32                  :
        case V_MADMK_F32                :
        case V_MADAK_F32                :
        case V_BCNT_U32_B32             :
        case V_MBCNT_LO_U32_B32         :                                                          
        case V_MBCNT_HI_U32_B32         :                    
        case V_ADD_I32                  :
        case V_SUB_I32                  :
        case V_SUBREV_I32               :
        case V_ADDC_U32                 :
        case V_SUBB_U32                 :
        case V_SUBBREV_U32              :
        case V_LDEXP_F32                :
        case V_CVT_PKACCUM_U8_F32       :
        case V_CVT_PKNORM_I16_F32       :
        case V_CVT_PKNORM_U16_F32       :
        case V_CVT_PKRTZ_F16_F32        :
        case V_CVT_PK_U16_U32           :
        case V_CVT_PK_I16_I32           :
            return 1;

        case V_CMP_CLASS_F16:
        case V_CMPX_CLASS_F16:
        case V_CMP_F_F32            :
        case V_CMP_LT_F32           :
        case V_CMP_EQ_F32           :
        case V_CMP_LE_F32           :
        case V_CMP_GT_F32           :
        case V_CMP_LG_F32           :
        case V_CMP_GE_F32           :
        case V_CMP_O_F32            :
        case V_CMP_U_F32            :
        case V_CMP_NGE_F32          :
        case V_CMP_NLG_F32          :
        case V_CMP_NGT_F32          :
        case V_CMP_NLE_F32          :
        case V_CMP_NEQ_F32          :
        case V_CMP_NLT_F32          :
        case V_CMP_TRU_F32          :
        case V_CMPX_F_F32           :
        case V_CMPX_LT_F32          :
        case V_CMPX_EQ_F32          :
        case V_CMPX_LE_F32          :
        case V_CMPX_GT_F32          :
        case V_CMPX_LG_F32          :
        case V_CMPX_GE_F32          :
        case V_CMPX_O_F32           :
        case V_CMPX_U_F32           :
        case V_CMPX_NGE_F32         :
        case V_CMPX_NLG_F32         :
        case V_CMPX_NGT_F32         :
        case V_CMPX_NLE_F32         :
        case V_CMPX_NEQ_F32         :
        case V_CMPX_NLT_F32         :
        case V_CMPX_TRU_F32         :
        case V_CMP_F_F64            :
        case V_CMP_LT_F64           :
        case V_CMP_EQ_F64           :
        case V_CMP_LE_F64           :
        case V_CMP_GT_F64           :
        case V_CMP_LG_F64           :
        case V_CMP_GE_F64           :
        case V_CMP_O_F64            :
        case V_CMP_U_F64            :
        case V_CMP_NGE_F64          :
        case V_CMP_NLG_F64          :
        case V_CMP_NGT_F64          :
        case V_CMP_NLE_F64          :
        case V_CMP_NEQ_F64          :
        case V_CMP_NLT_F64          :
        case V_CMP_TRU_F64          :
        case V_CMPX_F_F64           :
        case V_CMPX_LT_F64          :
        case V_CMPX_EQ_F64          :
        case V_CMPX_LE_F64          :
        case V_CMPX_GT_F64          :
        case V_CMPX_LG_F64          :
        case V_CMPX_GE_F64          :
        case V_CMPX_O_F64           :
        case V_CMPX_U_F64           :
        case V_CMPX_NGE_F64         :
        case V_CMPX_NLG_F64         :
        case V_CMPX_NGT_F64         :
        case V_CMPX_NLE_F64         :
        case V_CMPX_NEQ_F64         :
        case V_CMPX_NLT_F64         :
        case V_CMPX_TRU_F64         :
        case V_CMPS_F_F32           :
        case V_CMPS_LT_F32          :
        case V_CMPS_EQ_F32          :
        case V_CMPS_LE_F32          :
        case V_CMPS_GT_F32          :
        case V_CMPS_LG_F32          :
        case V_CMPS_GE_F32          :
        case V_CMPS_O_F32           :
        case V_CMPS_U_F32           :
        case V_CMPS_NGE_F32         :
        case V_CMPS_NLG_F32         :
        case V_CMPS_NGT_F32         :
        case V_CMPS_NLE_F32         :
        case V_CMPS_NEQ_F32         :
        case V_CMPS_NLT_F32         :
        case V_CMPS_TRU_F32         :
        case V_CMPSX_F_F32          :
        case V_CMPSX_LT_F32         :
        case V_CMPSX_EQ_F32         :
        case V_CMPSX_LE_F32         :
        case V_CMPSX_GT_F32         :
        case V_CMPSX_LG_F32         :
        case V_CMPSX_GE_F32         :
        case V_CMPSX_O_F32          :
        case V_CMPSX_U_F32          :
        case V_CMPSX_NGE_F32        :
        case V_CMPSX_NLG_F32        :
        case V_CMPSX_NGT_F32        :
        case V_CMPSX_NLE_F32        :
        case V_CMPSX_NEQ_F32        :
        case V_CMPSX_NLT_F32        :
        case V_CMPSX_TRU_F32        :
        case V_CMPS_F_F64           :
        case V_CMPS_LT_F64          :
        case V_CMPS_EQ_F64          :
        case V_CMPS_LE_F64          :
        case V_CMPS_GT_F64          :
        case V_CMPS_LG_F64          :
        case V_CMPS_GE_F64          :
        case V_CMPS_O_F64           :
        case V_CMPS_U_F64           :
        case V_CMPS_NGE_F64         :
        case V_CMPS_NLG_F64         :
        case V_CMPS_NGT_F64         :
        case V_CMPS_NLE_F64         :
        case V_CMPS_NEQ_F64         :
        case V_CMPS_NLT_F64         :
        case V_CMPS_TRU_F64         :
        case V_CMPSX_F_F64          :
        case V_CMPSX_LT_F64         :
        case V_CMPSX_EQ_F64         :
        case V_CMPSX_LE_F64         :
        case V_CMPSX_GT_F64         :
        case V_CMPSX_LG_F64         :
        case V_CMPSX_GE_F64         :
        case V_CMPSX_O_F64          :
        case V_CMPSX_U_F64          :
        case V_CMPSX_NGE_F64        :
        case V_CMPSX_NLG_F64        :
        case V_CMPSX_NGT_F64        :
        case V_CMPSX_NLE_F64        :
        case V_CMPSX_NEQ_F64        :
        case V_CMPSX_NLT_F64        :
        case V_CMPSX_TRU_F64        :
        case V_CMP_F_I32            :
        case V_CMP_LT_I32           :
        case V_CMP_EQ_I32           :
        case V_CMP_LE_I32           :
        case V_CMP_GT_I32           :
        case V_CMP_LG_I32           :
        case V_CMP_GE_I32           :
        case V_CMP_TRU_I32          :
        case V_CMPX_F_I32           :
        case V_CMPX_LT_I32          :
        case V_CMPX_EQ_I32          :
        case V_CMPX_LE_I32          :
        case V_CMPX_GT_I32          :
        case V_CMPX_LG_I32          :
        case V_CMPX_GE_I32          :
        case V_CMPX_TRU_I32         :
        case V_CMP_F_I64            :
        case V_CMP_LT_I64           :
        case V_CMP_EQ_I64           :
        case V_CMP_LE_I64           :
        case V_CMP_GT_I64           :
        case V_CMP_LG_I64           :
        case V_CMP_GE_I64           :
        case V_CMP_TRU_I64          :
        case V_CMPX_F_I64           :
        case V_CMPX_LT_I64          :
        case V_CMPX_EQ_I64          :
        case V_CMPX_LE_I64          :
        case V_CMPX_GT_I64          :
        case V_CMPX_LG_I64          :
        case V_CMPX_GE_I64          :
        case V_CMPX_TRU_I64         :
        case V_CMP_F_U32            :
        case V_CMP_LT_U32           :
        case V_CMP_EQ_U32           :
        case V_CMP_LE_U32           :
        case V_CMP_GT_U32           :
        case V_CMP_LG_U32           :
        case V_CMP_GE_U32           :
        case V_CMP_TRU_U32          :
        case V_CMPX_F_U32           :
        case V_CMPX_LT_U32          :
        case V_CMPX_EQ_U32          :
        case V_CMPX_LE_U32          :
        case V_CMPX_GT_U32          :
        case V_CMPX_LG_U32          :
        case V_CMPX_GE_U32          :
        case V_CMPX_TRU_U32         :
        case V_CMP_F_U64            :
        case V_CMP_LT_U64           :
        case V_CMP_EQ_U64           :
        case V_CMP_LE_U64           :
        case V_CMP_GT_U64           :
        case V_CMP_LG_U64           :
        case V_CMP_GE_U64           :
        case V_CMP_TRU_U64          :
        case V_CMPx_F_U64           :
        case V_CMPx_LT_U64          :
        case V_CMPx_EQ_U64          :
        case V_CMPx_LE_U64          :
        case V_CMPx_GT_U64          :
        case V_CMPx_LG_U64          :
        case V_CMPx_GE_U64          :
        case V_CMPx_TRU_U64         :
        case V_CMP_CLASS_F32        :
        case V_CMPX_CLASS_F32       :
        case V_CMP_CLASS_F64        :
        case V_CMPX_CLASS_F64       :
        case V_CMP_F_F16       :
        case V_CMP_LT_F16      :
        case V_CMP_EQ_F16      :
        case V_CMP_LE_F16      :
        case V_CMP_GT_F16      :
        case V_CMP_LG_F16      :
        case V_CMP_GE_F16      :
        case V_CMP_O_F16       :
        case V_CMP_U_F16       :
        case V_CMP_NGE_F16     :
        case V_CMP_NLG_F16     :
        case V_CMP_NGT_F16     :
        case V_CMP_NLE_F16     :
        case V_CMP_NEQ_F16     :
        case V_CMP_NLT_F16     :
        case V_CMP_TRU_F16     :
        case V_CMPX_F_F16      :
        case V_CMPX_LT_F16     :
        case V_CMPX_EQ_F16     :
        case V_CMPX_LE_F16     :
        case V_CMPX_GT_F16     :
        case V_CMPX_LG_F16     :
        case V_CMPX_GE_F16     :
        case V_CMPX_O_F16      :
        case V_CMPX_U_F16      :
        case V_CMPX_NGE_F16    :
        case V_CMPX_NLG_F16    :
        case V_CMPX_NGT_F16    :
        case V_CMPX_NLE_F16    :
        case V_CMPX_NEQ_F16    :
        case V_CMPX_NLT_F16    :
        case V_CMPX_TRU_F16    :
        case V_CMP_F_U16       :
        case V_CMP_LT_U16      :
        case V_CMP_EQ_U16      :
        case V_CMP_LE_U16      :
        case V_CMP_GT_U16      :
        case V_CMP_LG_U16      :
        case V_CMP_GE_U16      :
        case V_CMP_TRU_U16     :
        case V_CMPx_F_U16      :
        case V_CMPx_LT_U16     :
        case V_CMPx_EQ_U16     :
        case V_CMPx_LE_U16     :
        case V_CMPx_GT_U16     :
        case V_CMPx_LG_U16     :
        case V_CMPx_GE_U16     :
        case V_CMPx_TRU_U16    :
        case V_CMP_F_I16       :
        case V_CMP_LT_I16      :
        case V_CMP_EQ_I16      :
        case V_CMP_LE_I16      :
        case V_CMP_GT_I16      :
        case V_CMP_LG_I16      :
        case V_CMP_GE_I16      :
        case V_CMP_TRU_I16     :
        case V_CMPx_F_I16      :
        case V_CMPx_LT_I16     :
        case V_CMPx_EQ_I16     :
        case V_CMPx_LE_I16     :
        case V_CMPx_GT_I16     :
        case V_CMPx_LG_I16     :
        case V_CMPx_GE_I16     :
        case V_CMPx_TRU_I16    :
            return 2;

        case V_MOV_B32                 :
        case V_READFIRSTLANE_B32       :                                        
        case V_CVT_I32_F64             :
        case V_CVT_F32_I32             :
        case V_CVT_F32_U32             :
        case V_CVT_U32_F32             :
        case V_CVT_I32_F32             :
        case V_MOV_FED_B32             :
        case V_CVT_F16_F32             :
        case V_CVT_F32_F16             :
        case V_CVT_RPI_I32_F32         :
        case V_CVT_FLR_I32_F32         :
        case V_CVT_OFF_F32_I4          :
        case V_CVT_F32_F64             :
        case V_CVT_F32_UBYTE0          :
        case V_CVT_F32_UBYTE1          :
        case V_CVT_F32_UBYTE2          :
        case V_CVT_F32_UBYTE3          :
        case V_CVT_U32_F64             :
        case V_FRACT_F32               :
        case V_TRUNC_F32               :
        case V_CEIL_F32                :
        case V_RNDNE_F32               :
        case V_FLOOR_F32               :
        case V_EXP_F32                 :
        case V_LOG_CLAMP_F32           :
        case V_LOG_F32                 :
        case V_RCP_CLAMP_F32           :
        case V_RCP_LEGACY_F32          :
        case V_RCP_F32                 :
        case V_RCP_IFLAG_F32           :
        case V_RSQ_CLAMP_F32           :
        case V_RSQ_LEGACY_F32          :
        case V_RSQ_F32                 :
        case V_SQRT_F32                :
        case V_SIN_F32                 :
        case V_COS_F32                 :
        case V_NOT_B32                 :
        case V_BFREV_B32               :
        case V_FFBH_U32                :
        case V_FFBL_B32                :
        case V_FFBH_I32                :
        case V_FREXP_EXP_I32_F64       :
        case V_FREXP_EXP_I32_F32       :                 
        case V_FREXP_MANT_F32          :
        case V_MOVRELD_B32             :
        case V_MOVRELS_B32             :
        case V_MOVRELSD_B32            :
        case V_LOG_LEGACY_F32          :
        case V_EXP_LEGACY_F32          :
        case V_MULLIT_F32       :   //  = D.f = S0.f * S1.f, replicate result into 4 components (0.0 * x = 0.0; special INF, NaN, overflow rules).
        case V_MUL_LO_U32       :   //  = D.u = S0.u * S1.u.
        case V_MUL_HI_U32       :   //  = D.u = (S0.u * S1.u)>>32.
        case V_MUL_LO_I32       :   //  = D.i = S0.i * S1.i.
        case V_MUL_HI_I32       :   //  = D.i = (S0.i * S1.i)>>32.
        case V_DIV_SCALE_F64    :    //  = D.d = Special case divide preop and flags(s0.d = Quotient,s1.d = Denominator, s2.d = Numerator) s0 must equal s1 or s2.
        case V_MAD_LEGACY_F32   :   //   = D.f = S0.f * S1.f + S2.f (DX9 rules, 0.0*x = 0.0).
        case V_MAD_F32          :   //   = D.f = S0.f * S1.f + S2.f.
        case V_MAD_I32_I24      :   //   = D.i = S0.i * S1.i + S2.iD.i = S0.i * S1.i + S2.i.
        case V_MAD_U32_U24      :   //   = D.u = S0.u * S1.u + S2.u.
        case V_CUBEID_F32       :   //   = Rm.w <- Rn,x, Rn,y, Rn.z.
        case V_CUBESC_F32       :   //   = Rm.y <- Rn,x, Rn,y, Rn.z.
        case V_CUBETC_F32       :   //   = Rm.x <- Rn,x, Rn,y, Rn.z.
        case V_CUBEMA_F32       :   //   = Rm.z <- Rn,x, Rn,y, Rn.z
        case V_BFE_U32          :   //   = D.u = (S0.u>>S1.u[4:0]) & ((1<<S2.u[4:0])-1); bitfield extract,S0=data, S1=field_offset, S2=field_width.
        case V_BFE_I32          :   //   = D.i = (S0.i>>S1.u[4:0]) & ((1<<S2.u[4:0])-1); bitfield extract, S0=data, S1=field_offset, S2=field_width.
        case V_BFI_B32          :   //   = D.u = (S0.u & S1.u) | (~S0.u & S2.u); bitfield insert.
        case V_FMA_F32          :   //   = D.f = S0.f * S1.f + S2.f
             return 1;
        
        
        case V_CVT_F64_I32             :
        case V_CVT_F64_F32             :
        case V_CVT_F64_U32             :
        case V_TRUNC_F64               :
        case V_CEIL_F64                :
        case V_RNDNE_F64               :
        case V_FLOOR_F64               :
        case V_RCP_F64                 :
        case V_RCP_CLAMP_F64           :
        case V_RSQ_F64                 :
        case V_RSQ_CLAMP_F64           :
        case V_SQRT_F64                :
        case V_FREXP_MANT_F64          :
        case V_FRACT_F64               :
        case V_LSHL_B64         :   //  = D = S0.u << S1.u[4:0].
        case V_LSHR_B64         :   //  = D = S0.u >> S1.u[4:0].
        case V_ASHR_I64         :   //  = D = S0.u >> S1.u[4:0].
        case V_ADD_F64          :   //  = D.d = S0.d + S1.d.
        case V_MUL_F64          :   //  = D.d = S0.d * S1.d.
        case V_MIN_F64          :   //  = D.d = min(S0.d, S1.d).
        case V_MAX_F64          :   //  = D.d = max(S0.d, S1.d).
        case V_LDEXP_F64        :   //  = D.d = pow(S0.d, S1.i[31:0]).
        case V_TRIG_PREOP_F64  : // = D.d = Look Up 2/PI (S0.d) with segment select  S1.u[4:0].
        case V_FMA_F64          :   //   = D.d = S0.d * S1.d + S2.d.
        case V_DIV_FIXUP_F64    :   //  = D.d = Special case divide fixup and flags(s0.d = Quotient,  s1.d = Denominator, s2.d = Numerator).
        case V_DIV_FMAS_F64     : // = D.d = Special case divide FMA with scale and flags(s0.d= Quotient, s1.d = Denominator, s2.d = Numerator).
            return 2;

        case V_DIV_SCALE_F32    :    //  = D.f = Special case divide preop and flags(s0.f = Quotient,s1.f = Denominator, s2.f = Numerator) s0 must equal s1 or s2.
        case V_LERP_U8          :   //  = D.u = ((S0.u[31:24] + S1.u[31:24] + S2.u[24]) >> 1) << 24 +
        case V_ALIGNBIT_B32     :   //  = D.u = ({S0,S1} >> S2.u[4:0]) & 0xFFFFFFFF.
        case V_ALIGNBYTE_B32    :   //  = D.u = ({S0,S1} >> (8*S2.u[4:0])) & 0xFFFFFFFF.
        case V_MIN3_F32         :   //  = D.f = min(S0.f, S1.f, S2.f).
        case V_MIN3_I32         :   //  = D.i = min(S0.i, S1.i, S2.i).
        case V_MIN3_U32         :   //  = 0x153 D.u = min(S0.u, S1.u, S2.u).
        case V_MAX3_F32         :   //  = D.f = max(S0.f, S1.f, S2.f).
        case V_MAX3_I32         :   //  = D.i = max(S0.i, S1.i, S2.i).
        case V_MAX3_U32         :   //  = D.u = max(S0.u, S1.u, S2.u).
        case V_MED3_F32         :   //  = D.f = median(S0.f, S1.f, S2.f).
        case V_MED3_I32         :   //  = D.i = median(S0.i, S1.i, S2.i).
        case V_MED3_U32         :   //  = D.u = median(S0.u, S1.u, S2.u).
        case V_SAD_U8           :   //  = D.u = Byte SAD with accum_lo(S0.u, S1.u, S2.u).
        case V_SAD_HI_U8        :   //  = D.u = Byte SAD with accum_hi(S0.u, S1.u, S2.u).
        case V_SAD_U16          :   //  = D.u = Word SAD with accum(S0.u, S1.u, S2.u).
        case V_SAD_U32          :   //  = D.u = Dword SAD with accum(S0.u, S1.u, S2.u).
        case V_CVT_PK_U8_F32    :   //  = f32->u8(s0.f), pack into byte(s1.u), of dword(s2).
        case V_DIV_FIXUP_F32    :   //  = D.f = Special case divide fixup and flags(s0.f = Quotient,   s1.f = Denominator, s2.f = Numerator).
        case V_DIV_FMAS_F32     : // = D.f = Special case divide FMA with scale and flags(s0.f = Quotient, s1.f = Denominator, s2.f = Numerator).
        case V_MSAD_U8          : // = D.u = Masked Byte SAD with accum_lo(S0.u, S1.u, S2.u).
        case V_QSAD_U8          : // = D.u = Quad-Byte SAD with accum_lo/hiu(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        case V_MQSAD_U8         : // = D.u = Masked Quad-Byte SAD with accum_lo/hi(S0.u[63:0],S1.u[31:0], S2.u[63:0]).       
        case V_QSAD_PK_U16_U8   : // : D.u = Quad-Byte SAD with accum_lo/hiu(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        case V_MQSAD_PK_U16_U8  : // = D.u = Masked Quad-Byte SAD with accum_lo/hi(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        case V_MQSAD_U32_U8     :  // : D.u128 = Masked Quad-Byte SAD with 32-bit accum_lo/hi(S0.u[63:0], S1.u[31:0], S2.u[127:0]).
        case V_MAD_U64_U32      :  // : {vcc_out,D.u64} = S0.u32 * S1.u32 + S2.u64.
        case V_MAD_I64_I32      :  // : {vcc_out,D.i64} = S0.i32 * S1.i32 + S2.i64.
        case V_INTERP_P1_F32  :
        case V_INTERP_P2_F32  :
        case V_INTERP_MOV_F32 :
        case V_INTERP_P1LL_F16:
        case V_INTERP_P1LV_F16:
        case V_INTERP_P2_F16  :
            return 1;
        }
    }

    uint GetSrc0WidthInDWORDs( VectorInstructions e )
    {
        switch( e )
        {
        default: return 0;
        case V_ADD_U32:
        case V_SUB_U32:
        case V_SUBREV_U32:
        case V_LSHLREV_B64 :
        case V_LSHRREV_B64 :
        case V_ASHRREV_I64 :
            return 1;


        case V_MAD_F16             :
        case V_MAD_U16             :
        case V_MAD_I16             :
        case V_PERM_B32            :
        case V_FMA_F16             :
        case V_DIV_FIXUP_F16       :
        case V_ADD_F16         :    
        case V_SUB_F16         :    
        case V_SUBREV_F16      :    
        case V_MUL_F16         :    
        case V_MAC_F16         :    
        case V_MADMK_F16       :    
        case V_MADAK_F16       :    
        case V_ADD_U16         :    
        case V_SUB_U16         :    
        case V_SUBREV_U16      :    
        case V_MUL_LO_U16      :    
        case V_LSHLREV_B16     :    
        case V_LSHRREV_B16     :    
        case V_ASHRREV_I16     :    
        case V_MAX_F16         :    
        case V_MIN_F16         :    
        case V_MAX_U16         :    
        case V_MAX_I16         :    
        case V_MIN_U16         :    
        case V_MIN_I16         :    
        case V_LDEXP_F16       :    
        case V_CVT_F16_U16         :    
        case V_CVT_F16_I16         :  
        case V_CVT_U16_F16         :  
        case V_CVT_I16_F16         :  
        case V_RCP_F16             :  
        case V_SQRT_F16            :  
        case V_RSQ_F16             :  
        case V_LOG_F16             :  
        case V_EXP_F16             :  
        case V_FREXP_MANT_F16      :  
        case V_FREXP_EXP_I16_F16   :  
        case V_FLOOR_F16           :  
        case V_CEIL_F16            :  
        case V_TRUNC_F16           :  
        case V_RNDNE_F16           :  
        case V_FRACT_F16           :  
        case V_SIN_F16             :  
        case V_COS_F16             :  
        case V_CNDMASK_B32              :
        case V_READLANE_B32             :                          
        case V_WRITELANE_B32            :
        case V_ADD_F32                  :
        case V_SUB_F32                  :
        case V_SUBREV_F32               :
        case V_MAC_LEGACY_F32           :
        case V_MUL_LEGACY_F32           :
        case V_MUL_F32                  :
        case V_MUL_I32_I24              :
        case V_MUL_HI_I32_I24           :
        case V_MUL_U32_U24              :
        case V_MUL_HI_U32_U24           :
        case V_MIN_LEGACY_F32           :
        case V_MAX_LEGACY_F32           :
        case V_MIN_F32                  :
        case V_MAX_F32                  :
        case V_MIN_I32                  :
        case V_MAX_I32                  :
        case V_MIN_U32                  :
        case V_MAX_U32                  :
        case V_LSHR_B32                 :
        case V_LSHRREV_B32              :
        case V_ASHR_I32                 :
        case V_ASHRREV_I32              :
        case V_LSHL_B32                 :
        case V_LSHLREV_B32              :
        case V_AND_B32                  :
        case V_OR_B32                   :
        case V_XOR_B32                  :
        case V_BFM_B32                  :
        case V_MAC_F32                  :
        case V_MADMK_F32                :
        case V_MADAK_F32                :
        case V_BCNT_U32_B32             :
        case V_MBCNT_LO_U32_B32         :                                                          
        case V_MBCNT_HI_U32_B32         :                    
        case V_ADD_I32                  :
        case V_SUB_I32                  :
        case V_SUBREV_I32               :
        case V_ADDC_U32                 :
        case V_SUBB_U32                 :
        case V_SUBBREV_U32              :
        case V_LDEXP_F32                :
        case V_CVT_PKACCUM_U8_F32       :
        case V_CVT_PKNORM_I16_F32       :
        case V_CVT_PKNORM_U16_F32       :
        case V_CVT_PKRTZ_F16_F32        :
        case V_CVT_PK_U16_U32           :
        case V_CVT_PK_I16_I32           :
            return 1;

        // VOPC
        case V_CMP_CLASS_F16:
        case V_CMPX_CLASS_F16:
        case V_CMP_F_F16           :
        case V_CMP_LT_F16          :
        case V_CMP_EQ_F16          :
        case V_CMP_LE_F16          :
        case V_CMP_GT_F16          :
        case V_CMP_LG_F16          :
        case V_CMP_GE_F16          :
        case V_CMP_O_F16           :
        case V_CMP_U_F16           :
        case V_CMP_NGE_F16         :
        case V_CMP_NLG_F16         :
        case V_CMP_NGT_F16         :
        case V_CMP_NLE_F16         :
        case V_CMP_NEQ_F16         :
        case V_CMP_NLT_F16         :
        case V_CMP_TRU_F16         :
        case V_CMPX_F_F16          :
        case V_CMPX_LT_F16         :
        case V_CMPX_EQ_F16         :
        case V_CMPX_LE_F16         :
        case V_CMPX_GT_F16         :
        case V_CMPX_LG_F16         :
        case V_CMPX_GE_F16         :
        case V_CMPX_O_F16          :
        case V_CMPX_U_F16          :
        case V_CMPX_NGE_F16        :
        case V_CMPX_NLG_F16        :
        case V_CMPX_NGT_F16        :
        case V_CMPX_NLE_F16        :
        case V_CMPX_NEQ_F16        :
        case V_CMPX_NLT_F16        :
        case V_CMPX_TRU_F16        :
        case V_CMP_F_U16           :
        case V_CMP_LT_U16          :
        case V_CMP_EQ_U16          :
        case V_CMP_LE_U16          :
        case V_CMP_GT_U16          :
        case V_CMP_LG_U16          :
        case V_CMP_GE_U16          :
        case V_CMP_TRU_U16         :
        case V_CMPx_F_U16          :
        case V_CMPx_LT_U16         :
        case V_CMPx_EQ_U16         :
        case V_CMPx_LE_U16         :
        case V_CMPx_GT_U16         :
        case V_CMPx_LG_U16         :
        case V_CMPx_GE_U16         :
        case V_CMPx_TRU_U16        :
        case V_CMP_F_I16           :
        case V_CMP_LT_I16          :
        case V_CMP_EQ_I16          :
        case V_CMP_LE_I16          :
        case V_CMP_GT_I16          :
        case V_CMP_LG_I16          :
        case V_CMP_GE_I16          :
        case V_CMP_TRU_I16         :
        case V_CMPx_F_I16          :
        case V_CMPx_LT_I16         :
        case V_CMPx_EQ_I16         :
        case V_CMPx_LE_I16         :
        case V_CMPx_GT_I16         :
        case V_CMPx_LG_I16         :
        case V_CMPx_GE_I16         :
        case V_CMPx_TRU_I16        :
        case V_CMP_F_F32            :
        case V_CMP_LT_F32           :
        case V_CMP_EQ_F32           :
        case V_CMP_LE_F32           :
        case V_CMP_GT_F32           :
        case V_CMP_LG_F32           :
        case V_CMP_GE_F32           :
        case V_CMP_O_F32            :
        case V_CMP_U_F32            :
        case V_CMP_NGE_F32          :
        case V_CMP_NLG_F32          :
        case V_CMP_NGT_F32          :
        case V_CMP_NLE_F32          :
        case V_CMP_NEQ_F32          :
        case V_CMP_NLT_F32          :
        case V_CMP_TRU_F32          :
        case V_CMPX_F_F32           :
        case V_CMPX_LT_F32          :
        case V_CMPX_EQ_F32          :
        case V_CMPX_LE_F32          :
        case V_CMPX_GT_F32          :
        case V_CMPX_LG_F32          :
        case V_CMPX_GE_F32          :
        case V_CMPX_O_F32           :
        case V_CMPX_U_F32           :
        case V_CMPX_NGE_F32         :
        case V_CMPX_NLG_F32         :
        case V_CMPX_NGT_F32         :
        case V_CMPX_NLE_F32         :
        case V_CMPX_NEQ_F32         :
        case V_CMPX_NLT_F32         :
        case V_CMPX_TRU_F32         :
            return 1;
        case V_CMP_F_F64            :
        case V_CMP_LT_F64           :
        case V_CMP_EQ_F64           :
        case V_CMP_LE_F64           :
        case V_CMP_GT_F64           :
        case V_CMP_LG_F64           :
        case V_CMP_GE_F64           :
        case V_CMP_O_F64            :
        case V_CMP_U_F64            :
        case V_CMP_NGE_F64          :
        case V_CMP_NLG_F64          :
        case V_CMP_NGT_F64          :
        case V_CMP_NLE_F64          :
        case V_CMP_NEQ_F64          :
        case V_CMP_NLT_F64          :
        case V_CMP_TRU_F64          :
        case V_CMPX_F_F64           :
        case V_CMPX_LT_F64          :
        case V_CMPX_EQ_F64          :
        case V_CMPX_LE_F64          :
        case V_CMPX_GT_F64          :
        case V_CMPX_LG_F64          :
        case V_CMPX_GE_F64          :
        case V_CMPX_O_F64           :
        case V_CMPX_U_F64           :
        case V_CMPX_NGE_F64         :
        case V_CMPX_NLG_F64         :
        case V_CMPX_NGT_F64         :
        case V_CMPX_NLE_F64         :
        case V_CMPX_NEQ_F64         :
        case V_CMPX_NLT_F64         :
        case V_CMPX_TRU_F64         :
            return 2;
        case V_CMPS_F_F32           :
        case V_CMPS_LT_F32          :
        case V_CMPS_EQ_F32          :
        case V_CMPS_LE_F32          :
        case V_CMPS_GT_F32          :
        case V_CMPS_LG_F32          :
        case V_CMPS_GE_F32          :
        case V_CMPS_O_F32           :
        case V_CMPS_U_F32           :
        case V_CMPS_NGE_F32         :
        case V_CMPS_NLG_F32         :
        case V_CMPS_NGT_F32         :
        case V_CMPS_NLE_F32         :
        case V_CMPS_NEQ_F32         :
        case V_CMPS_NLT_F32         :
        case V_CMPS_TRU_F32         :
        case V_CMPSX_F_F32          :
        case V_CMPSX_LT_F32         :
        case V_CMPSX_EQ_F32         :
        case V_CMPSX_LE_F32         :
        case V_CMPSX_GT_F32         :
        case V_CMPSX_LG_F32         :
        case V_CMPSX_GE_F32         :
        case V_CMPSX_O_F32          :
        case V_CMPSX_U_F32          :
        case V_CMPSX_NGE_F32        :
        case V_CMPSX_NLG_F32        :
        case V_CMPSX_NGT_F32        :
        case V_CMPSX_NLE_F32        :
        case V_CMPSX_NEQ_F32        :
        case V_CMPSX_NLT_F32        :
        case V_CMPSX_TRU_F32        :
            return 1;
        case V_CMPS_F_F64           :
        case V_CMPS_LT_F64          :
        case V_CMPS_EQ_F64          :
        case V_CMPS_LE_F64          :
        case V_CMPS_GT_F64          :
        case V_CMPS_LG_F64          :
        case V_CMPS_GE_F64          :
        case V_CMPS_O_F64           :
        case V_CMPS_U_F64           :
        case V_CMPS_NGE_F64         :
        case V_CMPS_NLG_F64         :
        case V_CMPS_NGT_F64         :
        case V_CMPS_NLE_F64         :
        case V_CMPS_NEQ_F64         :
        case V_CMPS_NLT_F64         :
        case V_CMPS_TRU_F64         :
        case V_CMPSX_F_F64          :
        case V_CMPSX_LT_F64         :
        case V_CMPSX_EQ_F64         :
        case V_CMPSX_LE_F64         :
        case V_CMPSX_GT_F64         :
        case V_CMPSX_LG_F64         :
        case V_CMPSX_GE_F64         :
        case V_CMPSX_O_F64          :
        case V_CMPSX_U_F64          :
        case V_CMPSX_NGE_F64        :
        case V_CMPSX_NLG_F64        :
        case V_CMPSX_NGT_F64        :
        case V_CMPSX_NLE_F64        :
        case V_CMPSX_NEQ_F64        :
        case V_CMPSX_NLT_F64        :
        case V_CMPSX_TRU_F64        :
            return 2;
        case V_CMP_F_I32            :
        case V_CMP_LT_I32           :
        case V_CMP_EQ_I32           :
        case V_CMP_LE_I32           :
        case V_CMP_GT_I32           :
        case V_CMP_LG_I32           :
        case V_CMP_GE_I32           :
        case V_CMP_TRU_I32          :
        case V_CMPX_F_I32           :
        case V_CMPX_LT_I32          :
        case V_CMPX_EQ_I32          :
        case V_CMPX_LE_I32          :
        case V_CMPX_GT_I32          :
        case V_CMPX_LG_I32          :
        case V_CMPX_GE_I32          :
        case V_CMPX_TRU_I32         :
            return 1;
        case V_CMP_F_I64            :
        case V_CMP_LT_I64           :
        case V_CMP_EQ_I64           :
        case V_CMP_LE_I64           :
        case V_CMP_GT_I64           :
        case V_CMP_LG_I64           :
        case V_CMP_GE_I64           :
        case V_CMP_TRU_I64          :
        case V_CMPX_F_I64           :
        case V_CMPX_LT_I64          :
        case V_CMPX_EQ_I64          :
        case V_CMPX_LE_I64          :
        case V_CMPX_GT_I64          :
        case V_CMPX_LG_I64          :
        case V_CMPX_GE_I64          :
        case V_CMPX_TRU_I64         :
            return 2;
        case V_CMP_F_U32            :
        case V_CMP_LT_U32           :
        case V_CMP_EQ_U32           :
        case V_CMP_LE_U32           :
        case V_CMP_GT_U32           :
        case V_CMP_LG_U32           :
        case V_CMP_GE_U32           :
        case V_CMP_TRU_U32          :
        case V_CMPX_F_U32           :
        case V_CMPX_LT_U32          :
        case V_CMPX_EQ_U32          :
        case V_CMPX_LE_U32          :
        case V_CMPX_GT_U32          :
        case V_CMPX_LG_U32          :
        case V_CMPX_GE_U32          :
        case V_CMPX_TRU_U32         :
            return 1;
        case V_CMP_F_U64            :
        case V_CMP_LT_U64           :
        case V_CMP_EQ_U64           :
        case V_CMP_LE_U64           :
        case V_CMP_GT_U64           :
        case V_CMP_LG_U64           :
        case V_CMP_GE_U64           :
        case V_CMP_TRU_U64          :
        case V_CMPx_F_U64           :
        case V_CMPx_LT_U64          :
        case V_CMPx_EQ_U64          :
        case V_CMPx_LE_U64          :
        case V_CMPx_GT_U64          :
        case V_CMPx_LG_U64          :
        case V_CMPx_GE_U64          :
        case V_CMPx_TRU_U64         :
        case V_CMP_CLASS_F64        :
        case V_CMPX_CLASS_F64       :
            return 2;
        case V_CMP_CLASS_F32        :
        case V_CMPX_CLASS_F32       :
            return 1;

        // VOP1
        case V_CVT_F64_I32      :       
        case V_CVT_F64_F32      :
        case V_CVT_F64_U32      : 
        case V_MOV_B32           :      
        case V_READFIRSTLANE_B32 :      
        case V_CVT_F32_I32       :      
        case V_CVT_F32_U32       :      
        case V_CVT_U32_F32       :      
        case V_CVT_I32_F32       :      
        case V_MOV_FED_B32       :      
        case V_CVT_F16_F32       :
        case V_CVT_F32_F16       :      
        case V_CVT_RPI_I32_F32   :      
        case V_CVT_FLR_I32_F32   :      
        case V_CVT_OFF_F32_I4    :
        case V_CVT_F32_UBYTE0    :      
        case V_CVT_F32_UBYTE1    :      
        case V_CVT_F32_UBYTE2    :      
        case V_CVT_F32_UBYTE3    :      
        case V_FRACT_F32         :      
        case V_TRUNC_F32         :      
        case V_CEIL_F32          :      
        case V_RNDNE_F32         :      
        case V_FLOOR_F32         :      
        case V_EXP_F32           :      
        case V_LOG_CLAMP_F32     :      
        case V_LOG_F32           :      
        case V_RCP_CLAMP_F32     :      
        case V_RCP_LEGACY_F32    :      
        case V_RCP_F32           :      
        case V_RCP_IFLAG_F32     :      
        case V_RSQ_CLAMP_F32     :      
        case V_RSQ_LEGACY_F32    :      
        case V_RSQ_F32           :      
        case V_SQRT_F32          :      
        case V_SIN_F32           :      
        case V_COS_F32           :      
        case V_NOT_B32           :      
        case V_BFREV_B32         :      
        case V_FFBH_U32          :      
        case V_FFBL_B32          :      
        case V_FFBH_I32          :      
        case V_FREXP_EXP_I32_F32 :                          
        case V_FREXP_MANT_F32    :      
        case V_MOVRELD_B32       :      
        case V_MOVRELS_B32       :      
        case V_MOVRELSD_B32      : 
        case V_LOG_LEGACY_F32    :      
        case V_EXP_LEGACY_F32    :
            return 1;
        case V_TRUNC_F64        :       
        case V_CEIL_F64         :       
        case V_RNDNE_F64        :       
        case V_FLOOR_F64        :    
        case V_RCP_F64          :       
        case V_RCP_CLAMP_F64    :       
        case V_RSQ_F64          :       
        case V_RSQ_CLAMP_F64    :       
        case V_SQRT_F64         :       
        case V_FREXP_MANT_F64   :       
        case V_FRACT_F64        :    
        case V_CVT_I32_F64       :      
        case V_CVT_F32_F64       :      
        case V_CVT_U32_F64       :      
        case V_FREXP_EXP_I32_F64 :      
            return 2;

        case V_DIV_SCALE_F32    :    //  = D.f = Special case divide preop and flags(s0.f = Quotient,s1.f = Denominator, s2.f = Numerator) s0 must equal s1 or s2.
        case V_MAD_LEGACY_F32   :   //   = D.f = S0.f * S1.f + S2.f (DX9 rules, 0.0*x = 0.0).
        case V_MAD_F32          :   //   = D.f = S0.f * S1.f + S2.f.
        case V_MAD_I32_I24      :   //   = D.i = S0.i * S1.i + S2.iD.i = S0.i * S1.i + S2.i.
        case V_MAD_U32_U24      :   //   = D.u = S0.u * S1.u + S2.u.
        case V_CUBEID_F32       :   //   = Rm.w <- Rn,x, Rn,y, Rn.z.
        case V_CUBESC_F32       :   //   = Rm.y <- Rn,x, Rn,y, Rn.z.
        case V_CUBETC_F32       :   //   = Rm.x <- Rn,x, Rn,y, Rn.z.
        case V_CUBEMA_F32       :   //   = Rm.z <- Rn,x, Rn,y, Rn.z
        case V_BFE_U32          :   //   = D.u = (S0.u>>S1.u[4:0]) & ((1<<S2.u[4:0])-1); bitfield extract,S0=data, S1=field_offset, S2=field_width.
        case V_BFE_I32          :   //   = D.i = (S0.i>>S1.u[4:0]) & ((1<<S2.u[4:0])-1); bitfield extract, S0=data, S1=field_offset, S2=field_width.
        case V_BFI_B32          :   //   = D.u = (S0.u & S1.u) | (~S0.u & S2.u); bitfield insert.
        case V_FMA_F32          :   //   = D.f = S0.f * S1.f + S2.f
        case V_LERP_U8          :   //  = D.u = ((S0.u[31:24] + S1.u[31:24] + S2.u[24]) >> 1) << 24 +
        case V_ALIGNBIT_B32     :   //  = D.u = ({S0,S1} >> S2.u[4:0]) & 0xFFFFFFFF.
        case V_ALIGNBYTE_B32    :   //  = D.u = ({S0,S1} >> (8*S2.u[4:0])) & 0xFFFFFFFF.
        case V_MULLIT_F32       :   //  = D.f = S0.f * S1.f, replicate result into 4 components (0.0 * x = 0.0; special INF, NaN, overflow rules).
        case V_MIN3_F32         :   //  = D.f = min(S0.f, S1.f, S2.f).
        case V_MIN3_I32         :   //  = D.i = min(S0.i, S1.i, S2.i).
        case V_MIN3_U32         :   //  = 0x153 D.u = min(S0.u, S1.u, S2.u).
        case V_MAX3_F32         :   //  = D.f = max(S0.f, S1.f, S2.f).
        case V_MAX3_I32         :   //  = D.i = max(S0.i, S1.i, S2.i).
        case V_MAX3_U32         :   //  = D.u = max(S0.u, S1.u, S2.u).
        case V_MED3_F32         :   //  = D.f = median(S0.f, S1.f, S2.f).
        case V_MED3_I32         :   //  = D.i = median(S0.i, S1.i, S2.i).
        case V_MED3_U32         :   //  = D.u = median(S0.u, S1.u, S2.u).
        case V_SAD_U8           :   //  = D.u = Byte SAD with accum_lo(S0.u, S1.u, S2.u).
        case V_SAD_HI_U8        :   //  = D.u = Byte SAD with accum_hi(S0.u, S1.u, S2.u).
        case V_SAD_U16          :   //  = D.u = Word SAD with accum(S0.u, S1.u, S2.u).
        case V_SAD_U32          :   //  = D.u = Dword SAD with accum(S0.u, S1.u, S2.u).
        case V_CVT_PK_U8_F32    :   //  = f32->u8(s0.f), pack into byte(s1.u), of dword(s2).
        case V_DIV_FIXUP_F32    :   //  = D.f = Special case divide fixup and flags(s0.f = Quotient,   s1.f = Denominator, s2.f = Numerator).
            return 1;
        case V_DIV_SCALE_F64    :    //  = D.d = Special case divide preop and flags(s0.d = Quotient,s1.d = Denominator, s2.d = Numerator) s0 must equal s1 or s2.
        case V_DIV_FIXUP_F64    :   //  = D.d = Special case divide fixup and flags(s0.d = Quotient,  s1.d = Denominator, s2.d = Numerator).
        case V_FMA_F64          :   //   = D.d = S0.d * S1.d + S2.d.
        case V_LSHL_B64         :   //  = D = S0.u << S1.u[4:0].
        case V_LSHR_B64         :   //  = D = S0.u >> S1.u[4:0].
        case V_ASHR_I64         :   //  = D = S0.u >> S1.u[4:0].
        case V_ADD_F64          :   //  = D.d = S0.d + S1.d.
        case V_MUL_F64          :   //  = D.d = S0.d * S1.d.
        case V_MIN_F64          :   //  = D.d = min(S0.d, S1.d).
        case V_MAX_F64          :   //  = D.d = max(S0.d, S1.d).
        case V_LDEXP_F64        :   //  = D.d = pow(S0.d, S1.i[31:0]).
        case V_DIV_FMAS_F64    : // = D.d = Special case divide FMA with scale and flags(s0.d= Quotient, s1.d = Denominator, s2.d = Numerator).
        case V_TRIG_PREOP_F64  : // = D.d = Look Up 2/PI (S0.d) with segment select  S1.u[4:0].
            return 2;
        case V_MUL_LO_U32       :   //  = D.u = S0.u * S1.u.
        case V_MUL_HI_U32       :   //  = D.u = (S0.u * S1.u)>>32.
        case V_MUL_LO_I32       :   //  = D.i = S0.i * S1.i.
        case V_MUL_HI_I32       :   //  = D.i = (S0.i * S1.i)>>32.
        case V_DIV_FMAS_F32    : // = D.f = Special case divide FMA with scale and flags(s0.f = Quotient, s1.f = Denominator, s2.f = Numerator).
        case V_MSAD_U8         : // = D.u = Masked Byte SAD with accum_lo(S0.u, S1.u, S2.u).
        case V_QSAD_U8         : // = D.u = Quad-Byte SAD with accum_lo/hiu(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        case V_MQSAD_U8        : // = D.u = Masked Quad-Byte SAD with accum_lo/hi(S0.u[63:0],S1.u[31:0], S2.u[63:0]).       
        case V_QSAD_PK_U16_U8  : // : D.u = Quad-Byte SAD with accum_lo/hiu(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        case V_MQSAD_PK_U16_U8 : // = D.u = Masked Quad-Byte SAD with accum_lo/hi(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        case V_MQSAD_U32_U8    :  // : D.u128 = Masked Quad-Byte SAD with 32-bit accum_lo/hi(S0.u[63:0], S1.u[31:0], S2.u[127:0]).
        case V_MAD_U64_U32     :  // : {vcc_out,D.u64} = S0.u32 * S1.u32 + S2.u64.
        case V_MAD_I64_I32     :  // : {vcc_out,D.i64} = S0.i32 * S1.i32 + S2.i64.
            return 1;

        case V_INTERP_P1_F32  :
        case V_INTERP_P2_F32  :
        case V_INTERP_MOV_F32 :
        case V_INTERP_P1LL_F16:
        case V_INTERP_P1LV_F16:
        case V_INTERP_P2_F16  :
            return 1;
        }
    }


    uint GetSrc1WidthInDWORDs(VectorInstructions e) 
    {
        switch(e)
        {
        default: return 0;
            // VOP2
        case V_ADD_U32:
        case V_SUB_U32:
        case V_SUBREV_U32:
            return 1;
        case V_LSHLREV_B64 :
        case V_LSHRREV_B64 :
        case V_ASHRREV_I64 :
            return 2;
        case V_MAD_F16            :
        case V_MAD_U16            :
        case V_MAD_I16            :
        case V_PERM_B32           :
        case V_FMA_F16            :
        case V_DIV_FIXUP_F16      :
        case V_ADD_F16            :  
        case V_SUB_F16            :  
        case V_SUBREV_F16         :  
        case V_MUL_F16            :  
        case V_MAC_F16            :  
        case V_MADMK_F16          :  
        case V_MADAK_F16          :  
        case V_ADD_U16            :  
        case V_SUB_U16            :  
        case V_SUBREV_U16         :  
        case V_MUL_LO_U16         :  
        case V_LSHLREV_B16        :  
        case V_LSHRREV_B16        :  
        case V_ASHRREV_I16        :  
        case V_MAX_F16            :  
        case V_MIN_F16            :  
        case V_MAX_U16            :  
        case V_MAX_I16            :  
        case V_MIN_U16            :  
        case V_MIN_I16            :  
        case V_LDEXP_F16          :  
        case V_CNDMASK_B32        :      
        case V_READLANE_B32             :                          
        case V_WRITELANE_B32            :
        case V_ADD_F32                  :
        case V_SUB_F32                  :
        case V_SUBREV_F32               :
        case V_MAC_LEGACY_F32           :
        case V_MUL_LEGACY_F32           :
        case V_MUL_F32                  :
        case V_MUL_I32_I24              :
        case V_MUL_HI_I32_I24           :
        case V_MUL_U32_U24              :
        case V_MUL_HI_U32_U24           :
        case V_MIN_LEGACY_F32           :
        case V_MAX_LEGACY_F32           :
        case V_MIN_F32                  :
        case V_MAX_F32                  :
        case V_MIN_I32                  :
        case V_MAX_I32                  :
        case V_MIN_U32                  :
        case V_MAX_U32                  :
        case V_LSHR_B32                 :
        case V_LSHRREV_B32              :
        case V_ASHR_I32                 :
        case V_ASHRREV_I32              :
        case V_LSHL_B32                 :
        case V_LSHLREV_B32              :
        case V_AND_B32                  :
        case V_OR_B32                   :
        case V_XOR_B32                  :
        case V_BFM_B32                  :
        case V_MAC_F32                  :
        case V_MADMK_F32                :
        case V_MADAK_F32                :
        case V_BCNT_U32_B32             :
        case V_MBCNT_LO_U32_B32         :                                                          
        case V_MBCNT_HI_U32_B32         :                    
        case V_ADD_I32                  :
        case V_SUB_I32                  :
        case V_SUBREV_I32               :
        case V_ADDC_U32                 :
        case V_SUBB_U32                 :
        case V_SUBBREV_U32              :
        case V_LDEXP_F32                :
        case V_CVT_PKACCUM_U8_F32       :
        case V_CVT_PKNORM_I16_F32       :
        case V_CVT_PKNORM_U16_F32       :
        case V_CVT_PKRTZ_F16_F32        :
        case V_CVT_PK_U16_U32           :
        case V_CVT_PK_I16_I32           :
            return 1;

        // VOPC
        case V_CMP_CLASS_F16:
        case V_CMPX_CLASS_F16:        
        case V_CMP_F_F16          :
        case V_CMP_LT_F16         :
        case V_CMP_EQ_F16         :
        case V_CMP_LE_F16         :
        case V_CMP_GT_F16         :
        case V_CMP_LG_F16         :
        case V_CMP_GE_F16         :
        case V_CMP_O_F16          :
        case V_CMP_U_F16          :
        case V_CMP_NGE_F16        :
        case V_CMP_NLG_F16        :
        case V_CMP_NGT_F16        :
        case V_CMP_NLE_F16        :
        case V_CMP_NEQ_F16        :
        case V_CMP_NLT_F16        :
        case V_CMP_TRU_F16        :
        case V_CMPX_F_F16         :
        case V_CMPX_LT_F16        :
        case V_CMPX_EQ_F16        :
        case V_CMPX_LE_F16        :
        case V_CMPX_GT_F16        :
        case V_CMPX_LG_F16        :
        case V_CMPX_GE_F16        :
        case V_CMPX_O_F16         :
        case V_CMPX_U_F16         :
        case V_CMPX_NGE_F16       :
        case V_CMPX_NLG_F16       :
        case V_CMPX_NGT_F16       :
        case V_CMPX_NLE_F16       :
        case V_CMPX_NEQ_F16       :
        case V_CMPX_NLT_F16       :
        case V_CMPX_TRU_F16       :
        case V_CMP_F_U16          :
        case V_CMP_LT_U16         :
        case V_CMP_EQ_U16         :
        case V_CMP_LE_U16         :
        case V_CMP_GT_U16         :
        case V_CMP_LG_U16         :
        case V_CMP_GE_U16         :
        case V_CMP_TRU_U16        :
        case V_CMPx_F_U16         :
        case V_CMPx_LT_U16        :
        case V_CMPx_EQ_U16        :
        case V_CMPx_LE_U16        :
        case V_CMPx_GT_U16        :
        case V_CMPx_LG_U16        :
        case V_CMPx_GE_U16        :
        case V_CMPx_TRU_U16       :
        case V_CMP_F_I16          :
        case V_CMP_LT_I16         :
        case V_CMP_EQ_I16         :
        case V_CMP_LE_I16         :
        case V_CMP_GT_I16         :
        case V_CMP_LG_I16         :
        case V_CMP_GE_I16         :
        case V_CMP_TRU_I16        :
        case V_CMPx_F_I16         :
        case V_CMPx_LT_I16        :
        case V_CMPx_EQ_I16        :
        case V_CMPx_LE_I16        :
        case V_CMPx_GT_I16        :
        case V_CMPx_LG_I16        :
        case V_CMPx_GE_I16        :
        case V_CMPx_TRU_I16       :
        case V_CMP_F_F32            :
        case V_CMP_LT_F32           :
        case V_CMP_EQ_F32           :
        case V_CMP_LE_F32           :
        case V_CMP_GT_F32           :
        case V_CMP_LG_F32           :
        case V_CMP_GE_F32           :
        case V_CMP_O_F32            :
        case V_CMP_U_F32            :
        case V_CMP_NGE_F32          :
        case V_CMP_NLG_F32          :
        case V_CMP_NGT_F32          :
        case V_CMP_NLE_F32          :
        case V_CMP_NEQ_F32          :
        case V_CMP_NLT_F32          :
        case V_CMP_TRU_F32          :
        case V_CMPX_F_F32           :
        case V_CMPX_LT_F32          :
        case V_CMPX_EQ_F32          :
        case V_CMPX_LE_F32          :
        case V_CMPX_GT_F32          :
        case V_CMPX_LG_F32          :
        case V_CMPX_GE_F32          :
        case V_CMPX_O_F32           :
        case V_CMPX_U_F32           :
        case V_CMPX_NGE_F32         :
        case V_CMPX_NLG_F32         :
        case V_CMPX_NGT_F32         :
        case V_CMPX_NLE_F32         :
        case V_CMPX_NEQ_F32         :
        case V_CMPX_NLT_F32         :
        case V_CMPX_TRU_F32         :
            return 1;
        case V_CMP_F_F64            :
        case V_CMP_LT_F64           :
        case V_CMP_EQ_F64           :
        case V_CMP_LE_F64           :
        case V_CMP_GT_F64           :
        case V_CMP_LG_F64           :
        case V_CMP_GE_F64           :
        case V_CMP_O_F64            :
        case V_CMP_U_F64            :
        case V_CMP_NGE_F64          :
        case V_CMP_NLG_F64          :
        case V_CMP_NGT_F64          :
        case V_CMP_NLE_F64          :
        case V_CMP_NEQ_F64          :
        case V_CMP_NLT_F64          :
        case V_CMP_TRU_F64          :
        case V_CMPX_F_F64           :
        case V_CMPX_LT_F64          :
        case V_CMPX_EQ_F64          :
        case V_CMPX_LE_F64          :
        case V_CMPX_GT_F64          :
        case V_CMPX_LG_F64          :
        case V_CMPX_GE_F64          :
        case V_CMPX_O_F64           :
        case V_CMPX_U_F64           :
        case V_CMPX_NGE_F64         :
        case V_CMPX_NLG_F64         :
        case V_CMPX_NGT_F64         :
        case V_CMPX_NLE_F64         :
        case V_CMPX_NEQ_F64         :
        case V_CMPX_NLT_F64         :
        case V_CMPX_TRU_F64         :
            return 2;
        case V_CMPS_F_F32           :
        case V_CMPS_LT_F32          :
        case V_CMPS_EQ_F32          :
        case V_CMPS_LE_F32          :
        case V_CMPS_GT_F32          :
        case V_CMPS_LG_F32          :
        case V_CMPS_GE_F32          :
        case V_CMPS_O_F32           :
        case V_CMPS_U_F32           :
        case V_CMPS_NGE_F32         :
        case V_CMPS_NLG_F32         :
        case V_CMPS_NGT_F32         :
        case V_CMPS_NLE_F32         :
        case V_CMPS_NEQ_F32         :
        case V_CMPS_NLT_F32         :
        case V_CMPS_TRU_F32         :
        case V_CMPSX_F_F32          :
        case V_CMPSX_LT_F32         :
        case V_CMPSX_EQ_F32         :
        case V_CMPSX_LE_F32         :
        case V_CMPSX_GT_F32         :
        case V_CMPSX_LG_F32         :
        case V_CMPSX_GE_F32         :
        case V_CMPSX_O_F32          :
        case V_CMPSX_U_F32          :
        case V_CMPSX_NGE_F32        :
        case V_CMPSX_NLG_F32        :
        case V_CMPSX_NGT_F32        :
        case V_CMPSX_NLE_F32        :
        case V_CMPSX_NEQ_F32        :
        case V_CMPSX_NLT_F32        :
        case V_CMPSX_TRU_F32        :
            return 1;
        case V_CMPS_F_F64           :
        case V_CMPS_LT_F64          :
        case V_CMPS_EQ_F64          :
        case V_CMPS_LE_F64          :
        case V_CMPS_GT_F64          :
        case V_CMPS_LG_F64          :
        case V_CMPS_GE_F64          :
        case V_CMPS_O_F64           :
        case V_CMPS_U_F64           :
        case V_CMPS_NGE_F64         :
        case V_CMPS_NLG_F64         :
        case V_CMPS_NGT_F64         :
        case V_CMPS_NLE_F64         :
        case V_CMPS_NEQ_F64         :
        case V_CMPS_NLT_F64         :
        case V_CMPS_TRU_F64         :
        case V_CMPSX_F_F64          :
        case V_CMPSX_LT_F64         :
        case V_CMPSX_EQ_F64         :
        case V_CMPSX_LE_F64         :
        case V_CMPSX_GT_F64         :
        case V_CMPSX_LG_F64         :
        case V_CMPSX_GE_F64         :
        case V_CMPSX_O_F64          :
        case V_CMPSX_U_F64          :
        case V_CMPSX_NGE_F64        :
        case V_CMPSX_NLG_F64        :
        case V_CMPSX_NGT_F64        :
        case V_CMPSX_NLE_F64        :
        case V_CMPSX_NEQ_F64        :
        case V_CMPSX_NLT_F64        :
        case V_CMPSX_TRU_F64        :
            return 2;
        case V_CMP_F_I32            :
        case V_CMP_LT_I32           :
        case V_CMP_EQ_I32           :
        case V_CMP_LE_I32           :
        case V_CMP_GT_I32           :
        case V_CMP_LG_I32           :
        case V_CMP_GE_I32           :
        case V_CMP_TRU_I32          :
        case V_CMPX_F_I32           :
        case V_CMPX_LT_I32          :
        case V_CMPX_EQ_I32          :
        case V_CMPX_LE_I32          :
        case V_CMPX_GT_I32          :
        case V_CMPX_LG_I32          :
        case V_CMPX_GE_I32          :
        case V_CMPX_TRU_I32         :
            return 1;
        case V_CMP_F_I64            :
        case V_CMP_LT_I64           :
        case V_CMP_EQ_I64           :
        case V_CMP_LE_I64           :
        case V_CMP_GT_I64           :
        case V_CMP_LG_I64           :
        case V_CMP_GE_I64           :
        case V_CMP_TRU_I64          :
        case V_CMPX_F_I64           :
        case V_CMPX_LT_I64          :
        case V_CMPX_EQ_I64          :
        case V_CMPX_LE_I64          :
        case V_CMPX_GT_I64          :
        case V_CMPX_LG_I64          :
        case V_CMPX_GE_I64          :
        case V_CMPX_TRU_I64         :
            return 2;
        case V_CMP_F_U32            :
        case V_CMP_LT_U32           :
        case V_CMP_EQ_U32           :
        case V_CMP_LE_U32           :
        case V_CMP_GT_U32           :
        case V_CMP_LG_U32           :
        case V_CMP_GE_U32           :
        case V_CMP_TRU_U32          :
        case V_CMPX_F_U32           :
        case V_CMPX_LT_U32          :
        case V_CMPX_EQ_U32          :
        case V_CMPX_LE_U32          :
        case V_CMPX_GT_U32          :
        case V_CMPX_LG_U32          :
        case V_CMPX_GE_U32          :
        case V_CMPX_TRU_U32         :
            return 1;
        case V_CMP_F_U64            :
        case V_CMP_LT_U64           :
        case V_CMP_EQ_U64           :
        case V_CMP_LE_U64           :
        case V_CMP_GT_U64           :
        case V_CMP_LG_U64           :
        case V_CMP_GE_U64           :
        case V_CMP_TRU_U64          :
        case V_CMPx_F_U64           :
        case V_CMPx_LT_U64          :
        case V_CMPx_EQ_U64          :
        case V_CMPx_LE_U64          :
        case V_CMPx_GT_U64          :
        case V_CMPx_LG_U64          :
        case V_CMPx_GE_U64          :
        case V_CMPx_TRU_U64         :
        case V_CMP_CLASS_F64        :
        case V_CMPX_CLASS_F64       :
            return 2;
        case V_CMP_CLASS_F32        :
        case V_CMPX_CLASS_F32       :
            return 1;

       
        case V_DIV_SCALE_F32    :    //  = D.f = Special case divide preop and flags(s0.f = Quotient,s1.f = Denominator, s2.f = Numerator) s0 must equal s1 or s2.
        case V_MAD_LEGACY_F32   :   //   = D.f = S0.f * S1.f + S2.f (DX9 rules, 0.0*x = 0.0).
        case V_MAD_F32          :   //   = D.f = S0.f * S1.f + S2.f.
        case V_MAD_I32_I24      :   //   = D.i = S0.i * S1.i + S2.iD.i = S0.i * S1.i + S2.i.
        case V_MAD_U32_U24      :   //   = D.u = S0.u * S1.u + S2.u.
        case V_CUBEID_F32       :   //   = Rm.w <- Rn,x, Rn,y, Rn.z.
        case V_CUBESC_F32       :   //   = Rm.y <- Rn,x, Rn,y, Rn.z.
        case V_CUBETC_F32       :   //   = Rm.x <- Rn,x, Rn,y, Rn.z.
        case V_CUBEMA_F32       :   //   = Rm.z <- Rn,x, Rn,y, Rn.z
        case V_BFE_U32          :   //   = D.u = (S0.u>>S1.u[4:0]) & ((1<<S2.u[4:0])-1); bitfield extract,S0=data, S1=field_offset, S2=field_width.
        case V_BFE_I32          :   //   = D.i = (S0.i>>S1.u[4:0]) & ((1<<S2.u[4:0])-1); bitfield extract, S0=data, S1=field_offset, S2=field_width.
        case V_BFI_B32          :   //   = D.u = (S0.u & S1.u) | (~S0.u & S2.u); bitfield insert.
        case V_FMA_F32          :   //   = D.f = S0.f * S1.f + S2.f
        case V_LERP_U8          :   //  = D.u = ((S0.u[31:24] + S1.u[31:24] + S2.u[24]) >> 1) << 24 +
        case V_ALIGNBIT_B32     :   //  = D.u = ({S0,S1} >> S2.u[4:0]) & 0xFFFFFFFF.
        case V_ALIGNBYTE_B32    :   //  = D.u = ({S0,S1} >> (8*S2.u[4:0])) & 0xFFFFFFFF.
        case V_MULLIT_F32       :   //  = D.f = S0.f * S1.f, replicate result into 4 components (0.0 * x = 0.0; special INF, NaN, overflow rules).
        case V_MIN3_F32         :   //  = D.f = min(S0.f, S1.f, S2.f).
        case V_MIN3_I32         :   //  = D.i = min(S0.i, S1.i, S2.i).
        case V_MIN3_U32         :   //  = 0x153 D.u = min(S0.u, S1.u, S2.u).
        case V_MAX3_F32         :   //  = D.f = max(S0.f, S1.f, S2.f).
        case V_MAX3_I32         :   //  = D.i = max(S0.i, S1.i, S2.i).
        case V_MAX3_U32         :   //  = D.u = max(S0.u, S1.u, S2.u).
        case V_MED3_F32         :   //  = D.f = median(S0.f, S1.f, S2.f).
        case V_MED3_I32         :   //  = D.i = median(S0.i, S1.i, S2.i).
        case V_MED3_U32         :   //  = D.u = median(S0.u, S1.u, S2.u).
        case V_SAD_U8           :   //  = D.u = Byte SAD with accum_lo(S0.u, S1.u, S2.u).
        case V_SAD_HI_U8        :   //  = D.u = Byte SAD with accum_hi(S0.u, S1.u, S2.u).
        case V_SAD_U16          :   //  = D.u = Word SAD with accum(S0.u, S1.u, S2.u).
        case V_SAD_U32          :   //  = D.u = Dword SAD with accum(S0.u, S1.u, S2.u).
        case V_CVT_PK_U8_F32    :   //  = f32->u8(s0.f), pack into byte(s1.u), of dword(s2).
        case V_DIV_FIXUP_F32    :   //  = D.f = Special case divide fixup and flags(s0.f = Quotient,   s1.f = Denominator, s2.f = Numerator).
        case V_LSHL_B64         :   //  = D = S0.u << S1.u[4:0].
        case V_LSHR_B64         :   //  = D = S0.u >> S1.u[4:0].
        case V_ASHR_I64         :   //  = D = S0.u >> S1.u[4:0].
        case V_LDEXP_F64        :   //  = D.d = pow(S0.d, S1.i[31:0]).
            return 1;
        case V_FMA_F64          :   //   = D.d = S0.d * S1.d + S2.d.
        case V_DIV_SCALE_F64    :    //  = D.d = Special case divide preop and flags(s0.d = Quotient,s1.d = Denominator, s2.d = Numerator) s0 must equal s1 or s2.
        case V_DIV_FIXUP_F64    :   //  = D.d = Special case divide fixup and flags(s0.d = Quotient,  s1.d = Denominator, s2.d = Numerator).
        case V_ADD_F64          :   //  = D.d = S0.d + S1.d.
        case V_MUL_F64          :   //  = D.d = S0.d * S1.d.
        case V_MIN_F64          :   //  = D.d = min(S0.d, S1.d).
        case V_MAX_F64          :   //  = D.d = max(S0.d, S1.d).
        case V_DIV_FMAS_F64    : // = D.d = Special case divide FMA with scale and flags(s0.d= Quotient, s1.d = Denominator, s2.d = Numerator).
        case V_TRIG_PREOP_F64  : // = D.d = Look Up 2/PI (S0.d) with segment select  S1.u[4:0].
            return 2;
        case V_MUL_LO_U32       :   //  = D.u = S0.u * S1.u.
        case V_MUL_HI_U32       :   //  = D.u = (S0.u * S1.u)>>32.
        case V_MUL_LO_I32       :   //  = D.i = S0.i * S1.i.
        case V_MUL_HI_I32       :   //  = D.i = (S0.i * S1.i)>>32.
        case V_DIV_FMAS_F32    : // = D.f = Special case divide FMA with scale and flags(s0.f = Quotient, s1.f = Denominator, s2.f = Numerator).
        case V_MSAD_U8         : // = D.u = Masked Byte SAD with accum_lo(S0.u, S1.u, S2.u).
        case V_QSAD_U8         : // = D.u = Quad-Byte SAD with accum_lo/hiu(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        case V_MQSAD_U8        : // = D.u = Masked Quad-Byte SAD with accum_lo/hi(S0.u[63:0],S1.u[31:0], S2.u[63:0]).       
        case V_QSAD_PK_U16_U8  : // : D.u = Quad-Byte SAD with accum_lo/hiu(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        case V_MQSAD_PK_U16_U8 : // = D.u = Masked Quad-Byte SAD with accum_lo/hi(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        case V_MQSAD_U32_U8    :  // : D.u128 = Masked Quad-Byte SAD with 32-bit accum_lo/hi(S0.u[63:0], S1.u[31:0], S2.u[127:0]).
        case V_MAD_U64_U32     :  // : {vcc_out,D.u64} = S0.u32 * S1.u32 + S2.u64.
        case V_MAD_I64_I32     :  // : {vcc_out,D.i64} = S0.i32 * S1.i32 + S2.i64.
            return 1;

        }
    }


    uint GetSrc2WidthInDWORDs( VectorInstructions e )
    {
        switch(e)
        {
        default: return 0;
        case V_DIV_SCALE_F32    :    //  = D.f = Special case divide preop and flags(s0.f = Quotient,s1.f = Denominator, s2.f = Numerator) s0 must equal s1 or s2.
        case V_MAD_LEGACY_F32   :   //   = D.f = S0.f * S1.f + S2.f (DX9 rules, 0.0*x = 0.0).
        case V_MAD_F32          :   //   = D.f = S0.f * S1.f + S2.f.
        case V_MAD_I32_I24      :   //   = D.i = S0.i * S1.i + S2.iD.i = S0.i * S1.i + S2.i.
        case V_MAD_U32_U24      :   //   = D.u = S0.u * S1.u + S2.u.
        case V_CUBEID_F32       :   //   = Rm.w <- Rn,x, Rn,y, Rn.z.
        case V_CUBESC_F32       :   //   = Rm.y <- Rn,x, Rn,y, Rn.z.
        case V_CUBETC_F32       :   //   = Rm.x <- Rn,x, Rn,y, Rn.z.
        case V_CUBEMA_F32       :   //   = Rm.z <- Rn,x, Rn,y, Rn.z
        case V_BFE_U32          :   //   = D.u = (S0.u>>S1.u[4:0]) & ((1<<S2.u[4:0])-1); bitfield extract,S0=data, S1=field_offset, S2=field_width.
        case V_BFE_I32          :   //   = D.i = (S0.i>>S1.u[4:0]) & ((1<<S2.u[4:0])-1); bitfield extract, S0=data, S1=field_offset, S2=field_width.
        case V_BFI_B32          :   //   = D.u = (S0.u & S1.u) | (~S0.u & S2.u); bitfield insert.
        case V_FMA_F32          :   //   = D.f = S0.f * S1.f + S2.f
        case V_LERP_U8          :   //  = D.u = ((S0.u[31:24] + S1.u[31:24] + S2.u[24]) >> 1) << 24 +
        case V_ALIGNBIT_B32     :   //  = D.u = ({S0,S1} >> S2.u[4:0]) & 0xFFFFFFFF.
        case V_ALIGNBYTE_B32    :   //  = D.u = ({S0,S1} >> (8*S2.u[4:0])) & 0xFFFFFFFF.
        case V_MIN3_F32         :   //  = D.f = min(S0.f, S1.f, S2.f).
        case V_MIN3_I32         :   //  = D.i = min(S0.i, S1.i, S2.i).
        case V_MIN3_U32         :   //  = 0x153 D.u = min(S0.u, S1.u, S2.u).
        case V_MAX3_F32         :   //  = D.f = max(S0.f, S1.f, S2.f).
        case V_MAX3_I32         :   //  = D.i = max(S0.i, S1.i, S2.i).
        case V_MAX3_U32         :   //  = D.u = max(S0.u, S1.u, S2.u).
        case V_MED3_F32         :   //  = D.f = median(S0.f, S1.f, S2.f).
        case V_MED3_I32         :   //  = D.i = median(S0.i, S1.i, S2.i).
        case V_MED3_U32         :   //  = D.u = median(S0.u, S1.u, S2.u).
        case V_SAD_U8           :   //  = D.u = Byte SAD with accum_lo(S0.u, S1.u, S2.u).
        case V_SAD_HI_U8        :   //  = D.u = Byte SAD with accum_hi(S0.u, S1.u, S2.u).
        case V_SAD_U16          :   //  = D.u = Word SAD with accum(S0.u, S1.u, S2.u).
        case V_SAD_U32          :   //  = D.u = Dword SAD with accum(S0.u, S1.u, S2.u).
        case V_CVT_PK_U8_F32    :   //  = f32->u8(s0.f), pack into byte(s1.u), of dword(s2).
        case V_DIV_FIXUP_F32    :   //  = D.f = Special case divide fixup and flags(s0.f = Quotient,   s1.f = Denominator, s2.f = Numerator).
            return 1;

        case V_FMA_F64          :   //   = D.d = S0.d * S1.d + S2.d.
        case V_DIV_SCALE_F64    :    //  = D.d = Special case divide preop and flags(s0.d = Quotient,s1.d = Denominator, s2.d = Numerator) s0 must equal s1 or s2.
        case V_DIV_FIXUP_F64    :   //  = D.d = Special case divide fixup and flags(s0.d = Quotient,  s1.d = Denominator, s2.d = Numerator).
        case V_DIV_FMAS_F64    : // = D.d = Special case divide FMA with scale and flags(s0.d= Quotient, s1.d = Denominator, s2.d = Numerator).
        case V_TRIG_PREOP_F64  : // = D.d = Look Up 2/PI (S0.d) with segment select  S1.u[4:0].
        case V_MAD_U64_U32     :  // : {vcc_out,D.u64} = S0.u32 * S1.u32 + S2.u64.
        case V_MAD_I64_I32     :  // : {vcc_out,D.i64} = S0.i32 * S1.i32 + S2.i64.
            return 2;
        case V_MUL_LO_U32       :   //  = D.u = S0.u * S1.u.
        case V_MUL_HI_U32       :   //  = D.u = (S0.u * S1.u)>>32.
        case V_MUL_LO_I32       :   //  = D.i = S0.i * S1.i.
        case V_MUL_HI_I32       :   //  = D.i = (S0.i * S1.i)>>32.
        case V_DIV_FMAS_F32    : // = D.f = Special case divide FMA with scale and flags(s0.f = Quotient, s1.f = Denominator, s2.f = Numerator).
        case V_MSAD_U8         : // = D.u = Masked Byte SAD with accum_lo(S0.u, S1.u, S2.u).
        case V_QSAD_U8         : // = D.u = Quad-Byte SAD with accum_lo/hiu(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        case V_MQSAD_U8        : // = D.u = Masked Quad-Byte SAD with accum_lo/hi(S0.u[63:0],S1.u[31:0], S2.u[63:0]).       
        case V_QSAD_PK_U16_U8  : // : D.u = Quad-Byte SAD with accum_lo/hiu(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        case V_MQSAD_PK_U16_U8 : // = D.u = Masked Quad-Byte SAD with accum_lo/hi(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        case V_MQSAD_U32_U8    :  // : D.u128 = Masked Quad-Byte SAD with 32-bit accum_lo/hi(S0.u[63:0], S1.u[31:0], S2.u[127:0]).
            return 1;
        }
    }


    uint GetDataWidthInDWORDs( DSInstructions eOp )
    {
        switch( eOp )
        {
        default:
            return 0;
        case DS_PERMUTE_B32:
        case DS_BPERMUTE_B32:
        case DS_ADD_U32                   :
        case DS_ADD_F32                   :
        case DS_SUB_U32                   :
        case DS_RSUB_U32                  :
        case DS_INC_U32                   :
        case DS_DEC_U32                   :
        case DS_MIN_I32                   :
        case DS_MAX_I32                   :
        case DS_MIN_U32                   :
        case DS_MAX_U32                   :
        case DS_AND_B32                   :
        case DS_OR_B32                    :
        case DS_XOR_B32                   :
        case DS_MSKOR_B32                 :
        case DS_WRITE_B32                 :
        case DS_WRITE2_B32                :
        case DS_WRITE2ST64_B32            :
        case DS_CMPST_B32                 :
        case DS_CMPST_F32                 :
        case DS_MIN_F32                   :
        case DS_MAX_F32                   :
        case DS_WRITE_B8                  :
        case DS_WRITE_B16                 :
        case DS_ADD_RTN_U32               :
        case DS_SUB_RTN_U32               :
        case DS_RSUB_RTN_U32              :
        case DS_INC_RTN_U32               :
        case DS_DEC_RTN_U32               :
        case DS_MIN_RTN_I32               :
        case DS_MAX_RTN_I32               :
        case DS_MIN_RTN_U32               :
        case DS_MAX_RTN_U32               :
        case DS_AND_RTN_B32               :
        case DS_OR_RTN_B32                :
        case DS_XOR_RTN_B32               :
        case DS_MSKOR_RTN_B32             :
        case DS_WRXCHG_RTN_B32            :
        case DS_WRXCHG2_RTN_B32           :
        case DS_WRXCHG2ST64_RTN_B32       :
        case DS_CMPST_RTN_B32             :
        case DS_CMPST_RTN_F32             :
        case DS_MIN_RTN_F32               :
        case DS_MAX_RTN_F32               :
        case DS_WRAP_RTN_B32              :
        case DS_SWIZZLE_B32               :
        case DS_READ_B32                  :
        case DS_READ2_B32                 :
        case DS_READ2ST64_B32             :
        case DS_READ_I8                   :
        case DS_READ_U8                   :
        case DS_READ_I16                  :
        case DS_READ_U16                  :
        case DS_CONSUME                   :
        case DS_APPEND                    :
        case DS_ORDERED_COUNT             :
            return 1;
        
        case DS_ADD_U64                  :
        case DS_SUB_U64                  :
        case DS_RSUB_U64                 :
        case DS_INC_U64                  :
        case DS_DEC_U64                  :
        case DS_MIN_I64                  :
        case DS_MAX_I64                  :
        case DS_MIN_U64                  :
        case DS_MAX_U64                  :
        case DS_AND_B64                  :
        case DS_OR_B64                   :
        case DS_XOR_B64                  :
        case DS_MSKOR_B64                :
        case DS_WRITE_B64                :
        case DS_WRITE2_B64               :
        case DS_WRITE2ST64_B64           :
        case DS_CMPST_B64                :
        case DS_CMPST_F64                :
        case DS_MIN_F64                  :
        case DS_MAX_F64                  :
        case DS_ADD_RTN_U64              :
        case DS_SUB_RTN_U64              :
        case DS_RSUB_RTN_U64             :
        case DS_INC_RTN_U64              :
        case DS_DEC_RTN_U64              :
        case DS_MIN_RTN_I64              :
        case DS_MAX_RTN_I64              :
        case DS_MIN_RTN_U64              :
        case DS_MAX_RTN_U64              :
        case DS_AND_RTN_B64              :
        case DS_OR_RTN_B64               :
        case DS_XOR_RTN_B64              :
        case DS_MSKOR_RTN_B64            :
        case DS_WRXCHG_RTN_B64           :
        case DS_WRXCHG2_RTN_B64          :
        case DS_WRXCHG2ST64_RTN_B64      :
        case DS_CMPST_RTN_B64            :
        case DS_CMPST_RTN_F64            :
        case DS_MIN_RTN_F64              :
        case DS_MAX_RTN_F64              :
        case DS_READ_B64                 :
        case DS_READ2_B64                :
        case DS_READ2ST64_B64            :
        case DS_CONDXCHG32_RTN_B64       :
            return 2;

        case DS_ADD_SRC2_U32             :
        case DS_SUB_SRC2_U32             :
        case DS_RSUB_SRC2_U32            :
        case DS_INC_SRC2_U32             :
        case DS_DEC_SRC2_U32             :
        case DS_MIN_SRC2_I32             :
        case DS_MAX_SRC2_I32             :
        case DS_MIN_SRC2_U32             :
        case DS_MAX_SRC2_U32             :
        case DS_AND_SRC2_B32             :
        case DS_OR_SRC2_B32              :
        case DS_XOR_SRC2_B32             :
        case DS_WRITE_SRC2_B32           :
        case DS_MIN_SRC2_F32             :
        case DS_MAX_SRC2_F32             :
            return 1;

        case DS_ADD_SRC2_U64             :
        case DS_SUB_SRC2_U64             :
        case DS_RSUB_SRC2_U64            :
        case DS_INC_SRC2_U64             :
        case DS_DEC_SRC2_U64             :
        case DS_MIN_SRC2_I64             :
        case DS_MAX_SRC2_I64             :
        case DS_MIN_SRC2_U64             :
        case DS_MAX_SRC2_U64             :
        case DS_AND_SRC2_B64             :
        case DS_OR_SRC2_B64              :
        case DS_XOR_SRC2_B64             :
        case DS_WRITE_SRC2_B64           :
        case DS_MIN_SRC2_F64             :
        case DS_MAX_SRC2_F64             :
            return 2;

        case DS_WRITE_B96                :
        case DS_READ_B96                 :
            return 3;

        case DS_WRITE_B128               :
        case DS_CONDXCHG32_RTN_B128      :
        case DS_READ_B128                :
            return 4;
        }
    }



    uint ScalarInstruction::GetResultWidthInDWORDs() const
    {
        switch( GetOpcode() )
        {
        default: return 0;
        case S_ADD_U32            :   //: D.u = S0.u + S1.u. SCC = carry out.
        case S_SUB_U32            :   //: D.u = S0.u - S1.u. SCC = carry out.
        case S_ADD_I32            :   //: D.u = S0.i + S1.i. SCC = overflow.
        case S_SUB_I32            :   //: D.u = S0.i - S1.i. SCC = overflow.
        case S_ADDC_U32           :   //: D.u = S0.u + S1.u + SCC. SCC = carry-out.
        case S_SUBB_U32           :   //: D.u = S0.u - S1.u - SCC. SCC = carry-out.
        case S_MIN_I32            :   //: D.i = (S0.i < S1.i) ? S0.i : S1.i. SCC = 1 if S0 is min.
        case S_MIN_U32            :   //: D.u = (S0.u < S1.u) ? S0.u : S1.u. SCC = 1 if S0 is min.
        case S_MAX_I32            :   //: D.i = (S0.i > S1.i) ? S0.i : S1.i. SCC = 1 if S0 is max.
        case S_MAX_U32            :   //: D.u = (S0.u > S1.u) ? S0.u : S1.u. SCC = 1 if S0 is max.
        case S_CSELECT_B32        :   //: D.u = SCC ? S0.u : S1.u.
        case S_AND_B32            :   //: D.u = S0.u & S1.u. SCC = 1 if result is non-zero.
        case S_OR_B32             :   //: D.u = S0.u | S1.u. SCC = 1 if result is non-zero.
        case S_XOR_B32            :   //: D.u = S0.u ^ S1.u. SCC = 1 if result is non-zero.
        case S_ANDN2_B32          :   //: D.u = S0.u & ~S1.u. SCC = 1 if result is non-zero.
        case S_ORN2_B32           :   //: D.u = S0.u | ~S1.u. SCC = 1 if result is non-zero.
        case S_NAND_B32           :   //: D.u = ~(S0.u & S1.u). SCC = 1 if result is non-zero.
        case S_NOR_B32            :   //: D.u = ~(S0.u | S1.u). SCC = 1 if result is non-zero.
        case S_XNOR_B32           :   //: D.u = ~(S0.u ^ S1.u). SCC = 1 if result is non-zero.
        case S_LSHL_B32           :   //: D.u = S0.u << S1.u[4:0]. SCC = 1 if result is non-zero.
        case S_LSHR_B32           :   //: D.u = S0.u >> S1.u[4:0]. SCC = 1 if result is non-zero.
        case S_ASHR_I32           :   //: D.i = signtext(S0.i) >> S1.i[4:0]. SCC = 1 if result is non-zero.
        case S_BFM_B32            :   //: D.u = ((1 << S0.u[4:0]) - 1) << S1.u[4:0]; bitfield mask.
        case S_MUL_I32            :   //: D.i = S0.i * S1.i.
        case S_BFE_U32            :    // : Bit field extract. S0 is data, S1[4:0] is field offset, S1[22:16] is field width. D.u = (S0.u >> S1.u[4:0]) & ((1 << S1.u[22:16]) - 1). SCC = 1 if resultis non-zero.
        case S_BFE_I32            :
        case S_ABSDIFF_I32        :  //  D.i = abs(S0.i >> S1.i). SCC = 1 if result is non-zero.
        case S_MOVK_I32           :  //: D.i = signext(SIMM16).
        case S_CMOVK_I32          :  //: if (SCC) D.i = signext(SIMM16); else NOP.
        case S_CMPK_EQ_I32        :  //: SCC = (D.i == signext(SIMM16).
        case S_CMPK_LG_I32        :  //: SCC = (D.i != signext(SIMM16).
        case S_CMPK_GT_I32        :  //: SCC = (D.i != signext(SIMM16)).
        case S_CMPK_GE_I32        :  //: SCC = (D.i >= signext(SIMM16)).
        case S_CMPK_LT_I32        :  //: SCC = (D.i < signext(SIMM16)).
        case S_CMPK_LE_I32        :  //: SCC = (D.i <= signext(SIMM16)).
        case S_CMPK_EQ_U32        :  //: SCC = (D.u == SIMM16).
        case S_CMPK_LG_U32        :  //: SCC = (D.u != SIMM16).
        case S_CMPK_GT_U32        :  //: SCC = (D.u > SIMM16).
        case S_CMPK_GE_U32        :  //: SCC = (D.u >= SIMM16).
        case S_CMPK_LT_U32        :  //: SCC = (D.u < SIMM16).
        case S_CMPK_LE_U32        : //: D.u = SCC = (D.u <= SIMM16).
        case S_ADDK_I32           : //: D.i = D.i + signext(SIMM16). SCC = overflow.
        case S_MULK_I32           : //: D.i = D.i * signext(SIMM16). SCC = overflow.
        case S_GETREG_B32         : // : D.u = hardware register. Read some or all of a hardware register
        case S_MOV_B32             ://: D.u = S0.u.
        case S_CMOV_B32            ://: if(SCC) D.u = S0.u; else NOP.
        case S_NOT_B32             ://: D.u = ~S0.u SCC = 1 if result non-zero.
        case S_WQM_B32             ://: D.u = WholeQuadMode(S0.u). SCC = 1 if result is non-zero.
        case S_BREV_B32            ://: D.u = S0.u[0:31] (reverse bits).
        case S_BCNT0_I32_B32       ://: D.i = CountZeroBits(S0.u). SCC = 1 if result is non-zero.
        case S_BCNT0_I32_B64       ://: D.i = CountZeroBits(S0.u). SCC = 1 if result is non-zero.
        case S_BCNT1_I32_B32       ://: D.i = CountOneBits(S0.u). SCC = 1 if result is non-zero.
        case S_BCNT1_I32_B64       ://: D.i = CountOneBits(S0.u). SCC = 1 if result is non-zero.
        case S_FF0_I32_B32         ://: D.i = FindFirstZero(S0.u) from LSB; if no zeros, return -1.
        case S_FF0_I32_B64         ://: D.i = FindFirstZero(S0.u) from LSB; if no zeros, return -1.
        case S_FF1_I32_B32         ://: D.i = FindFirstOne(S0.u) from LSB; if no ones, return -1.
        case S_FF1_I32_B64         ://: D.i = FindFirstOne(S0.u) from LSB; if no ones, return -1.
        case S_FLBIT_I32_B32       ://: D.i = FindFirstOne(S0.u) from MSB; if no ones, return -1.
        case S_FLBIT_I32_B64       ://: D.i = FindFirstOne(S0.u) from MSB; if no ones, return -1.
        case S_FLBIT_I32           ://: D.i = Find first bit opposite of sign bit from MSB. If S0 == -1, return -1.
        case S_FLBIT_I32_I64       ://: D.i = Find first bit opposite of sign bit from MSB. If S0 == -1, return -1.
        case S_SEXT_I32_I8         ://: D.i = signext(S0.i[7:0]).
        case S_SEXT_I32_I16        ://: D.i = signext(S0.i[15:0]).
        case S_BITSET0_B32         ://: D.u[S0.u[4:0]] = 0.
        case S_BITSET1_B32         ://: D.u[S0.u[4:0]] = 1.
        case S_QUADMASK_B32        ://: D.u = QuadMask(S0.u). D[0] = OR(S0[3:0]), D[1] = OR(S0[7:4]) .... SCC = 1 if result is non-zero.
        case S_MOVRELS_B32         ://: SGPR[D.u] = SGPR[S0.u + M0.u].
        case S_MOVRELD_B32         ://: SGPR[D.u + M0.u] = SGPR[S0.u].
        case S_ABS_I32             :      //: D.i = abs(S0.i). SCC=1 if result is non-zero.
        case S_MOV_FED_B32         :      //: D.u = S0.u, introduce edc double error upon write to dest sgpr.  
        case S_CMP_EQ_I32          :     //: SCC = (S0.i == S1.i).
        case S_CMP_LG_I32          :     //: SCC = (S0.i != S1.i).
        case S_CMP_GT_I32          :     //: SCC = (S0.i > S1.i).
        case S_CMP_GE_I32          :     //: SCC = (S0.i >= S1.i).
        case S_CMP_LT_I32          :     //: SCC = (S0.i < S1.i).
        case S_CMP_LE_I32          :     //: SCC = (S0.i <= S1.i).
        case S_CMP_EQ_U32          :     //: SCC = (S0.u == S1.u).
        case S_CMP_LG_U32          :     //: SCC = (S0.u != S1.u).
        case S_CMP_GT_U32          :     //: SCC = (S0.u > S1.u).
        case S_CMP_GE_U32          :     //: SCC = (S0.u >= S1.u).
        case S_CMP_LT_U32          :     //: SCC = (S0.u < S1.u).
        case S_CMP_LE_U32          :     //: SCC = (S0.u <= S1.u).
        case S_BITCMP0_B32         :     //: SCC = (S0.u[S1.u[4:0]] == 0).
        case S_BITCMP1_B32         :     //: SCC = (S0.u[S1.u[4:0]] == 1).
        case S_BITCMP0_B64         :     //: SCC = (S0.u[S1.u[5:0]] == 0).
        case S_BITCMP1_B64         :     //: SCC = (S0.u[S1.u[5:0]] == 1).
        case S_SETVSKIP            :     //: VSKIP = S0.u[S1.u[4:0]].
        case S_CMP_EQ_U64:
        case S_CMP_NE_U64:
            return 1;

        case S_AND_B64               :   //: D.u = S0.u & S1.u. SCC = 1 if result is non-zero.
        case S_OR_B64                :   //: D.u = S0.u | S1.u. SCC = 1 if result is non-zero.
        case S_XOR_B64               :   //: D.u = S0.u ^ S1.u. SCC = 1 if result is non-zero.
        case S_ANDN2_B64             :   //: D.u = S0.u & ~S1.u. SCC = 1 if result is non-zero.
        case S_ORN2_B64              :   //: D.u = S0.u | ~S1.u. SCC = 1 if result is non-zero.
        case S_NAND_B64              :   //: D.u = ~(S0.u & S1.u). SCC = 1 if result is non-zero.
        case S_NOR_B64               :   //: D.u = ~(S0.u | S1.u). SCC = 1 if result is non-zero.
        case S_XNOR_B64              :   //: D.u = ~(S0.u ^ S1.u). SCC = 1 if result is non-zero.
        case S_LSHL_B64              :   //: D.u = S0.u << S1.u[5:0]. SCC = 1 if result is non-zero.
        case S_LSHR_B64              :   //: D.u = S0.u >> S1.u[5:0]. SCC = 1 if result is non-zero.
        case S_ASHR_I64              :   //: D.i = signtext(S0.i) >> S1.i[5:0]. SCC = 1 if result is non-zero.
        case S_BFM_B64               :   //: D.u = ((1 << S0.u[5:0]) - 1) << S1.u[5:0]; bitfield mask.
        case S_BFE_U64               :
        case S_BFE_I64               :
        case S_MOV_B64               ://: D/u = S0.u.
        case S_CMOV_B64              ://: if(SCC) D.u = S0.u; else NOP.
        case S_NOT_B64               ://: D.u = ~S0.u SCC = 1 if result non-zero.
        case S_WQM_B64               ://: D.u = WholeQuadMode(S0.u). SCC = 1 if result is non-zero.
        case S_BREV_B64              ://: D.u = S0.u[0:63] (reverse bits).
        case S_BITSET0_B64           ://: D.u[S0.u[5:0]] = 0.
        case S_BITSET1_B64           ://: D.u[S0.u[5:0]] = 1.
        case S_GETPC_B64             ://: D.u = PC + 4; destination receives the byte address of the next instruction.
        case S_SWAPPC_B64            ://: D.u = PC + 4; PC = S0.u.
        case S_AND_SAVEEXEC_B64      ://: D.u = EXEC, EXEC = S0.u & EXEC. SCC = 1 if the new value of EXEC is non-zero.
        case S_OR_SAVEEXEC_B64       ://: D.u = EXEC, EXEC = S0.u | EXEC. SCC = 1 if the newvalue of EXEC is non-zero.
        case S_XOR_SAVEEXEC_B64      ://: D.u = EXEC, EXEC = S0.u ^ EXEC. SCC = 1 if the new value of EXEC is non-zero.
        case S_ANDN2_SAVEEXEC_B64    ://: D.u = EXEC, EXEC = S0.u & ~EXEC. SCC =1 if the new value of EXEC is non-zero.
        case S_ORN2_SAVEEXEC_B64     ://: D.u = EXEC, EXEC = S0.u | ~EXEC. SCC = 1 if the new value of EXEC is non-zero.
        case S_NAND_SAVEEXEC_B64     ://: D.u = EXEC, EXEC = ~(S0.u & EXEC). SCC =1 if the new value of EXEC is non-zero.
        case S_NOR_SAVEEXEC_B64      ://: D.u = EXEC, EXEC = ~(S0.u | EXEC). SCC = 1 if the new value of EXEC is non-zero.
        case S_XNOR_SAVEEXEC_B64     ://: D.u = EXEC, EXEC = ~(S0.u ^ EXEC). SCC = 1 if the new value of EXEC is non-zero.
        case S_QUADMASK_B64          ://: D.u = QuadMask(S0.u). D[0] = OR(S0[3:0]),D[1] = OR(S0[7:4]) .... SCC = 1 if result is non-zero
        case S_CSELECT_B64           :   //: D.u = SCC ? S0.u : S1.u.
            return 2;

        case S_CBRANCH_G_FORK           : // Conditional branch using branch stack. Arg0 = compare mask (VCC or any SGPR), Arg1 = 64-bit byte address of target instruction.
        case S_CBRANCH_I_FORK           : //: Conditional branch using branch-stack.
        case S_SETREG_B32               : // : hardware register = D.u. Write some or all of the LSBs of D
        case S_SETREG_IMM32_B32         : //: This instruction uses a 32-bit literal constant. Write
        case S_RFE_B64                  ://: Return from Exception; PC = TTMP1,0.
        case S_SETPC_B64                ://: PC = S0.u; S0.u is a byte address of the instruction to jump to.
        case S_MOVRELS_B64              ://: SGPR[D.u] = SGPR[S0.u + M0.u].
        case S_MOVRELD_B64              ://: SGPR[D.u + M0.u] = SGPR[S0.u].
        case S_CBRANCH_JOIN             ://: Conditional branch join point. Arg0 = saved CSP value. No dest.
        case S_NOP                      :  //: do nothing. Repeat NOP 1..8 times based on SIMM16[2:0]. 0 = 1 time, 7 = 8 times.
        case S_ENDPGM                   :  //: end of program; terminate wavefront.
        case S_BRANCH                   :  //: PC = PC + signext(SIMM16 * 4) + 4.
        case S_CBRANCH_SCC0             :  //: if(SCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        case S_CBRANCH_SCC1             :  //: if(SCC == 1) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        case S_CBRANCH_VCCZ             :  //: if(VCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        case S_CBRANCH_VCCNZ            :  //: if(VCC != 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        case S_CBRANCH_EXECZ            :  //: if(EXEC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        case S_CBRANCH_EXECNZ           :  //: if(EXEC != 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        case S_BARRIER                  :  //: Sync waves within a thread group.                              
        case S_WAITCNT                  :  //: Wait for count of outstanding lds, vector-memory and
        case S_SETHALT                  :  //: set HALT bit to value of SIMM16[0]. 1=halt, 0=resume. Halt is ignored while priv=1.
        case S_SLEEP                    :  //: Cause a wave to sleep for approximately 64*SIMM16[2:0] clocks.
        case S_SETPRIO                  :  //: User settable wave priority. 0 = lowest, 3 = highest.
        case S_SENDMSG                  :  //: Send a message.
        case S_SENDMSGHALT              :  //: Send a message and then HALT.
        case S_TRAP                     :  //: Enter the trap handler. TrapID = SIMM16[7:0]. Wait for all instructions to complete, 
        case S_ICACHE_INV               :  //: Invalidate entire L1 I cache.
        case S_INCPERFLEVEL             :  //: Increment performance counter specified in SIMM16[3:0] by 1.
        case S_DECPERFLEVEL             :  //: Decrement performance counter specified in SIMM16[3:0] by 1.
        case S_TTRACEDATA               :  //: Send M0 as user data to thread-trace.
        case S_CBRANCH_CDBGSYS          :  // : If (conditional_debug_system != 0) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
        case S_CBRANCH_CDBGUSER         :  // : If (conditional_debug_user != 0) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
        case S_CBRANCH_CDBGSYS_OR_USER  :  // : If (conditional_debug_system || conditional_debug_user) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
        case S_CBRANCH_CDBGSYS_AND_USER :  // : If (conditional_debug_system && conditional_debug_user) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
        case S_RFE_RESTORE_B64:
            return 0;
        }

    }

    uint ScalarInstruction::GetArg0WidthInDWORDs() const
    {
        switch( GetOpcode() )
        {
        default: return 0;
        case S_ADD_U32            :   //: D.u = S0.u + S1.u. SCC = carry out.
        case S_SUB_U32            :   //: D.u = S0.u - S1.u. SCC = carry out.
        case S_ADD_I32            :   //: D.u = S0.i + S1.i. SCC = overflow.
        case S_SUB_I32            :   //: D.u = S0.i - S1.i. SCC = overflow.
        case S_ADDC_U32           :   //: D.u = S0.u + S1.u + SCC. SCC = carry-out.
        case S_SUBB_U32           :   //: D.u = S0.u - S1.u - SCC. SCC = carry-out.
        case S_MIN_I32            :   //: D.i = (S0.i < S1.i) ? S0.i : S1.i. SCC = 1 if S0 is min.
        case S_MIN_U32            :   //: D.u = (S0.u < S1.u) ? S0.u : S1.u. SCC = 1 if S0 is min.
        case S_MAX_I32            :   //: D.i = (S0.i > S1.i) ? S0.i : S1.i. SCC = 1 if S0 is max.
        case S_MAX_U32            :   //: D.u = (S0.u > S1.u) ? S0.u : S1.u. SCC = 1 if S0 is max.
        case S_CSELECT_B32        :   //: D.u = SCC ? S0.u : S1.u.
        case S_AND_B32            :   //: D.u = S0.u & S1.u. SCC = 1 if result is non-zero.
        case S_OR_B32             :   //: D.u = S0.u | S1.u. SCC = 1 if result is non-zero.
        case S_XOR_B32            :   //: D.u = S0.u ^ S1.u. SCC = 1 if result is non-zero.
        case S_ANDN2_B32          :   //: D.u = S0.u & ~S1.u. SCC = 1 if result is non-zero.
        case S_ORN2_B32           :   //: D.u = S0.u | ~S1.u. SCC = 1 if result is non-zero.
        case S_NAND_B32           :   //: D.u = ~(S0.u & S1.u). SCC = 1 if result is non-zero.
        case S_NOR_B32            :   //: D.u = ~(S0.u | S1.u). SCC = 1 if result is non-zero.
        case S_XNOR_B32           :   //: D.u = ~(S0.u ^ S1.u). SCC = 1 if result is non-zero.
        case S_LSHL_B32           :   //: D.u = S0.u << S1.u[4:0]. SCC = 1 if result is non-zero.
        case S_LSHR_B32           :   //: D.u = S0.u >> S1.u[4:0]. SCC = 1 if result is non-zero.
        case S_ASHR_I32           :   //: D.i = signtext(S0.i) >> S1.i[4:0]. SCC = 1 if result is non-zero.
        case S_BFM_B32            :   //: D.u = ((1 << S0.u[4:0]) - 1) << S1.u[4:0]; bitfield mask.
        case S_MUL_I32            :   //: D.i = S0.i * S1.i.
        case S_BFE_U32            :    // : Bit field extract. S0 is data, S1[4:0] is field offset, S1[22:16] is field width. D.u = (S0.u >> S1.u[4:0]) & ((1 << S1.u[22:16]) - 1). SCC = 1 if resultis non-zero.
        case S_BFE_I32            :
        case S_ABSDIFF_I32        :  //  D.i = abs(S0.i >> S1.i). SCC = 1 if result is non-zero.
        case S_MOVK_I32           :  //: D.i = signext(SIMM16).
        case S_CMOVK_I32          :  //: if (SCC) D.i = signext(SIMM16); else NOP.
        case S_CMPK_EQ_I32        :  //: SCC = (D.i == signext(SIMM16).
        case S_CMPK_LG_I32        :  //: SCC = (D.i != signext(SIMM16).
        case S_CMPK_GT_I32        :  //: SCC = (D.i != signext(SIMM16)).
        case S_CMPK_GE_I32        :  //: SCC = (D.i >= signext(SIMM16)).
        case S_CMPK_LT_I32        :  //: SCC = (D.i < signext(SIMM16)).
        case S_CMPK_LE_I32        :  //: SCC = (D.i <= signext(SIMM16)).
        case S_CMPK_EQ_U32        :  //: SCC = (D.u == SIMM16).
        case S_CMPK_LG_U32        :  //: SCC = (D.u != SIMM16).
        case S_CMPK_GT_U32        :  //: SCC = (D.u > SIMM16).
        case S_CMPK_GE_U32        :  //: SCC = (D.u >= SIMM16).
        case S_CMPK_LT_U32        :  //: SCC = (D.u < SIMM16).
        case S_CMPK_LE_U32        : //: D.u = SCC = (D.u <= SIMM16).
        case S_ADDK_I32           : //: D.i = D.i + signext(SIMM16). SCC = overflow.
        case S_MULK_I32           : //: D.i = D.i * signext(SIMM16). SCC = overflow.
        case S_GETREG_B32         : // : D.u = hardware register. Read some or all of a hardware register
        case S_MOV_B32             ://: D.u = S0.u.
        case S_CMOV_B32            ://: if(SCC) D.u = S0.u; else NOP.
        case S_NOT_B32             ://: D.u = ~S0.u SCC = 1 if result non-zero.
        case S_WQM_B32             ://: D.u = WholeQuadMode(S0.u). SCC = 1 if result is non-zero.
        case S_BREV_B32            ://: D.u = S0.u[0:31] (reverse bits).
        case S_BCNT0_I32_B32       ://: D.i = CountZeroBits(S0.u). SCC = 1 if result is non-zero.
        case S_BCNT1_I32_B32       ://: D.i = CountOneBits(S0.u). SCC = 1 if result is non-zero.
        case S_FF0_I32_B32         ://: D.i = FindFirstZero(S0.u) from LSB; if no zeros, return -1.
        case S_FF1_I32_B32         ://: D.i = FindFirstOne(S0.u) from LSB; if no ones, return -1.
        case S_FLBIT_I32_B32       ://: D.i = FindFirstOne(S0.u) from MSB; if no ones, return -1.
        case S_FLBIT_I32           ://: D.i = Find first bit opposite of sign bit from MSB. If S0 == -1, return -1.
        case S_SEXT_I32_I8         ://: D.i = signext(S0.i[7:0]).
        case S_SEXT_I32_I16        ://: D.i = signext(S0.i[15:0]).
        case S_BITSET0_B32         ://: D.u[S0.u[4:0]] = 0.
        case S_BITSET1_B32         ://: D.u[S0.u[4:0]] = 1.
        case S_QUADMASK_B32        ://: D.u = QuadMask(S0.u). D[0] = OR(S0[3:0]), D[1] = OR(S0[7:4]) .... SCC = 1 if result is non-zero.
        case S_MOVRELS_B32         ://: SGPR[D.u] = SGPR[S0.u + M0.u].
        case S_MOVRELD_B32         ://: SGPR[D.u + M0.u] = SGPR[S0.u].
        case S_ABS_I32             :      //: D.i = abs(S0.i). SCC=1 if result is non-zero.
        case S_MOV_FED_B32         :      //: D.u = S0.u, introduce edc double error upon write to dest sgpr.  
        case S_CMP_EQ_I32          :     //: SCC = (S0.i == S1.i).
        case S_CMP_LG_I32          :     //: SCC = (S0.i != S1.i).
        case S_CMP_GT_I32          :     //: SCC = (S0.i > S1.i).
        case S_CMP_GE_I32          :     //: SCC = (S0.i >= S1.i).
        case S_CMP_LT_I32          :     //: SCC = (S0.i < S1.i).
        case S_CMP_LE_I32          :     //: SCC = (S0.i <= S1.i).
        case S_CMP_EQ_U32          :     //: SCC = (S0.u == S1.u).
        case S_CMP_LG_U32          :     //: SCC = (S0.u != S1.u).
        case S_CMP_GT_U32          :     //: SCC = (S0.u > S1.u).
        case S_CMP_GE_U32          :     //: SCC = (S0.u >= S1.u).
        case S_CMP_LT_U32          :     //: SCC = (S0.u < S1.u).
        case S_CMP_LE_U32          :     //: SCC = (S0.u <= S1.u).
        case S_BITCMP0_B32         :     //: SCC = (S0.u[S1.u[4:0]] == 0).
        case S_BITCMP1_B32         :     //: SCC = (S0.u[S1.u[4:0]] == 1).
        case S_SETVSKIP            :     //: VSKIP = S0.u[S1.u[4:0]].
        case S_SETREG_B32          : // : hardware register = D.u. Write some or all of the LSBs of D
        case S_SETREG_IMM32_B32    : //: This instruction uses a 32-bit literal constant. Write
        case S_SET_GPR_IDX_IDX     : 
        case S_SET_GPR_IDX_ON:
            return 1;

        case S_MOVRELS_B64         ://: SGPR[D.u] = SGPR[S0.u + M0.u].
        case S_MOVRELD_B64         ://: SGPR[D.u + M0.u] = SGPR[S0.u].
        case S_BITCMP0_B64         :     //: SCC = (S0.u[S1.u[5:0]] == 0).
        case S_BITCMP1_B64         :     //: SCC = (S0.u[S1.u[5:0]] == 1).
        case S_BCNT0_I32_B64       ://: D.i = CountZeroBits(S0.u). SCC = 1 if result is non-zero.
        case S_BCNT1_I32_B64       ://: D.i = CountOneBits(S0.u). SCC = 1 if result is non-zero.
        case S_FF0_I32_B64         ://: D.i = FindFirstZero(S0.u) from LSB; if no zeros, return -1.
        case S_FF1_I32_B64         ://: D.i = FindFirstOne(S0.u) from LSB; if no ones, return -1.
        case S_FLBIT_I32_B64       ://: D.i = FindFirstOne(S0.u) from MSB; if no ones, return -1.
        case S_FLBIT_I32_I64       ://: D.i = Find first bit opposite of sign bit from MSB. If S0 == -1, return -1.
        case S_AND_B64               :   //: D.u = S0.u & S1.u. SCC = 1 if result is non-zero.
        case S_OR_B64                :   //: D.u = S0.u | S1.u. SCC = 1 if result is non-zero.
        case S_XOR_B64               :   //: D.u = S0.u ^ S1.u. SCC = 1 if result is non-zero.
        case S_ANDN2_B64             :   //: D.u = S0.u & ~S1.u. SCC = 1 if result is non-zero.
        case S_ORN2_B64              :   //: D.u = S0.u | ~S1.u. SCC = 1 if result is non-zero.
        case S_NAND_B64              :   //: D.u = ~(S0.u & S1.u). SCC = 1 if result is non-zero.
        case S_NOR_B64               :   //: D.u = ~(S0.u | S1.u). SCC = 1 if result is non-zero.
        case S_XNOR_B64              :   //: D.u = ~(S0.u ^ S1.u). SCC = 1 if result is non-zero.
        case S_LSHL_B64              :   //: D.u = S0.u << S1.u[5:0]. SCC = 1 if result is non-zero.
        case S_LSHR_B64              :   //: D.u = S0.u >> S1.u[5:0]. SCC = 1 if result is non-zero.
        case S_ASHR_I64              :   //: D.i = signtext(S0.i) >> S1.i[5:0]. SCC = 1 if result is non-zero.
        case S_BFM_B64               :   //: D.u = ((1 << S0.u[5:0]) - 1) << S1.u[5:0]; bitfield mask.
        case S_BFE_U64               :
        case S_BFE_I64               :
        case S_MOV_B64               ://: D/u = S0.u.
        case S_CMOV_B64              ://: if(SCC) D.u = S0.u; else NOP.
        case S_NOT_B64               ://: D.u = ~S0.u SCC = 1 if result non-zero.
        case S_WQM_B64               ://: D.u = WholeQuadMode(S0.u). SCC = 1 if result is non-zero.
        case S_BREV_B64              ://: D.u = S0.u[0:63] (reverse bits).
        case S_BITSET0_B64           ://: D.u[S0.u[5:0]] = 0.
        case S_BITSET1_B64           ://: D.u[S0.u[5:0]] = 1.
        case S_GETPC_B64             ://: D.u = PC + 4; destination receives the byte address of the next instruction.
        case S_SWAPPC_B64            ://: D.u = PC + 4; PC = S0.u.
        case S_AND_SAVEEXEC_B64      ://: D.u = EXEC, EXEC = S0.u & EXEC. SCC = 1 if the new value of EXEC is non-zero.
        case S_OR_SAVEEXEC_B64       ://: D.u = EXEC, EXEC = S0.u | EXEC. SCC = 1 if the newvalue of EXEC is non-zero.
        case S_XOR_SAVEEXEC_B64      ://: D.u = EXEC, EXEC = S0.u ^ EXEC. SCC = 1 if the new value of EXEC is non-zero.
        case S_ANDN2_SAVEEXEC_B64    ://: D.u = EXEC, EXEC = S0.u & ~EXEC. SCC =1 if the new value of EXEC is non-zero.
        case S_ORN2_SAVEEXEC_B64     ://: D.u = EXEC, EXEC = S0.u | ~EXEC. SCC = 1 if the new value of EXEC is non-zero.
        case S_NAND_SAVEEXEC_B64     ://: D.u = EXEC, EXEC = ~(S0.u & EXEC). SCC =1 if the new value of EXEC is non-zero.
        case S_NOR_SAVEEXEC_B64      ://: D.u = EXEC, EXEC = ~(S0.u | EXEC). SCC = 1 if the new value of EXEC is non-zero.
        case S_XNOR_SAVEEXEC_B64     ://: D.u = EXEC, EXEC = ~(S0.u ^ EXEC). SCC = 1 if the new value of EXEC is non-zero.
        case S_QUADMASK_B64          ://: D.u = QuadMask(S0.u). D[0] = OR(S0[3:0]),D[1] = OR(S0[7:4]) .... SCC = 1 if result is non-zero
        case S_CSELECT_B64           :   //: D.u = SCC ? S0.u : S1.u.
        case S_CBRANCH_G_FORK        : // Conditional branch using branch stack. Arg0 = compare mask (VCC or any SGPR), Arg1 = 64-bit byte address of target instruction.
        case S_CBRANCH_I_FORK        : //: Conditional branch using branch-stack.
        case S_CBRANCH_JOIN             ://: Conditional branch join point. Arg0 = saved CSP value. No dest.
        case S_RFE_B64                  ://: Return from Exception; PC = TTMP1,0.
        case S_SETPC_B64                ://: PC = S0.u; S0.u is a byte address of the instruction to jump to.
        case S_RFE_RESTORE_B64     :
        case S_CMP_EQ_U64:
        case S_CMP_NE_U64:
            return 2;

        case S_NOP                      :  //: do nothing. Repeat NOP 1..8 times based on SIMM16[2:0]. 0 = 1 time, 7 = 8 times.
        case S_ENDPGM                   :  //: end of program; terminate wavefront.
        case S_BRANCH                   :  //: PC = PC + signext(SIMM16 * 4) + 4.
        case S_CBRANCH_SCC0             :  //: if(SCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        case S_CBRANCH_SCC1             :  //: if(SCC == 1) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        case S_CBRANCH_VCCZ             :  //: if(VCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        case S_CBRANCH_VCCNZ            :  //: if(VCC != 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        case S_CBRANCH_EXECZ            :  //: if(EXEC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        case S_CBRANCH_EXECNZ           :  //: if(EXEC != 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        case S_BARRIER                  :  //: Sync waves within a thread group.                              
        case S_WAITCNT                  :  //: Wait for count of outstanding lds, vector-memory and
        case S_SETHALT                  :  //: set HALT bit to value of SIMM16[0]. 1=halt, 0=resume. Halt is ignored while priv=1.
        case S_SLEEP                    :  //: Cause a wave to sleep for approximately 64*SIMM16[2:0] clocks.
        case S_SETPRIO                  :  //: User settable wave priority. 0 = lowest, 3 = highest.
        case S_SENDMSG                  :  //: Send a message.
        case S_SENDMSGHALT              :  //: Send a message and then HALT.
        case S_TRAP                     :  //: Enter the trap handler. TrapID = SIMM16[7:0]. Wait for all instructions to complete, 
        case S_ICACHE_INV               :  //: Invalidate entire L1 I cache.
        case S_INCPERFLEVEL             :  //: Increment performance counter specified in SIMM16[3:0] by 1.
        case S_DECPERFLEVEL             :  //: Decrement performance counter specified in SIMM16[3:0] by 1.
        case S_TTRACEDATA               :  //: Send M0 as user data to thread-trace.
        case S_CBRANCH_CDBGSYS          :  // : If (conditional_debug_system != 0) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
        case S_CBRANCH_CDBGUSER         :  // : If (conditional_debug_user != 0) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
        case S_CBRANCH_CDBGSYS_OR_USER  :  // : If (conditional_debug_system || conditional_debug_user) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
        case S_CBRANCH_CDBGSYS_AND_USER :  // : If (conditional_debug_system && conditional_debug_user) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
            return 0;
        }
    }

    uint ScalarInstruction::GetArg1WidthInDWORDs() const
    {
        switch( GetOpcode() )
        {
        default:
            return 0;
        case S_ADD_U32               :  //: D.u = S0.u + S1.u. SCC = carry out.
        case S_SUB_U32               :  //: D.u = S0.u - S1.u. SCC = carry out.
        case S_ADD_I32               :  //: D.u = S0.i + S1.i. SCC = overflow.
        case S_SUB_I32               :  //: D.u = S0.i - S1.i. SCC = overflow.
        case S_ADDC_U32              :  //: D.u = S0.u + S1.u + SCC. SCC = carry-out.
        case S_SUBB_U32              :  //: D.u = S0.u - S1.u - SCC. SCC = carry-out.
        case S_MIN_I32               :  //: D.i = (S0.i < S1.i) ? S0.i : S1.i. SCC = 1 if S0 is min.
        case S_MIN_U32               :  //: D.u = (S0.u < S1.u) ? S0.u : S1.u. SCC = 1 if S0 is min.
        case S_MAX_I32               :  //: D.i = (S0.i > S1.i) ? S0.i : S1.i. SCC = 1 if S0 is max.
        case S_MAX_U32               :  //: D.u = (S0.u > S1.u) ? S0.u : S1.u. SCC = 1 if S0 is max.
        case S_AND_B32               :  //: D.u = S0.u & S1.u. SCC = 1 if result is non-zero.
        case S_CSELECT_B32           :  //: D.u = SCC ? S0.u : S1.u.
        case S_OR_B32                :  //: D.u = S0.u | S1.u. SCC = 1 if result is non-zero.
        case S_XOR_B32               :  //: D.u = S0.u ^ S1.u. SCC = 1 if result is non-zero.
        case S_ANDN2_B32             :  //: D.u = S0.u & ~S1.u. SCC = 1 if result is non-zero.
        case S_ORN2_B32              :  //: D.u = S0.u | ~S1.u. SCC = 1 if result is non-zero.
        case S_NAND_B32              :  //: D.u = ~(S0.u & S1.u). SCC = 1 if result is non-zero.
        case S_NOR_B32               :  //: D.u = ~(S0.u | S1.u). SCC = 1 if result is non-zero.
        case S_XNOR_B32              :  //: D.u = ~(S0.u ^ S1.u). SCC = 1 if result is non-zero.
        case S_LSHL_B32              :  //: D.u = S0.u << S1.u[4:0]. SCC = 1 if result is non-zero.
        case S_LSHR_B32              :  //: D.u = S0.u >> S1.u[4:0]. SCC = 1 if result is non-zero.
        case S_ASHR_I32              :  //: D.i = signtext(S0.i) >> S1.i[4:0]. SCC = 1 if result is non-zero.
        case S_BFM_B32               :  //: D.u = ((1 << S0.u[4:0]) - 1) << S1.u[4:0]; bitfield mask.
        case S_LSHL_B64              :  //: D.u = S0.u << S1.u[5:0]. SCC = 1 if result is non-zero.
        case S_LSHR_B64              :  //: D.u = S0.u >> S1.u[5:0]. SCC = 1 if result is non-zero.
        case S_ASHR_I64              :  //: D.i = signtext(S0.i) >> S1.i[5:0]. SCC = 1 if result is non-zero.
        case S_BFM_B64               :  //: D.u = ((1 << S0.u[5:0]) - 1) << S1.u[5:0]; bitfield mask.
        case S_BFE_U32               :   // : Bit field extract. S0 is data, S1[4:0] is field offset, S1[22:16] is field width. D.u = (S0.u >> S1.u[4:0]) & ((1 << S1.u[22:16]) - 1). SCC = 1 if resultis non-zero.
        case S_BFE_I32               :
        case S_BFE_U64               :
        case S_BFE_I64               :
        case S_ABSDIFF_I32           : //  D.i = abs(S0.i >> S1.i). SCC = 1 if result is non-zero.
        case S_MUL_I32               :  //: D.i = S0.i * S1.i.
        case S_CMP_EQ_I32            :    //: SCC = (S0.i == S1.i).
        case S_CMP_LG_I32            :    //: SCC = (S0.i != S1.i).
        case S_CMP_GT_I32            :    //: SCC = (S0.i > S1.i).
        case S_CMP_GE_I32            :    //: SCC = (S0.i >= S1.i).
        case S_CMP_LT_I32            :    //: SCC = (S0.i < S1.i).
        case S_CMP_LE_I32            :    //: SCC = (S0.i <= S1.i).
        case S_CMP_EQ_U32            :    //: SCC = (S0.u == S1.u).
        case S_CMP_LG_U32            :    //: SCC = (S0.u != S1.u).
        case S_CMP_GT_U32            :    //: SCC = (S0.u > S1.u).
        case S_CMP_GE_U32            :    //: SCC = (S0.u >= S1.u).
        case S_CMP_LT_U32            :    //: SCC = (S0.u < S1.u).
        case S_CMP_LE_U32            :    //: SCC = (S0.u <= S1.u).
        case S_BITCMP0_B32           :    //: SCC = (S0.u[S1.u[4:0]] == 0).
        case S_BITCMP1_B32           :    //: SCC = (S0.u[S1.u[4:0]] == 1).
        case S_BITCMP0_B64           :    //: SCC = (S0.u[S1.u[5:0]] == 0).
        case S_BITCMP1_B64           :    //: SCC = (S0.u[S1.u[5:0]] == 1).
        case S_SETVSKIP              :    //: VSKIP = S0.u[S1.u[4:0]].
        case S_RFE_RESTORE_B64:
        case S_SET_GPR_IDX_ON:
            return 1;
             
        case S_CSELECT_B64    :   //: D.u = SCC ? S0.u : S1.u.       
        case S_AND_B64        :   //: D.u = S0.u & S1.u. SCC = 1 if result is non-zero.
        case S_OR_B64         :   //: D.u = S0.u | S1.u. SCC = 1 if result is non-zero.
        case S_XOR_B64        :   //: D.u = S0.u ^ S1.u. SCC = 1 if result is non-zero.
        case S_ANDN2_B64      :   //: D.u = S0.u & ~S1.u. SCC = 1 if result is non-zero.
        case S_ORN2_B64       :   //: D.u = S0.u | ~S1.u. SCC = 1 if result is non-zero.
        case S_NAND_B64       :   //: D.u = ~(S0.u & S1.u). SCC = 1 if result is non-zero.
        case S_NOR_B64        :   //: D.u = ~(S0.u | S1.u). SCC = 1 if result is non-zero.
        case S_XNOR_B64       :   //: D.u = ~(S0.u ^ S1.u). SCC = 1 if result is non-zero.
        case S_CBRANCH_G_FORK : // Conditional branch using branch stack. Arg0 = compare mask (VCC or any SGPR), Arg1 = 64-bit byte address of target instruction.
        case S_CMP_EQ_U64:
        case S_CMP_NE_U64:
            return 2;
        }
    }



    uint GetResultWidthInDWORDs(BufferInstructions e) 
    {
        switch( e )
        {
        case BUFFER_LOAD_FORMAT_X            :
        case BUFFER_STORE_FORMAT_X           :
        case BUFFER_LOAD_UBYTE               :
        case BUFFER_LOAD_SBYTE               :
        case BUFFER_LOAD_USHORT              :
        case BUFFER_LOAD_SSHORT              :
        case BUFFER_LOAD_DWORD               :
        case BUFFER_STORE_BYTE               :
        case BUFFER_STORE_SHORT              :
        case BUFFER_STORE_DWORD              :
            return 1;

        case BUFFER_STORE_FORMAT_XY          :
        case BUFFER_LOAD_FORMAT_XY           :
        case BUFFER_LOAD_DWORDX2             :
        case BUFFER_STORE_DWORDX2            :
            return 2;

        case BUFFER_STORE_FORMAT_XYZ         :
        case BUFFER_LOAD_FORMAT_XYZ          :
        case BUFFER_LOAD_DWORDX3             :
        case BUFFER_STORE_DWORDX3            :
            return 3;
        case BUFFER_STORE_FORMAT_XYZW        :
        case BUFFER_LOAD_FORMAT_XYZW         :            
        case BUFFER_LOAD_DWORDX4             :
        case BUFFER_STORE_DWORDX4            :
            return 4;

        case BUFFER_ATOMIC_SWAP              :
        case BUFFER_ATOMIC_CMPSWAP           :
        case BUFFER_ATOMIC_ADD               :
        case BUFFER_ATOMIC_SUB               :
        case BUFFER_ATOMIC_RSUB              :
        case BUFFER_ATOMIC_SMIN              :
        case BUFFER_ATOMIC_UMIN              :
        case BUFFER_ATOMIC_SMAX              :
        case BUFFER_ATOMIC_UMAX              :
        case BUFFER_ATOMIC_AND               :
        case BUFFER_ATOMIC_OR                :
        case BUFFER_ATOMIC_XOR               :
        case BUFFER_ATOMIC_INC               :
        case BUFFER_ATOMIC_DEC               :
        case BUFFER_ATOMIC_FCMPSWAP          :
        case BUFFER_ATOMIC_FMIN              :
        case BUFFER_ATOMIC_FMAX              :
            return 1;

        case BUFFER_ATOMIC_SWAP_X2           :
        case BUFFER_ATOMIC_CMPSWAP_X2        :
        case BUFFER_ATOMIC_ADD_X2            :
        case BUFFER_ATOMIC_SUB_X2            :
        case BUFFER_ATOMIC_RSUB_X2           :
        case BUFFER_ATOMIC_SMIN_X2           :
        case BUFFER_ATOMIC_UMIN_X2           :
        case BUFFER_ATOMIC_SMAX_X2           :
        case BUFFER_ATOMIC_UMAX_X2           :
        case BUFFER_ATOMIC_AND_X2            :
        case BUFFER_ATOMIC_OR_X2             :
        case BUFFER_ATOMIC_XOR_X2            :
        case BUFFER_ATOMIC_INC_X2            :
        case BUFFER_ATOMIC_DEC_X2            :
        case BUFFER_ATOMIC_FCMPSWAP_X2       :
        case BUFFER_ATOMIC_FMIN_X2           :
        case BUFFER_ATOMIC_FMAX_X2           :
            return 2;
        case TBUFFER_LOAD_FORMAT_X      :
        case TBUFFER_STORE_FORMAT_X     :
        case TBUFFER_LOAD_FORMAT_D16_X  :    
        case TBUFFER_STORE_FORMAT_D16_X :
            return 1;
        case TBUFFER_LOAD_FORMAT_XY     :
        case TBUFFER_STORE_FORMAT_XY    :
        case TBUFFER_LOAD_FORMAT_D16_XY :
        case TBUFFER_STORE_FORMAT_D16_XY:  
            return 2;
        case TBUFFER_LOAD_FORMAT_XYZ    :
        case TBUFFER_STORE_FORMAT_XYZ   :
        case TBUFFER_LOAD_FORMAT_D16_XYZ :  
        case TBUFFER_STORE_FORMAT_D16_XYZ:
            return 3;
        case TBUFFER_LOAD_FORMAT_XYZW   :
        case TBUFFER_STORE_FORMAT_XYZW  :
        case TBUFFER_LOAD_FORMAT_D16_XYZW :
        case TBUFFER_STORE_FORMAT_D16_XYZW: 
            return 4;

        case BUFFER_STORE_LDS_DWORD        :     
        case BUFFER_LOAD_FORMAT_D16_X      : 
        case BUFFER_STORE_FORMAT_D16_X     : 
            return 1;
        case BUFFER_LOAD_FORMAT_D16_XY     : 
        case BUFFER_STORE_FORMAT_D16_XY    : 
            return 2;
        case BUFFER_LOAD_FORMAT_D16_XYZ    : 
        case BUFFER_STORE_FORMAT_D16_XYZ   : 
            return 3;
        case BUFFER_LOAD_FORMAT_D16_XYZW   : 
        case BUFFER_STORE_FORMAT_D16_XYZW  : 
            return 4;

        default:
        case BUFFER_WBINVL1_VOL      :
        case BUFFER_WBINVL1_SC               :
        case BUFFER_WBINVL1                  :
            return 0;
        }
    }


    bool IsTBufferInstruction( BufferInstructions e )
    {
        switch(e)
        {
        case TBUFFER_LOAD_FORMAT_X      :
        case TBUFFER_STORE_FORMAT_X     :
        case TBUFFER_LOAD_FORMAT_XY     :
        case TBUFFER_STORE_FORMAT_XY    :
        case TBUFFER_LOAD_FORMAT_XYZ    :
        case TBUFFER_STORE_FORMAT_XYZ   :
        case TBUFFER_LOAD_FORMAT_XYZW   :
        case TBUFFER_STORE_FORMAT_XYZW  :
        case TBUFFER_LOAD_FORMAT_D16_X     :  
        case TBUFFER_LOAD_FORMAT_D16_XY    :
        case TBUFFER_LOAD_FORMAT_D16_XYZ   :
        case TBUFFER_LOAD_FORMAT_D16_XYZW  :
        case TBUFFER_STORE_FORMAT_D16_X    :
        case TBUFFER_STORE_FORMAT_D16_XY   :
        case TBUFFER_STORE_FORMAT_D16_XYZ  :
        case TBUFFER_STORE_FORMAT_D16_XYZW :
            return true;
        default:
            return false;
        }
    }



    bool ScalarInstruction::IsControlFlowOp() const
    {
        switch( GetOpcode() )
        {
        case S_CBRANCH_G_FORK              :
        case S_CBRANCH_I_FORK              :
        case S_SETPC_B64                   :
        case S_SWAPPC_B64                  :
        case S_RFE_B64                     :
        case S_CBRANCH_JOIN                :
        case S_ENDPGM                      :
        case S_BRANCH                      :
        case S_CBRANCH_SCC0                :
        case S_CBRANCH_SCC1                :
        case S_CBRANCH_VCCZ                :
        case S_CBRANCH_VCCNZ               :
        case S_CBRANCH_EXECZ               :
        case S_CBRANCH_EXECNZ              :
        case S_BARRIER                     :
        case S_SETHALT                     :
        case S_SENDMSGHALT                 :
        case S_TRAP                        :               
        case S_CBRANCH_CDBGSYS             :
        case S_CBRANCH_CDBGUSER            :
        case S_CBRANCH_CDBGSYS_OR_USER     :
        case S_CBRANCH_CDBGSYS_AND_USER    :
        case S_RFE_RESTORE_B64:
        case S_ENDPGM_SAVED:
            return true;
        default:
            return false;
        }
    }

    bool ScalarInstruction::IsConditionalJump() const
    {
        switch( GetOpcode() )
        {
        case S_CBRANCH_G_FORK              :
        case S_CBRANCH_I_FORK              :
        case S_CBRANCH_JOIN                :
        case S_CBRANCH_SCC0                :
        case S_CBRANCH_SCC1                :
        case S_CBRANCH_VCCZ                :
        case S_CBRANCH_VCCNZ               :
        case S_CBRANCH_EXECZ               :
        case S_CBRANCH_EXECNZ              :
        case S_CBRANCH_CDBGSYS             :
        case S_CBRANCH_CDBGUSER            :
        case S_CBRANCH_CDBGSYS_OR_USER     :
        case S_CBRANCH_CDBGSYS_AND_USER    :
            return true;

        default:
            return false;
        }
    }

    bool ScalarInstruction::IsUnconditionalJump() const
    {
        switch( GetOpcode() )
        {
        case S_SETPC_B64                   :
        case S_SWAPPC_B64                  :
        case S_RFE_B64                     :
        case S_TRAP                        :               
        case S_RFE_RESTORE_B64             :
        case S_BRANCH                      :
            return true;

        default:
            return false;
        }
    }

    bool ScalarInstruction::IsWavefrontHalt() const
    {
        switch( GetOpcode() )
        {
        case S_ENDPGM                      :
        case S_SETHALT                     :
        case S_SENDMSGHALT                 :
            return true;
        default:
            return false;
        }
    }


    uint ScalarMemoryInstruction::GetResourceWidthInDWORDs() const
    {
        switch( GetOpcode() )
        {
        case S_LOAD_DWORD          : 
        case S_LOAD_DWORDX2        : 
        case S_LOAD_DWORDX4        : 
        case S_LOAD_DWORDX8        : 
        case S_LOAD_DWORDX16       : 
        case S_STORE_DWORD         : 
        case S_STORE_DWORDX2       : 
        case S_STORE_DWORDX4       : 
        case S_ATC_PROBE           : 
            return 2;
        case S_BUFFER_LOAD_DWORD   : 
        case S_BUFFER_LOAD_DWORDX2 : 
        case S_BUFFER_LOAD_DWORDX4 : 
        case S_BUFFER_LOAD_DWORDX8 : 
        case S_BUFFER_LOAD_DWORDX16: 
        case S_BUFFER_STORE_DWORD  : 
        case S_BUFFER_STORE_DWORDX2: 
        case S_BUFFER_STORE_DWORDX4: 
        case S_ATC_PROBE_BUFFER    : 
            return 4;
        default:
            return 0;
        }
    }


    uint ScalarMemoryInstruction::GetResultWidthInDWORDs() const
    {
        switch( GetOpcode() )
        {
            case S_LOAD_DWORD          :    return 1;
            case S_LOAD_DWORDX2        :    return 2;
            case S_LOAD_DWORDX4        :    return 4;
            case S_LOAD_DWORDX8        :    return 8;
            case S_LOAD_DWORDX16       :    return 16;
            case S_BUFFER_LOAD_DWORD   :    return 1;
            case S_BUFFER_LOAD_DWORDX2 :    return 2;
            case S_BUFFER_LOAD_DWORDX4 :    return 4;
            case S_BUFFER_LOAD_DWORDX8 :    return 8;
            case S_BUFFER_LOAD_DWORDX16:    return 16;            
            case S_MEMTIME             :    return 2;      
            case S_STORE_DWORD         :    return 1;
            case S_STORE_DWORDX2       :    return 2;
            case S_STORE_DWORDX4       :    return 4;
            case S_BUFFER_STORE_DWORD  :    return 1;
            case S_BUFFER_STORE_DWORDX2:    return 2;
            case S_BUFFER_STORE_DWORDX4:    return 4;
            case S_MEMREALTIME         :    return 2;
            case S_ATC_PROBE           :    
            case S_ATC_PROBE_BUFFER    :    
            case S_DCACHE_WB           :     
            case S_DCACHE_WB_VOL       :     
            case S_DCACHE_INV_VOL      : 
            case S_DCACHE_INV          :   
            default:
                return 0;
        }
    }

   
    uint GetSamplerWidthInDWORDs( ImageInstructions e )
    {
        switch( e )
        {
        case IMAGE_LOAD               :
        case IMAGE_LOAD_MIP           :
        case IMAGE_LOAD_PCK           :
        case IMAGE_LOAD_PCK_SGN       :
        case IMAGE_LOAD_MIP_PCK       :
        case IMAGE_LOAD_MIP_PCK_SGN   :
        case IMAGE_STORE              :
        case IMAGE_STORE_MIP          :
        case IMAGE_STORE_PCK          :
        case IMAGE_STORE_MIP_PCK      :
        case IMAGE_GET_RESINFO        :
        case IMAGE_ATOMIC_SWAP        :
        case IMAGE_ATOMIC_CMPSWAP     :
        case IMAGE_ATOMIC_ADD         :
        case IMAGE_ATOMIC_SUB         :
        case IMAGE_ATOMIC_RSUB        :
        case IMAGE_ATOMIC_SMIN        :
        case IMAGE_ATOMIC_UMIN        :
        case IMAGE_ATOMIC_SMAX        :
        case IMAGE_ATOMIC_UMAX        :
        case IMAGE_ATOMIC_AND         :
        case IMAGE_ATOMIC_OR          :
        case IMAGE_ATOMIC_XOR         :
        case IMAGE_ATOMIC_INC         :
        case IMAGE_ATOMIC_DEC         :
        case IMAGE_ATOMIC_FCMPSWAP    :
        case IMAGE_ATOMIC_FMIN        :
        case IMAGE_ATOMIC_FMAX        :
            return 0;
        default:
            return 4;
        }
    }

    uint ImageInstruction::GetResultWidthInDWORDS() const
    {
        switch( GetOpcode() )
        {
            // Gathers always return four results, regardless of DMask
            //  The ISA doc does not indicate this
            //
        case IMAGE_GATHER4           :
        case IMAGE_GATHER4_CL        :
        case IMAGE_GATHER4_L         :
        case IMAGE_GATHER4_B         :
        case IMAGE_GATHER4_B_CL      :
        case IMAGE_GATHER4_LZ        :
        case IMAGE_GATHER4_C         :
        case IMAGE_GATHER4_C_CL      :
        case IMAGE_GATHER4_C_L       :
        case IMAGE_GATHER4_C_B       :
        case IMAGE_GATHER4_C_B_CL    :
        case IMAGE_GATHER4_C_LZ      :
        case IMAGE_GATHER4_O         :
        case IMAGE_GATHER4_CL_O      :
        case IMAGE_GATHER4_L_O       :
        case IMAGE_GATHER4_B_O       :
        case IMAGE_GATHER4_B_CL_O    :
        case IMAGE_GATHER4_LZ_O      :
        case IMAGE_GATHER4_C_O       :
        case IMAGE_GATHER4_C_CL_O    :
        case IMAGE_GATHER4_C_L_O     :
        case IMAGE_GATHER4_C_B_O     :
        case IMAGE_GATHER4_C_B_CL_O  :
        case IMAGE_GATHER4_C_LZ_O    : 
            return 4;
        default:
            {
                // result width is the number of set bits in the dmask field
                uint n = GetDMask(); 
                uint c=0;
                while( n )
                {
                    c += (n&1);
                    n>>=1;
                }
                return c;
            }
            break;

        }
    }


    bool ImageInstruction::IsFilteredFetch( ) const
    {
        switch( GetOpcode() )
        {
        case IMAGE_SAMPLE          :   //: sample texture map.
        case IMAGE_SAMPLE_CL       :   //: sample texture map, with LOD clamp specified in shader.
        case IMAGE_SAMPLE_D        :   //: sample texture map, with user derivatives.
        case IMAGE_SAMPLE_D_CL     :   //: sample texture map, with LOD clamp specified in shader, with user derivatives.
        case IMAGE_SAMPLE_L        :   //: sample texture map, with user LOD.
        case IMAGE_SAMPLE_B        :   //: sample texture map, with lod bias.
        case IMAGE_SAMPLE_B_CL     :   //: sample texture map, with LOD clamp specified in shader, with lod bias.
        case IMAGE_SAMPLE_LZ       :   //: sample texture map, from level 0.
        case IMAGE_SAMPLE_C        :   //: sample texture map, with PCF.
        case IMAGE_SAMPLE_C_CL     :   //: SAMPLE_C, with LOD clamp specified in shader.
        case IMAGE_SAMPLE_C_D      :   //: SAMPLE_C, with user derivatives.
        case IMAGE_SAMPLE_C_D_CL   :   //: SAMPLE_C, with LOD clamp specified in shader,with user derivatives.
        case IMAGE_SAMPLE_C_L      :   //: SAMPLE_C, with user LOD.
        case IMAGE_SAMPLE_C_B      :   //: SAMPLE_C, with lod bias.
        case IMAGE_SAMPLE_C_B_CL   :   //: SAMPLE_C, with LOD clamp specified in shader, with lod bias.
        case IMAGE_SAMPLE_C_LZ     : //     : SAMPLE_C, from level 0.
        case IMAGE_SAMPLE_O        : // : sample texture map, with user offsets.
        case IMAGE_SAMPLE_CL_O     : //     : SAMPLE_O with LOD clamp specified in shader.
        case IMAGE_SAMPLE_D_O      : // : SAMPLE_O, with user derivatives.
        case IMAGE_SAMPLE_D_CL_O   : //     : SAMPLE_O, with LOD clamp specified in shader,with user derivatives.
        case IMAGE_SAMPLE_L_O      : // : SAMPLE_O, with user LOD.
        case IMAGE_SAMPLE_B_O      : // : SAMPLE_O, with lod bias.
        case IMAGE_SAMPLE_B_CL_O   : // : SAMPLE_O, with LOD clamp specified in shader,with lod bias.
        case IMAGE_SAMPLE_LZ_O     : // : SAMPLE_O, from level 0.
        case IMAGE_SAMPLE_C_O      : // : SAMPLE_C with user specified offsets.
        case IMAGE_SAMPLE_C_CL_O   : // : SAMPLE_C_O, with LOD clamp specified in shader.
        case IMAGE_SAMPLE_C_D_O    : // : SAMPLE_C_O, with user derivatives.
        case IMAGE_SAMPLE_C_D_CL_O : // : SAMPLE_C_O, with LOD clamp specified in shader, with user derivatives.
        case IMAGE_SAMPLE_C_L_O    : // : SAMPLE_C_O, with user LOD.
        case IMAGE_SAMPLE_C_B_O    : // : SAMPLE_C_O, with lod bias.
        case IMAGE_SAMPLE_C_B_CL_O : // : SAMPLE_C_O, with LOD clamp specified in shader, with lod bias.
        case IMAGE_SAMPLE_C_LZ_O   : // : SAMPLE_C_O, from level 0.
        case IMAGE_SAMPLE_CD        : // : sample texture map, with user derivatives (LOD per quad)
        case IMAGE_SAMPLE_CD_CL     : // : sample texture map, with LOD clamp specified in  shader, with user derivatives (LOD per quad).
        case IMAGE_SAMPLE_C_CD      : // : SAMPLE_C, with user derivatives (LOD per quad).
        case IMAGE_SAMPLE_C_CD_CL   : // : SAMPLE_C, with LOD clamp specified in shader, with user derivatives (LOD per quad).
        case IMAGE_SAMPLE_CD_O      : // : SAMPLE_O, with user derivatives (LOD per quad).
        case IMAGE_SAMPLE_CD_CL_O   : // : SAMPLE_O, with LOD clamp specified in shader, with user derivatives (LOD per quad).
        case IMAGE_SAMPLE_C_CD_O    : // : SAMPLE_C_O, with user derivatives (LOD per quad).
        case IMAGE_SAMPLE_C_CD_CL_O : 
            return true;
        default:
            return false;
        }
    }


    bool ImageInstruction::IsGradientFetch( ) const
    {
        switch( GetOpcode() )
        {
        case IMAGE_SAMPLE_D        :   //: sample texture map, with user derivatives.
        case IMAGE_SAMPLE_D_CL     :   //: sample texture map, with LOD clamp specified in shader, with user derivatives.
        case IMAGE_SAMPLE_C_D      :   //: SAMPLE_C, with user derivatives.
        case IMAGE_SAMPLE_C_D_CL   :   //: SAMPLE_C, with LOD clamp specified in shader,with user derivatives.
        case IMAGE_SAMPLE_D_O      : // : SAMPLE_O, with user derivatives.
        case IMAGE_SAMPLE_D_CL_O   : //     : SAMPLE_O, with LOD clamp specified in shader,with user derivatives.
        case IMAGE_SAMPLE_C_D_O    : // : SAMPLE_C_O, with user derivatives.
        case IMAGE_SAMPLE_C_D_CL_O : // : SAMPLE_C_O, with LOD clamp specified in shader, with user derivatives.
        case IMAGE_SAMPLE_CD        : // : sample texture map, with user derivatives (LOD per quad)
        case IMAGE_SAMPLE_CD_CL     : // : sample texture map, with LOD clamp specified in  shader, with user derivatives (LOD per quad).
        case IMAGE_SAMPLE_C_CD      : // : SAMPLE_C, with user derivatives (LOD per quad).
        case IMAGE_SAMPLE_C_CD_CL   : // : SAMPLE_C, with LOD clamp specified in shader, with user derivatives (LOD per quad).
        case IMAGE_SAMPLE_CD_O      : // : SAMPLE_O, with user derivatives (LOD per quad).
        case IMAGE_SAMPLE_CD_CL_O   : // : SAMPLE_O, with LOD clamp specified in shader, with user derivatives (LOD per quad).
        case IMAGE_SAMPLE_C_CD_O    : // : SAMPLE_C_O, with user derivatives (LOD per quad).
        case IMAGE_SAMPLE_C_CD_CL_O : 
            return true;
        default:
            return false;
        }
    }

    bool ImageInstruction::IsGather() const
    {
        switch( GetOpcode() )
        {
        case IMAGE_GATHER4         :// : gather 4 single component elements (2x2).
        case IMAGE_GATHER4_CL      :// : gather 4 single component elements (2x2) with user LOD clamp.
        case IMAGE_GATHER4_L       :// : gather 4 single component elements (2x2) with user LOD.
        case IMAGE_GATHER4_B       :// : gather 4 single component elements (2x2) with user bias.
        case IMAGE_GATHER4_B_CL    :// : gather 4 single component elements (2x2) with user bias and clamp.
        case IMAGE_GATHER4_LZ      :// : gather 4 single component elements (2x2) at level 0.
        case IMAGE_GATHER4_C       :// : gather 4 single component elements (2x2) with PCF.
        case IMAGE_GATHER4_C_CL    :// : gather 4 single component elements (2x2) with user LOD clamp and PCF.
        case IMAGE_GATHER4_C_L     :// : gather 4 single component elements (2x2) with user LOD and PCF.
        case IMAGE_GATHER4_C_B     :// : gather 4 single component elements (2x2) with user bias and PCF.
        case IMAGE_GATHER4_C_B_CL  :// : gather 4 single component elements (2x2) with user bias, clamp and PCF.
        case IMAGE_GATHER4_C_LZ    :// : gather 4 single component elements (2x2) at level 0, with PCF.
        case IMAGE_GATHER4_O       :// : GATHER4, with user offsets.
        case IMAGE_GATHER4_CL_O    :// : GATHER4_CL, with user offsets.
        case IMAGE_GATHER4_L_O     :// : GATHER4_L, with user offsets.
        case IMAGE_GATHER4_B_O     :// : GATHER4_B, with user offsets.
        case IMAGE_GATHER4_B_CL_O  :// : GATHER4_B_CL, with user offsets.
        case IMAGE_GATHER4_LZ_O    :// : GATHER4_LZ, with user offsets.
        case IMAGE_GATHER4_C_O     :// : GATHER4_C, with user offsets.
        case IMAGE_GATHER4_C_CL_O  :// : GATHER4_C_CL, with user offsets.
        case IMAGE_GATHER4_C_L_O   :// : GATHER4_C_L, with user offsets.
        case IMAGE_GATHER4_C_B_O   :// : GATHER4_B, with user offsets.
        case IMAGE_GATHER4_C_B_CL_O:// : GATHER4_B_CL, with user offsets.
        case IMAGE_GATHER4_C_LZ_O  :// : GATHER4_C_LZ, with user offsets.
            return true;
        default:
            return false;
        }
    }

    bool ImageInstruction::IsUnfilteredLoadStore() const
    {
        switch( GetOpcode() )
        {
        case IMAGE_LOAD            :  // Image memory load with format conversion specified in T#. No sampler.
        case IMAGE_LOAD_MIP        :  // Image memory load with user-supplied mip level. No sampler.
        case IMAGE_LOAD_PCK        :  // Image memory load with no format conversion. No sampler.
        case IMAGE_LOAD_PCK_SGN    :  // Image memory load with with no format conversion and sign extension. No sampler.
        case IMAGE_LOAD_MIP_PCK    :  // Image memory load with user-supplied mip level, no format conversion. No sampler.
        case IMAGE_LOAD_MIP_PCK_SGN:  // Image memory load with user-supplied mip level, no format conversion and with sign extension. No sampler.
        case IMAGE_STORE           :  //: Image memory store with format conversion specified in T#. No sampler.
        case IMAGE_STORE_MIP       :  //: Image memory store with format conversion specified in T# to user specified mip level. No sampler.
        case IMAGE_STORE_PCK       :  //: Image memory store of packed data without format conversion. No sampler.
        case IMAGE_STORE_MIP_PCK   : //: Image memory store of packed data without format conversion to user-supplied mip level. No sampler.
            return true;
        default:
            return false;
        }
    }


    bool ImageInstruction::IsMemoryWrite() const
    {
        switch( GetOpcode() )
        {
        case IMAGE_STORE          :   //: Image memory store with format conversion specified in T#. No sampler.
        case IMAGE_STORE_MIP      :   //: Image memory store with format conversion specified in T# to user specified mip level. No sampler.
        case IMAGE_STORE_PCK      :   //: Image memory store of packed data without format conversion. No sampler.
        case IMAGE_STORE_MIP_PCK  :  //: Image memory store of packed data without format conversion to user-supplied mip level. No sampler.
        case IMAGE_ATOMIC_SWAP    :   //: dst=src, returns previous value if glc==1.
        case IMAGE_ATOMIC_CMPSWAP :   //: dst = (dst==cmp) ? src : dst. Returns previous value if glc==1.
        case IMAGE_ATOMIC_ADD     :   //: dst += src. Returns previous value if glc==1.
        case IMAGE_ATOMIC_SUB     :   //: dst -= src. Returns previous value if glc==1.
        case IMAGE_ATOMIC_RSUB    :   //: dst = src-dst. Returns previous value if glc==1.  SI ONLY
        case IMAGE_ATOMIC_SMIN    :   //: dst = (src < dst) ? src : dst (signed). Returns previous value if glc==1.
        case IMAGE_ATOMIC_UMIN    :   //: dst = (src < dst) ? src : dst (unsigned). Returns previous value if glc==1.
        case IMAGE_ATOMIC_SMAX    :   //: dst = (src > dst) ? src : dst (signed). Returns previous value if glc==1.
        case IMAGE_ATOMIC_UMAX    :   //: dst = (src > dst) ? src : dst (unsigned). Returns previous value if glc==1.
        case IMAGE_ATOMIC_AND     :   //: dst &= src. Returns previous value if glc==1.
        case IMAGE_ATOMIC_OR      :   //: dst |= src. Returns previous value if glc==1.
        case IMAGE_ATOMIC_XOR     :   //: dst ^= src. Returns previous value if glc==1.
        case IMAGE_ATOMIC_INC     :   //: dst = (dst >= src) ? 0 : dst+1. Returns previous value if glc==1.
        case IMAGE_ATOMIC_DEC     :   //: dst = ((dst==0 || (dst > src)) ? src : dst-1. Returns previous value if glc==1.
        case IMAGE_ATOMIC_FCMPSWAP:   //: dst = (dst == cmp) ? src : dst, returns previous value of dst if glc==1 - double and float atomic compare swap. Obeys floating point compare rules for special values.
        case IMAGE_ATOMIC_FMIN    :   //: dst = (src < dst) ? src : dst, returns previous value of dst if glc==1 - double and float atomic min (handles NaN/INF/denorm).
        case IMAGE_ATOMIC_FMAX    :   //: dst = (src > dst) ? src : dst, returns previous value of dst if glc==1 - double and float atomic min (handles NaN/INF/denorm).
            return true;
        }
        return false;
    }

    bool BufferInstruction::IsMemoryWrite() const
    {
        switch( GetOpcode() )
        {
        case BUFFER_STORE_FORMAT_X       :
        case BUFFER_STORE_FORMAT_XY      :
        case BUFFER_STORE_FORMAT_XYZ     :
        case BUFFER_STORE_FORMAT_XYZW    :
        case BUFFER_STORE_BYTE           :
        case BUFFER_STORE_SHORT          :
        case BUFFER_STORE_DWORD          :
        case BUFFER_STORE_DWORDX2        :
        case BUFFER_STORE_DWORDX4        :
        case BUFFER_STORE_DWORDX3        :
        case BUFFER_ATOMIC_SWAP          :
        case BUFFER_ATOMIC_CMPSWAP       :
        case BUFFER_ATOMIC_ADD           :
        case BUFFER_ATOMIC_SUB           :
        case BUFFER_ATOMIC_RSUB          :
        case BUFFER_ATOMIC_SMIN          :
        case BUFFER_ATOMIC_UMIN          :
        case BUFFER_ATOMIC_SMAX          :
        case BUFFER_ATOMIC_UMAX          :
        case BUFFER_ATOMIC_AND           :
        case BUFFER_ATOMIC_OR            :
        case BUFFER_ATOMIC_XOR           :
        case BUFFER_ATOMIC_INC           :
        case BUFFER_ATOMIC_DEC           :
        case BUFFER_ATOMIC_FCMPSWAP      :
        case BUFFER_ATOMIC_FMIN          :
        case BUFFER_ATOMIC_FMAX          :
        case BUFFER_ATOMIC_SWAP_X2       :
        case BUFFER_ATOMIC_CMPSWAP_X2    :
        case BUFFER_ATOMIC_ADD_X2        :
        case BUFFER_ATOMIC_SUB_X2        :
        case BUFFER_ATOMIC_RSUB_X2       :
        case BUFFER_ATOMIC_SMIN_X2       :
        case BUFFER_ATOMIC_UMIN_X2       :
        case BUFFER_ATOMIC_SMAX_X2       :
        case BUFFER_ATOMIC_UMAX_X2       :
        case BUFFER_ATOMIC_AND_X2        :
        case BUFFER_ATOMIC_OR_X2         :
        case BUFFER_ATOMIC_XOR_X2        :
        case BUFFER_ATOMIC_INC_X2        :
        case BUFFER_ATOMIC_DEC_X2        :
        case BUFFER_ATOMIC_FCMPSWAP_X2   :
        case BUFFER_ATOMIC_FMIN_X2       :
        case BUFFER_ATOMIC_FMAX_X2       :
        case BUFFER_STORE_LDS_DWORD      :   
        case BUFFER_STORE_FORMAT_D16_X   :   
        case BUFFER_STORE_FORMAT_D16_XY  :   
        case BUFFER_STORE_FORMAT_D16_XYZ :   
        case BUFFER_STORE_FORMAT_D16_XYZW:   
        case TBUFFER_STORE_FORMAT_X            :
        case TBUFFER_STORE_FORMAT_XY           :
        case TBUFFER_STORE_FORMAT_XYZ          :
        case TBUFFER_STORE_FORMAT_XYZW         :
        case TBUFFER_STORE_FORMAT_D16_X        :
        case TBUFFER_STORE_FORMAT_D16_XY       :
        case TBUFFER_STORE_FORMAT_D16_XYZ      :
        case TBUFFER_STORE_FORMAT_D16_XYZW     :
            return true;
        }
        return false;
    }

}