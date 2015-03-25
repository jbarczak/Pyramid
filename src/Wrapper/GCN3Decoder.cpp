
#pragma unmanaged
#include "GCNEnums.h"
#include "GCNIsa.h"
#include "GCN3Decoder.h"
#include <string.h>
#include <math.h>

namespace GCN{
namespace _GCN3Decoder_INTERNAL
{
    struct EnumLUT
    {
        uint eEnum;
        uint  nEncoding;
    };

    static uint LUTLookup( const EnumLUT* pLUT, size_t nLUTSize,uint en, uint TInvalid )
    {
        size_t n = nLUTSize/sizeof(EnumLUT);
        for( size_t i=0; i<n; i++ ) 
            if( pLUT[i].nEncoding == en ) 
                return pLUT[i].eEnum; 
        return TInvalid;
    }

#define BEGIN_TRANSLATOR(T,Name) static const EnumLUT LUT_##T_##Name[] = {
#define ENUM(Enum,Value) { Enum, Value },
#define END_TRANSLATOR(T,Name,TInvalid) \
    };\
    T Translate_##Name( uint en ) {\
        const EnumLUT* pLUT = LUT_##T_##Name;\
        size_t n = sizeof(LUT_##T_##Name);\
        return (T) LUTLookup(pLUT,n,en,TInvalid);\
    };



    BEGIN_TRANSLATOR(ScalarInstructions,SOP2Opcodes)
        ENUM( S_ADD_U32             ,        0 )
        ENUM( S_SUB_U32             ,        1 )
        ENUM( S_ADD_I32             ,        2 )
        ENUM( S_SUB_I32             ,        3 )
        ENUM( S_ADDC_U32            ,        4 )
        ENUM( S_SUBB_U32            ,        5 )
        ENUM( S_MIN_I32             ,        6 )
        ENUM( S_MIN_U32             ,        7 )
        ENUM( S_MAX_I32             ,        8 )
        ENUM( S_MAX_U32             ,        9 )
        ENUM( S_CSELECT_B32         ,        10)
        ENUM( S_CSELECT_B64         ,        11)
        ENUM( S_AND_B32             ,        12)
        ENUM( S_AND_B64             ,        13)
        ENUM( S_OR_B32              ,        14)
        ENUM( S_OR_B64              ,        15)
        ENUM( S_XOR_B32             ,        16)
        ENUM( S_XOR_B64             ,        17)
        ENUM( S_ANDN2_B32           ,        18)
        ENUM( S_ANDN2_B64           ,        19)
        ENUM( S_ORN2_B32            ,        20)
        ENUM( S_ORN2_B64            ,        21)
        ENUM( S_NAND_B32            ,        22)
        ENUM( S_NAND_B64            ,        23)
        ENUM( S_NOR_B32             ,        24)
        ENUM( S_NOR_B64             ,        25)
        ENUM( S_XNOR_B32            ,        26)
        ENUM( S_XNOR_B64            ,        27)
        ENUM( S_LSHL_B32            ,        28)
        ENUM( S_LSHL_B64            ,        29)
        ENUM( S_LSHR_B32            ,        30)
        ENUM( S_LSHR_B64            ,        31)
        ENUM( S_ASHR_I32            ,        32)
        ENUM( S_ASHR_I64            ,        33)
        ENUM( S_BFM_B32             ,        34)
        ENUM( S_BFM_B64             ,        35)
        ENUM( S_MUL_I32             ,        36)
        ENUM( S_BFE_U32             ,        37)
        ENUM( S_BFE_I32             ,        38)
        ENUM( S_BFE_U64             ,        39)
        ENUM( S_BFE_I64             ,        40)
        ENUM( S_CBRANCH_G_FORK      ,        41)
        ENUM( S_ABSDIFF_I32         ,        42)
        ENUM( S_RFE_RESTORE_B64     ,        43)
    END_TRANSLATOR(ScalarInstructions,SOP2Opcodes,S_INVALID);


    BEGIN_TRANSLATOR(ScalarInstructions,SOPKOpcodes)
        ENUM( S_MOVK_I32            , 0     )
        ENUM( S_CMOVK_I32           , 1     )
        ENUM( S_CMPK_EQ_I32         , 2     )
        ENUM( S_CMPK_LG_I32         , 3     )
        ENUM( S_CMPK_GT_I32         , 4     )
        ENUM( S_CMPK_GE_I32         , 5     )
        ENUM( S_CMPK_LT_I32         , 6     )
        ENUM( S_CMPK_LE_I32         , 7     )
        ENUM( S_CMPK_EQ_U32         , 8     )
        ENUM( S_CMPK_LG_U32         , 9     )
        ENUM( S_CMPK_GT_U32         , 10    )
        ENUM( S_CMPK_GE_U32         , 11    )
        ENUM( S_CMPK_LT_U32         , 12    )
        ENUM( S_CMPK_LE_U32         , 13    )
        ENUM( S_ADDK_I32            , 14    )
        ENUM( S_MULK_I32            , 15    )
        ENUM( S_CBRANCH_I_FORK      , 16    )
        ENUM( S_GETREG_B32          , 17    )
        ENUM( S_SETREG_B32          , 18    )
        ENUM( S_SETREG_IMM32_B32    , 20    )
    END_TRANSLATOR(ScalarInstructions,SOPKOpcodes,S_INVALID);

    BEGIN_TRANSLATOR(ScalarInstructions,SOP1Opcodes)
        ENUM(   S_MOV_B32              , 0   )
        ENUM(   S_MOV_B64              , 1   )
        ENUM(   S_CMOV_B32             , 2   )
        ENUM(   S_CMOV_B64             , 3   )
        ENUM(   S_NOT_B32              , 4   )
        ENUM(   S_NOT_B64              , 5   )
        ENUM(   S_WQM_B32              , 6   )
        ENUM(   S_WQM_B64              , 7   )
        ENUM(   S_BREV_B32             , 8   )
        ENUM(   S_BREV_B64             , 9   )
        ENUM(   S_BCNT0_I32_B32        , 10  )
        ENUM(   S_BCNT0_I32_B64        , 11  )
        ENUM(   S_BCNT1_I32_B32        , 12  )
        ENUM(   S_BCNT1_I32_B64        , 13  )
        ENUM(   S_FF0_I32_B32          , 14  )
        ENUM(   S_FF0_I32_B64          , 15  )
        ENUM(   S_FF1_I32_B32          , 16  )
        ENUM(   S_FF1_I32_B64          , 17  )
        ENUM(   S_FLBIT_I32_B32        , 18  )
        ENUM(   S_FLBIT_I32_B64        , 19  )
        ENUM(   S_FLBIT_I32            , 20  )
        ENUM(   S_FLBIT_I32_I64        , 21  )
        ENUM(   S_SEXT_I32_I8          , 22  )
        ENUM(   S_SEXT_I32_I16         , 23  )
        ENUM(   S_BITSET0_B32          , 24  )
        ENUM(   S_BITSET0_B64          , 25  )
        ENUM(   S_BITSET1_B32          , 26  )
        ENUM(   S_BITSET1_B64          , 27  )
        ENUM(   S_GETPC_B64            , 28  )
        ENUM(   S_SETPC_B64            , 29  )
        ENUM(   S_SWAPPC_B64           , 30  )
        ENUM(   S_RFE_B64              , 31  )
        ENUM(   S_AND_SAVEEXEC_B64     , 32  )
        ENUM(   S_OR_SAVEEXEC_B64      , 33  )
        ENUM(   S_XOR_SAVEEXEC_B64     , 34  )
        ENUM(   S_ANDN2_SAVEEXEC_B64   , 35  )
        ENUM(   S_ORN2_SAVEEXEC_B64    , 36  )
        ENUM(   S_NAND_SAVEEXEC_B64    , 37  )
        ENUM(   S_NOR_SAVEEXEC_B64     , 38  )
        ENUM(   S_XNOR_SAVEEXEC_B64    , 39  )
        ENUM(   S_QUADMASK_B32         , 40  )
        ENUM(   S_QUADMASK_B64         , 41  )
        ENUM(   S_MOVRELS_B32          , 42  )
        ENUM(   S_MOVRELS_B64          , 43  )
        ENUM(   S_MOVRELD_B32          , 44  )
        ENUM(   S_MOVRELD_B64          , 45  )
        ENUM(   S_CBRANCH_JOIN         , 46  )
        ENUM(   S_ABS_I32              , 48  )
        ENUM(   S_SET_GPR_IDX_IDX      , 49  )
    END_TRANSLATOR(ScalarInstructions,SOP1Opcodes,S_INVALID)


    BEGIN_TRANSLATOR(ScalarInstructions,SOPCOpcodes)
        ENUM( S_CMP_EQ_I32     ,0 )   //: SCC = (S0.i == S1.i).
        ENUM( S_CMP_LG_I32     ,1 )   //: SCC = (S0.i != S1.i).
        ENUM( S_CMP_GT_I32     ,2 )   //: SCC = (S0.i > S1.i).
        ENUM( S_CMP_GE_I32     ,3 )   //: SCC = (S0.i >= S1.i).
        ENUM( S_CMP_LT_I32     ,4 )   //: SCC = (S0.i < S1.i).
        ENUM( S_CMP_LE_I32     ,5 )   //: SCC = (S0.i <= S1.i).
        ENUM( S_CMP_EQ_U32     ,6 )   //: SCC = (S0.u == S1.u).
        ENUM( S_CMP_LG_U32     ,7 )   //: SCC = (S0.u != S1.u).
        ENUM( S_CMP_GT_U32     ,8 )   //: SCC = (S0.u > S1.u).
        ENUM( S_CMP_GE_U32     ,9 )   //: SCC = (S0.u >= S1.u).
        ENUM( S_CMP_LT_U32     ,10)   //: SCC = (S0.u < S1.u).
        ENUM( S_CMP_LE_U32     ,11)   //: SCC = (S0.u <= S1.u).
        ENUM( S_BITCMP0_B32    ,12)   //: SCC = (S0.u[S1.u[4:0]] == 0).
        ENUM( S_BITCMP1_B32    ,13)   //: SCC = (S0.u[S1.u[4:0]] == 1).
        ENUM( S_BITCMP0_B64    ,14)   //: SCC = (S0.u[S1.u[5:0]] == 0).
        ENUM( S_BITCMP1_B64    ,15)   //: SCC = (S0.u[S1.u[5:0]] == 1).
        ENUM( S_SETVSKIP       ,16)   //: VSKIP = S0.u[S1.u[4:0]].
        ENUM( S_SET_GPR_IDX_ON    , 17  )
        ENUM( S_CMP_EQ_U64        , 18  )
        ENUM( S_CMP_NE_U64        , 19  )
    END_TRANSLATOR(ScalarInstructions,SOPCOpcodes,S_INVALID)


