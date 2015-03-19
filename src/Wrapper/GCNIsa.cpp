#pragma unmanaged
#include "GCNIsa.h"

namespace GCN
{
    InstructionFormat Instruction::GetFormat() const
    {
        uint32 n = *((const uint32*)this);

        //
        // The instruction format is inferred from magic numbers packed into the upper bits of the first DWORD
        //   vop2    is    0.......
        //   vop1    is    0111 111
        //   vopc    is    0111 110
        //   sop2    is    10......
        //   sopK    is    1011 .......
        //   sopc    is    1011 1111 0
        //   sopp    is    1011 1111 1
        //   sop1    is    1011 1110 1
        //   vinterp is    1100 10
        //   smrd    is    1100 0  (for GCN3 SMEM its: 1100 00)
        //   vop3    is    1101 00  
        //   flat    is    1101 11
        //   ds      is    1101 10
        //   mubuff  is    1110 00
        //   mtbuff  is    1110 10
        //   mimg    is    1111 00
        //   exp     is    1111 10


        switch( n & (0xFF800000) )       // 1111 1111 1000 ... (9 bit)
        {
        case 0xBF000000: return IF_SOPC; // 1011 1111 0....
        case 0xBF800000: return IF_SOPP; // 1011 1111 1....
        case 0xBE800000: return IF_SOP1; // 1011 1110 1....
        default:
            switch( n & (0xFE000000) )       // 1111 1110 ... (7 bit)
            {
            case 0x7E000000: return IF_VOP1; // 0111 1110 ...
            case 0x7C000000: return IF_VOPC; // 0111 1100 ...
            default:
                switch( n & (0xFC000000) )          // 1111 1100 ... (6 bit)
                {
                case 0xC8000000: return IF_VINTERP; // 1100 10.. ....
                case 0xD0000000: return IF_VOP3;    // 1101 00.. ...
                case 0xDC000000: return IF_FLAT;    // 1101 11.. ....
                case 0xD8000000: return IF_DS;      // 1101 10.. ....
                case 0xE0000000: return IF_MUBUFF;  // 1110 00..
                case 0xE8000000: return IF_MTBUFF;  // 1110 10..
                case 0xF0000000: return IF_MIMG;    // 1111 00..
                case 0xF8000000: return IF_EXP;     // 1111 10..
                default:
                    switch( n & (0xF8000000) ) // 1111 1000 (5bit)
                    {
                    case 0xC0000000: return IF_SMEM; // 1100 0... ....
                    case 0xB0000000: return IF_SOPK; // 1011 0.........
                    case 0xB8000000: return IF_SOPK; // 1011 1.........
                    default:
                        if( (n & 0xC0000000) == 0x80000000 ) // 1100....
                            return IF_SOP2; // 1000 ....
                        if( !(n & 0x80000000) )
                            return IF_VOP2; // 0......
                    }
                }
            }
        }

        return IF_UNKNOWN;       
    }


    uint Instruction::GetLength() const
    {
        switch( GetFormat() )
        {
        case IF_VOP2:       return ((const VOP2Instruction*)this)->GetLength();
        case IF_VOP1:       return ((const VOP1Instruction*)this)->GetLength();
        case IF_VOPC:       return ((const VOPCInstruction*)this)->GetLength();
        case IF_SOP2:       return ((const SOP2Instruction*)this)->GetLength();
        case IF_SOPK:       return ((const SOPKInstruction*)this)->GetLength();
        case IF_SOP1:       return ((const SOP1Instruction*)this)->GetLength();
        case IF_SOPC:       return ((const SOPCInstruction*)this)->GetLength();
        case IF_SOPP:       return ((const SOPPInstruction*)this)->GetLength();
        case IF_SMEM:       return ((const SMEMInstruction*)this)->GetLength();
        case IF_VOP3:       return ((const VOP3Instruction*)this)->GetLength();
        case IF_VINTERP:    return ((const VINTERPInstruction*)this)->GetLength();
        case IF_DS:         return ((const DSInstruction*)this)->GetLength();
        case IF_MTBUFF:     return ((const MTBUFFInstruction*)this)->GetLength();
        case IF_MUBUFF:     return ((const MUBUFFInstruction*)this)->GetLength();
        case IF_MIMG:       return ((const MIMGInstruction*)this)->GetLength();
        case IF_EXP:        return ((const EXPInstruction*)this)->GetLength();
        case IF_FLAT:       return ((const FLATInstruction*)this)->GetLength();
        case IF_UNKNOWN:    
        default:
            return 0;
        }
    }

    bool Instruction::IsBranch() const
    {
        switch( GetFormat() )
        {
        case IF_VOP2:       return ((const VOP2Instruction*)this)->IsBranch();
        case IF_VOP1:       return ((const VOP1Instruction*)this)->IsBranch();
        case IF_VOPC:       return ((const VOPCInstruction*)this)->IsBranch();
        case IF_SOP2:       return ((const SOP2Instruction*)this)->IsBranch();
        case IF_SOPK:       return ((const SOPKInstruction*)this)->IsBranch();
        case IF_SOP1:       return ((const SOP1Instruction*)this)->IsBranch();
        case IF_SOPC:       return ((const SOPCInstruction*)this)->IsBranch();
        case IF_SOPP:       return ((const SOPPInstruction*)this)->IsBranch();
        case IF_SMEM:       return ((const SMEMInstruction*)this)->IsBranch();
        case IF_VOP3:       return ((const VOP3Instruction*)this)->IsBranch();
        case IF_VINTERP:    return ((const VINTERPInstruction*)this)->IsBranch();
        case IF_DS:         return ((const DSInstruction*)this)->IsBranch();
        case IF_MTBUFF:     return ((const MTBUFFInstruction*)this)->IsBranch();
        case IF_MUBUFF:     return ((const MUBUFFInstruction*)this)->IsBranch();
        case IF_MIMG:       return ((const MIMGInstruction*)this)->IsBranch();
        case IF_EXP:        return ((const EXPInstruction*)this)->IsBranch();
        case IF_FLAT:       return ((const FLATInstruction*)this)->IsBranch();
        case IF_UNKNOWN:    
        default:
            return false;
        }
    }

    const Instruction* Instruction::GetBranchTarget() const
    {
        switch( GetFormat() )
        {
        case IF_VOP2:       return ((const VOP2Instruction*)this)->GetBranchTarget();
        case IF_VOP1:       return ((const VOP1Instruction*)this)->GetBranchTarget();
        case IF_VOPC:       return ((const VOPCInstruction*)this)->GetBranchTarget();
        case IF_SOP2:       return ((const SOP2Instruction*)this)->GetBranchTarget();
        case IF_SOPK:       return ((const SOPKInstruction*)this)->GetBranchTarget();
        case IF_SOP1:       return ((const SOP1Instruction*)this)->GetBranchTarget();
        case IF_SOPC:       return ((const SOPCInstruction*)this)->GetBranchTarget();
        case IF_SOPP:       return ((const SOPPInstruction*)this)->GetBranchTarget();
        case IF_SMEM:       return ((const SMEMInstruction*)this)->GetBranchTarget();
        case IF_VOP3:       return ((const VOP3Instruction*)this)->GetBranchTarget();
        case IF_VINTERP:    return ((const VINTERPInstruction*)this)->GetBranchTarget();
        case IF_DS:         return ((const DSInstruction*)this)->GetBranchTarget();
        case IF_MTBUFF:     return ((const MTBUFFInstruction*)this)->GetBranchTarget();
        case IF_MUBUFF:     return ((const MUBUFFInstruction*)this)->GetBranchTarget();
        case IF_MIMG:       return ((const MIMGInstruction*)this)->GetBranchTarget();
        case IF_EXP:        return ((const EXPInstruction*)this)->GetBranchTarget();
        case IF_FLAT:       return ((const FLATInstruction*)this)->GetBranchTarget();
        case IF_UNKNOWN:    
        default:
            return false;
        }
    }