    BEGIN_TRANSLATOR(ScalarInstructions,SOPPOpcodes)
        ENUM(S_NOP                      ,0  )  //: do nothing. Repeat NOP 1..8 times based on SIMM16[2:0]. 0 = 1 time, 7 = 8 times.
        ENUM(S_ENDPGM                   ,1  )  //: end of program; terminate wavefront.
        ENUM(S_BRANCH                   ,2  )  //: PC = PC + signext(SIMM16 * 4) + 4.
        ENUM(S_CBRANCH_SCC0             ,4  )  //: if(SCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        ENUM(S_CBRANCH_SCC1             ,5  )  //: if(SCC == 1) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        ENUM(S_CBRANCH_VCCZ             ,6  )  //: if(VCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        ENUM(S_CBRANCH_VCCNZ            ,7  )  //: if(VCC != 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        ENUM(S_CBRANCH_EXECZ            ,8  )  //: if(EXEC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        ENUM(S_CBRANCH_EXECNZ           ,9  )  //: if(EXEC != 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        ENUM(S_BARRIER                  ,10 )  //: Sync waves within a thread group.
        ENUM(S_SETKILL                  ,11 )
        ENUM(S_WAITCNT                  ,12 )  //: Wait for count of outstanding lds, vector-memory and
        ENUM(S_SETHALT                  ,13 )  //: set HALT bit to value of SIMM16[0]. 1=halt, 0=resume. Halt is ignored while priv=1.
        ENUM(S_SLEEP                    ,14 )  //: Cause a wave to sleep for approximately 64*SIMM16[2:0] clocks.
        ENUM(S_SETPRIO                  ,15 )  //: User settable wave priority. 0 = lowest, 3 = highest.
        ENUM(S_SENDMSG                  ,16 )  //: Send a message.
        ENUM(S_SENDMSGHALT              ,17 )  //: Send a message and then HALT.
        ENUM(S_TRAP                     ,18 )  //: Enter the trap handler. TrapID = SIMM16[7:0]. Wait for all instructions to complete, 
        ENUM(S_ICACHE_INV               ,19 )  //: Invalidate entire L1 I cache.
        ENUM(S_INCPERFLEVEL             ,20 )  //: Increment performance counter specified in SIMM16[3:0] by 1.
        ENUM(S_DECPERFLEVEL             ,21 )  //: Decrement performance counter specified in SIMM16[3:0] by 1.
        ENUM(S_TTRACEDATA               ,22 )  //: Send M0 as user data to thread-trace.
        ENUM(S_CBRANCH_CDBGSYS          ,23 )// : If (conditional_debug_system != 0) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
        ENUM(S_CBRANCH_CDBGUSER         ,24 )// : If (conditional_debug_user != 0) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
        ENUM(S_CBRANCH_CDBGSYS_OR_USER  ,25 )// : If (conditional_debug_system || conditional_debug_user) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
        ENUM(S_CBRANCH_CDBGSYS_AND_USER ,26 )// : If (conditional_debug_system && conditional_debug_user) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
        ENUM( S_ENDPGM_SAVED     , 27 ) 
        ENUM( S_SET_GPR_IDX_OFF  , 28 ) 
        ENUM( S_SET_GPR_IDX_MODE , 29 ) 
        
    END_TRANSLATOR(ScalarInstructions,SOPPOpcodes,S_INVALID)

    BEGIN_TRANSLATOR(ScalarMemoryInstructions,SMEMOpcodes)
       ENUM(  S_LOAD_DWORD                , 0   )
       ENUM(  S_LOAD_DWORDX2              , 1   )
       ENUM(  S_LOAD_DWORDX4              , 2   )
       ENUM(  S_LOAD_DWORDX8              , 3   )
       ENUM(  S_LOAD_DWORDX16             , 4   )
       ENUM(  S_BUFFER_LOAD_DWORD         , 8   )
       ENUM(  S_BUFFER_LOAD_DWORDX2       , 9   )
       ENUM(  S_BUFFER_LOAD_DWORDX4       , 10  )
       ENUM(  S_BUFFER_LOAD_DWORDX8       , 11  )
       ENUM(  S_BUFFER_LOAD_DWORDX16      , 12  )
       ENUM(  S_STORE_DWORD               , 16  )
       ENUM(  S_STORE_DWORDX2             , 17  )
       ENUM(  S_STORE_DWORDX4             , 18  )
       ENUM(  S_BUFFER_STORE_DWORD        , 24  )
       ENUM(  S_BUFFER_STORE_DWORDX2      , 25  )
       ENUM(  S_BUFFER_STORE_DWORDX4      , 26  )
       ENUM(  S_DCACHE_INV                , 32  )
       ENUM(  S_DCACHE_WB                 , 33  )
       ENUM(  S_DCACHE_INV_VOL            , 34  )
       ENUM(  S_DCACHE_WB_VOL             , 35  )
       ENUM(  S_MEMTIME                   , 36  )
       ENUM(  S_MEMREALTIME               , 37  )
       ENUM(  S_ATC_PROBE                 , 38  )
       ENUM(  S_ATC_PROBE_BUFFER          , 39  )
    END_TRANSLATOR(ScalarMemoryInstructions,SMEMOpcodes,S_INVALID)


    BEGIN_TRANSLATOR(VectorInstructions,VOP2Opcodes)
       ENUM( V_CNDMASK_B32          , 0  )
       ENUM( V_ADD_F32              , 1  )
       ENUM( V_SUB_F32              , 2  )
       ENUM( V_SUBREV_F32           , 3  )
       ENUM( V_MUL_LEGACY_F32       , 4  )
       ENUM( V_MUL_F32              , 5  )
       ENUM( V_MUL_I32_I24          , 6  )
       ENUM( V_MUL_HI_I32_I24       , 7  )
       ENUM( V_MUL_U32_U24          , 8  )
       ENUM( V_MUL_HI_U32_U24       , 9  )
       ENUM( V_MIN_F32              , 10 )
       ENUM( V_MAX_F32              , 11 )
       ENUM( V_MIN_I32              , 12 )
       ENUM( V_MAX_I32              , 13 )
       ENUM( V_MIN_U32              , 14 )
       ENUM( V_MAX_U32              , 15 )
       ENUM( V_LSHRREV_B32          , 16 )
       ENUM( V_ASHRREV_I32          , 17 )
       ENUM( V_LSHLREV_B32          , 18 )
       ENUM( V_AND_B32              , 19 )
       ENUM( V_OR_B32               , 20 )
       ENUM( V_XOR_B32              , 21 )
       ENUM( V_MAC_F32              , 22 )
       ENUM( V_MADMK_F32            , 23 )
       ENUM( V_MADAK_F32            , 24 )
       ENUM( V_ADD_U32              , 25 )
       ENUM( V_SUB_U32              , 26 )
       ENUM( V_SUBREV_U32           , 27 )
       ENUM( V_ADDC_U32             , 28 )
       ENUM( V_SUBB_U32             , 29 )
       ENUM( V_SUBBREV_U32          , 30 )
       ENUM( V_ADD_F16              , 31 )
       ENUM( V_SUB_F16              , 32 )
       ENUM( V_SUBREV_F16           , 33 )
       ENUM( V_MUL_F16              , 34 )
       ENUM( V_MAC_F16              , 35 )
       ENUM( V_MADMK_F16            , 36 )
       ENUM( V_MADAK_F16            , 37 )
       ENUM( V_ADD_U16              , 38 )
       ENUM( V_SUB_U16              , 39 )
       ENUM( V_SUBREV_U16           , 40 )
       ENUM( V_MUL_LO_U16           , 41 )
       ENUM( V_LSHLREV_B16          , 42 )
       ENUM( V_LSHRREV_B16          , 43 )
       ENUM( V_ASHRREV_I16          , 44 )
       ENUM( V_MAX_F16              , 45 )
       ENUM( V_MIN_F16              , 46 )
       ENUM( V_MAX_U16              , 47 )
       ENUM( V_MAX_I16              , 48 )
       ENUM( V_MIN_U16              , 49 )
       ENUM( V_MIN_I16              , 50 )
       ENUM( V_LDEXP_F16            , 51 )
    END_TRANSLATOR(VectorInstructions,VOP2Opcodes,V_INVALID)


    BEGIN_TRANSLATOR(VectorInstructions,VOP1Opcodes)
        ENUM(  V_NOP                    , 0   )
        ENUM(  V_MOV_B32                , 1   )
        ENUM(  V_READFIRSTLANE_B32      , 2   )
        ENUM(  V_CVT_I32_F64            , 3   )
        ENUM(  V_CVT_F64_I32            , 4   )
        ENUM(  V_CVT_F32_I32            , 5   )
        ENUM(  V_CVT_F32_U32            , 6   )
        ENUM(  V_CVT_U32_F32            , 7   )
        ENUM(  V_CVT_I32_F32            , 8   )
        ENUM(  V_CVT_F16_F32            , 10  )
        ENUM(  V_CVT_F32_F16            , 11  )
        ENUM(  V_CVT_RPI_I32_F32        , 12  )
        ENUM(  V_CVT_FLR_I32_F32        , 13  )
        ENUM(  V_CVT_OFF_F32_I4         , 14  )
        ENUM(  V_CVT_F32_F64            , 15  )
        ENUM(  V_CVT_F64_F32            , 16  )
        ENUM(  V_CVT_F32_UBYTE0         , 17  )
        ENUM(  V_CVT_F32_UBYTE1         , 18  )
        ENUM(  V_CVT_F32_UBYTE2         , 19  )
        ENUM(  V_CVT_F32_UBYTE3         , 20  )
        ENUM(  V_CVT_U32_F64            , 21  )
        ENUM(  V_CVT_F64_U32            , 22  )
        ENUM(  V_TRUNC_F64              , 23  )
        ENUM(  V_CEIL_F64               , 24  )
        ENUM(  V_RNDNE_F64              , 25  )
        ENUM(  V_FLOOR_F64              , 26  )
        ENUM(  V_FRACT_F32              , 27  )
        ENUM(  V_TRUNC_F32              , 28  )
        ENUM(  V_CEIL_F32               , 29  )
        ENUM(  V_RNDNE_F32              , 30  )
        ENUM(  V_FLOOR_F32              , 31  )
        ENUM(  V_EXP_F32                , 32  )
        ENUM(  V_LOG_F32                , 33  )
        ENUM(  V_RCP_F32                , 34  )
        ENUM(  V_RCP_IFLAG_F32          , 35  )
        ENUM(  V_RSQ_F32                , 36  )
        ENUM(  V_RCP_F64                , 37  )
        ENUM(  V_RSQ_F64                , 38  )
        ENUM(  V_SQRT_F32               , 39  )
        ENUM(  V_SQRT_F64               , 40  )
        ENUM(  V_SIN_F32                , 41  )
        ENUM(  V_COS_F32                , 42  )
        ENUM(  V_NOT_B32                , 43  )
        ENUM(  V_BFREV_B32              , 44  )
        ENUM(  V_FFBH_U32               , 45  )
        ENUM(  V_FFBL_B32               , 46  )
        ENUM(  V_FFBH_I32               , 47  )
        ENUM(  V_FREXP_EXP_I32_F64      , 48  )
        ENUM(  V_FREXP_MANT_F64         , 49  )
        ENUM(  V_FRACT_F64              , 50  )
        ENUM(  V_FREXP_EXP_I32_F32      , 51  )
        ENUM(  V_FREXP_MANT_F32         , 52  )
        ENUM(  V_CLREXCP                , 53  )
        ENUM(  V_MOVRELD_B32            , 54  )
        ENUM(  V_MOVRELS_B32            , 55  )
        ENUM(  V_MOVRELSD_B32           , 56  )
        ENUM(  V_CVT_F16_U16            , 57  )
        ENUM(  V_CVT_F16_I16            , 58  )
        ENUM(  V_CVT_U16_F16            , 59  )
        ENUM(  V_CVT_I16_F16            , 60  )
        ENUM(  V_RCP_F16                , 61  )
        ENUM(  V_SQRT_F16               , 62  )
        ENUM(  V_RSQ_F16                , 63  )
        ENUM(  V_LOG_F16                , 64  )
        ENUM(  V_EXP_F16                , 65  )
        ENUM(  V_FREXP_MANT_F16         , 66  )
        ENUM(  V_FREXP_EXP_I16_F16      , 67  )
        ENUM(  V_FLOOR_F16              , 68  )
        ENUM(  V_CEIL_F16               , 69  )
        ENUM(  V_TRUNC_F16              , 70  )
        ENUM(  V_RNDNE_F16              , 71  )
        ENUM(  V_FRACT_F16              , 72  )
        ENUM(  V_SIN_F16                , 73  )
        ENUM(  V_COS_F16                , 74  )
        ENUM(  V_EXP_LEGACY_F32         , 75  )
        ENUM(  V_LOG_LEGACY_F32         , 76  )        
    END_TRANSLATOR(VectorInstructions,VOP1Opcodes,V_INVALID)

    static VectorInstructions Translate_VOPCOpcodes( uint n )
    {
        switch( n & 0xf0 )
        {
        case 0x10:
            switch( n & 0xf ) 
            {
            case 0: return V_CMP_CLASS_F32;
            case 1: return V_CMPX_CLASS_F32;
            case 2: return V_CMP_CLASS_F64;
            case 3: return V_CMPX_CLASS_F64;
            case 4: return V_CMP_CLASS_F16;
            case 5: return V_CMPX_CLASS_F16;
            }
        case 0X20: return (VectorInstructions)( V_CMP_F_F16  + (n&0xf)); 
        case 0x30: return (VectorInstructions)( V_CMPX_F_F16 + (n&0xf)); 
        case 0x40: return (VectorInstructions)( V_CMP_F_F32  + (n&0xf)); 
        case 0x50: return (VectorInstructions)( V_CMPX_F_F32 + (n&0xf)); 
        case 0x60: return (VectorInstructions)( V_CMP_F_F64  + (n&0xf)); 
        case 0x70: return (VectorInstructions)( V_CMPX_F_F64 + (n&0xf)); 
        default:
            switch( n&0xf8 )
            {
            case 0xA0: return (VectorInstructions)( V_CMP_F_I16  + (n&0x7)); 
            case 0xA8: return (VectorInstructions)( V_CMP_F_U16  + (n&0x7)); 
            case 0xB0: return (VectorInstructions)( V_CMPx_F_I16 + (n&0x7)); 
            case 0xB8: return (VectorInstructions)( V_CMPx_F_U16 + (n&0x7)); 
            case 0xC0: return (VectorInstructions)( V_CMP_F_I32  + (n&0x7)); 
            case 0xC8: return (VectorInstructions)( V_CMP_F_U32  + (n&0x7)); 
            case 0xD0: return (VectorInstructions)( V_CMPX_F_I32 + (n&0x7)); 
            case 0xD8: return (VectorInstructions)( V_CMPX_F_U32 + (n&0x7)); 
            case 0xE0: return (VectorInstructions)( V_CMP_F_I64  + (n&0x7)); 
            case 0xE8: return (VectorInstructions)( V_CMP_F_U64  + (n&0x7)); 
            case 0xF0: return (VectorInstructions)( V_CMPX_F_I64 + (n&0x7)); 
            case 0xF8: return (VectorInstructions)( V_CMPx_F_U64 + (n&0x7)); 
            }
        }
        return V_INVALID;
    }





    BEGIN_TRANSLATOR(VectorInstructions,VOP3OpcodesExtra)
       ENUM( V_MAD_LEGACY_F32      , 448   )
       ENUM( V_MAD_F32             , 449   )
       ENUM( V_MAD_I32_I24         , 450   )
       ENUM( V_MAD_U32_U24         , 451   )
       ENUM( V_CUBEID_F32          , 452   )
       ENUM( V_CUBESC_F32          , 453   )
       ENUM( V_CUBETC_F32          , 454   )
       ENUM( V_CUBEMA_F32          , 455   )
       ENUM( V_BFE_U32             , 456   )
       ENUM( V_BFE_I32             , 457   )
       ENUM( V_BFI_B32             , 458   )
       ENUM( V_FMA_F32             , 459   )
       ENUM( V_FMA_F64             , 460   )
       ENUM( V_LERP_U8             , 461   )
       ENUM( V_ALIGNBIT_B32        , 462   )
       ENUM( V_ALIGNBYTE_B32       , 463   )
       ENUM( V_MIN3_F32            , 464   )
       ENUM( V_MIN3_I32            , 465   )
       ENUM( V_MIN3_U32            , 466   )
       ENUM( V_MAX3_F32            , 467   )
       ENUM( V_MAX3_I32            , 468   )
       ENUM( V_MAX3_U32            , 469   )
       ENUM( V_MED3_F32            , 470   )
       ENUM( V_MED3_I32            , 471   )
       ENUM( V_MED3_U32            , 472   )
       ENUM( V_SAD_U8              , 473   )
       ENUM( V_SAD_HI_U8           , 474   )
       ENUM( V_SAD_U16             , 475   )
       ENUM( V_SAD_U32             , 476   )
       ENUM( V_CVT_PK_U8_F32       , 477   )
       ENUM( V_DIV_FIXUP_F32       , 478   )
       ENUM( V_DIV_FIXUP_F64       , 479   )
       ENUM( V_DIV_SCALE_F32       , 480   )  
       ENUM( V_DIV_SCALE_F64       , 481   )                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
       ENUM( V_DIV_FMAS_F32        , 482   )
       ENUM( V_DIV_FMAS_F64        , 483   )
       ENUM( V_MSAD_U8             , 484   )
       ENUM( V_QSAD_PK_U16_U8      , 485   )
       ENUM( V_MQSAD_PK_U16_U8     , 486   )
       ENUM( V_MQSAD_U32_U8        , 487   )
       ENUM( V_MAD_U64_U32         , 488   )
       ENUM( V_MAD_I64_I32         , 489   )
       ENUM( V_MAD_F16             , 490   )
       ENUM( V_MAD_U16             , 491   )
       ENUM( V_MAD_I16             , 492   )
       ENUM( V_PERM_B32            , 493   )
       ENUM( V_FMA_F16             , 494   )
       ENUM( V_DIV_FIXUP_F16       , 495   )
       ENUM( V_CVT_PKACCUM_U8_F32  , 496   )
       ENUM( V_INTERP_P1_F32       , 624   )
       ENUM( V_INTERP_P2_F32       , 625   )
       ENUM( V_INTERP_MOV_F32      , 626   )
       ENUM( V_INTERP_P1LL_F16     , 628   )
       ENUM( V_INTERP_P1LV_F16     , 629   )
       ENUM( V_INTERP_P2_F16       , 630   )
       ENUM( V_ADD_F64             , 640   )
       ENUM( V_MUL_F64             , 641   )
       ENUM( V_MIN_F64             , 642   )
       ENUM( V_MAX_F64             , 643   )
       ENUM( V_LDEXP_F64           , 644   )
       ENUM( V_MUL_LO_U32          , 645   )
       ENUM( V_MUL_HI_U32          , 646   )
       ENUM( V_MUL_HI_I32          , 647   )
       ENUM( V_LDEXP_F32           , 648   )
       ENUM( V_READLANE_B32        , 649   )
       ENUM( V_WRITELANE_B32       , 650   )
       ENUM( V_BCNT_U32_B32        , 651   )
       ENUM( V_MBCNT_LO_U32_B32    , 652   )
       ENUM( V_MBCNT_HI_U32_B32    , 653   )
       ENUM( V_LSHLREV_B64         , 655   )
       ENUM( V_LSHRREV_B64         , 656   )
       ENUM( V_ASHRREV_I64         , 657   )
       ENUM( V_TRIG_PREOP_F64      , 658   )
       ENUM( V_BFM_B32             , 659   )
       ENUM( V_CVT_PKNORM_I16_F32  , 660   )
       ENUM( V_CVT_PKNORM_U16_F32  , 661   )
       ENUM( V_CVT_PKRTZ_F16_F32   , 662   )
       ENUM( V_CVT_PK_U16_U32      , 663   )
       ENUM( V_CVT_PK_I16_I32      , 664   )

    END_TRANSLATOR(VectorInstructions,VOP3OpcodesExtra,V_INVALID)
        
    static VectorInstructions Translate_VOP3Opcodes( uint n )
    {
        if( n <= 255 )
            return Translate_VOPCOpcodes(n);
        if( n >= 256 && n <= 319 )
            return Translate_VOP2Opcodes(n-256);
        if( n >= 320 && n <= 447 )
            return Translate_VOP1Opcodes(n-320);

        return Translate_VOP3OpcodesExtra(n);
    }

    static VectorInstructions Translate_VINTERPOpcodes( uint n )
    {
        switch(n)
        {
        case 0: return V_INTERP_P1_F32  ; //  : D = P10 * S + P0; parameter interpolation.
        case 1: return V_INTERP_P2_F32  ; //  : D = P20 * S + D; parameter interpolation.
        case 2: return V_INTERP_MOV_F32 ; //  : D = {P10,P20,P0}[S]; parameter load.
        default: return V_INVALID;
        }
    }