    uint SOP2Instruction::GetResultWidthInDWORDS() const
    {
        switch( GetOpcode() )
        {
        case S_ADD_U32           : 
        case S_SUB_U32           : 
        case S_ADD_I32           : 
        case S_SUB_I32           : 
        case S_ADDC_U32          : 
        case S_SUBB_U32          : 
        case S_MIN_I32           : 
        case S_MIN_U32           : 
        case S_MAX_I32           : 
        case S_MAX_U32           : 
        case S_CSELECT_B32       :
        case S_AND_B32           : 
        case S_OR_B32            : 
        case S_XOR_B32           : 
        case S_ANDN2_B32         : 
        case S_ORN2_B32          : 
        case S_NAND_B32          : 
        case S_NOR_B32           : 
        case S_XNOR_B32          : 
        case S_LSHL_B32          : 
        case S_LSHR_B32          : 
        case S_ASHR_I32          : 
        case S_BFM_B32           : 
        case S_MUL_I32           : 
        case S_BFE_U32           : 
        case S_BFE_I32           : 
        case S_ABSDIFF_I32       : 
            return 1;

        case S_CSELECT_B64      : 
        case S_AND_B64          : 
        case S_OR_B64           : 
        case S_XOR_B64          : 
        case S_ANDN2_B64        : 
        case S_ORN2_B64         : 
        case S_NAND_B64         : 
        case S_NOR_B64          : 
        case S_XNOR_B64         : 
        case S_LSHL_B64         : 
        case S_LSHR_B64         : 
        case S_ASHR_I64         : 
        case S_BFM_B64          : 
        case S_BFE_U64          : 
        case S_BFE_I64          : 
            return 2;
        default:
        case S_CBRANCH_G_FORK   : 
            return 0;
        }
    }

    uint SOP1Instruction::GetResultWidthInDWORDS() const
    {
        switch( GetOpcode() )
        {                          
        case S_MOV_B32             : 
        case S_CMOV_B32            : 
        case S_NOT_B32             : 
        case S_WQM_B32             : 
        case S_BREV_B32            : 
        case S_BCNT0_I32_B32       : 
        case S_BCNT0_I32_B64       : 
        case S_BCNT1_I32_B32       : 
        case S_BCNT1_I32_B64       : 
        case S_FF0_I32_B32         : 
        case S_FF0_I32_B64         : 
        case S_FF1_I32_B32         : 
        case S_FF1_I32_B64         : 
        case S_FLBIT_I32_B32       : 
        case S_FLBIT_I32_B64       : 
        case S_FLBIT_I32           : 
        case S_FLBIT_I32_I64       : 
        case S_SEXT_I32_I8         : 
        case S_SEXT_I32_I16        : 
        case S_BITSET0_B32         : 
        case S_BITSET1_B32         : 
        case S_ABS_I32             : 
        case S_MOV_FED_B32         : 
        case S_MOVRELS_B32         : 
        case S_MOVRELD_B32         :
            return 1;
        case S_MOV_B64               :
        case S_CMOV_B64              :
        case S_NOT_B64               :
        case S_WQM_B64               :
        case S_BREV_B64              :
        case S_BITSET0_B64           :
        case S_BITSET1_B64           :
        case S_GETPC_B64             :
        case S_SETPC_B64             :
        case S_SWAPPC_B64            :
        case S_RFE_B64               :
        case S_AND_SAVEEXEC_B64      :
        case S_OR_SAVEEXEC_B64       :
        case S_XOR_SAVEEXEC_B64      :
        case S_ANDN2_SAVEEXEC_B64    :
        case S_ORN2_SAVEEXEC_B64     :
        case S_NAND_SAVEEXEC_B64     :
        case S_NOR_SAVEEXEC_B64      :
        case S_XNOR_SAVEEXEC_B64     :
        case S_MOVRELS_B64           :
        case S_MOVRELD_B64           :                                                             
            return 2;

        case S_QUADMASK_B64          :
            return 16;
        case S_QUADMASK_B32        :
            return 8;
        
        case S_CBRANCH_JOIN:
        default:
            return 0;
        }
    }
    uint SOP1Instruction::GetOperandWidthInDWORDS() const
    {
        switch( GetOpcode() )
        {                          
        case S_MOV_B32             : 
        case S_CMOV_B32            : 
        case S_NOT_B32             : 
        case S_WQM_B32             : 
        case S_BREV_B32            : 
        case S_BCNT0_I32_B32       : 
        case S_BCNT1_I32_B32       : 
        case S_FF0_I32_B32         : 
        case S_FF1_I32_B32         : 
        case S_FLBIT_I32_B32       : 
        case S_FLBIT_I32           : 
        case S_SEXT_I32_I8         : 
        case S_SEXT_I32_I16        : 
        case S_BITSET0_B32         : 
        case S_BITSET1_B32         : 
        case S_QUADMASK_B32        : 
        case S_ABS_I32             : 
        case S_MOV_FED_B32         : 
        case S_MOVRELS_B32         : 
        case S_MOVRELD_B32         :
        case S_MOVRELS_B64           :
        case S_MOVRELD_B64           :  
        
            return 1;

        case S_MOV_B64               :
        case S_CMOV_B64              :
        case S_NOT_B64               :
        case S_WQM_B64               :
        case S_BREV_B64              :
        case S_BITSET0_B64           :
        case S_BITSET1_B64           :
        case S_GETPC_B64             :
        case S_SETPC_B64             :
        case S_SWAPPC_B64            :
        case S_RFE_B64               :
        case S_AND_SAVEEXEC_B64      :
        case S_OR_SAVEEXEC_B64       :
        case S_XOR_SAVEEXEC_B64      :
        case S_ANDN2_SAVEEXEC_B64    :
        case S_ORN2_SAVEEXEC_B64     :
        case S_NAND_SAVEEXEC_B64     :
        case S_NOR_SAVEEXEC_B64      :
        case S_XNOR_SAVEEXEC_B64     :
        case S_QUADMASK_B64          :
        case S_CBRANCH_JOIN:
        case S_BCNT0_I32_B64       : 
        case S_BCNT1_I32_B64       : 
        case S_FF0_I32_B64         : 
        case S_FF1_I32_B64         : 
        case S_FLBIT_I32_B64       : 
        case S_FLBIT_I32_I64       : 
            return 2;
        default:
            return 0;
        }
    }