    BEGIN_TRANSLATOR(DSInstructions,DSOpcodes)
        ENUM( DS_ADD_U32              , 0   )  
        ENUM( DS_SUB_U32              , 1   )
        ENUM( DS_RSUB_U32             , 2   )
        ENUM( DS_INC_U32              , 3   )
        ENUM( DS_DEC_U32              , 4   )
        ENUM( DS_MIN_I32              , 5   )
        ENUM( DS_MAX_I32              , 6   )
        ENUM( DS_MIN_U32              , 7   )
        ENUM( DS_MAX_U32              , 8   )
        ENUM( DS_AND_B32              , 9   )
        ENUM( DS_OR_B32               , 10   )
        ENUM( DS_XOR_B32              , 11   )
        ENUM( DS_MSKOR_B32            , 12   )
        ENUM( DS_WRITE_B32            , 13   )
        ENUM( DS_WRITE2_B32           , 14   )
        ENUM( DS_WRITE2ST64_B32       , 15   )
        ENUM( DS_CMPST_B32            , 16   )
        ENUM( DS_CMPST_F32            , 17   )
        ENUM( DS_MIN_F32              , 18   )
        ENUM( DS_MAX_F32              , 19   )
        ENUM( DS_NOP                  , 20   )
        ENUM( DS_ADD_F32              , 21   )
        ENUM( DS_WRITE_B8             , 30   )
        ENUM( DS_WRITE_B16            , 31   )
        ENUM( DS_ADD_RTN_U32          , 32   )
        ENUM( DS_SUB_RTN_U32          , 33   )
        ENUM( DS_RSUB_RTN_U32         , 34   )
        ENUM( DS_INC_RTN_U32          , 35   )
        ENUM( DS_DEC_RTN_U32          , 36   )
        ENUM( DS_MIN_RTN_I32          , 37   )
        ENUM( DS_MAX_RTN_I32          , 38   )
        ENUM( DS_MIN_RTN_U32          , 39   )
        ENUM( DS_MAX_RTN_U32          , 40   )
        ENUM( DS_AND_RTN_B32          , 41   )
        ENUM( DS_OR_RTN_B32           , 42   )
        ENUM( DS_XOR_RTN_B32          , 43   )
        ENUM( DS_MSKOR_RTN_B32        , 44   )
        ENUM( DS_WRXCHG_RTN_B32       , 45   )
        ENUM( DS_WRXCHG2_RTN_B32      , 46   )
        ENUM( DS_WRXCHG2ST64_RTN_B32  , 47   )
        ENUM( DS_CMPST_RTN_B32        , 48   )
        ENUM( DS_CMPST_RTN_F32        , 49   )
        ENUM( DS_MIN_RTN_F32          , 50   )
        ENUM( DS_MAX_RTN_F32          , 51   )
        ENUM( DS_WRAP_RTN_B32         , 52   )
        ENUM( DS_SWIZZLE_B32          , 61   )
        ENUM( DS_READ_B32             , 54   )
        ENUM( DS_READ2_B32            , 55   )
        ENUM( DS_READ2ST64_B32        , 56   )
        ENUM( DS_READ_I8              , 57   )
        ENUM( DS_READ_U8              , 58   )
        ENUM( DS_READ_I16             , 59   )
        ENUM( DS_READ_U16             , 60   )
        ENUM( DS_PERMUTE_B32          , 62   )
        ENUM( DS_BPERMUTE_B32         , 63   )
        ENUM( DS_ADD_U64              , 64   )
        ENUM( DS_SUB_U64              , 65   )
        ENUM( DS_RSUB_U64             , 66   )
        ENUM( DS_INC_U64              , 67   )
        ENUM( DS_DEC_U64              , 68   )
        ENUM( DS_MIN_I64              , 69   )
        ENUM( DS_MAX_I64              , 70   )
        ENUM( DS_MIN_U64              , 71   )
        ENUM( DS_MAX_U64              , 72   )
        ENUM( DS_AND_B64              , 73   )
        ENUM( DS_OR_B64               , 74   )
        ENUM( DS_XOR_B64              , 75   )
        ENUM( DS_MSKOR_B64            , 76   )
        ENUM( DS_WRITE_B64            , 77   )
        ENUM( DS_WRITE2_B64           , 78   )
        ENUM( DS_WRITE2ST64_B64       , 79   )
        ENUM( DS_CMPST_B64            , 80   )
        ENUM( DS_CMPST_F64            , 81   )
        ENUM( DS_MIN_F64              , 82   )
        ENUM( DS_MAX_F64              , 83   )
        ENUM( DS_ADD_RTN_U64          , 96   )
        ENUM( DS_SUB_RTN_U64          , 97   )
        ENUM( DS_RSUB_RTN_U64         , 98   )
        ENUM( DS_INC_RTN_U64          , 99   )
        ENUM( DS_DEC_RTN_U64          , 100  )
        ENUM( DS_MIN_RTN_I64          , 101  )
        ENUM( DS_MAX_RTN_I64          , 102  )
        ENUM( DS_MIN_RTN_U64          , 103  )
        ENUM( DS_MAX_RTN_U64          , 104  )
        ENUM( DS_AND_RTN_B64          , 105  )
        ENUM( DS_OR_RTN_B64           , 106  )
        ENUM( DS_XOR_RTN_B64          , 107  )
        ENUM( DS_MSKOR_RTN_B64        , 108  )
        ENUM( DS_WRXCHG_RTN_B64       , 109  )
        ENUM( DS_WRXCHG2_RTN_B64      , 110  )
        ENUM( DS_WRXCHG2ST64_RTN_B64  , 111  )
        ENUM( DS_CMPST_RTN_B64        , 112  )
        ENUM( DS_CMPST_RTN_F64        , 113  )
        ENUM( DS_MIN_RTN_F64          , 114  )
        ENUM( DS_MAX_RTN_F64          , 115  )
        ENUM( DS_READ_B64             , 118  )
        ENUM( DS_READ2_B64            , 119  )
        ENUM( DS_READ2ST64_B64        , 120  )
        ENUM( DS_CONDXCHG32_RTN_B64   , 126  )
        ENUM( DS_ADD_SRC2_U32         , 128  )
        ENUM( DS_SUB_SRC2_U32         , 129  )
        ENUM( DS_RSUB_SRC2_U32        , 130  )
        ENUM( DS_INC_SRC2_U32         , 131  )
        ENUM( DS_DEC_SRC2_U32         , 132  )
        ENUM( DS_MIN_SRC2_I32         , 133  )
        ENUM( DS_MAX_SRC2_I32         , 134  )
        ENUM( DS_MIN_SRC2_U32         , 135  )
        ENUM( DS_MAX_SRC2_U32         , 136  )
        ENUM( DS_AND_SRC2_B32         , 137  )
        ENUM( DS_OR_SRC2_B32          , 138  )
        ENUM( DS_XOR_SRC2_B32         , 139  )
        ENUM( DS_WRITE_SRC2_B32       , 140  )
        ENUM( DS_MIN_SRC2_F32         , 146  )
        ENUM( DS_MAX_SRC2_F32         , 147  )
        ENUM( DS_GWS_SEMA_RELEASE_ALL , 152  )
        ENUM( DS_GWS_INIT             , 153  )
        ENUM( DS_GWS_SEMA_V           , 154  )
        ENUM( DS_GWS_SEMA_BR          , 155  )
        ENUM( DS_GWS_SEMA_P           , 156  )
        ENUM( DS_GWS_BARRIER          , 157  )
        ENUM( DS_CONSUME              , 189  )
        ENUM( DS_APPEND               , 190  )
        ENUM( DS_ORDERED_COUNT        , 191  )
        ENUM( DS_ADD_SRC2_U64         , 192  )
        ENUM( DS_SUB_SRC2_U64         , 193  )
        ENUM( DS_RSUB_SRC2_U64        , 194  )
        ENUM( DS_INC_SRC2_U64         , 195  )
        ENUM( DS_DEC_SRC2_U64         , 196  )
        ENUM( DS_MIN_SRC2_I64         , 197  )
        ENUM( DS_MAX_SRC2_I64         , 198  )
        ENUM( DS_MIN_SRC2_U64         , 199  )
        ENUM( DS_MAX_SRC2_U64         , 200  )
        ENUM( DS_AND_SRC2_B64         , 201  )
        ENUM( DS_OR_SRC2_B64          , 202  )
        ENUM( DS_XOR_SRC2_B64         , 203  )
        ENUM( DS_WRITE_SRC2_B64       , 204  )
        ENUM( DS_MIN_SRC2_F64         , 210  )
        ENUM( DS_MAX_SRC2_F64         , 211  )
        ENUM( DS_WRITE_B96            , 222  )
        ENUM( DS_WRITE_B128           , 223  )
        ENUM( DS_CONDXCHG32_RTN_B128  , 253  )
        ENUM( DS_READ_B96             , 254  )
        ENUM( DS_READ_B128            , 255  )   
    END_TRANSLATOR(DSInstructions,DSOpcodes,DS_INVALID)


    BEGIN_TRANSLATOR(BufferInstructions,MUBUFFOpcodes)
       ENUM(BUFFER_LOAD_FORMAT_X        , 0  ) 
       ENUM(BUFFER_LOAD_FORMAT_XY       , 1  )  
       ENUM(BUFFER_LOAD_FORMAT_XYZ      , 2  ) 
       ENUM(BUFFER_LOAD_FORMAT_XYZW     , 3  ) 
       ENUM(BUFFER_STORE_FORMAT_X       , 4  ) 
       ENUM(BUFFER_STORE_FORMAT_XY      , 5  ) 
       ENUM(BUFFER_STORE_FORMAT_XYZ     , 6  ) 
       ENUM(BUFFER_STORE_FORMAT_XYZW    , 7  ) 
       ENUM(BUFFER_LOAD_FORMAT_D16_X    , 8  ) 
       ENUM(BUFFER_LOAD_FORMAT_D16_XY   , 9  ) 
       ENUM(BUFFER_LOAD_FORMAT_D16_XYZ  , 10 ) 
       ENUM(BUFFER_LOAD_FORMAT_D16_XYZW , 11 ) 
       ENUM(BUFFER_STORE_FORMAT_D16_X   , 12 ) 
       ENUM(BUFFER_STORE_FORMAT_D16_XY  , 13 ) 
       ENUM(BUFFER_STORE_FORMAT_D16_XYZ , 14 ) 
       ENUM(BUFFER_STORE_FORMAT_D16_XYZW, 15 ) 
       ENUM(BUFFER_LOAD_UBYTE           , 16 ) 
       ENUM(BUFFER_LOAD_SBYTE           , 17 ) 
       ENUM(BUFFER_LOAD_USHORT          , 18 ) 
       ENUM(BUFFER_LOAD_SSHORT          , 19 ) 
       ENUM(BUFFER_LOAD_DWORD           , 20 ) 
       ENUM(BUFFER_LOAD_DWORDX2         , 21 ) 
       ENUM(BUFFER_LOAD_DWORDX3         , 22 ) 
       ENUM(BUFFER_LOAD_DWORDX4         , 23 ) 
       ENUM(BUFFER_STORE_BYTE           , 24 ) 
       ENUM(BUFFER_STORE_SHORT          , 26 ) 
       ENUM(BUFFER_STORE_DWORD          , 28 ) 
       ENUM(BUFFER_STORE_DWORDX2        , 29 ) 
       ENUM(BUFFER_STORE_DWORDX3        , 30 ) 
       ENUM(BUFFER_STORE_DWORDX4        , 31 ) 
       ENUM(BUFFER_STORE_LDS_DWORD      , 61 ) 
       ENUM(BUFFER_WBINVL1              , 62 ) 
       ENUM(BUFFER_WBINVL1_VOL          , 63 ) 
       ENUM(BUFFER_ATOMIC_SWAP          , 64 ) 
       ENUM(BUFFER_ATOMIC_CMPSWAP       , 65 ) 
       ENUM(BUFFER_ATOMIC_ADD           , 66 ) 
       ENUM(BUFFER_ATOMIC_SUB           , 67 ) 
       ENUM(BUFFER_ATOMIC_SMIN          , 68 ) 
       ENUM(BUFFER_ATOMIC_UMIN          , 69 ) 
       ENUM(BUFFER_ATOMIC_SMAX          , 70 ) 
       ENUM(BUFFER_ATOMIC_UMAX          , 71 ) 
       ENUM(BUFFER_ATOMIC_AND           , 72 ) 
       ENUM(BUFFER_ATOMIC_OR            , 73 ) 
       ENUM(BUFFER_ATOMIC_XOR           , 74 ) 
       ENUM(BUFFER_ATOMIC_INC           , 75 ) 
       ENUM(BUFFER_ATOMIC_DEC           , 76 ) 
       ENUM(BUFFER_ATOMIC_SWAP_X2       , 96 ) 
       ENUM(BUFFER_ATOMIC_CMPSWAP_X2    , 97 ) 
       ENUM(BUFFER_ATOMIC_ADD_X2        , 98 ) 
       ENUM(BUFFER_ATOMIC_SUB_X2        , 99 ) 
       ENUM(BUFFER_ATOMIC_SMIN_X2       , 100) 
       ENUM(BUFFER_ATOMIC_UMIN_X2       , 101) 
       ENUM(BUFFER_ATOMIC_SMAX_X2       , 102) 
       ENUM(BUFFER_ATOMIC_UMAX_X2       , 103) 
       ENUM(BUFFER_ATOMIC_AND_X2        , 104) 
       ENUM(BUFFER_ATOMIC_OR_X2         , 105) 
       ENUM(BUFFER_ATOMIC_XOR_X2        , 106) 
       ENUM(BUFFER_ATOMIC_INC_X2        , 107) 
       ENUM(BUFFER_ATOMIC_DEC_X2        , 108) 
    END_TRANSLATOR(BufferInstructions,MUBUFFOpcodes,BUFFER_INVALID)

    BEGIN_TRANSLATOR(BufferInstructions,MTBUFFOpcodes)
         ENUM( TBUFFER_LOAD_FORMAT_X             , 0   )
         ENUM( TBUFFER_LOAD_FORMAT_XY            , 1   )
         ENUM( TBUFFER_LOAD_FORMAT_XYZ           , 2   )
         ENUM( TBUFFER_LOAD_FORMAT_XYZW          , 3   )
         ENUM( TBUFFER_STORE_FORMAT_X            , 4   )
         ENUM( TBUFFER_STORE_FORMAT_XY           , 5   )
         ENUM( TBUFFER_STORE_FORMAT_XYZ          , 6   )
         ENUM( TBUFFER_STORE_FORMAT_XYZW         , 7   )
         ENUM( TBUFFER_LOAD_FORMAT_D16_X         , 8   )
         ENUM( TBUFFER_LOAD_FORMAT_D16_XY        , 9   )
         ENUM( TBUFFER_LOAD_FORMAT_D16_XYZ       , 10  )
         ENUM( TBUFFER_LOAD_FORMAT_D16_XYZW      , 11  )
         ENUM( TBUFFER_STORE_FORMAT_D16_X        , 12  )
         ENUM( TBUFFER_STORE_FORMAT_D16_XY       , 13  )
         ENUM( TBUFFER_STORE_FORMAT_D16_XYZ      , 14  )
         ENUM( TBUFFER_STORE_FORMAT_D16_XYZW     , 15  )
    END_TRANSLATOR(BufferInstructions,MTBUFFOpcodes,TBUFFER_INVALID)

    BEGIN_TRANSLATOR(ImageInstructions,MIMGOpcodes)
        ENUM( IMAGE_LOAD                 , 0    )
        ENUM( IMAGE_LOAD_MIP             , 1    )
        ENUM( IMAGE_LOAD_PCK             , 2    )
        ENUM( IMAGE_LOAD_PCK_SGN         , 3    )
        ENUM( IMAGE_LOAD_MIP_PCK         , 4    )
        ENUM( IMAGE_LOAD_MIP_PCK_SGN     , 5    )
        ENUM( IMAGE_STORE                , 8    )
        ENUM( IMAGE_STORE_MIP            , 9    )
        ENUM( IMAGE_STORE_PCK            , 10   )
        ENUM( IMAGE_STORE_MIP_PCK        , 11   )
        ENUM( IMAGE_GET_RESINFO          , 14   )
        ENUM( IMAGE_ATOMIC_SWAP          , 15   )
        ENUM( IMAGE_ATOMIC_CMPSWAP       , 16   )
        ENUM( IMAGE_ATOMIC_ADD           , 17   )
        ENUM( IMAGE_ATOMIC_SUB           , 18   )
        ENUM( IMAGE_ATOMIC_SMIN          , 20   )
        ENUM( IMAGE_ATOMIC_UMIN          , 21   )
        ENUM( IMAGE_ATOMIC_SMAX          , 22   )
        ENUM( IMAGE_ATOMIC_UMAX          , 23   )
        ENUM( IMAGE_ATOMIC_AND           , 24   )
        ENUM( IMAGE_ATOMIC_OR            , 25   )
        ENUM( IMAGE_ATOMIC_XOR           , 26   )
        ENUM( IMAGE_ATOMIC_INC           , 27   )
        ENUM( IMAGE_ATOMIC_DEC           , 28   )
        ENUM( IMAGE_SAMPLE               , 32   )
        ENUM( IMAGE_SAMPLE_CL            , 33   )
        ENUM( IMAGE_SAMPLE_D             , 34   )
        ENUM( IMAGE_SAMPLE_D_CL          , 35   )
        ENUM( IMAGE_SAMPLE_L             , 36   )
        ENUM( IMAGE_SAMPLE_B             , 37   )
        ENUM( IMAGE_SAMPLE_B_CL          , 38   )
        ENUM( IMAGE_SAMPLE_LZ            , 39   )
        ENUM( IMAGE_SAMPLE_C             , 40   )
        ENUM( IMAGE_SAMPLE_C_CL          , 41   )
        ENUM( IMAGE_SAMPLE_C_D           , 42   )
        ENUM( IMAGE_SAMPLE_C_D_CL        , 43   )
        ENUM( IMAGE_SAMPLE_C_L           , 44   )
        ENUM( IMAGE_SAMPLE_C_B           , 45   )
        ENUM( IMAGE_SAMPLE_C_B_CL        , 46   )
        ENUM( IMAGE_SAMPLE_C_LZ          , 47   )
        ENUM( IMAGE_SAMPLE_O             , 48   )
        ENUM( IMAGE_SAMPLE_CL_O          , 49   )
        ENUM( IMAGE_SAMPLE_D_O           , 50   )
        ENUM( IMAGE_SAMPLE_D_CL_O        , 51   )
        ENUM( IMAGE_SAMPLE_L_O           , 52   )
        ENUM( IMAGE_SAMPLE_B_O           , 53   )
        ENUM( IMAGE_SAMPLE_B_CL_O        , 54   )
        ENUM( IMAGE_SAMPLE_LZ_O          , 55   )
        ENUM( IMAGE_SAMPLE_C_O           , 56   )
        ENUM( IMAGE_SAMPLE_C_CL_O        , 57   )
        ENUM( IMAGE_SAMPLE_C_D_O         , 58   )
        ENUM( IMAGE_SAMPLE_C_D_CL_O      , 59   )
        ENUM( IMAGE_SAMPLE_C_L_O         , 60   )
        ENUM( IMAGE_SAMPLE_C_B_O         , 61   )
        ENUM( IMAGE_SAMPLE_C_B_CL_O      , 62   )
        ENUM( IMAGE_SAMPLE_C_LZ_O        , 63   )
        ENUM( IMAGE_GATHER4              , 64   )
        ENUM( IMAGE_GATHER4_CL           , 65   )
        ENUM( IMAGE_GATHER4_L            , 66   )
        ENUM( IMAGE_GATHER4_B            , 67   )
        ENUM( IMAGE_GATHER4_B_CL         , 68   )
        ENUM( IMAGE_GATHER4_LZ           , 69   )
        ENUM( IMAGE_GATHER4_C            , 70   )
        ENUM( IMAGE_GATHER4_C_CL         , 71   )
        ENUM( IMAGE_GATHER4_C_L          , 76   )
        ENUM( IMAGE_GATHER4_C_B          , 77   )
        ENUM( IMAGE_GATHER4_C_B_CL       , 78   )
        ENUM( IMAGE_GATHER4_C_LZ         , 79   )
        ENUM( IMAGE_GATHER4_O            , 80   )
        ENUM( IMAGE_GATHER4_CL_O         , 81   )
        ENUM( IMAGE_GATHER4_L_O          , 84   )
        ENUM( IMAGE_GATHER4_B_O          , 85   )
        ENUM( IMAGE_GATHER4_B_CL_O       , 86   )
        ENUM( IMAGE_GATHER4_LZ_O         , 87   )
        ENUM( IMAGE_GATHER4_C_O          , 88   )
        ENUM( IMAGE_GATHER4_C_CL_O       , 89   )
        ENUM( IMAGE_GATHER4_C_L_O        , 92   )
        ENUM( IMAGE_GATHER4_C_B_O        , 93   )
        ENUM( IMAGE_GATHER4_C_B_CL_O     , 94   )
        ENUM( IMAGE_GATHER4_C_LZ_O       , 95   )
        ENUM( IMAGE_GET_LOD              , 96   )
        ENUM( IMAGE_SAMPLE_CD            , 104  )
        ENUM( IMAGE_SAMPLE_CD_CL         , 105  )
        ENUM( IMAGE_SAMPLE_C_CD          , 106  )
        ENUM( IMAGE_SAMPLE_C_CD_CL       , 107  )
        ENUM( IMAGE_SAMPLE_CD_O          , 108  )
        ENUM( IMAGE_SAMPLE_CD_CL_O       , 109  )
        ENUM( IMAGE_SAMPLE_C_CD_O        , 110  )
        ENUM( IMAGE_SAMPLE_C_CD_CL_O     , 111  )
    END_TRANSLATOR(ImageInstructions,MIMGOpcodes,IMAGE_INVALID)


    BEGIN_TRANSLATOR(TBufferNumberFormats,MTBUFF_NumberFormat)
        ENUM(NF_UNORM       , 0)
        ENUM(NF_SNORM       , 1)
        ENUM(NF_USCALED     , 2)
        ENUM(NF_SSCALED     , 3)
        ENUM(NF_UINT        , 4)
        ENUM(NF_SINT        , 5)
        ENUM(NF_FLOAT       , 7)
    END_TRANSLATOR(TBufferNumberFormats,MTBUFF_NumberFormat,NF_INVALID)

    BEGIN_TRANSLATOR(TBufferDataFormats,MTBUFF_DataFormat)
        ENUM( DF_8          , 1    )
        ENUM( DF_16         , 2    )
        ENUM( DF_8_8        , 3    )
        ENUM( DF_32         , 4    )
        ENUM( DF_16_16      , 5    )
        ENUM( DF_10_11_11   , 6    )
        ENUM( DF_10_10_10_2 , 8    )
        ENUM( DF_2_10_10_10 , 9    )
        ENUM( DF_8_8_8_8    , 10   )
        ENUM( DF_32_32      , 11   )
        ENUM( DF_16_16_16_16, 12   )
        ENUM( DF_32_32_32   , 13   )
        ENUM( DF_32_32_32_32, 14   )
    END_TRANSLATOR(TBufferDataFormats,MTBUFF_DataFormat,DF_INVALID)

    BEGIN_TRANSLATOR(ExportTargets, EXPTarget)
        ENUM( EXP_MRT0     ,0   )
        ENUM( EXP_MRT1     ,1   )
        ENUM( EXP_MRT2     ,2   )
        ENUM( EXP_MRT3     ,3   )
        ENUM( EXP_MRT4     ,4   )
        ENUM( EXP_MRT5     ,5   )
        ENUM( EXP_MRT6     ,6   )
        ENUM( EXP_MRT7     ,7   )
        ENUM( EXP_Z        ,8   )
        ENUM( EXP_NULL     ,9   )
        ENUM( EXP_POS0     ,12  )
        ENUM( EXP_POS1     ,13  )
        ENUM( EXP_POS2     ,14  )
        ENUM( EXP_POS3     ,15  )
        ENUM( EXP_PARAM0   ,32  )
        ENUM( EXP_PARAM1   ,33 ) 
        ENUM( EXP_PARAM2   ,34 ) 
        ENUM( EXP_PARAM3   ,35 ) 
        ENUM( EXP_PARAM4   ,36 ) 
        ENUM( EXP_PARAM5   ,37 ) 
        ENUM( EXP_PARAM6   ,38 ) 
        ENUM( EXP_PARAM7   ,39 ) 
        ENUM( EXP_PARAM8   ,40 ) 
        ENUM( EXP_PARAM9   ,41 ) 
        ENUM( EXP_PARAM10   ,42 ) 
        ENUM( EXP_PARAM11   ,43 ) 
        ENUM( EXP_PARAM12   ,44 ) 
        ENUM( EXP_PARAM13   ,45 ) 
        ENUM( EXP_PARAM14   ,46 ) 
        ENUM( EXP_PARAM15   ,47 ) 
        ENUM( EXP_PARAM16   ,48 ) 
        ENUM( EXP_PARAM17   ,49 ) 
        ENUM( EXP_PARAM18   ,50 ) 
        ENUM( EXP_PARAM19   ,51 ) 
        ENUM( EXP_PARAM20   ,52 ) 
        ENUM( EXP_PARAM21   ,53 ) 
        ENUM( EXP_PARAM22   ,54 ) 
        ENUM( EXP_PARAM23   ,55 ) 
        ENUM( EXP_PARAM24   ,56 ) 
        ENUM( EXP_PARAM25   ,57 ) 
        ENUM( EXP_PARAM26   ,58 ) 
        ENUM( EXP_PARAM27   ,59 ) 
        ENUM( EXP_PARAM28   ,60 ) 
        ENUM( EXP_PARAM29   ,61 ) 
        ENUM( EXP_PARAM30   ,62 ) 
        ENUM( EXP_PARAM31   ,63 ) 
    END_TRANSLATOR(ExportTargets,EXPTarget,EXP_INVALID)


    BEGIN_TRANSLATOR(Dests,ScalarDestOddballs)
        ENUM(SRC_FSCR_LO          , 102)
        ENUM(SRC_FSCR_HI          , 103)
        ENUM(SRC_XNACK_MASK_LO    , 104)
        ENUM(SRC_XNACK_MASK_HI    , 105)
        ENUM(SRC_VCC_LO           , 106)
        ENUM(SRC_VCC_HI           , 107)
        ENUM(SRC_TBA_LO           , 108)
        ENUM(SRC_TBA_HI           , 109)
        ENUM(SRC_TMA_LO           , 110)
        ENUM(SRC_TMA_HI           , 111)
        ENUM(SRC_M0               , 124)
        ENUM(SRC_EXEC_LO          , 126)
        ENUM(SRC_EXEC_HI          , 127)
    END_TRANSLATOR(Dests,ScalarDestOddballs, DEST_INVALID)