    uint SMEMInstruction::GetResultWidthInDWORDs() const
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
            case S_DCACHE_INV_VOL      : 
            case S_DCACHE_INV          :   
            default:
                return 0;
        }
    }

    uint VOP1Instruction::GetLength() const
    {
        switch( GetOpcode())
        {
        default:
            return 4 + 4*(GetSrc0() == LITERAL);
        case V_NOP:
        case V_CLREXCP:             
            return 4;
        }
    }

    uint VOP1Instruction::GetOperandWidthInDWORDS() const
    {
        switch( GetOpcode())
        {
        default:
        case V_NOP:
        case V_CLREXCP:             
            return 0;
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
        }
    }

    uint VOP1Instruction::GetResultWidthInDWORDS() const
    {
        switch( GetOpcode())
        {
        default:
        case V_NOP:
        case V_CLREXCP:             
            return 0;
        case V_CVT_F64_I32      :       
        case V_CVT_F64_F32      :
        case V_CVT_F64_U32      :      
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
            return 2;

        case V_MOV_B32           :      
        case V_READFIRSTLANE_B32 :      
        case V_CVT_I32_F64       :      
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
        case V_CVT_F32_F64       :      
        case V_CVT_F32_UBYTE0    :      
        case V_CVT_F32_UBYTE1    :      
        case V_CVT_F32_UBYTE2    :      
        case V_CVT_F32_UBYTE3    :      
        case V_CVT_U32_F64       :      
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
        case V_FREXP_EXP_I32_F64 :      
        case V_FREXP_EXP_I32_F32 :                          
        case V_FREXP_MANT_F32    :      
        case V_MOVRELD_B32       :      
        case V_MOVRELS_B32       :      
        case V_MOVRELSD_B32      : 
        case V_LOG_LEGACY_F32    :      
        case V_EXP_LEGACY_F32    :
            return 1;
        }
    }

    uint VOPCInstruction::GetOperandWidthInDWORDs() const
    {
        switch(GetOpcode() )
        {
        default: return 0;
        case V_CMP_F_F32       :
        case V_CMP_LT_F32      :
        case V_CMP_EQ_F32      :
        case V_CMP_LE_F32      :
        case V_CMP_GT_F32      :
        case V_CMP_LG_F32      :
        case V_CMP_GE_F32      :
        case V_CMP_O_F32       :
        case V_CMP_U_F32       :
        case V_CMP_NGE_F32     :
        case V_CMP_NLG_F32     :
        case V_CMP_NGT_F32     :
        case V_CMP_NLE_F32     :
        case V_CMP_NEQ_F32     :
        case V_CMP_NLT_F32     :
        case V_CMP_TRU_F32     :
        case V_CMPX_F_F32      :
        case V_CMPX_LT_F32     :
        case V_CMPX_EQ_F32     :
        case V_CMPX_LE_F32     :
        case V_CMPX_GT_F32     :
        case V_CMPX_LG_F32     :
        case V_CMPX_GE_F32     :
        case V_CMPX_O_F32      :
        case V_CMPX_U_F32      :
        case V_CMPX_NGE_F32    :
        case V_CMPX_NLG_F32    :
        case V_CMPX_NGT_F32    :
        case V_CMPX_NLE_F32    :
        case V_CMPX_NEQ_F32    :
        case V_CMPX_NLT_F32    :
        case V_CMPX_TRU_F32    :
        case V_CMPS_F_F32      :
        case V_CMPS_LT_F32     :
        case V_CMPS_EQ_F32     :
        case V_CMPS_LE_F32     :
        case V_CMPS_GT_F32     :
        case V_CMPS_LG_F32     :
        case V_CMPS_GE_F32     :
        case V_CMPS_O_F32      :
        case V_CMPS_U_F32      :
        case V_CMPS_NGE_F32    :
        case V_CMPS_NLG_F32    :
        case V_CMPS_NGT_F32    :
        case V_CMPS_NLE_F32    :
        case V_CMPS_NEQ_F32    :
        case V_CMPS_NLT_F32    :
        case V_CMPS_TRU_F32    :
        case V_CMPSX_F_F32     :
        case V_CMPSX_LT_F32    :
        case V_CMPSX_EQ_F32    :
        case V_CMPSX_LE_F32    :
        case V_CMPSX_GT_F32    :
        case V_CMPSX_LG_F32    :
        case V_CMPSX_GE_F32    :
        case V_CMPSX_O_F32     :
        case V_CMPSX_U_F32     :
        case V_CMPSX_NGE_F32   :
        case V_CMPSX_NLG_F32   :
        case V_CMPSX_NGT_F32   :
        case V_CMPSX_NLE_F32   :
        case V_CMPSX_NEQ_F32   :
        case V_CMPSX_NLT_F32   :
        case V_CMPSX_TRU_F32   :
        case V_CMP_F_I32       :
        case V_CMP_LT_I32      :
        case V_CMP_EQ_I32      :
        case V_CMP_LE_I32      :
        case V_CMP_GT_I32      :
        case V_CMP_LG_I32      :
        case V_CMP_GE_I32      :
        case V_CMP_TRU_I32     :
        case V_CMPX_F_I32      :
        case V_CMPX_LT_I32     :
        case V_CMPX_EQ_I32     :
        case V_CMPX_LE_I32     :
        case V_CMPX_GT_I32     :
        case V_CMPX_LG_I32     :
        case V_CMPX_GE_I32     :
        case V_CMPX_TRU_I32    :
        case V_CMP_F_U32       :
        case V_CMP_LT_U32      :
        case V_CMP_EQ_U32      :
        case V_CMP_LE_U32      :
        case V_CMP_GT_U32      :
        case V_CMP_LG_U32      :
        case V_CMP_GE_U32      :
        case V_CMP_TRU_U32     :
        case V_CMPX_F_U32      :
        case V_CMPX_LT_U32     :
        case V_CMPX_EQ_U32     :
        case V_CMPX_LE_U32     :
        case V_CMPX_GT_U32     :
        case V_CMPX_LG_U32     :
        case V_CMPX_GE_U32     :
        case V_CMPX_TRU_U32    :
        case V_CMP_CLASS_F32   :
        case V_CMPX_CLASS_F32  :
            return 1;
                
        case V_CMP_F_F64       :
        case V_CMP_LT_F64      :
        case V_CMP_EQ_F64      :
        case V_CMP_LE_F64      :
        case V_CMP_GT_F64      :
        case V_CMP_LG_F64      :
        case V_CMP_GE_F64      :
        case V_CMP_O_F64       :
        case V_CMP_U_F64       :
        case V_CMP_NGE_F64     :
        case V_CMP_NLG_F64     :
        case V_CMP_NGT_F64     :
        case V_CMP_NLE_F64     :
        case V_CMP_NEQ_F64     :
        case V_CMP_NLT_F64     :
        case V_CMP_TRU_F64     :
        case V_CMPX_F_F64      :
        case V_CMPX_LT_F64     :
        case V_CMPX_EQ_F64     :
        case V_CMPX_LE_F64     :
        case V_CMPX_GT_F64     :
        case V_CMPX_LG_F64     :
        case V_CMPX_GE_F64     :
        case V_CMPX_O_F64      :
        case V_CMPX_U_F64      :
        case V_CMPX_NGE_F64    :
        case V_CMPX_NLG_F64    :
        case V_CMPX_NGT_F64    :
        case V_CMPX_NLE_F64    :
        case V_CMPX_NEQ_F64    :
        case V_CMPX_NLT_F64    :
        case V_CMPX_TRU_F64    :
        case V_CMPS_F_F64      :
        case V_CMPS_LT_F64     :
        case V_CMPS_EQ_F64     :
        case V_CMPS_LE_F64     :
        case V_CMPS_GT_F64     :
        case V_CMPS_LG_F64     :
        case V_CMPS_GE_F64     :
        case V_CMPS_O_F64      :
        case V_CMPS_U_F64      :
        case V_CMPS_NGE_F64    :
        case V_CMPS_NLG_F64    :
        case V_CMPS_NGT_F64    :
        case V_CMPS_NLE_F64    :
        case V_CMPS_NEQ_F64    :
        case V_CMPS_NLT_F64    :
        case V_CMPS_TRU_F64    :
        case V_CMPSX_F_F64     :
        case V_CMPSX_LT_F64    :
        case V_CMPSX_EQ_F64    :
        case V_CMPSX_LE_F64    :
        case V_CMPSX_GT_F64    :
        case V_CMPSX_LG_F64    :
        case V_CMPSX_GE_F64    :
        case V_CMPSX_O_F64     :
        case V_CMPSX_U_F64     :
        case V_CMPSX_NGE_F64   :
        case V_CMPSX_NLG_F64   :
        case V_CMPSX_NGT_F64   :
        case V_CMPSX_NLE_F64   :
        case V_CMPSX_NEQ_F64   :
        case V_CMPSX_NLT_F64   :
        case V_CMPSX_TRU_F64   :
        case V_CMP_F_I64       :
        case V_CMP_LT_I64      :
        case V_CMP_EQ_I64      :
        case V_CMP_LE_I64      :
        case V_CMP_GT_I64      :
        case V_CMP_LG_I64      :
        case V_CMP_GE_I64      :
        case V_CMP_TRU_I64     :
        case V_CMPX_F_I64      :
        case V_CMPX_LT_I64     :
        case V_CMPX_EQ_I64     :
        case V_CMPX_LE_I64     :
        case V_CMPX_GT_I64     :
        case V_CMPX_LG_I64     :
        case V_CMPX_GE_I64     :
        case V_CMPX_TRU_I64    :
        case V_CMP_F_U64       :
        case V_CMP_LT_U64      :
        case V_CMP_EQ_U64      :
        case V_CMP_LE_U64      :
        case V_CMP_GT_U64      :
        case V_CMP_LG_U64      :
        case V_CMP_GE_U64      :
        case V_CMP_TRU_U64     :
        case V_CMPx_F_U64      :
        case V_CMPx_LT_U64     :
        case V_CMPx_EQ_U64     :
        case V_CMPx_LE_U64     :
        case V_CMPx_GT_U64     :
        case V_CMPx_LG_U64     :
        case V_CMPx_GE_U64     :
        case V_CMPx_TRU_U64    :
        case V_CMP_CLASS_F64   :
        case V_CMPX_CLASS_F64  :
            return 2;
        }
    }


    bool VOP3Instruction::IsVOP3bOp() const
    {
        // NOTE: ISA Doc, section 6, claims that vcmp is vop3b.  But AMD disassembler thinks its VOP3a
        //
        switch( GetOpcode() )
        {
        case V3_ADD_I32      :   //  : D.u = S0.u + S1.u; VCC=carry-out (VOP3:sgpr=carry-out).
        case V3_SUB_I32      :   //  : D.u = S0.u - S1.u; VCC=carry-out (VOP3:sgpr=carry-out).
        case V3_SUBREV_I32   :   //  : D.u = S1.u - S0.u; VCC=carry-out (VOP3:sgpr=carry-out).
        case V3_ADDC_U32     :   //  : D.u = S0.u + S1.u + VCC; VCC=carry-out (VOP3:sgpr=carryout,S2.u=carry-in).
        case V3_SUBB_U32     :   //  : D.u = S0.u - S1.u - VCC; VCC=carry-out (VOP3:sgpr=carry-out,S2.u=carry-in).
        case V3_SUBBREV_U32  :   //  : D.u = S1.u - S0.u - VCC; VCC=carry-out (VOP3:sgpr=carryout,S2.u=carry-in).
        case V3_DIV_SCALE_F32:   //  = D.f = Special case divide preop and flags(s0.f = Quotient,s1.f = Denominator, s2.f = Numerator) s0 must equal s1 or s2.
        case V3_DIV_SCALE_F64:   //  = D.d = Special case divide preop and flags(s0.d = Quotient,s1.d = Denominator, s2.d = Numerator) s0 must equal s1 or s2.
            return true;
        default:
            return false;
        }
    }

    uint VOP3Instruction::GetOperandSizeInDWORDS() const
    {
        switch( GetOpcode() )
        {
                   
        default:     
        case V3_NOP:
        case V3_CLREXCP:
            return 0;   

        case V3_CMP_F_F32         :
        case V3_CMP_LT_F32        :
        case V3_CMP_EQ_F32        :
        case V3_CMP_LE_F32        :
        case V3_CMP_GT_F32        :
        case V3_CMP_LG_F32        :
        case V3_CMP_GE_F32        :
        case V3_CMP_O_F32         :
        case V3_CMP_U_F32         :
        case V3_CMP_NGE_F32       :
        case V3_CMP_NLG_F32       :
        case V3_CMP_NGT_F32       :
        case V3_CMP_NLE_F32       :
        case V3_CMP_NEQ_F32       :
        case V3_CMP_NLT_F32       :
        case V3_CMP_TRU_F32       :
        case V3_CMPX_F_F32        :
        case V3_CMPX_LT_F32       :
        case V3_CMPX_EQ_F32       :
        case V3_CMPX_LE_F32       :
        case V3_CMPX_GT_F32       :
        case V3_CMPX_LG_F32       :
        case V3_CMPX_GE_F32       :
        case V3_CMPX_O_F32        :
        case V3_CMPX_U_F32        :
        case V3_CMPX_NGE_F32      :
        case V3_CMPX_NLG_F32      :
        case V3_CMPX_NGT_F32      :
        case V3_CMPX_NLE_F32      :
        case V3_CMPX_NEQ_F32      :
        case V3_CMPX_NLT_F32      :
        case V3_CMPX_TRU_F32      :
        case V3_CMPS_F_F32        :
        case V3_CMPS_LT_F32       :
        case V3_CMPS_EQ_F32       :
        case V3_CMPS_LE_F32       :
        case V3_CMPS_GT_F32       :
        case V3_CMPS_LG_F32       :
        case V3_CMPS_GE_F32       :
        case V3_CMPS_O_F32        :
        case V3_CMPS_U_F32        :
        case V3_CMPS_NGE_F32      :
        case V3_CMPS_NLG_F32      :
        case V3_CMPS_NGT_F32      :
        case V3_CMPS_NLE_F32      :
        case V3_CMPS_NEQ_F32      :
        case V3_CMPS_NLT_F32      :
        case V3_CMPS_TRU_F32      :
        case V3_CMPSX_F_F32       :
        case V3_CMPSX_LT_F32      :
        case V3_CMPSX_EQ_F32      :
        case V3_CMPSX_LE_F32      :
        case V3_CMPSX_GT_F32      :
        case V3_CMPSX_LG_F32      :
        case V3_CMPSX_GE_F32      :
        case V3_CMPSX_O_F32       :
        case V3_CMPSX_U_F32       :
        case V3_CMPSX_NGE_F32     :
        case V3_CMPSX_NLG_F32     :
        case V3_CMPSX_NGT_F32     :
        case V3_CMPSX_NLE_F32     :
        case V3_CMPSX_NEQ_F32     :
        case V3_CMPSX_NLT_F32     :
        case V3_CMPSX_TRU_F32     :
        case V3_CMP_F_I32         :
        case V3_CMP_LT_I32        :
        case V3_CMP_EQ_I32        :
        case V3_CMP_LE_I32        :
        case V3_CMP_GT_I32        :
        case V3_CMP_LG_I32        :
        case V3_CMP_GE_I32        :
        case V3_CMP_TRU_I32       :
        case V3_CMPX_F_I32        :
        case V3_CMPX_LT_I32       :
        case V3_CMPX_EQ_I32       :
        case V3_CMPX_LE_I32       :
        case V3_CMPX_GT_I32       :
        case V3_CMPX_LG_I32       :
        case V3_CMPX_GE_I32       :
        case V3_CMPX_TRU_I32      :
        case V3_CMP_F_U32         :
        case V3_CMP_LT_U32        :
        case V3_CMP_EQ_U32        :
        case V3_CMP_LE_U32        :
        case V3_CMP_GT_U32        :
        case V3_CMP_LG_U32        :
        case V3_CMP_GE_U32        :
        case V3_CMP_TRU_U32       :
        case V3_CMPX_F_U32        :
        case V3_CMPX_LT_U32       :
        case V3_CMPX_EQ_U32       :
        case V3_CMPX_LE_U32       :
        case V3_CMPX_GT_U32       :
        case V3_CMPX_LG_U32       :
        case V3_CMPX_GE_U32       :
        case V3_CMPX_TRU_U32      :
            return 1;

        case V3_CMP_F_F64         :
        case V3_CMP_LT_F64        :
        case V3_CMP_EQ_F64        :
        case V3_CMP_LE_F64        :
        case V3_CMP_GT_F64        :
        case V3_CMP_LG_F64        :
        case V3_CMP_GE_F64        :
        case V3_CMP_O_F64         :
        case V3_CMP_U_F64         :
        case V3_CMP_NGE_F64       :
        case V3_CMP_NLG_F64       :
        case V3_CMP_NGT_F64       :
        case V3_CMP_NLE_F64       :
        case V3_CMP_NEQ_F64       :
        case V3_CMP_NLT_F64       :
        case V3_CMP_TRU_F64       :
        case V3_CMPX_F_F64        :
        case V3_CMPX_LT_F64       :
        case V3_CMPX_EQ_F64       :
        case V3_CMPX_LE_F64       :
        case V3_CMPX_GT_F64       :
        case V3_CMPX_LG_F64       :
        case V3_CMPX_GE_F64       :
        case V3_CMPX_O_F64        :
        case V3_CMPX_U_F64        :
        case V3_CMPX_NGE_F64      :
        case V3_CMPX_NLG_F64      :
        case V3_CMPX_NGT_F64      :
        case V3_CMPX_NLE_F64      :
        case V3_CMPX_NEQ_F64      :
        case V3_CMPX_NLT_F64      :
        case V3_CMPX_TRU_F64      :
        case V3_CMPS_F_F64        :
        case V3_CMPS_LT_F64       :
        case V3_CMPS_EQ_F64       :
        case V3_CMPS_LE_F64       :
        case V3_CMPS_GT_F64       :
        case V3_CMPS_LG_F64       :
        case V3_CMPS_GE_F64       :
        case V3_CMPS_O_F64        :
        case V3_CMPS_U_F64        :
        case V3_CMPS_NGE_F64      :
        case V3_CMPS_NLG_F64      :
        case V3_CMPS_NGT_F64      :
        case V3_CMPS_NLE_F64      :
        case V3_CMPS_NEQ_F64      :
        case V3_CMPS_NLT_F64      :
        case V3_CMPS_TRU_F64      :
        case V3_CMPSX_F_F64       :
        case V3_CMPSX_LT_F64      :
        case V3_CMPSX_EQ_F64      :
        case V3_CMPSX_LE_F64      :
        case V3_CMPSX_GT_F64      :
        case V3_CMPSX_LG_F64      :
        case V3_CMPSX_GE_F64      :
        case V3_CMPSX_O_F64       :
        case V3_CMPSX_U_F64       :
        case V3_CMPSX_NGE_F64     :
        case V3_CMPSX_NLG_F64     :
        case V3_CMPSX_NGT_F64     :
        case V3_CMPSX_NLE_F64     :
        case V3_CMPSX_NEQ_F64     :
        case V3_CMPSX_NLT_F64     :
        case V3_CMPSX_TRU_F64     :
        case V3_CMP_F_I64         :
        case V3_CMP_LT_I64        :
        case V3_CMP_EQ_I64        :
        case V3_CMP_LE_I64        :
        case V3_CMP_GT_I64        :
        case V3_CMP_LG_I64        :
        case V3_CMP_GE_I64        :
        case V3_CMP_TRU_I64       :
        case V3_CMPX_F_I64        :
        case V3_CMPX_LT_I64       :
        case V3_CMPX_EQ_I64       :
        case V3_CMPX_LE_I64       :
        case V3_CMPX_GT_I64       :
        case V3_CMPX_LG_I64       :
        case V3_CMPX_GE_I64       :
        case V3_CMPX_TRU_I64      :
        case V3_CMP_F_U64         :
        case V3_CMP_LT_U64        :
        case V3_CMP_EQ_U64        :
        case V3_CMP_LE_U64        :
        case V3_CMP_GT_U64        :
        case V3_CMP_LG_U64        :
        case V3_CMP_GE_U64        :
        case V3_CMP_TRU_U64       :
        case V3_CMPx_F_U64        :
        case V3_CMPx_LT_U64       :
        case V3_CMPx_EQ_U64       :
        case V3_CMPx_LE_U64       :
        case V3_CMPx_GT_U64       :
        case V3_CMPx_LG_U64       :
        case V3_CMPx_GE_U64       :
        case V3_CMPx_TRU_U64      :
        case V3_CMP_CLASS_F64     :
        case V3_CMPX_CLASS_F64    :
            return 2;


        case V3_CMP_CLASS_F32     :
        case V3_CMPX_CLASS_F32    :
        case V3_CNDMASK_B32        :
        case V3_READLANE_B32       :
        case V3_WRITELANE_B32      :
        case V3_ADD_F32            :
        case V3_SUB_F32            :
        case V3_SUBREV_F32         :
        case V3_MAC_LEGACY_F32     :
        case V3_MUL_LEGACY_F32     :
        case V3_MUL_F32            :
        case V3_MUL_I32_I24        :
        case V3_MUL_HI_I32_I24     :
        case V3_MUL_U32_U24        :
        case V3_MUL_HI_U32_U24     :
        case V3_MIN_LEGACY_F32     :
        case V3_MAX_LEGACY_F32     :
        case V3_MIN_F32            :
        case V3_MAX_F32            :
        case V3_MIN_I32            :
        case V3_MAX_I32            :
        case V3_MIN_U32            :
        case V3_MAX_U32            :
        case V3_LSHR_B32           :
        case V3_LSHRREV_B32        :
        case V3_ASHR_I32           :
        case V3_ASHRREV_I32        :
        case V3_LSHL_B32           :
        case V3_LSHLREV_B32        :
        case V3_AND_B32            :
        case V3_OR_B32             :
        case V3_XOR_B32            :
        case V3_BFM_B32            :
        case V3_MAC_F32            :
        case V3_MADMK_F32          :
        case V3_MADAK_F32          :
        case V3_BCNT_U32_B32       :
        case V3_MBCNT_LO_U32_B32:                 
        case V3_MBCNT_HI_U32_B32 :
        case V3_ADD_I32              :
        case V3_SUB_I32              :
        case V3_SUBREV_I32           :
        case V3_ADDC_U32             :
        case V3_SUBB_U32             :
        case V3_SUBBREV_U32          :
        case V3_DIV_SCALE_F32        :
        case V3_LDEXP_F32            :
        case V3_CVT_PKACCUM_U8_F32   :
        case V3_CVT_PKNORM_I16_F32   :
        case V3_CVT_PKNORM_U16_F32   :
        case V3_CVT_PKRTZ_F16_F32    :
        case V3_CVT_PK_U16_U32       :
        case V3_CVT_PK_I16_I32       :   
        case V3_MAD_LEGACY_F32       :
        case V3_MAD_F32              :
        case V3_MAD_I32_I24          :
        case V3_MAD_U32_U24          :
        case V3_CUBEID_F32           :
        case V3_CUBESC_F32           :
        case V3_CUBETC_F32           :
        case V3_CUBEMA_F32           :
        case V3_BFE_U32              :
        case V3_BFE_I32              :
        case V3_BFI_B32              :
        case V3_FMA_F32              :
        case V3_FMA_F64              :
        case V3_LERP_U8              :
        case V3_ALIGNBIT_B32         :
        case V3_ALIGNBYTE_B32        :
        case V3_MULLIT_F32           :
        case V3_MIN3_F32             :
        case V3_MIN3_I32             :
        case V3_MIN3_U32             :
        case V3_MAX3_F32             :
        case V3_MAX3_I32             :
        case V3_MAX3_U32             :
        case V3_MED3_F32             :
        case V3_MED3_I32             :
        case V3_MED3_U32             :
        case V3_SAD_U8               :
        case V3_SAD_HI_U8            :
        case V3_SAD_U16              :
        case V3_SAD_U32              :
        case V3_CVT_PK_U8_F32        :
        case V3_DIV_FIXUP_F32        :
        case V3_MUL_LO_U32           :
        case V3_MUL_HI_U32           :
        case V3_MUL_LO_I32           :
        case V3_MUL_HI_I32           :
        case V3_DIV_FMAS_F32         :
        case V3_MSAD_U8              :
        case V3_QSAD_U8              :
        case V3_MQSAD_U8             :
        case V3_MQSAD_U32_U8   :   // : D.u128 = Masked Quad-Byte SAD with 32-bit accum_lo/hi(S0.u[63:0], S1.u[31:0], S2.u[127:0]).
        case V3_MAD_U64_U32    :   // : {vcc_out,D.u64} = S0.u32 * S1.u32 + S2.u64.
        case V3_MAD_I64_I32    :   // : {vcc_out,D.i64} = S0.i32 * S1.i32 + S2.i64.
        case V3_MOV_B32      :     
        case V3_READFIRSTLANE_B32 :
        case V3_CVT_F64_I32          :
        case V3_CVT_F32_I32          :
        case V3_CVT_F32_U32          :
        case V3_CVT_U32_F32          :
        case V3_CVT_I32_F32          :
        case V3_MOV_FED_B32          :
        case V3_CVT_F16_F32          :
        case V3_CVT_F32_F16          :
        case V3_CVT_RPI_I32_F32      :
        case V3_CVT_FLR_I32_F32      :
        case V3_CVT_OFF_F32_I4       :
        case V3_CVT_F64_F32          :
        case V3_CVT_F32_UBYTE0       :
        case V3_CVT_F32_UBYTE1       :
        case V3_CVT_F32_UBYTE2       :
        case V3_CVT_F32_UBYTE3       :
        case V3_CVT_F64_U32          :
        case V3_FRACT_F32            :
        case V3_TRUNC_F32            :
        case V3_CEIL_F32             :
        case V3_RNDNE_F32            :
        case V3_FLOOR_F32            :
        case V3_EXP_F32              :
        case V3_LOG_CLAMP_F32        :
        case V3_LOG_F32              :
        case V3_RCP_CLAMP_F32        :
        case V3_RCP_LEGACY_F32       :
        case V3_RCP_F32              :
        case V3_RCP_IFLAG_F32        :
        case V3_RSQ_CLAMP_F32        :
        case V3_RSQ_LEGACY_F32       :
        case V3_RSQ_F32              :
        case V3_SQRT_F32             :
        case V3_SIN_F32              :
        case V3_COS_F32              :
        case V3_NOT_B32              :
        case V3_BFREV_B32            :
        case V3_FFBH_U32             :
        case V3_FFBL_B32             :
        case V3_FFBH_I32             :
        case V3_FREXP_EXP_I32_F32    :
        case V3_MOVRELD_B32          :
        case V3_MOVRELS_B32          :
        case V3_MOVRELSD_B32         :
        case V3_FREXP_MANT_F32       :
            return 1;

        case V3_DIV_SCALE_F64:        
        case V3_DIV_FIXUP_F64        :
        case V3_LSHL_B64             :
        case V3_LSHR_B64             :
        case V3_ASHR_I64             :
        case V3_ADD_F64              :
        case V3_MUL_F64              :
        case V3_MIN_F64              :
        case V3_MAX_F64              :
        case V3_LDEXP_F64            :
        case V3_TRIG_PREOP_F64       :
        case V3_DIV_FMAS_F64         :
        case V3_CVT_I32_F64          :
        case V3_CVT_F32_F64          :
        case V3_CVT_U32_F64          :
        case V3_RCP_F64              :
        case V3_RCP_CLAMP_F64        :
        case V3_RSQ_F64              :
        case V3_RSQ_CLAMP_F64        :
        case V3_SQRT_F64             :
        case V3_FREXP_EXP_I32_F64    :
        case V3_FREXP_MANT_F64       :
        case V3_FRACT_F64            :
            return 2;
        }
    }


    uint VOP3Instruction::GetResultSizeInDWORDS() const
    {
        switch( GetOpcode() )
        {
        default:     
        case V3_NOP:
        case V3_CLREXCP:
            return 0;   

        case V3_CMP_F_F32         :
        case V3_CMP_LT_F32        :
        case V3_CMP_EQ_F32        :
        case V3_CMP_LE_F32        :
        case V3_CMP_GT_F32        :
        case V3_CMP_LG_F32        :
        case V3_CMP_GE_F32        :
        case V3_CMP_O_F32         :
        case V3_CMP_U_F32         :
        case V3_CMP_NGE_F32       :
        case V3_CMP_NLG_F32       :
        case V3_CMP_NGT_F32       :
        case V3_CMP_NLE_F32       :
        case V3_CMP_NEQ_F32       :
        case V3_CMP_NLT_F32       :
        case V3_CMP_TRU_F32       :
        case V3_CMPX_F_F32        :
        case V3_CMPX_LT_F32       :
        case V3_CMPX_EQ_F32       :
        case V3_CMPX_LE_F32       :
        case V3_CMPX_GT_F32       :
        case V3_CMPX_LG_F32       :
        case V3_CMPX_GE_F32       :
        case V3_CMPX_O_F32        :
        case V3_CMPX_U_F32        :
        case V3_CMPX_NGE_F32      :
        case V3_CMPX_NLG_F32      :
        case V3_CMPX_NGT_F32      :
        case V3_CMPX_NLE_F32      :
        case V3_CMPX_NEQ_F32      :
        case V3_CMPX_NLT_F32      :
        case V3_CMPX_TRU_F32      :
        case V3_CMPS_F_F32        :
        case V3_CMPS_LT_F32       :
        case V3_CMPS_EQ_F32       :
        case V3_CMPS_LE_F32       :
        case V3_CMPS_GT_F32       :
        case V3_CMPS_LG_F32       :
        case V3_CMPS_GE_F32       :
        case V3_CMPS_O_F32        :
        case V3_CMPS_U_F32        :
        case V3_CMPS_NGE_F32      :
        case V3_CMPS_NLG_F32      :
        case V3_CMPS_NGT_F32      :
        case V3_CMPS_NLE_F32      :
        case V3_CMPS_NEQ_F32      :
        case V3_CMPS_NLT_F32      :
        case V3_CMPS_TRU_F32      :
        case V3_CMPSX_F_F32       :
        case V3_CMPSX_LT_F32      :
        case V3_CMPSX_EQ_F32      :
        case V3_CMPSX_LE_F32      :
        case V3_CMPSX_GT_F32      :
        case V3_CMPSX_LG_F32      :
        case V3_CMPSX_GE_F32      :
        case V3_CMPSX_O_F32       :
        case V3_CMPSX_U_F32       :
        case V3_CMPSX_NGE_F32     :
        case V3_CMPSX_NLG_F32     :
        case V3_CMPSX_NGT_F32     :
        case V3_CMPSX_NLE_F32     :
        case V3_CMPSX_NEQ_F32     :
        case V3_CMPSX_NLT_F32     :
        case V3_CMPSX_TRU_F32     :
        case V3_CMP_F_I32         :
        case V3_CMP_LT_I32        :
        case V3_CMP_EQ_I32        :
        case V3_CMP_LE_I32        :
        case V3_CMP_GT_I32        :
        case V3_CMP_LG_I32        :
        case V3_CMP_GE_I32        :
        case V3_CMP_TRU_I32       :
        case V3_CMPX_F_I32        :
        case V3_CMPX_LT_I32       :
        case V3_CMPX_EQ_I32       :
        case V3_CMPX_LE_I32       :
        case V3_CMPX_GT_I32       :
        case V3_CMPX_LG_I32       :
        case V3_CMPX_GE_I32       :
        case V3_CMPX_TRU_I32      :
        case V3_CMP_F_U32         :
        case V3_CMP_LT_U32        :
        case V3_CMP_EQ_U32        :
        case V3_CMP_LE_U32        :
        case V3_CMP_GT_U32        :
        case V3_CMP_LG_U32        :
        case V3_CMP_GE_U32        :
        case V3_CMP_TRU_U32       :
        case V3_CMPX_F_U32        :
        case V3_CMPX_LT_U32       :
        case V3_CMPX_EQ_U32       :
        case V3_CMPX_LE_U32       :
        case V3_CMPX_GT_U32       :
        case V3_CMPX_LG_U32       :
        case V3_CMPX_GE_U32       :
        case V3_CMPX_TRU_U32      :        
        case V3_CMP_F_F64         :
        case V3_CMP_LT_F64        :
        case V3_CMP_EQ_F64        :
        case V3_CMP_LE_F64        :
        case V3_CMP_GT_F64        :
        case V3_CMP_LG_F64        :
        case V3_CMP_GE_F64        :
        case V3_CMP_O_F64         :
        case V3_CMP_U_F64         :
        case V3_CMP_NGE_F64       :
        case V3_CMP_NLG_F64       :
        case V3_CMP_NGT_F64       :
        case V3_CMP_NLE_F64       :
        case V3_CMP_NEQ_F64       :
        case V3_CMP_NLT_F64       :
        case V3_CMP_TRU_F64       :
        case V3_CMPX_F_F64        :
        case V3_CMPX_LT_F64       :
        case V3_CMPX_EQ_F64       :
        case V3_CMPX_LE_F64       :
        case V3_CMPX_GT_F64       :
        case V3_CMPX_LG_F64       :
        case V3_CMPX_GE_F64       :
        case V3_CMPX_O_F64        :
        case V3_CMPX_U_F64        :
        case V3_CMPX_NGE_F64      :
        case V3_CMPX_NLG_F64      :
        case V3_CMPX_NGT_F64      :
        case V3_CMPX_NLE_F64      :
        case V3_CMPX_NEQ_F64      :
        case V3_CMPX_NLT_F64      :
        case V3_CMPX_TRU_F64      :
        case V3_CMPS_F_F64        :
        case V3_CMPS_LT_F64       :
        case V3_CMPS_EQ_F64       :
        case V3_CMPS_LE_F64       :
        case V3_CMPS_GT_F64       :
        case V3_CMPS_LG_F64       :
        case V3_CMPS_GE_F64       :
        case V3_CMPS_O_F64        :
        case V3_CMPS_U_F64        :
        case V3_CMPS_NGE_F64      :
        case V3_CMPS_NLG_F64      :
        case V3_CMPS_NGT_F64      :
        case V3_CMPS_NLE_F64      :
        case V3_CMPS_NEQ_F64      :
        case V3_CMPS_NLT_F64      :
        case V3_CMPS_TRU_F64      :
        case V3_CMPSX_F_F64       :
        case V3_CMPSX_LT_F64      :
        case V3_CMPSX_EQ_F64      :
        case V3_CMPSX_LE_F64      :
        case V3_CMPSX_GT_F64      :
        case V3_CMPSX_LG_F64      :
        case V3_CMPSX_GE_F64      :
        case V3_CMPSX_O_F64       :
        case V3_CMPSX_U_F64       :
        case V3_CMPSX_NGE_F64     :
        case V3_CMPSX_NLG_F64     :
        case V3_CMPSX_NGT_F64     :
        case V3_CMPSX_NLE_F64     :
        case V3_CMPSX_NEQ_F64     :
        case V3_CMPSX_NLT_F64     :
        case V3_CMPSX_TRU_F64     :
        case V3_CMP_F_I64         :
        case V3_CMP_LT_I64        :
        case V3_CMP_EQ_I64        :
        case V3_CMP_LE_I64        :
        case V3_CMP_GT_I64        :
        case V3_CMP_LG_I64        :
        case V3_CMP_GE_I64        :
        case V3_CMP_TRU_I64       :
        case V3_CMPX_F_I64        :
        case V3_CMPX_LT_I64       :
        case V3_CMPX_EQ_I64       :
        case V3_CMPX_LE_I64       :
        case V3_CMPX_GT_I64       :
        case V3_CMPX_LG_I64       :
        case V3_CMPX_GE_I64       :
        case V3_CMPX_TRU_I64      :
        case V3_CMP_F_U64         :
        case V3_CMP_LT_U64        :
        case V3_CMP_EQ_U64        :
        case V3_CMP_LE_U64        :
        case V3_CMP_GT_U64        :
        case V3_CMP_LG_U64        :
        case V3_CMP_GE_U64        :
        case V3_CMP_TRU_U64       :
        case V3_CMPx_F_U64        :
        case V3_CMPx_LT_U64       :
        case V3_CMPx_EQ_U64       :
        case V3_CMPx_LE_U64       :
        case V3_CMPx_GT_U64       :
        case V3_CMPx_LG_U64       :
        case V3_CMPx_GE_U64       :
        case V3_CMPx_TRU_U64      :
        case V3_CMP_CLASS_F64     :
        case V3_CMPX_CLASS_F64    :
        case V3_CMP_CLASS_F32     :
        case V3_CMPX_CLASS_F32    :
        case V3_CNDMASK_B32        :
        case V3_READLANE_B32       :
        case V3_WRITELANE_B32      :
        case V3_ADD_F32            :
        case V3_SUB_F32            :
        case V3_SUBREV_F32         :
        case V3_MAC_LEGACY_F32     :
        case V3_MUL_LEGACY_F32     :
        case V3_MUL_F32            :
        case V3_MUL_I32_I24        :
        case V3_MUL_HI_I32_I24     :
        case V3_MUL_U32_U24        :
        case V3_MUL_HI_U32_U24     :
        case V3_MIN_LEGACY_F32     :
        case V3_MAX_LEGACY_F32     :
        case V3_MIN_F32            :
        case V3_MAX_F32            :
        case V3_MIN_I32            :
        case V3_MAX_I32            :
        case V3_MIN_U32            :
        case V3_MAX_U32            :
        case V3_LSHR_B32           :
        case V3_LSHRREV_B32        :
        case V3_ASHR_I32           :
        case V3_ASHRREV_I32        :
        case V3_LSHL_B32           :
        case V3_LSHLREV_B32        :
        case V3_AND_B32            :
        case V3_OR_B32             :
        case V3_XOR_B32            :
        case V3_BFM_B32            :
        case V3_MAC_F32            :
        case V3_MADMK_F32          :
        case V3_MADAK_F32          :
        case V3_BCNT_U32_B32       :
        case V3_MBCNT_LO_U32_B32:                 
        case V3_MBCNT_HI_U32_B32 :
        case V3_ADD_I32              :
        case V3_SUB_I32              :
        case V3_SUBREV_I32           :
        case V3_ADDC_U32             :
        case V3_SUBB_U32             :
        case V3_SUBBREV_U32          :
        case V3_DIV_SCALE_F32        :
        case V3_LDEXP_F32            :
        case V3_CVT_PKACCUM_U8_F32   :
        case V3_CVT_PKNORM_I16_F32   :
        case V3_CVT_PKNORM_U16_F32   :
        case V3_CVT_PKRTZ_F16_F32    :
        case V3_CVT_PK_U16_U32       :
        case V3_CVT_PK_I16_I32       :   
        case V3_MAD_LEGACY_F32       :
        case V3_MAD_F32              :
        case V3_MAD_I32_I24          :
        case V3_MAD_U32_U24          :
        case V3_CUBEID_F32           :
        case V3_CUBESC_F32           :
        case V3_CUBETC_F32           :
        case V3_CUBEMA_F32           :
        case V3_BFE_U32              :
        case V3_BFE_I32              :
        case V3_BFI_B32              :
        case V3_FMA_F32              :
        case V3_FMA_F64              :
        case V3_LERP_U8              :
        case V3_ALIGNBIT_B32         :
        case V3_ALIGNBYTE_B32        :
        case V3_MULLIT_F32           :
        case V3_MIN3_F32             :
        case V3_MIN3_I32             :
        case V3_MIN3_U32             :
        case V3_MAX3_F32             :
        case V3_MAX3_I32             :
        case V3_MAX3_U32             :
        case V3_MED3_F32             :
        case V3_MED3_I32             :
        case V3_MED3_U32             :
        case V3_SAD_U8               :
        case V3_SAD_HI_U8            :
        case V3_SAD_U16              :
        case V3_SAD_U32              :
        case V3_CVT_PK_U8_F32        :
        case V3_DIV_FIXUP_F32        :
        case V3_MUL_LO_U32           :
        case V3_MUL_HI_U32           :
        case V3_MUL_LO_I32           :
        case V3_MUL_HI_I32           :
        case V3_DIV_FMAS_F32         :
        case V3_MSAD_U8              :
        case V3_QSAD_U8              :
        case V3_MQSAD_U8             :
        case V3_MAD_U64_U32    :   // : {vcc_out,D.u64} = S0.u32 * S1.u32 + S2.u64.
        case V3_MAD_I64_I32    :   // : {vcc_out,D.i64} = S0.i32 * S1.i32 + S2.i64.
        case V3_MOV_B32      :     
        case V3_READFIRSTLANE_B32 :
        case V3_CVT_F32_I32          :
        case V3_CVT_F32_U32          :
        case V3_CVT_U32_F32          :
        case V3_CVT_I32_F32          :
        case V3_MOV_FED_B32          :
        case V3_CVT_F16_F32          :
        case V3_CVT_F32_F16          :
        case V3_CVT_RPI_I32_F32      :
        case V3_CVT_FLR_I32_F32      :
        case V3_CVT_OFF_F32_I4       :
        case V3_CVT_F32_UBYTE0       :
        case V3_CVT_F32_UBYTE1       :
        case V3_CVT_F32_UBYTE2       :
        case V3_CVT_F32_UBYTE3       :
        case V3_FRACT_F32            :
        case V3_TRUNC_F32            :
        case V3_CEIL_F32             :
        case V3_RNDNE_F32            :
        case V3_FLOOR_F32            :
        case V3_EXP_F32              :
        case V3_LOG_CLAMP_F32        :
        case V3_LOG_F32              :
        case V3_RCP_CLAMP_F32        :
        case V3_RCP_LEGACY_F32       :
        case V3_RCP_F32              :
        case V3_RCP_IFLAG_F32        :
        case V3_RSQ_CLAMP_F32        :
        case V3_RSQ_LEGACY_F32       :
        case V3_RSQ_F32              :
        case V3_SQRT_F32             :
        case V3_SIN_F32              :
        case V3_COS_F32              :
        case V3_NOT_B32              :
        case V3_BFREV_B32            :
        case V3_FFBH_U32             :
        case V3_FFBL_B32             :
        case V3_FFBH_I32             :
        case V3_FREXP_EXP_I32_F32    :
        case V3_MOVRELD_B32          :
        case V3_MOVRELS_B32          :
        case V3_MOVRELSD_B32         :
        case V3_FREXP_MANT_F32       :
            return 1;

        case V3_CVT_F64_I32          :
        case V3_CVT_F64_F32          :
        case V3_CVT_F64_U32          :
        case V3_DIV_SCALE_F64:       
        case V3_DIV_FIXUP_F64        :
        case V3_LSHL_B64             :
        case V3_LSHR_B64             :
        case V3_ASHR_I64             :
        case V3_ADD_F64              :
        case V3_MUL_F64              :
        case V3_MIN_F64              :
        case V3_MAX_F64              :
        case V3_LDEXP_F64            :
        case V3_TRIG_PREOP_F64       :
        case V3_DIV_FMAS_F64         :
        case V3_CVT_I32_F64          :
        case V3_CVT_F32_F64          :
        case V3_CVT_U32_F64          :
        case V3_RCP_F64              :
        case V3_RCP_CLAMP_F64        :
        case V3_RSQ_F64              :
        case V3_RSQ_CLAMP_F64        :
        case V3_SQRT_F64             :
        case V3_FREXP_EXP_I32_F64    :
        case V3_FREXP_MANT_F64       :
        case V3_FRACT_F64            :
            return 2;
        case V3_MQSAD_U32_U8   :   // : D.u128 = Masked Quad-Byte SAD with 32-bit accum_lo/hi(S0.u[63:0], S1.u[31:0], S2.u[127:0]).
            return 4; 
        }
    }



    uint MUBUFFInstruction::GetResultWidthInDWORDS() const
    {
        switch( GetOpcode() )
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
            
        default:
        case BUFFER_WBINVL1_SC               :
        case BUFFER_WBINVL1                  :
            return 0;
        }
    }



    uint MTBUFFInstruction::GetResultWidthInDWORDS() const
    {
        switch( GetOpcode() )
        {
        case TBUFFER_LOAD_FORMAT_X      :
        case TBUFFER_STORE_FORMAT_X     :
            return 1;
        case TBUFFER_LOAD_FORMAT_XY     :
        case TBUFFER_STORE_FORMAT_XY    :
            return 2;
        case TBUFFER_LOAD_FORMAT_XYZ    :
        case TBUFFER_STORE_FORMAT_XYZ   :
            return 3;
        case TBUFFER_LOAD_FORMAT_XYZW   :
        case TBUFFER_STORE_FORMAT_XYZW  :
            return 4;
        default:
            return 0;
        }
    }

    uint MIMGInstruction::GetSamplerWidthInDWORDS() const
    {
        switch( GetOpcode() )
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

    uint MIMGInstruction::GetResultWidthInDWORDS() const
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


    uint DSInstruction::GetDataWidthInDWORDS() const
    {
        switch( GetOpcode() )
        {
        default:
            return 0;

        case DS_ADD_U32                   :
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

    bool SOP2Instruction::IsBranch() const
    {
        return GetOpcode() == S_CBRANCH_G_FORK;
    }

    bool SOPKInstruction::IsBranch() const
    {
        return GetOpcode() == S_CBRANCH_I_FORK;
    }

    const Instruction* SOPKInstruction::GetBranchTarget() const
    {
        if( !IsBranch() ) return 0;
        int offset = this->ReadSIMM16();
        const uint32* pThis = (const uint32*)this;
        return (const Instruction*) (pThis+1+offset);
    }

    bool SOP1Instruction::IsBranch() const
    {
        switch( GetOpcode() )
        {
        case S_SETPC_B64:
        case S_SWAPPC_B64:
        case S_CBRANCH_JOIN:
        case S_RFE_B64:
            return true;
        default:
            return false;
        }
    }

    bool SOPPInstruction::IsBranch() const
    {
        switch( GetOpcode() )
        {
        case S_ENDPGM                :
        case S_BRANCH                :
        case S_CBRANCH_SCC0          :
        case S_CBRANCH_SCC1          :
        case S_CBRANCH_VCCZ          :
        case S_CBRANCH_VCCNZ         :
        case S_CBRANCH_EXECZ         :
        case S_CBRANCH_EXECNZ        :
        case S_BARRIER               :
        case S_TRAP                  :            
        case S_CBRANCH_CDBGSYS            :
        case S_CBRANCH_CDBGUSER           :
        case S_CBRANCH_CDBGSYS_OR_USER    :
        case S_CBRANCH_CDBGSYS_AND_USER   :
            return true;
        default:
            return false;
        }
    }

    const Instruction* SOPPInstruction::GetBranchTarget() const
    {
        switch( GetOpcode() )
        {
        case S_BRANCH                :
        case S_CBRANCH_SCC0          :
        case S_CBRANCH_SCC1          :
        case S_CBRANCH_VCCZ          :
        case S_CBRANCH_VCCNZ         :
        case S_CBRANCH_EXECZ         :
        case S_CBRANCH_EXECNZ        :
        case S_CBRANCH_CDBGSYS            :
        case S_CBRANCH_CDBGUSER           :
        case S_CBRANCH_CDBGSYS_OR_USER    :
        case S_CBRANCH_CDBGSYS_AND_USER   :
            {
                int n = this->ReadSIMM16();
                int offset = this->ReadSIMM16();
                const uint32* pThis = (const uint32*)this;
                return (const Instruction*) (pThis+1+offset);
            }
        default:
            return 0;
        }
    }


}