    BEGIN_TRANSLATOR(Sources,ScalarSourceOddballs)
        ENUM(SRC_FSCR_LO          , 102)
        ENUM(SRC_FSCR_HI          , 103)
        ENUM(SRC_XNACK_MASK_LO    , 104)
        ENUM(SRC_XNACK_MASK_HI    , 105)
        ENUM(SRC_VCC_LO           , 106)
        ENUM(SRC_VCC_HI           , 107)
        ENUM(SRC_TBA_LO           , 108)
        ENUM(SRC_TBA_HI           , 109)
        ENUM(SRC_TMA_LO           , 110)
        ENUM(SRC_TMA_HI           , 111)
        ENUM(SRC_M0               , 124)
        ENUM(SRC_EXEC_LO          , 126)
        ENUM(SRC_EXEC_HI          , 127)
        ENUM(SRC_C_ZERO           , 128)
        ENUM(SRC_CF_ONEHALF       , 240)
        ENUM(SRC_CF_MINUS_ONEHALF , 241)
        ENUM(SRC_CF_ONE           , 242)
        ENUM(SRC_CF_MINUS_ONE     , 243)
        ENUM(SRC_CF_TWO           , 244)
        ENUM(SRC_CF_MINUS_TWO     , 245)
        ENUM(SRC_CF_FOUR          , 246)
        ENUM(SRC_CF_MINUS_FOUR    , 247)
        ENUM(SRC_CF_INV_2PI       , 248)
        ENUM(SRC_VCCZ             , 251)
        ENUM(SRC_EXECZ            , 252)
        ENUM(SRC_SCC              , 253)
        ENUM(SRC_LDS_DIRECT       , 254)
        ENUM(SRC_LITERAL          , 255)
    END_TRANSLATOR(Sources,ScalarSourceOddballs,SRC_INVALID)

    Dests Translate_SDest( uint n )
    {
        if( n <= 101 )
            return (Dests)(DEST_SGPR_FIRST+n);
        else
            return Translate_ScalarDestOddballs(n);
    }

    Sources Translate_SSrc( uint n )
    {
        if( n <= 101 )
            return (Sources)(SRC_SGPR_FIRST+n); // SGPRs
        if( n >= 112 && n <= 123 )
            return (Sources)(SRC_TTMP_FIRST+(n-112)); // trap handler temps
        if( n >= 256 && n<= 511 )
            return (Sources)(SRC_VGPR_FIRST+(n-256)); // VGPRs
        if( n >= 129 && n <= 192 )
            return(Sources) (SRC_CI_POSITIVE_FIRST+(n-129));
        if( n >= 193 && n <= 208 )
            return (Sources) (SRC_CI_NEGATIVE_FIRST+(n-193));

        return Translate_ScalarSourceOddballs(n);
    }

    Sources Translate_VSrc( uint n )
    {
        return (Sources)(SRC_VGPR_FIRST+n);
    }
    Dests Translate_VDest( uint n )
    {
        return (Dests)(DEST_VGPR_FIRST+n);
    }

    class Instruction
    {
    public:

        uint32 ReadTrailingLiteral() const {
            const uint32* pDWORD = (const uint32*)this;
            return pDWORD[1];
        }

    protected:

        int SignExt( uint n, uint sign )
        {
            uint bit = (1<<sign);
        }

        uint ReadBits( uint hi, uint lo ) const
        {
            uint shift = lo % 32;
            uint mask = (1<<(hi-lo+1))-1;
            return ( ((const uint32*)this)[lo/32]>>shift)&mask;
        }
        bool ReadBit( uint bit ) const
        {
            uint shift = bit % 32;
            return (( ((const uint32*)this)[bit/32]>>shift)&1) != 0;
        }
    private:
    };


    class SOP2Instruction : public Instruction
    {
    public:
        bool HasLiteral() const { return ((GetSrc0() == SRC_LITERAL || GetSrc1() == SRC_LITERAL)); }
        uint   GetLength()              const { return 4 + 4*HasLiteral(); };
        ScalarInstructions   GetOpcode()const { return Translate_SOP2Opcodes(ReadBits( 29, 23 )); }
        Sources   GetSrc0()             const { return Translate_SSrc(ReadBits( 7,0 )); }
        Sources   GetSrc1()             const { return Translate_SSrc(ReadBits( 15,8)); }
        Dests   GetDest()               const { return Translate_SDest(ReadBits( 22,16)); }
    };

    class SOP1Instruction : public Instruction
    {
    public:
        bool HasLiteral() const { return (GetSrc0() == SRC_LITERAL); }
        uint   GetLength()                const { return 4 + 4*HasLiteral(); };
        ScalarInstructions   GetOpcode()  const { return Translate_SOP1Opcodes(ReadBits( 15,8 )); }
        Sources   GetSrc0()               const { return Translate_SSrc( ReadBits(7,0) ); }
        Dests     GetDest()               const { return Translate_SDest(ReadBits(22,16)); }
    };

    class SOPKInstruction : public Instruction
    {
    public:
        InstructionFormat GetFormat() const { return IF_SOPK; }
        bool HasLiteral() const { return (GetOpcode() == S_SETREG_IMM32_B32); }
        uint GetLength() const { return 4 + 4*HasLiteral(); };

        ScalarInstructions GetOpcode() const { return Translate_SOPKOpcodes(ReadBits( 27,23 )); }
        Dests GetDest()           const { return Translate_SDest( ReadBits(22,16) ); }

        int ReadSIMM16() const { return *((const int16*)this); }
        uint ReadSIMMBits( uint hi, uint lo) const { return ReadBits(hi,lo); }
        uint32 ReadIMM32() const { return ReadBits(63,32); }
        
        const uint8* GetBranchTarget() const
        {
            if( GetOpcode() != S_CBRANCH_I_FORK )
                return 0;
            int offset = this->ReadSIMM16();
            const uint32* pThis = (const uint32*)this;
            return (const uint8*) (pThis+1+offset);
        }

    };

    class SOPCInstruction : public Instruction
    {
    public:
        InstructionFormat GetFormat() const { return IF_SOPC; }
        bool HasLiteral() const { return (GetSrc0() == SRC_LITERAL || GetSrc1() == SRC_LITERAL);}
        uint GetLength()  const { return 4 + 4*HasLiteral(); };

        ScalarInstructions GetOpcode()  const { return Translate_SOPCOpcodes(ReadBits( 22,16 )); }
        Sources GetSrc0()            const { return Translate_SSrc(ReadBits(7,0)); }
        Sources GetSrc1()            const { return Translate_SSrc(ReadBits(15,8 )); }
        uint GetSrc1Raw() const { return ReadBits(15,8); }
        Dests GetDest() const { return GetOpcode() == S_SETVSKIP ? DEST_VSKIP : DEST_SCC; }
        
    };

    class SOPPInstruction : public Instruction
    {
    public:
        InstructionFormat GetFormat() const { return IF_SOPP; }
        uint GetLength() const  { return 4; }

        int ReadSIMM16() const { return *((const int16*)this); }
        uint ReadSIMMBits( uint hi, uint lo ) const { return ReadBits(hi,lo); }

        ScalarInstructions GetOpcode() const { return Translate_SOPPOpcodes(ReadBits( 22,16 )); }
        const uint8* GetBranchTarget() const
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
                    return (const uint8*) (pThis+1+offset);
                }
            default:
                return 0;
            }
            
        }

        
    };

    
    class SMEMInstruction : public Instruction
    {
    public:
        uint GetLength()       const { return 8; }
        ScalarMemoryInstructions GetOpcode()  const { return Translate_SMEMOpcodes(ReadBits(25,18)); }
        Dests GetSData()                 const { return Translate_SDest(ReadBits( 12,6)); }
        Dests GetSBase()                 const { return Translate_SDest(2*ReadBits( 5,0 )); } // base of SGPR pair containing address
        uint GetOffset()               const { return ReadBits(51,32); }
        bool IsOffsetIMM()             const { return ReadBit(17); }
        // SI and CI use dword offsets, but VI uses byte.
    };

    class VOP2Instruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_VOP2; }
        bool HasLiteral() const { 
            uint eOp = GetOpcode();
            return (GetSrc0() == SRC_LITERAL ||
                    eOp == V_MADAK_F32 || 
                    eOp == V_MADMK_F32 ||
                    eOp == V_MADMK_F16 ||
                    eOp == V_MADMK_F16 ); 
        }
        uint GetLength() const { return 4 + 4*HasLiteral(); }

        VectorInstructions GetOpcode() const { return Translate_VOP2Opcodes(ReadBits(30,25)); }
        Sources GetSrc0()           const { return Translate_SSrc(ReadBits(8,0)); }
        Sources GetVSrc1()          const { return Translate_VSrc(ReadBits(16,9)); }
        Dests GetVDst()           const   { return Translate_VDest(ReadBits(24,17)); }
        
        uint GetResultWidthInDWORDS() const { return 1; }
    };

    class VOP1Instruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_VOP1; }
        bool HasLiteral() const { 
            switch( GetOpcode())
            {
            default:
                return GetSrc0() == SRC_LITERAL;
            case V_NOP:
            case V_CLREXCP:             
                return false;
            }
        }
        uint GetLength() const 
        {
            return 4 + 4*HasLiteral();
        }

        VectorInstructions GetOpcode() const { return Translate_VOP1Opcodes( ReadBits(16,9) ); }
        Dests GetVDst()           const { return Translate_VDest( ReadBits(24,17) ); }
        Sources GetSrc0()         const { return Translate_SSrc(ReadBits(8,0)); }
    };

    class VOPCInstruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_VOPC; }
        bool HasLiteral() const { return (GetSrc0() == SRC_LITERAL); }
        uint GetLength() const { return 4 + 4*HasLiteral(); }

        VectorInstructions GetOpcode() const { return Translate_VOPCOpcodes(ReadBits(24,17)); }
        Sources GetSrc0()           const { return Translate_SSrc(ReadBits( 8,0 )); }
        Sources GetVSrc1()          const { return Translate_VSrc(ReadBits( 16,9 )); }
      
        uint GetOperandWidthInDWORDs() const;
        

        const Instruction* GetBranchTarget() const { return 0; }
        bool IsBranch() const { return false; }
    };

    class VINTERPInstruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_VINTERP; }
        uint GetLength() const { return 4; }

        VectorInstructions GetOpcode() const { return Translate_VINTERPOpcodes( ReadBits(17,16) ); }
        uint GetAttributeChannel() const { return ReadBits(9,8); };
        uint GetAttributeIndex() const { return ReadBits(15,10); };
        Sources GetVSrc0() const { return Translate_VSrc( ReadBits(7,0) ); }
        Dests GetVDst() const { return Translate_VDest(ReadBits(25,18)); }

    };

    class VOP3Instruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_VOP3; }
        uint GetLength() const { return 8; }
        
        VectorInstructions GetOpcode() const { return Translate_VOP3Opcodes(ReadBits(25,16));}
       
        Dests GetVDst() const { 
            uint dst = ReadBits(7,0);
            if( IsCompare() )
                return Translate_SDest(dst);
            return Translate_VDest(dst); 
        }
        Sources GetSrc0() const { return Translate_SSrc(ReadBits(40,32)); }
        Sources GetSrc1() const { return Translate_SSrc(ReadBits(49,41)); }
        Sources GetSrc2() const { return Translate_SSrc(ReadBits(58,50)); }
        uint GetSourceNegateMask() const { return ReadBits(63,61); };
        uint GetOMod() const { return ReadBits(60,59); }
     
        bool IsCompare() const { return ReadBits(25,16) < 255; } // opcodes 0-255 are vcmps.

        bool IsVOP3bOp() const
        {
            // NOTE: ISA Doc, section 6, claims that vcmp is vop3b.  But AMD disassembler thinks its VOP3a
            //
            switch( GetOpcode() )
            {
            case V_ADD_I32      :   //  : D.u = S0.u + S1.u; VCC=carry-out (VOP3:sgpr=carry-out).
            case V_SUB_I32      :   //  : D.u = S0.u - S1.u; VCC=carry-out (VOP3:sgpr=carry-out).
            case V_SUBREV_I32   :   //  : D.u = S1.u - S0.u; VCC=carry-out (VOP3:sgpr=carry-out).
            case V_ADD_U32      :
            case V_SUB_U32      :
            case V_SUBREV_U32   :
            case V_ADDC_U32     :   //  : D.u = S0.u + S1.u + VCC; VCC=carry-out (VOP3:sgpr=carryout,S2.u=carry-in).
            case V_SUBB_U32     :   //  : D.u = S0.u - S1.u - VCC; VCC=carry-out (VOP3:sgpr=carry-out,S2.u=carry-in).
            case V_SUBBREV_U32  :   //  : D.u = S1.u - S0.u - VCC; VCC=carry-out (VOP3:sgpr=carryout,S2.u=carry-in).
            case V_DIV_SCALE_F32:   //  = D.f = Special case divide preop and flags(s0.f = Quotient,s1.f = Denominator, s2.f = Numerator) s0 must equal s1 or s2.
            case V_DIV_SCALE_F64:   //  = D.d = Special case divide preop and flags(s0.d = Quotient,s1.d = Denominator, s2.d = Numerator) s0 must equal s1 or s2.
                return true;
            default:
                return false;
            }
        }

        // VOP3a only
        uint GetSourceAbsMask() const { return ReadBits(10,8); }
        bool GetClamp() const { return ReadBit(15); }

        // VOP3b only
        Dests GetSDst() const { return Translate_SDest(ReadBits(14,8)); }

    };

    class MUBUFFInstruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_MUBUFF; }
        BufferInstructions GetOpcode() const { return Translate_MUBUFFOpcodes(ReadBits(24,18)); }
        uint GetLength() const { return 8; }
        
        uint GetOffset() const { return ReadBits(11,0); }
        Sources GetVAddr() const { return Translate_VSrc(ReadBits(39,32)); }
        Sources GetVData() const { return Translate_VSrc(ReadBits(47,40)); }
        Sources GetSResource() const { return (Sources)(SRC_SGPR_FIRST+4*ReadBits(52,48)); }
        Sources GetSOffset() const { return Translate_SSrc(ReadBits(63,56)); }
        
        bool IsOffN() const     { return ReadBit(12); }
        bool IsIdxN() const     { return ReadBit(13); }
        bool IsAddr64() const   { return false; } // this bit was removed in VI
        bool IsDirectToLDS() const { return ReadBit(16); }
        bool GetSLCBit() const { return ReadBit(17); }
        bool GetGLCBit() const { return ReadBit(14); }
        bool GetTFEBit() const { return ReadBit(55); }
        
    };

    class MTBUFFInstruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_MTBUFF; }
        BufferInstructions GetOpcode() const { return Translate_MTBUFFOpcodes(ReadBits(18,15)); }
        uint GetLength() const { return 8; }

        uint GetResultWidthInDWORDS() const ;

        TBufferNumberFormats GetNumberFormat() const { return Translate_MTBUFF_NumberFormat( ReadBits(25,23) ); }
        TBufferDataFormats GetDataFormat() const { return Translate_MTBUFF_DataFormat( ReadBits(22,19) ); }

        uint GetOffset() const    { return ReadBits(11,0); }
        Sources GetVAddr() const     { return Translate_VSrc(ReadBits(39,32)); }
        Sources GetVData() const     { return Translate_VSrc(ReadBits(47,40)); }
        Sources GetSResource() const { return (Sources)(SRC_SGPR_FIRST+4*ReadBits(52,48)); }
        Sources GetSOffset() const { return Translate_SSrc(ReadBits(63,56)); }
        
        bool IsOffN() const     { return ReadBit(12); }
        bool IsIdxN() const     { return ReadBit(13); }
        bool IsAddr64() const   { return false; } // removed in VI
        bool GetSLCBit() const  { return ReadBit(54); }
        bool GetGLCBit() const  { return ReadBit(14); }
        bool GetTFEBit() const  { return ReadBit(55); }
        
    };
    
   

    class MIMGInstruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_MIMG; }
        ImageInstructions GetOpcode() const { return Translate_MIMGOpcodes( ReadBits(24,18) ); }
        uint GetLength() const { return 8; }

        bool IsSLC() const { return ReadBit(25); }
        bool IsGLC() const { return ReadBit(13); }
        bool IsTFE() const { return ReadBit(16); }
        bool IsLWE() const { return ReadBit(17); }
        bool IsArray() const { return ReadBit(14); }
        bool IsUnormalized() const { return ReadBit(12); }
        bool IsRes256() const { return !ReadBit(15);};
        bool IsD16() const { return ReadBit(63); }
       
        uint GetDMask() const { return ReadBits(11,8); }
        Sources GetSSampler() const  { return (Sources)(SRC_SGPR_FIRST+4*ReadBits(57,53)); }
        Sources GetSResource() const { return (Sources)(SRC_SGPR_FIRST+4*ReadBits(52,48)); }
        Sources GetVData() const { return Translate_VSrc( ReadBits(47,40) ); }
        Sources GetVAddr() const { return Translate_VSrc( ReadBits(39,32) ); }

    };

    class DSInstruction : public Instruction
    {
    public:
        uint GetOffset0() const { return ReadBits(7,0); }  // Instructions can have one 16 bit offset field or two 8-bit offset fields
        uint GetOffset1() const { return ReadBits(15,8); }
        uint GetOffset16() const { return ReadBits(15,0); }
        DSInstructions GetOpcode() const { return Translate_DSOpcodes(ReadBits(24,17)); }
        Dests GetVDest()  const { return Translate_VDest(ReadBits(63,56)); }
        Sources GetVData0() const { return Translate_VSrc(ReadBits(47,40)); }
        Sources GetVData1() const { return Translate_VSrc(ReadBits(55,48)); }
        Sources GetVAddr()  const { return Translate_VSrc(ReadBits(39,32)); }
        bool IsGDS()     const { return ReadBit(16); }  
    };

    class EXPInstruction : public Instruction
    {
    public:
        InstructionFormat GetFormat() const { return IF_EXP; }
        uint GetLength() const { return 8; }
  
        bool GetCompressBit() const { return ReadBit(10); }
        bool GetDoneBit() const { return ReadBit(11); }
        bool GetValidMaskBit() const { return ReadBit(12); }

        ExportTargets GetTarget() const { return Translate_EXPTarget(ReadBits(9,4)); }
        uint GetExportMask() const { return ReadBits(3,0); }
        Sources GetVSrc0() const { return Translate_VSrc(ReadBits(39,32)); }
        Sources GetVSrc1() const { return Translate_VSrc(ReadBits(47,40)); }
        Sources GetVSrc2() const { return Translate_VSrc(ReadBits(55,48)); }
        Sources GetVSrc3() const { return Translate_VSrc(ReadBits(63,56)); }
    };


}}




namespace GCN
{
    InstructionFormat GCN3Decoder::ReadInstructionFormat( const uint8* pLocation )
    {
        uint32 n = *((const uint32*)pLocation);

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
        //   vinterp is    1100 10 (according to doc) but 1101 10 according to sane reality
        //   smem    is    1100 00 (changed from 1100 0... in SI)
        //   vop3    is    1101 00  
        //   flat    is    1101 11
        //   ds      is    1101 10
        //   mubuff  is    1110 00
        //   mtbuff  is    1110 10
        //   mimg    is    1111 00
        //   exp     is    1100 01 (changed from 1111 10... in SI)

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
                case 0xD4000000:                    // 1101 10.....
                case 0xC8000000: return IF_VINTERP; // 1100 10.. ....                
                case 0xD0000000: return IF_VOP3;    // 1101 00.. ...
                case 0xDC000000: return IF_FLAT;    // 1101 11.. ....
                case 0xD8000000: return IF_DS;      // 1101 10.. ....
                case 0xE0000000: return IF_MUBUFF;  // 1110 00..
                case 0xE8000000: return IF_MTBUFF;  // 1110 10..
                case 0xF0000000: return IF_MIMG;    // 1111 00..
                case 0xC4000000: return IF_EXP;     // 1100 01..
                case 0xC0000000: return IF_SMEM;    // 1100 00..
                default:
                    switch( n & (0xF8000000) ) // 1111 1000 (5bit)
                    {
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

    size_t GCN3Decoder::DetermineInstructionLength( const uint8* pLocation, InstructionFormat eEncoding )
    {
        using namespace _GCN3Decoder_INTERNAL;
        switch( eEncoding )
        {
        case IF_VOP2:       return ((const VOP2Instruction*)pLocation)->GetLength();
        case IF_VOP1:       return ((const VOP1Instruction*)pLocation)->GetLength();
        case IF_VOPC:       return ((const VOPCInstruction*)pLocation)->GetLength();
        case IF_SOP2:       return ((const SOP2Instruction*)pLocation)->GetLength();
        case IF_SOPK:       return ((const SOPKInstruction*)pLocation)->GetLength();
        case IF_SOP1:       return ((const SOP1Instruction*)pLocation)->GetLength();
        case IF_SOPC:       return ((const SOPCInstruction*)pLocation)->GetLength();

        case IF_SOPP:
        case IF_VINTERP:    
            return 4;

        case IF_DS:         
        case IF_VOP3:       
        case IF_MTBUFF:     
        case IF_MUBUFF:     
        case IF_MIMG:       
        case IF_EXP:        
        case IF_FLAT:   
        case IF_SMEM:
            return 8;
                
        case IF_UNKNOWN:
        default:
            return 0;
        }
    }

    void GCN3Decoder::Decode( Instruction* pInst, const uint8* pLocation, InstructionFormat eEncoding ) 
    {   
        memset(pInst,0,sizeof(*pInst));


        using namespace _GCN3Decoder_INTERNAL;
        switch( eEncoding )
        {
        case IF_SOP2:       
            {
                auto* pIt = (const SOP2Instruction*)pLocation;
                auto& Fields          = pInst->Fields.Scalar;              
                pInst->m_eClass       = IC_SCALAR;
                Fields.m_Dest         = pIt->GetDest();
                Fields.m_eOpcode      = pIt->GetOpcode();
                Fields.m_nSourceCount = 2;
                Fields.m_Sources[0]   = pIt->GetSrc0();
                Fields.m_Sources[1]   = pIt->GetSrc1();
                if( pIt->HasLiteral() )
                    Fields.m_Literal.UInt = pIt->ReadTrailingLiteral();
            }
            break;
        case IF_SOPK: 
            {
                auto* pIt = (const SOPKInstruction*) pLocation;
                auto& Fields = pInst->Fields.Scalar;
                pInst->m_eClass = IC_SCALAR;
                Fields.m_Dest          = pIt->GetDest();
                Fields.m_eOpcode       = pIt->GetOpcode();
                Fields.m_nSourceCount  = 0;
                Fields.m_nSIMM16       = pIt->ReadSIMM16();
                Fields.m_pBranchTarget = pIt->GetBranchTarget();
                if( pIt->HasLiteral() )
                    Fields.m_Literal.UInt = pIt->ReadTrailingLiteral();
            }
            break;
        case IF_SOP1:       
            {
                auto* pIt = (const SOP1Instruction*) pLocation;
                auto& Fields = pInst->Fields.Scalar;
                pInst->m_eClass = IC_SCALAR;
                Fields.m_Dest         = pIt->GetDest();
                Fields.m_eOpcode      = pIt->GetOpcode();
                Fields.m_nSourceCount = 1;
                Fields.m_Sources[0]   = pIt->GetSrc0();
                if( pIt->HasLiteral())
                    Fields.m_Literal.UInt = pIt->ReadTrailingLiteral();

            }
            break;
        case IF_SOPC: 
            {
                auto* pIt = (const SOPCInstruction*) pLocation;
                auto& Fields = pInst->Fields.Scalar;
                pInst->m_eClass = IC_SCALAR;
                Fields.m_eOpcode = pIt->GetOpcode();
                Fields.m_Dest = pIt->GetDest();
                Fields.m_nSourceCount = 2;
                Fields.m_Sources[0] = pIt->GetSrc0();
                Fields.m_Sources[1] = pIt->GetSrc1();
                if( pIt->HasLiteral() )
                    Fields.m_Literal.UInt = pIt->ReadTrailingLiteral();

                //   - S_SET_GPR_IDX_ON (sopc) is a special little fella.  His Src1 field gets ripped out and used as an immediate
                //       * Move to the SIMM16 field in the scalar instruction class
                if( Fields.m_eOpcode == S_SET_GPR_IDX_ON )
                   Fields.m_nSIMM16 = pIt->GetSrc1Raw();
                
            }
            break;
        case IF_SOPP:    
            {
                auto* pIt = (const SOPPInstruction*) pLocation;
                auto& Fields = pInst->Fields.Scalar;
                pInst->m_eClass = IC_SCALAR;
                Fields.m_eOpcode= pIt->GetOpcode();
                Fields.m_Dest = DEST_INVALID;
                Fields.m_nSourceCount = 0;
                Fields.m_nSIMM16 = pIt->ReadSIMM16();
                Fields.m_pBranchTarget = pIt->GetBranchTarget();
            }
            break;
        case IF_SMEM: 
            {
                auto* pIt = (const SMEMInstruction*) pLocation;
                auto& Fields = pInst->Fields.ScalarMem;
                pInst->m_eClass = IC_SCALAR_MEM;
                Fields.m_bIsOffsetIMM = pIt->IsOffsetIMM();
                Fields.m_eOpcode      = pIt->GetOpcode();
                Fields.m_Dest         = pIt->GetSData();
                Fields.m_nBaseReg     = pIt->GetSBase();
                Fields.m_nOffset      = pIt->GetOffset();
                Fields.m_bIsGLC       = false;
            }
            break;

        case IF_VOP2: 
            {
                auto* pIt = (const VOP2Instruction*) pLocation;
                auto& Fields = pInst->Fields.Vector;
                pInst->m_eClass = IC_VECTOR;
                Fields.m_eOpcode = pIt->GetOpcode();
                Fields.m_nSrcCount  = 2;
                Fields.m_nDestCount = 1;
                Fields.m_Sources[0] = pIt->GetSrc0();
                Fields.m_Sources[1] = pIt->GetVSrc1();
                Fields.m_Dests[0] = pIt->GetVDst();
                if( pIt->HasLiteral() )
                    Fields.m_Literal.UInt = pIt->ReadTrailingLiteral();

                // convert float16 literals to float32 ones to make the disassembler's job easier
                switch( Fields.m_eOpcode )
                {
                case V_MADAK_F16:
                case V_MADMK_F16:
                    {
                        uint32 f16 = Fields.m_Literal.UInt;
                        uint32 sign  = (f16 & 0x8000)<<16;
                        int   exp    = ((f16 & 0x7C00)>>5)-15;
                        uint mant    = f16 & 0x3fff;

                        switch( exp ) // nan
                        {
                        default:    // normalized
                            Fields.m_Literal.UInt = sign|((exp+127)<<23)|(mant<<13);
                            break;
                        case 16:    // inf/nan
                            Fields.m_Literal.UInt = sign|(0xff<<23)|(mant<<13);
                            break;
                        case -15:  // FP16 denorm
                            Fields.m_Literal.Float  = ldexpf(sign?-1.0f:1.0f,-14) * (mant/1023.0f);
                            break;
                        }

                    }
                    break;
                }
            }
            break;
        case IF_VOP1:       
            {
                auto* pIt = (const VOP1Instruction*) pLocation;
                auto& Fields = pInst->Fields.Vector;
                pInst->m_eClass = IC_VECTOR;
                Fields.m_eOpcode = pIt->GetOpcode();
                Fields.m_nSrcCount  = 1;
                Fields.m_nDestCount = 1;
                Fields.m_Sources[0] = pIt->GetSrc0();
                Fields.m_Dests[0] = pIt->GetVDst();
                if( pIt->HasLiteral() )
                    Fields.m_Literal.UInt = pIt->ReadTrailingLiteral();
            }
            break;
        case IF_VOPC:       
            {
                auto* pIt = (const VOPCInstruction*) pLocation;
                auto& Fields = pInst->Fields.Vector;
                pInst->m_eClass = IC_VECTOR;                
                Fields.m_eOpcode = pIt->GetOpcode();
                Fields.m_nSrcCount  = 2;
                Fields.m_nDestCount = 1;
                Fields.m_Sources[0] = pIt->GetSrc0();
                Fields.m_Sources[1] = pIt->GetVSrc1();
                Fields.m_Dests[0]   = DEST_VCC;
                if( pIt->HasLiteral() )
                    Fields.m_Literal.UInt = pIt->ReadTrailingLiteral();
            }
            break;
        case IF_VOP3:      
            {
                auto* pIt = (const VOP3Instruction*) pLocation;
                auto& Fields = pInst->Fields.Vector;
                Fields.m_eOpcode = pIt->GetOpcode();
                pInst->m_eClass = IC_VECTOR;
                
                // TODO: Detect VOP3-encoded interp ops here and treat them as VC_INTERP?
                //   so we can get better disassembly output?
                //   Are they allowed to use abs, clamp, and all that goodness??
                //   How do the attribute fields get encoded?  Doc is not helpful here...

                if( pIt->IsVOP3bOp() )
                {
                    Fields.m_nDestCount = 2;
                    Fields.m_Dests[0] = pIt->GetVDst();
                    Fields.m_Dests[1] = pIt->GetSDst();
                }
                else
                {
                    Fields.m_nDestCount = 1;
                    Fields.m_Dests[0] = pIt->GetVDst();
                    Fields.m_nSourceAbs = pIt->GetSourceAbsMask();
                }
                     
                Fields.m_nClamp = pIt->GetClamp();
                Fields.m_nSourceNegate= pIt->GetSourceNegateMask();
                Fields.m_nOMOD = pIt->GetOMod();
                Fields.m_nSrcCount  = GetSourceCountForInstruction( Fields.m_eOpcode );
                Fields.m_Sources[0] = pIt->GetSrc0();
                Fields.m_Sources[1] = pIt->GetSrc1();
                Fields.m_Sources[2] = pIt->GetSrc2();
            }
            break;

        case IF_VINTERP:
            {
                auto* pIt = (const VINTERPInstruction*) pLocation;
                auto& Fields = pInst->Fields.Interp;
                pInst->m_eClass = IC_VECTOR_INTERP;
                Fields.m_eOpcode = pIt->GetOpcode();
                Fields.m_nAttributeChannel = pIt->GetAttributeChannel();
                Fields.m_nAttributeIndex = pIt->GetAttributeIndex();
                Fields.m_VDst = pIt->GetVDst();
                Fields.m_VSrc = pIt->GetVSrc0();
            }
            break;

        case IF_EXP:        
            {
                auto* pIt = (const EXPInstruction*) pLocation;
                auto& Fields = pInst->Fields.Export;
                pInst->m_eClass = IC_EXPORT;                
                Fields.m_ChannelMask = pIt->GetExportMask();
                Fields.m_Compress = pIt->GetCompressBit();
                Fields.m_Done = pIt->GetDoneBit();
                Fields.m_eTarget    = pIt->GetTarget();
                Fields.m_ValidMask  = pIt->GetValidMaskBit();
                Fields.m_Sources[0] = pIt->GetVSrc0();
                Fields.m_Sources[1] = pIt->GetVSrc1();
                Fields.m_Sources[2] = pIt->GetVSrc2();
                Fields.m_Sources[3] = pIt->GetVSrc3();
            }
            break;
        
        case IF_DS:       
            {
                auto* pIt = (const DSInstruction*) pLocation;
                auto& Fields = pInst->Fields.DS;
                pInst->m_eClass = IC_DS;
                Fields.m_bGDS = pIt->IsGDS();
                Fields.m_Dest = pIt->GetVDest();
                Fields.m_eOpcode = pIt->GetOpcode();
                Fields.m_nOffset0 = pIt->GetOffset0();
                Fields.m_nOffset1 = pIt->GetOffset1();
                Fields.m_nOffset16 = pIt->GetOffset16();
                Fields.m_VAddr = pIt->GetVAddr();
                Fields.m_VData0 = pIt->GetVData0();
                Fields.m_VData1 = pIt->GetVData1();
            }
            break;

        case IF_MTBUFF:     
            {
                auto* pIt = (const MTBUFFInstruction*) pLocation;
                auto& Fields = pInst->Fields.Buffer;
                pInst->m_eClass = IC_BUFFER;
                
                Fields.m_bAddr64 = pIt->IsAddr64();
                Fields.m_bGLC = pIt->GetGLCBit();
                Fields.m_bIdxN = pIt->IsIdxN();
                Fields.m_bLDSDirect = 0;
                Fields.m_bOffN = pIt->IsOffN();
                Fields.m_bSLC = pIt->GetSLCBit();
                Fields.m_bTFE = pIt->GetTFEBit();
                Fields.m_eDataFormat = pIt->GetDataFormat();
                Fields.m_eNumberFormat = pIt->GetNumberFormat();
                Fields.m_eOpcode = pIt->GetOpcode();
                Fields.m_nOffset = pIt->GetOffset();
                Fields.m_SOffset = pIt->GetSOffset();
                Fields.m_SResource = pIt->GetSResource();
                Fields.m_VAddr = pIt->GetVAddr();
                Fields.m_VData = pIt->GetVData();
            }
            break;

        case IF_MUBUFF:    
            {
                auto* pIt = (const MUBUFFInstruction*) pLocation;
                auto& Fields = pInst->Fields.Buffer;
                pInst->m_eClass = IC_BUFFER;
                Fields.m_bAddr64 = pIt->IsAddr64();
                Fields.m_bGLC = pIt->GetGLCBit();
                Fields.m_bIdxN = pIt->IsIdxN();
                Fields.m_bLDSDirect = pIt->IsDirectToLDS();
                Fields.m_bOffN = pIt->IsOffN();
                Fields.m_bSLC = pIt->GetSLCBit();
                Fields.m_bTFE = pIt->GetTFEBit();
                Fields.m_eOpcode = pIt->GetOpcode();
                Fields.m_nOffset = pIt->GetOffset();
                Fields.m_SOffset = pIt->GetSOffset();
                Fields.m_SResource = pIt->GetSResource();
                Fields.m_VAddr = pIt->GetVAddr();
                Fields.m_VData = pIt->GetVData();

                
                //  - BUFFER_STORE_LDS_DWORD
                //          set the LDS bit, so that disassembler treats it like an LDS direct read
                ///         Not sure if this is supposed to use M0 or a vgpr and of course the doc don't say
                if( Fields.m_eOpcode == BUFFER_STORE_LDS_DWORD )
                    Fields.m_bLDSDirect = true;
            }
            break;

        case IF_MIMG:     
            {
                auto* pIt = (const MIMGInstruction*) pLocation;
                auto& Fields = pInst->Fields.Image;
                pInst->m_eClass = IC_IMAGE;
                Fields.m_bArray = pIt->IsArray();
                Fields.m_bGLC = pIt->IsGLC();
                Fields.m_bLWE = pIt->IsLWE();
                Fields.m_bRes256 = pIt->IsRes256();
                Fields.m_bSLC = pIt->IsSLC();
                Fields.m_bTFE = pIt->IsTFE();
                Fields.m_bUnnormalized = pIt->IsUnormalized();
                Fields.m_eOpcode = pIt->GetOpcode();
                Fields.m_nDMask = pIt->GetDMask();
                Fields.m_SResource = pIt->GetSResource();
                Fields.m_SSampler = pIt->GetSSampler();
                Fields.m_VAddr = pIt->GetVAddr();
                Fields.m_VData = pIt->GetVData();
                Fields.m_bIsD16 = pIt->IsD16();
            }
            break;

        case IF_FLAT:               
        default:
            return;
        }




    }




}