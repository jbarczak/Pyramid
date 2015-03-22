//
//    Helper class responsible for decoding GCN instructions into a standardized form
//
//     A decoder abstraction is necessary because AMD decided to change the 
//       microcode formats and opcode numbers for GCN3.   
//
//      Copyright 2015 Joshua Barczak, all rights reserved.
//
#pragma unmanaged
#include "GCNEnums.h"
#include "GCNIsa.h"
#include "GCN1Decoder.h"

#include <string.h>

namespace GCN{
namespace _GCN1Decoder_INTERNAL
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
        ENUM(S_ADD_U32              ,0 )
        ENUM(S_SUB_U32              ,1 )
        ENUM(S_ADD_I32              ,2 )
        ENUM(S_SUB_I32              ,3 )
        ENUM(S_ADDC_U32             ,4 )
        ENUM(S_SUBB_U32             ,5 )
        ENUM(S_MIN_I32              ,6 )
        ENUM(S_MIN_U32              ,7 )
        ENUM(S_MAX_I32              ,8 )
        ENUM(S_MAX_U32              ,9 )
        ENUM(S_CSELECT_B32          ,10)
        ENUM(S_CSELECT_B64          ,11)                        
        ENUM(S_AND_B32              ,14)
        ENUM(S_AND_B64              ,15)
        ENUM(S_OR_B32               ,16)
        ENUM(S_OR_B64               ,17)
        ENUM(S_XOR_B32              ,18)
        ENUM(S_XOR_B64              ,19)
        ENUM(S_ANDN2_B32            ,20)
        ENUM(S_ANDN2_B64            ,21)
        ENUM(S_ORN2_B32             ,22)
        ENUM(S_ORN2_B64             ,23)
        ENUM(S_NAND_B32             ,24)
        ENUM(S_NAND_B64             ,25)
        ENUM(S_NOR_B32              ,26)
        ENUM(S_NOR_B64              ,27)
        ENUM(S_XNOR_B32             ,28)
        ENUM(S_XNOR_B64             ,29)
        ENUM(S_LSHL_B32             ,30)
        ENUM(S_LSHL_B64             ,31)
        ENUM(S_LSHR_B32             ,32)
        ENUM(S_LSHR_B64             ,33)
        ENUM(S_ASHR_I32             ,34)
        ENUM(S_ASHR_I64             ,35)
        ENUM(S_BFM_B32              ,36)
        ENUM(S_BFM_B64              ,37)
        ENUM(S_MUL_I32              ,38)
        ENUM(S_BFE_U32              ,39)
        ENUM(S_BFE_I32              ,40)
        ENUM(S_BFE_U64              ,41)
        ENUM(S_BFE_I64              ,42)
        ENUM(S_CBRANCH_G_FORK       ,43)
        ENUM(S_ABSDIFF_I32          ,44)
    END_TRANSLATOR(ScalarInstructions,SOP2Opcodes,S_INVALID);


    BEGIN_TRANSLATOR(ScalarInstructions,SOPKOpcodes)
        ENUM( S_MOVK_I32              ,0 )
        ENUM( S_CMOVK_I32             ,2 )
        ENUM( S_CMPK_EQ_I32           ,3 )
        ENUM( S_CMPK_LG_I32           ,4 )
        ENUM( S_CMPK_GT_I32           ,5 )
        ENUM( S_CMPK_GE_I32           ,6 )
        ENUM( S_CMPK_LT_I32           ,7 )
        ENUM( S_CMPK_LE_I32           ,8 )
        ENUM( S_CMPK_EQ_U32           ,9 )
        ENUM( S_CMPK_LG_U32           ,10)
        ENUM( S_CMPK_GT_U32           ,11)
        ENUM( S_CMPK_GE_U32           ,12)
        ENUM( S_CMPK_LT_U32           ,13)
        ENUM( S_CMPK_LE_U32           ,14)
        ENUM( S_ADDK_I32              ,15)
        ENUM( S_MULK_I32              ,16)
        ENUM( S_CBRANCH_I_FORK        ,17)
        ENUM( S_GETREG_B32            ,18) 
        ENUM( S_SETREG_B32            ,19) 
        ENUM( S_SETREG_IMM32_B32      ,21)
    END_TRANSLATOR(ScalarInstructions,SOPKOpcodes,S_INVALID);

    BEGIN_TRANSLATOR(ScalarInstructions,SOP1Opcodes)
        ENUM(S_MOV_B32             ,   3 ) //: D.u = S0.u.
        ENUM(S_MOV_B64             ,   4 ) //: D/u = S0.u.
        ENUM(S_CMOV_B32            ,   5 ) //: if(SCC) D.u = S0.u; else NOP.
        ENUM(S_CMOV_B64            ,   6 ) //: if(SCC) D.u = S0.u; else NOP.
        ENUM(S_NOT_B32             ,   7 ) //: D.u = ~S0.u SCC = 1 if result non-zero.
        ENUM(S_NOT_B64             ,   8 ) //: D.u = ~S0.u SCC = 1 if result non-zero.
        ENUM(S_WQM_B32             ,   9 ) //: D.u = WholeQuadMode(S0.u). SCC = 1 if result is non-zero.
        ENUM(S_WQM_B64             ,   10) //: D.u = WholeQuadMode(S0.u). SCC = 1 if result is non-zero.
        ENUM(S_BREV_B32            ,   11) //: D.u = S0.u[0:31] (reverse bits).
        ENUM(S_BREV_B64            ,   12) //: D.u = S0.u[0:63] (reverse bits).
        ENUM(S_BCNT0_I32_B32       ,   13) //: D.i = CountZeroBits(S0.u). SCC = 1 if result is non-zero.
        ENUM(S_BCNT0_I32_B64       ,   14) //: D.i = CountZeroBits(S0.u). SCC = 1 if result is non-zero.
        ENUM(S_BCNT1_I32_B32       ,   15) //: D.i = CountOneBits(S0.u). SCC = 1 if result is non-zero.
        ENUM(S_BCNT1_I32_B64       ,   16) //: D.i = CountOneBits(S0.u). SCC = 1 if result is non-zero.
        ENUM(S_FF0_I32_B32         ,   17) //: D.i = FindFirstZero(S0.u) from LSB; if no zeros, return -1.
        ENUM(S_FF0_I32_B64         ,   18) //: D.i = FindFirstZero(S0.u) from LSB; if no zeros, return -1.
        ENUM(S_FF1_I32_B32         ,   19) //: D.i = FindFirstOne(S0.u) from LSB; if no ones, return -1.
        ENUM(S_FF1_I32_B64         ,   20) //: D.i = FindFirstOne(S0.u) from LSB; if no ones, return -1.
        ENUM(S_FLBIT_I32_B32       ,   21) //: D.i = FindFirstOne(S0.u) from MSB; if no ones, return -1.
        ENUM(S_FLBIT_I32_B64       ,   22) //: D.i = FindFirstOne(S0.u) from MSB; if no ones, return -1.
        ENUM(S_FLBIT_I32           ,   23) //: D.i = Find first bit opposite of sign bit from MSB. If S0 == -1, return -1.
        ENUM(S_FLBIT_I32_I64       ,   24) //: D.i = Find first bit opposite of sign bit from MSB. If S0 == -1, return -1.
        ENUM(S_SEXT_I32_I8         ,   25) //: D.i = signext(S0.i[7:0]).
        ENUM(S_SEXT_I32_I16        ,   26) //: D.i = signext(S0.i[15:0]).
        ENUM(S_BITSET0_B32         ,   27) //: D.u[S0.u[4:0]] = 0.
        ENUM(S_BITSET0_B64         ,   28) //: D.u[S0.u[5:0]] = 0.
        ENUM(S_BITSET1_B32         ,   29) //: D.u[S0.u[4:0]] = 1.
        ENUM(S_BITSET1_B64         ,   30) //: D.u[S0.u[5:0]] = 1.
        ENUM(S_GETPC_B64           ,   31) //: D.u = PC + 4; destination receives the byte address of the next instruction.
        ENUM(S_SETPC_B64           ,   32) //: PC = S0.u; S0.u is a byte address of the instruction to jump to.
        ENUM(S_SWAPPC_B64          ,   33) //: D.u = PC + 4; PC = S0.u.
        ENUM(S_RFE_B64             ,   34) //: Return from Exception; PC = TTMP1,0.
        ENUM(S_AND_SAVEEXEC_B64    ,   36) //: D.u = EXEC, EXEC = S0.u & EXEC. SCC = 1 if the new value of EXEC is non-zero.
        ENUM(S_OR_SAVEEXEC_B64     ,   37) //: D.u = EXEC, EXEC = S0.u | EXEC. SCC = 1 if the newvalue of EXEC is non-zero.
        ENUM(S_XOR_SAVEEXEC_B64    ,   38) //: D.u = EXEC, EXEC = S0.u ^ EXEC. SCC = 1 if the new value of EXEC is non-zero.
        ENUM(S_ANDN2_SAVEEXEC_B64  ,   39) //: D.u = EXEC, EXEC = S0.u & ~EXEC. SCC =1 if the new value of EXEC is non-zero.
        ENUM(S_ORN2_SAVEEXEC_B64   ,   40) //: D.u = EXEC, EXEC = S0.u | ~EXEC. SCC = 1 if the new value of EXEC is non-zero.
        ENUM(S_NAND_SAVEEXEC_B64   ,   41) //: D.u = EXEC, EXEC = ~(S0.u & EXEC). SCC =1 if the new value of EXEC is non-zero.
        ENUM(S_NOR_SAVEEXEC_B64    ,   42)//: D.u = EXEC, EXEC = ~(S0.u | EXEC). SCC = 1 if the new value of EXEC is non-zero.
        ENUM(S_XNOR_SAVEEXEC_B64   ,   43)//: D.u = EXEC, EXEC = ~(S0.u ^ EXEC). SCC = 1 if the new value of EXEC is non-zero.
        ENUM(S_QUADMASK_B32        ,   44)//: D.u = QuadMask(S0.u). D[0] = OR(S0[3:0]), D[1] = OR(S0[7:4]) .... SCC = 1 if result is non-zero.
        ENUM(S_QUADMASK_B64        ,   45)//: D.u = QuadMask(S0.u). D[0] = OR(S0[3:0]),D[1] = OR(S0[7:4]) .... SCC = 1 if result is non-zero
        ENUM(S_MOVRELS_B32         ,   46)//: SGPR[D.u] = SGPR[S0.u + M0.u].
        ENUM(S_MOVRELS_B64         ,   47)//: SGPR[D.u] = SGPR[S0.u + M0.u].
        ENUM(S_MOVRELD_B32         ,   48)//: SGPR[D.u + M0.u] = SGPR[S0.u].
        ENUM(S_MOVRELD_B64         ,   49)//: SGPR[D.u + M0.u] = SGPR[S0.u].
        ENUM(S_CBRANCH_JOIN        ,   50)//: Conditional branch join point. Arg0 = saved CSP value. No dest.
        ENUM(S_ABS_I32             ,   52)      //: D.i = abs(S0.i). SCC=1 if result is non-zero.
        ENUM(S_MOV_FED_B32         ,   53)      //: D.u = S0.u, introduce edc double error upon write to dest sgpr.
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
    END_TRANSLATOR(ScalarInstructions,SOPPOpcodes,S_INVALID)

    BEGIN_TRANSLATOR(ScalarMemoryInstructions,SMRDOpcodes)
        ENUM(S_LOAD_DWORD               ,0 ) // : Read from read-only constant memory.
        ENUM(S_LOAD_DWORDX2             ,1 ) // : Read from read-only constant memory.
        ENUM(S_LOAD_DWORDX4             ,2 ) // : Read from read-only constant memory.
        ENUM(S_LOAD_DWORDX8             ,3 ) // : Read from read-only constant memory.
        ENUM(S_LOAD_DWORDX16            ,4 ) // : Read from read-only constant memory.
        ENUM(S_BUFFER_LOAD_DWORD        ,8 ) // : Read from read-only constant memory.
        ENUM(S_BUFFER_LOAD_DWORDX2      ,9 ) // : Read from read-only constant memory.
        ENUM(S_BUFFER_LOAD_DWORDX4      ,10) // : Read from read-only constant memory.
        ENUM(S_BUFFER_LOAD_DWORDX8      ,11) // : Read from read-only constant memory.
        ENUM(S_BUFFER_LOAD_DWORDX16     ,12) // : Read from read-only constant memory.
        ENUM(S_DCACHE_INV_VOL           ,29) // : Invalidate all volatile lines in L1 constant cache.
        ENUM(S_MEMTIME                  ,30) // : Return current 64-bit timestamp.
        ENUM(S_DCACHE_INV               ,31) // : Invalidate entire L1 K cache.
    END_TRANSLATOR(ScalarMemoryInstructions,SMRDOpcodes,S_INVALID)


    BEGIN_TRANSLATOR(VectorInstructions,VOP2Opcodes)
        ENUM(V_CNDMASK_B32              , 0 )
        ENUM(V_READLANE_B32             , 1 )
        ENUM(V_WRITELANE_B32            , 2 )
        ENUM(V_ADD_F32                  , 3 )
        ENUM(V_SUB_F32                  , 4 )
        ENUM(V_SUBREV_F32               , 5 )
        ENUM(V_MAC_LEGACY_F32           , 6 )
        ENUM(V_MUL_LEGACY_F32           , 7 )
        ENUM(V_MUL_F32                  , 8 )
        ENUM(V_MUL_I32_I24              , 9 )
        ENUM(V_MUL_HI_I32_I24           , 10)
        ENUM(V_MUL_U32_U24              , 11)
        ENUM(V_MUL_HI_U32_U24           , 12)
        ENUM(V_MIN_LEGACY_F32           , 13)
        ENUM(V_MAX_LEGACY_F32           , 14)
        ENUM(V_MIN_F32                  , 15)
        ENUM(V_MAX_F32                  , 16)
        ENUM(V_MIN_I32                  , 17)
        ENUM(V_MAX_I32                  , 18)
        ENUM(V_MIN_U32                  , 19)
        ENUM(V_MAX_U32                  , 20)
        ENUM(V_LSHR_B32                 , 21)
        ENUM(V_LSHRREV_B32              , 22)
        ENUM(V_ASHR_I32                 , 23)
        ENUM(V_ASHRREV_I32              , 24)
        ENUM(V_LSHL_B32                 , 25)
        ENUM(V_LSHLREV_B32              , 26)
        ENUM(V_AND_B32                  , 27)
        ENUM(V_OR_B32                   , 28)
        ENUM(V_XOR_B32                  , 29)
        ENUM(V_BFM_B32                  , 30)
        ENUM(V_MAC_F32                  , 31)
        ENUM(V_MADMK_F32                , 32)
        ENUM(V_MADAK_F32                , 33)
        ENUM(V_BCNT_U32_B32             , 34)
        ENUM(V_MBCNT_LO_U32_B32         , 35)
        ENUM(V_MBCNT_HI_U32_B32         , 36)                          
        ENUM(V_ADD_I32                  , 37)
        ENUM(V_SUB_I32                  , 38)
        ENUM(V_SUBREV_I32               , 39)
        ENUM(V_ADDC_U32                 , 40)
        ENUM(V_SUBB_U32                 , 41)
        ENUM(V_SUBBREV_U32              , 42)
        ENUM(V_LDEXP_F32                , 43)
        ENUM(V_CVT_PKACCUM_U8_F32       , 44)
        ENUM(V_CVT_PKNORM_I16_F32       , 45)
        ENUM(V_CVT_PKNORM_U16_F32       , 46)
        ENUM(V_CVT_PKRTZ_F16_F32        , 47)
        ENUM(V_CVT_PK_U16_U32           , 48)
        ENUM(V_CVT_PK_I16_I32           , 49)
    END_TRANSLATOR(VectorInstructions,VOP2Opcodes,V_INVALID)


    BEGIN_TRANSLATOR(VectorInstructions,VOP1Opcodes)
        ENUM(V_NOP                      , 0  )
        ENUM(V_MOV_B32                  , 1  )
        ENUM(V_READFIRSTLANE_B32        , 2  )
        ENUM(V_CVT_I32_F64              , 3  )
        ENUM(V_CVT_F64_I32              , 4  )
        ENUM(V_CVT_F32_I32              , 5  )
        ENUM(V_CVT_F32_U32              , 6  )
        ENUM(V_CVT_U32_F32              , 7  )
        ENUM(V_CVT_I32_F32              , 8  )
        ENUM(V_MOV_FED_B32              , 9  )
        ENUM(V_CVT_F16_F32              , 10 )
        ENUM(V_CVT_F32_F16              , 11 )
        ENUM(V_CVT_RPI_I32_F32          , 12 )
        ENUM(V_CVT_FLR_I32_F32          , 13 )
        ENUM(V_CVT_OFF_F32_I4           , 14 )
        ENUM(V_CVT_F32_F64              , 15 )
        ENUM(V_CVT_F64_F32              , 16 )
        ENUM(V_CVT_F32_UBYTE0           , 17 )
        ENUM(V_CVT_F32_UBYTE1           , 18 )
        ENUM(V_CVT_F32_UBYTE2           , 19 )
        ENUM(V_CVT_F32_UBYTE3           , 20 )
        ENUM(V_CVT_U32_F64              , 21 )
        ENUM(V_CVT_F64_U32              , 22 )
        ENUM(V_TRUNC_F64                , 23 )
        ENUM(V_CEIL_F64                 , 24 )
        ENUM(V_RNDNE_F64                , 25 )
        ENUM(V_FLOOR_F64                , 26 )
        ENUM(V_FRACT_F32                , 32 )
        ENUM(V_TRUNC_F32                , 33 )
        ENUM(V_CEIL_F32                 , 34 )
        ENUM(V_RNDNE_F32                , 35 )
        ENUM(V_FLOOR_F32                , 36 )
        ENUM(V_EXP_F32                  , 37 )
        ENUM(V_LOG_CLAMP_F32            , 38 )
        ENUM(V_LOG_F32                  , 39 )
        ENUM(V_RCP_CLAMP_F32            , 40 )
        ENUM(V_RCP_LEGACY_F32           , 41 )
        ENUM(V_RCP_F32                  , 42 )
        ENUM(V_RCP_IFLAG_F32            , 43 )
        ENUM(V_RSQ_CLAMP_F32            , 44 )
        ENUM(V_RSQ_LEGACY_F32           , 45 )
        ENUM(V_RSQ_F32                  , 46 )
        ENUM(V_RCP_F64                  , 47 )
        ENUM(V_RCP_CLAMP_F64            , 48 )
        ENUM(V_RSQ_F64                  , 49 )
        ENUM(V_RSQ_CLAMP_F64            , 50 )
        ENUM(V_SQRT_F32                 , 51 )
        ENUM(V_SQRT_F64                 , 52 )
        ENUM(V_SIN_F32                  , 53 )
        ENUM(V_COS_F32                  , 54 )
        ENUM(V_NOT_B32                  , 55 )
        ENUM(V_BFREV_B32                , 56 )
        ENUM(V_FFBH_U32                 , 57 )
        ENUM(V_FFBL_B32                 , 58 )
        ENUM(V_FFBH_I32                 , 59 )
        ENUM(V_FREXP_EXP_I32_F64        , 60 )
        ENUM(V_FREXP_MANT_F64           , 61 )
        ENUM(V_FRACT_F64                , 62 )
        ENUM(V_FREXP_EXP_I32_F32        , 63 )                                      
        ENUM(V_FREXP_MANT_F32           , 64 )                        
        ENUM(V_CLREXCP                  , 65 )
        ENUM(V_MOVRELD_B32              , 66 )
        ENUM(V_MOVRELS_B32              , 67 )
        ENUM(V_MOVRELSD_B32             , 68 )
        ENUM(V_LOG_LEGACY_F32           , 69 )
        ENUM(V_EXP_LEGACY_F32           , 70 )
    END_TRANSLATOR(VectorInstructions,VOP1Opcodes,V_INVALID)


    static VectorInstructions Translate_VOPCOpcodes( uint n )
    {
        switch( n & 0xf0)
        {
        case 0x00:  return (VectorInstructions)(V_CMP_F_F32   + (n&0xf));
        case 0x10:  return (VectorInstructions)(V_CMPX_F_F32  + (n&0xf));
        case 0x20:  return (VectorInstructions)(V_CMP_F_F64   + (n&0xf));
        case 0x30:  return (VectorInstructions)(V_CMPX_F_F64  + (n&0xf));
        case 0x40:  return (VectorInstructions)(V_CMPS_F_F32  + (n&0xf));
        case 0x50:  return (VectorInstructions)(V_CMPSX_F_F32 + (n&0xf));
        case 0x60:  return (VectorInstructions)(V_CMPS_F_F64  + (n&0xf));
        case 0x70:  return (VectorInstructions)(V_CMPSX_F_F64 + (n&0xf));
        default:
            switch( n & 0xf8 )
            {
            case 0x80: return (VectorInstructions)(V_CMP_F_I32   + (n&0xf));
            case 0x90: return (VectorInstructions)(V_CMPX_F_I32  + (n&0xf));
            case 0xA0: return (VectorInstructions)(V_CMP_F_I64   + (n&0xf));
            case 0xB0: return (VectorInstructions)(V_CMPX_F_I64  + (n&0xf));
            case 0xC0: return (VectorInstructions)(V_CMP_F_U32   + (n&0xf));
            case 0xD0: return (VectorInstructions)(V_CMPX_F_U32  + (n&0xf));
            case 0xE0: return (VectorInstructions)(V_CMP_F_U64   + (n&0xf));
            case 0xF0: return (VectorInstructions)(V_CMPx_F_U64  + (n&0xf));
            case 0x88: return V_CMP_CLASS_F32;
            case 0x98: return V_CMPX_CLASS_F32;
            case 0xA8: return V_CMP_CLASS_F64;
            case 0xB8: return V_CMPX_CLASS_F64;
            }
        }
        return V_INVALID;
    }


    BEGIN_TRANSLATOR(VectorInstructions,VOP3OpcodesExtra)
         ENUM(V_CNDMASK_B32          ,256)
         ENUM(V_READLANE_B32         ,257)                                    
         ENUM(V_WRITELANE_B32        ,258)                                    
         ENUM(V_ADD_F32              ,259)
         ENUM(V_SUB_F32              ,260)
         ENUM(V_SUBREV_F32           ,261)
         ENUM(V_MAC_LEGACY_F32       ,262)
         ENUM(V_MUL_LEGACY_F32       ,263)
         ENUM(V_MUL_F32              ,264)
         ENUM(V_MUL_I32_I24          ,265)
         ENUM(V_MUL_HI_I32_I24       ,266)
         ENUM(V_MUL_U32_U24          ,267)
         ENUM(V_MUL_HI_U32_U24       ,268)
         ENUM(V_MIN_LEGACY_F32       ,269)
         ENUM(V_MAX_LEGACY_F32       ,270)
         ENUM(V_MIN_F32              ,271)
         ENUM(V_MAX_F32              ,272)
         ENUM(V_MIN_I32              ,273)
         ENUM(V_MAX_I32              ,274)
         ENUM(V_MIN_U32              ,275)
         ENUM(V_MAX_U32              ,276)
         ENUM(V_LSHR_B32             ,277)
         ENUM(V_LSHRREV_B32          ,278)
         ENUM(V_ASHR_I32             ,279)
         ENUM(V_ASHRREV_I32          ,280)
         ENUM(V_LSHL_B32             ,281)
         ENUM(V_LSHLREV_B32          ,282)
         ENUM(V_AND_B32              ,283)
         ENUM(V_OR_B32               ,284)
         ENUM(V_XOR_B32              ,285)
         ENUM(V_BFM_B32              ,286)
         ENUM(V_MAC_F32              ,287)
         ENUM(V_MADMK_F32            ,288)
         ENUM(V_MADAK_F32            ,289)
         ENUM(V_BCNT_U32_B32         ,290)
         ENUM(V_MBCNT_LO_U32_B32     ,291)
         ENUM(V_MBCNT_HI_U32_B32     ,292)
         ENUM(V_ADD_I32              ,293)
         ENUM(V_SUB_I32              ,294)
         ENUM(V_SUBREV_I32           ,295)
         ENUM(V_ADDC_U32             ,296)
         ENUM(V_SUBB_U32             ,297)
         ENUM(V_SUBBREV_U32          ,298)
         ENUM(V_DIV_SCALE_F32        ,365)
         ENUM(V_DIV_SCALE_F64        ,366)
         ENUM(V_LDEXP_F32            ,299)
         ENUM(V_CVT_PKACCUM_U8_F32   ,300)
         ENUM(V_CVT_PKNORM_I16_F32   ,301)
         ENUM(V_CVT_PKNORM_U16_F32   ,302)
         ENUM(V_CVT_PKRTZ_F16_F32    ,303)
         ENUM(V_CVT_PK_U16_U32       ,304)
         ENUM(V_CVT_PK_I16_I32       ,305)
         ENUM(V_MAD_LEGACY_F32       ,320)  
         ENUM(V_MAD_F32              ,321)  
         ENUM(V_MAD_I32_I24          ,322)  
         ENUM(V_MAD_U32_U24          ,323)  
         ENUM(V_CUBEID_F32           ,324)  
         ENUM(V_CUBESC_F32           ,325)  
         ENUM(V_CUBETC_F32           ,326)  
         ENUM(V_CUBEMA_F32           ,327)  
         ENUM(V_BFE_U32              ,328)  
         ENUM(V_BFE_I32              ,329)  
         ENUM(V_BFI_B32              ,330)  
         ENUM(V_FMA_F32              ,331)  
         ENUM(V_FMA_F64              ,332)  
         ENUM(V_LERP_U8              ,333)  
         ENUM(V_ALIGNBIT_B32         ,334)  
         ENUM(V_ALIGNBYTE_B32        ,335)  
         ENUM(V_MULLIT_F32           ,336)  
         ENUM(V_MIN3_F32             ,337)  
         ENUM(V_MIN3_I32             ,338)  
         ENUM(V_MIN3_U32             ,339)  
         ENUM(V_MAX3_F32             ,340)  
         ENUM(V_MAX3_I32             ,341)  
         ENUM(V_MAX3_U32             ,342)  
         ENUM(V_MED3_F32             ,343)  
         ENUM(V_MED3_I32             ,344)  
         ENUM(V_MED3_U32             ,345)  
         ENUM(V_SAD_U8               ,346)  
         ENUM(V_SAD_HI_U8            ,347)  
         ENUM(V_SAD_U16              ,348)  
         ENUM(V_SAD_U32              ,349)  
         ENUM(V_CVT_PK_U8_F32        ,350) 
         ENUM(V_DIV_FIXUP_F32        ,351) 
         ENUM(V_DIV_FIXUP_F64        ,352) 
         ENUM(V_LSHL_B64             ,353) 
         ENUM(V_LSHR_B64             ,354) 
         ENUM(V_ASHR_I64             ,355) 
         ENUM(V_ADD_F64              ,356) 
         ENUM(V_MUL_F64              ,357) 
         ENUM(V_MIN_F64              ,358) 
         ENUM(V_MAX_F64              ,359) 
         ENUM(V_LDEXP_F64            ,360) 
         ENUM(V_MUL_LO_U32           ,361) 
         ENUM(V_MUL_HI_U32           ,362) 
         ENUM(V_MUL_LO_I32           ,363) 
         ENUM(V_MUL_HI_I32           ,364) 
         ENUM(V_DIV_FMAS_F32         ,367) 
         ENUM(V_DIV_FMAS_F64         ,368) 
         ENUM(V_MSAD_U8              ,369) 
         ENUM(V_QSAD_U8              ,370) 
         ENUM(V_MQSAD_U8             ,371) 
         ENUM(V_QSAD_PK_U16_U8       ,370) 
         ENUM(V_MQSAD_PK_U16_U8      ,371) 
         ENUM(V_TRIG_PREOP_F64       ,372)
         ENUM(V_MQSAD_U32_U8         ,373)  
         ENUM(V_MAD_U64_U32          ,374)  
         ENUM(V_MAD_I64_I32          ,375)  
         ENUM(V_NOP                  ,384)
         ENUM(V_MOV_B32              ,385)
         ENUM(V_READFIRSTLANE_B32    ,386)
         ENUM(V_CVT_I32_F64          ,387)
         ENUM(V_CVT_F64_I32          ,388)
         ENUM(V_CVT_F32_I32          ,389)
         ENUM(V_CVT_F32_U32          ,390)
         ENUM(V_CVT_U32_F32          ,391)
         ENUM(V_CVT_I32_F32          ,392)
         ENUM(V_MOV_FED_B32          ,393)
         ENUM(V_CVT_F16_F32          ,394)
         ENUM(V_CVT_F32_F16          ,395)
         ENUM(V_CVT_RPI_I32_F32      ,396)
         ENUM(V_CVT_FLR_I32_F32      ,397)
         ENUM(V_CVT_OFF_F32_I4       ,398)
         ENUM(V_CVT_F32_F64          ,399)
         ENUM(V_CVT_F64_F32          ,400)
         ENUM(V_CVT_F32_UBYTE0       ,401)
         ENUM(V_CVT_F32_UBYTE1       ,402)
         ENUM(V_CVT_F32_UBYTE2       ,403)
         ENUM(V_CVT_F32_UBYTE3       ,404)
         ENUM(V_CVT_U32_F64          ,405)
         ENUM(V_CVT_F64_U32          ,406)
         ENUM(V_FRACT_F32            ,416)
         ENUM(V_TRUNC_F32            ,417)
         ENUM(V_CEIL_F32             ,418)
         ENUM(V_RNDNE_F32            ,419)
         ENUM(V_FLOOR_F32            ,420)
         ENUM(V_EXP_F32              ,421)
         ENUM(V_LOG_CLAMP_F32        ,422)
         ENUM(V_LOG_F32              ,423)
         ENUM(V_RCP_CLAMP_F32        ,424)
         ENUM(V_RCP_LEGACY_F32       ,425)
         ENUM(V_RCP_F32              ,426)
         ENUM(V_RCP_IFLAG_F32        ,427)
         ENUM(V_RSQ_CLAMP_F32        ,428)
         ENUM(V_RSQ_LEGACY_F32       ,429)
         ENUM(V_RSQ_F32              ,430)
         ENUM(V_RCP_F64              ,431)
         ENUM(V_RCP_CLAMP_F64        ,432)
         ENUM(V_RSQ_F64              ,433)
         ENUM(V_RSQ_CLAMP_F64        ,434)
         ENUM(V_SQRT_F32             ,435)
         ENUM(V_SQRT_F64             ,436)  
         ENUM(V_SIN_F32              ,437)  
         ENUM(V_COS_F32              ,438)  
         ENUM(V_NOT_B32              ,439)  
         ENUM(V_BFREV_B32            ,440)  
         ENUM(V_FFBH_U32             ,441)  
         ENUM(V_FFBL_B32             ,442)  
         ENUM(V_FFBH_I32             ,443)  
         ENUM(V_FREXP_EXP_I32_F64    ,444)  
         ENUM(V_FREXP_MANT_F64       ,445)  
         ENUM(V_FRACT_F64            ,446)  
         ENUM(V_FREXP_EXP_I32_F32    ,447)  
         ENUM(V_FREXP_MANT_F32       ,448)                                                                             
         ENUM(V_CLREXCP              ,449)    
         ENUM(V_MOVRELD_B32          ,450)    
         ENUM(V_MOVRELS_B32          ,451)    
         ENUM(V_MOVRELSD_B32         ,452)    
    END_TRANSLATOR(VectorInstructions,VOP3OpcodesExtra,V_INVALID)
        
    static VectorInstructions Translate_VOP3Opcodes( uint n )
    {
        if( n <= 255 )
            return Translate_VOPCOpcodes(n);
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

    BEGIN_TRANSLATOR(Dests,ScalarDestOddballs)
        ENUM(SRC_FSCR_LO          , 104)
        ENUM(SRC_FSCR_HI          , 105)
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
        ENUM(SRC_FSCR_LO          , 104)
        ENUM(SRC_FSCR_HI          , 105)
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
        ENUM(SRC_VCCZ             , 251)
        ENUM(SRC_EXECZ            , 252)
        ENUM(SRC_SCC              , 253)
        ENUM(SRC_LDS_DIRECT       , 254)
        ENUM(SRC_LITERAL          , 255)
    END_TRANSLATOR(Sources,ScalarSourceOddballs,SRC_INVALID)
    
    BEGIN_TRANSLATOR(DSInstructions,DSOpcodes)
      ENUM(DS_ADD_U32              , 0 )   
      ENUM(DS_SUB_U32              , 1 )   
      ENUM(DS_RSUB_U32             , 2 )   
      ENUM(DS_INC_U32              , 3 )   
      ENUM(DS_DEC_U32              , 4 )   
      ENUM(DS_MIN_I32              , 5 )   
      ENUM(DS_MAX_I32              , 6 )   
      ENUM(DS_MIN_U32              , 7 )   
      ENUM(DS_MAX_U32              , 8 )   
      ENUM(DS_AND_B32              , 9 )   
      ENUM(DS_OR_B32               ,10 )   
      ENUM(DS_XOR_B32              ,11 )   
      ENUM(DS_MSKOR_B32            ,12 )   
      ENUM(DS_WRITE_B32            ,13 )   
      ENUM(DS_WRITE2_B32           ,14 )   
      ENUM(DS_WRITE2ST64_B32       ,15 )   
      ENUM(DS_CMPST_B32            ,16 )   
      ENUM(DS_CMPST_F32            ,17 )   
      ENUM(DS_MIN_F32              ,18 )   
      ENUM(DS_MAX_F32              ,19 )   
      ENUM(DS_NOP                  ,20 )  
      ENUM(DS_GWS_SEMA_RELEASE_ALL ,24 )  
      ENUM(DS_GWS_INIT             ,25 )   
      ENUM(DS_GWS_SEMA_V           ,26 )   
      ENUM(DS_GWS_SEMA_BR          ,27 )   
      ENUM(DS_GWS_SEMA_P           ,28 )   
      ENUM(DS_GWS_BARRIER          ,29 )   
      ENUM(DS_WRITE_B8             ,30 )   
      ENUM(DS_WRITE_B16            ,31 )   
      ENUM(DS_ADD_RTN_U32          ,32 )   
      ENUM(DS_SUB_RTN_U32          ,33 )   
      ENUM(DS_RSUB_RTN_U32         ,34 )   
      ENUM(DS_INC_RTN_U32          ,35 )   
      ENUM(DS_DEC_RTN_U32          ,36 )   
      ENUM(DS_MIN_RTN_I32          ,37 )   
      ENUM(DS_MAX_RTN_I32          ,38 )   
      ENUM(DS_MIN_RTN_U32          ,39 )   
      ENUM(DS_MAX_RTN_U32          ,40 )   
      ENUM(DS_AND_RTN_B32          ,41 )   
      ENUM(DS_OR_RTN_B32           ,42 )   
      ENUM(DS_XOR_RTN_B32          ,43 )   
      ENUM(DS_MSKOR_RTN_B32        ,44 )   
      ENUM(DS_WRXCHG_RTN_B32       ,45 )   
      ENUM(DS_WRXCHG2_RTN_B32      ,46 )   
      ENUM(DS_WRXCHG2ST64_RTN_B32  ,47 )   
      ENUM(DS_CMPST_RTN_B32        ,48 )   
      ENUM(DS_CMPST_RTN_F32        ,49 )   
      ENUM(DS_MIN_RTN_F32          ,50 )   
      ENUM(DS_MAX_RTN_F32          ,51 )   
      ENUM(DS_WRAP_RTN_B32         ,52 )   
      ENUM(DS_SWIZZLE_B32          ,53 )   
      ENUM(DS_READ_B32             ,54 )   
      ENUM(DS_READ2_B32            ,55 )   
      ENUM(DS_READ2ST64_B32        ,56 )   
      ENUM(DS_READ_I8              ,57 )   
      ENUM(DS_READ_U8              ,58 )   
      ENUM(DS_READ_I16             ,59 )   
      ENUM(DS_READ_U16             ,60 )   
      ENUM(DS_CONSUME              ,61 )   
      ENUM(DS_APPEND               ,62 )   
      ENUM(DS_ORDERED_COUNT        ,63 )   
      ENUM(DS_ADD_U64              ,64 )   
      ENUM(DS_SUB_U64              ,65 )   
      ENUM(DS_RSUB_U64             ,66 )   
      ENUM(DS_INC_U64              ,67 )   
      ENUM(DS_DEC_U64              ,68 )   
      ENUM(DS_MIN_I64              ,69 )   
      ENUM(DS_MAX_I64              ,70 )   
      ENUM(DS_MIN_U64              ,71 )   
      ENUM(DS_MAX_U64              ,72 )   
      ENUM(DS_AND_B64              ,73 )   
      ENUM(DS_OR_B64               ,74 )   
      ENUM(DS_XOR_B64              ,75 )   
      ENUM(DS_MSKOR_B64            ,76 )   
      ENUM(DS_WRITE_B64            ,77 )   
      ENUM(DS_WRITE2_B64           ,78 )   
      ENUM(DS_WRITE2ST64_B64       ,79 )   
      ENUM(DS_CMPST_B64            ,80 )   
      ENUM(DS_CMPST_F64            ,81 )   
      ENUM(DS_MIN_F64              ,82 )   
      ENUM(DS_MAX_F64              ,83 )   
      ENUM(DS_ADD_RTN_U64          ,96 )   
      ENUM(DS_SUB_RTN_U64          ,97 )   
      ENUM(DS_RSUB_RTN_U64         ,98 )   
      ENUM(DS_INC_RTN_U64          ,99 )   
      ENUM(DS_DEC_RTN_U64          ,100)   
      ENUM(DS_MIN_RTN_I64          ,101)   
      ENUM(DS_MAX_RTN_I64          ,102)   
      ENUM(DS_MIN_RTN_U64          ,103)   
      ENUM(DS_MAX_RTN_U64          ,104)   
      ENUM(DS_AND_RTN_B64          ,105)   
      ENUM(DS_OR_RTN_B64           ,106)   
      ENUM(DS_XOR_RTN_B64          ,107)   
      ENUM(DS_MSKOR_RTN_B64        ,108)   
      ENUM(DS_WRXCHG_RTN_B64       ,109)   
      ENUM(DS_WRXCHG2_RTN_B64      ,110)   
      ENUM(DS_WRXCHG2ST64_RTN_B64  ,111)   
      ENUM(DS_CMPST_RTN_B64        ,112)   
      ENUM(DS_CMPST_RTN_F64        ,113)   
      ENUM(DS_MIN_RTN_F64          ,114)   
      ENUM(DS_MAX_RTN_F64          ,115)   
      ENUM(DS_READ_B64             ,118)   
      ENUM(DS_READ2_B64            ,119)   
      ENUM(DS_READ2ST64_B64        ,120)   
      ENUM(DS_CONDXCHG32_RTN_B64   ,126)   
      ENUM(DS_ADD_SRC2_U32         ,128)   
      ENUM(DS_SUB_SRC2_U32         ,129)   
      ENUM(DS_RSUB_SRC2_U32        ,130)   
      ENUM(DS_INC_SRC2_U32         ,131)   
      ENUM(DS_DEC_SRC2_U32         ,132)   
      ENUM(DS_MIN_SRC2_I32         ,133)   
      ENUM(DS_MAX_SRC2_I32         ,134)   
      ENUM(DS_MIN_SRC2_U32         ,135)   
      ENUM(DS_MAX_SRC2_U32         ,136)   
      ENUM(DS_AND_SRC2_B32         ,137)   
      ENUM(DS_OR_SRC2_B32          ,138)   
      ENUM(DS_XOR_SRC2_B32         ,139)   
      ENUM(DS_WRITE_SRC2_B32       ,140)   
      ENUM(DS_MIN_SRC2_F32         ,146)   
      ENUM(DS_MAX_SRC2_F32         ,147)   
      ENUM(DS_ADD_SRC2_U64         ,192)   
      ENUM(DS_SUB_SRC2_U64         ,193)   
      ENUM(DS_RSUB_SRC2_U64        ,194)   
      ENUM(DS_INC_SRC2_U64         ,195)   
      ENUM(DS_DEC_SRC2_U64         ,196)   
      ENUM(DS_MIN_SRC2_I64         ,197)   
      ENUM(DS_MAX_SRC2_I64         ,198)   
      ENUM(DS_MIN_SRC2_U64         ,199)   
      ENUM(DS_MAX_SRC2_U64         ,200)   
      ENUM(DS_AND_SRC2_B64         ,201)   
      ENUM(DS_OR_SRC2_B64          ,202)   
      ENUM(DS_XOR_SRC2_B64         ,203)   
      ENUM(DS_WRITE_SRC2_B64       ,204)   
      ENUM(DS_MIN_SRC2_F64         ,210)   
      ENUM(DS_MAX_SRC2_F64         ,211)   
      ENUM(DS_WRITE_B96            ,222)  
      ENUM(DS_WRITE_B128           ,223)  
      ENUM(DS_CONDXCHG32_RTN_B128  ,253)  
      ENUM(DS_READ_B96             ,254)  
      ENUM(DS_READ_B128            ,255) 
    END_TRANSLATOR(DSInstructions,DSOpcodes,DS_INVALID)


    BEGIN_TRANSLATOR(BufferInstructions,MUBUFFOpcodes)
    ENUM( BUFFER_LOAD_FORMAT_X       ,0    )
    ENUM( BUFFER_LOAD_FORMAT_XY      ,1    )
    ENUM( BUFFER_LOAD_FORMAT_XYZ     ,2    )
    ENUM( BUFFER_LOAD_FORMAT_XYZW    ,3    )
    ENUM( BUFFER_STORE_FORMAT_X      ,4    )
    ENUM( BUFFER_STORE_FORMAT_XY     ,5    )
    ENUM( BUFFER_STORE_FORMAT_XYZ    ,6    )
    ENUM( BUFFER_STORE_FORMAT_XYZW   ,7    )
    ENUM( BUFFER_LOAD_UBYTE          ,8    )
    ENUM( BUFFER_LOAD_SBYTE          ,9    )
    ENUM( BUFFER_LOAD_USHORT         ,10   )
    ENUM( BUFFER_LOAD_SSHORT         ,11   )
    ENUM( BUFFER_LOAD_DWORD          ,12   )
    ENUM( BUFFER_LOAD_DWORDX2        ,13   )
    ENUM( BUFFER_LOAD_DWORDX4        ,14   )
    ENUM( BUFFER_LOAD_DWORDX3        ,15   )
    ENUM( BUFFER_STORE_BYTE          ,24   )
    ENUM( BUFFER_STORE_SHORT         ,26   )
    ENUM( BUFFER_STORE_DWORD         ,28   )
    ENUM( BUFFER_STORE_DWORDX2       ,29   )
    ENUM( BUFFER_STORE_DWORDX4       ,30   )
    ENUM( BUFFER_STORE_DWORDX3       ,31   )
    ENUM( BUFFER_ATOMIC_SWAP         ,48   )
    ENUM( BUFFER_ATOMIC_CMPSWAP      ,49   )
    ENUM( BUFFER_ATOMIC_ADD          ,50   )
    ENUM( BUFFER_ATOMIC_SUB          ,51   )
    ENUM( BUFFER_ATOMIC_RSUB         ,52   )
    ENUM( BUFFER_ATOMIC_SMIN         ,53   )
    ENUM( BUFFER_ATOMIC_UMIN         ,54   )
    ENUM( BUFFER_ATOMIC_SMAX         ,55   )
    ENUM( BUFFER_ATOMIC_UMAX         ,56   )
    ENUM( BUFFER_ATOMIC_AND          ,57   )
    ENUM( BUFFER_ATOMIC_OR           ,58   )
    ENUM( BUFFER_ATOMIC_XOR          ,59   )
    ENUM( BUFFER_ATOMIC_INC          ,60   )
    ENUM( BUFFER_ATOMIC_DEC          ,61   )
    ENUM( BUFFER_ATOMIC_FCMPSWAP     ,62   )
    ENUM( BUFFER_ATOMIC_FMIN         ,63   )
    ENUM( BUFFER_ATOMIC_FMAX         ,64   )
    ENUM( BUFFER_ATOMIC_SWAP_X2      ,80   )
    ENUM( BUFFER_ATOMIC_CMPSWAP_X2   ,81   )
    ENUM( BUFFER_ATOMIC_ADD_X2       ,82   )
    ENUM( BUFFER_ATOMIC_SUB_X2       ,83   )
    ENUM( BUFFER_ATOMIC_RSUB_X2      ,84   )
    ENUM( BUFFER_ATOMIC_SMIN_X2      ,85   )
    ENUM( BUFFER_ATOMIC_UMIN_X2      ,86   )
    ENUM( BUFFER_ATOMIC_SMAX_X2      ,87   )
    ENUM( BUFFER_ATOMIC_UMAX_X2      ,88   )
    ENUM( BUFFER_ATOMIC_AND_X2       ,89   )
    ENUM( BUFFER_ATOMIC_OR_X2        ,90   )
    ENUM( BUFFER_ATOMIC_XOR_X2       ,91   )
    ENUM( BUFFER_ATOMIC_INC_X2       ,92   )
    ENUM( BUFFER_ATOMIC_DEC_X2       ,93   )
    ENUM( BUFFER_ATOMIC_FCMPSWAP_X2  ,94   )
    ENUM( BUFFER_ATOMIC_FMIN_X2      ,95   )
    ENUM( BUFFER_ATOMIC_FMAX_X2      ,96   )
    ENUM( BUFFER_WBINVL1_SC          ,112  )
    ENUM( BUFFER_WBINVL1             ,113  )
    END_TRANSLATOR(BufferInstructions,MUBUFFOpcodes,BUFFER_INVALID)

    BEGIN_TRANSLATOR(BufferInstructions,MTBUFFOpcodes)
        ENUM(TBUFFER_LOAD_FORMAT_X     ,0) //  : Untyped buffer load 1 Dword with format conversion.
        ENUM(TBUFFER_LOAD_FORMAT_XY    ,1) //  : Untyped buffer load 2 Dwords with format conversion.
        ENUM(TBUFFER_LOAD_FORMAT_XYZ   ,2) //  : Untyped buffer load 3 Dwords with format conversion.
        ENUM(TBUFFER_LOAD_FORMAT_XYZW  ,3) //  : Untyped buffer load 4 Dwords with format conversion.
        ENUM(TBUFFER_STORE_FORMAT_X    ,4) //  : Untyped buffer store 1 Dword with format conversion.
        ENUM(TBUFFER_STORE_FORMAT_XY   ,5) //  : Untyped buffer store 2 Dwords with format conversion.
        ENUM(TBUFFER_STORE_FORMAT_XYZ  ,6) //  : Untyped buffer store 3 Dwords with format conversion.
        ENUM(TBUFFER_STORE_FORMAT_XYZW ,7) //  : Untyped buffer store 4 Dwords with format conversion.
    END_TRANSLATOR(BufferInstructions,MTBUFFOpcodes,TBUFFER_INVALID)

    BEGIN_TRANSLATOR(ImageInstructions,MIMGOpcodes)
        ENUM(IMAGE_LOAD                ,0    )
        ENUM(IMAGE_LOAD_MIP            ,1    )
        ENUM(IMAGE_LOAD_PCK            ,2    )
        ENUM(IMAGE_LOAD_PCK_SGN        ,3    )
        ENUM(IMAGE_LOAD_MIP_PCK        ,4    )
        ENUM(IMAGE_LOAD_MIP_PCK_SGN    ,5    )
        ENUM(IMAGE_STORE               ,8    )
        ENUM(IMAGE_STORE_MIP           ,9    )
        ENUM(IMAGE_STORE_PCK           ,10   )
        ENUM(IMAGE_STORE_MIP_PCK       ,11   )
        ENUM(IMAGE_GET_RESINFO         ,14   )
        ENUM(IMAGE_ATOMIC_SWAP         ,15   )
        ENUM(IMAGE_ATOMIC_CMPSWAP      ,16   )
        ENUM(IMAGE_ATOMIC_ADD          ,17   )
        ENUM(IMAGE_ATOMIC_SUB          ,18   )
        ENUM(IMAGE_ATOMIC_RSUB         ,19   )
        ENUM(IMAGE_ATOMIC_SMIN         ,20   )
        ENUM(IMAGE_ATOMIC_UMIN         ,21   )
        ENUM(IMAGE_ATOMIC_SMAX         ,22   )
        ENUM(IMAGE_ATOMIC_UMAX         ,23   )
        ENUM(IMAGE_ATOMIC_AND          ,24   )
        ENUM(IMAGE_ATOMIC_OR           ,25   )
        ENUM(IMAGE_ATOMIC_XOR          ,26   )
        ENUM(IMAGE_ATOMIC_INC          ,27   )
        ENUM(IMAGE_ATOMIC_DEC          ,28   )
        ENUM(IMAGE_ATOMIC_FCMPSWAP     ,29   )
        ENUM(IMAGE_ATOMIC_FMIN         ,30   )
        ENUM(IMAGE_ATOMIC_FMAX         ,31   )
        ENUM(IMAGE_SAMPLE              ,32   )
        ENUM(IMAGE_SAMPLE_CL           ,33   )
        ENUM(IMAGE_SAMPLE_D            ,34   )
        ENUM(IMAGE_SAMPLE_D_CL         ,35   )
        ENUM(IMAGE_SAMPLE_L            ,36   )
        ENUM(IMAGE_SAMPLE_B            ,37   )
        ENUM(IMAGE_SAMPLE_B_CL         ,38   )
        ENUM(IMAGE_SAMPLE_LZ           ,39   )
        ENUM(IMAGE_SAMPLE_C            ,40   )
        ENUM(IMAGE_SAMPLE_C_CL         ,41   )
        ENUM(IMAGE_SAMPLE_C_D          ,42   )
        ENUM(IMAGE_SAMPLE_C_D_CL       ,43   )
        ENUM(IMAGE_SAMPLE_C_L          ,44   )
        ENUM(IMAGE_SAMPLE_C_B          ,45   )
        ENUM(IMAGE_SAMPLE_C_B_CL       ,46   )
        ENUM(IMAGE_SAMPLE_C_LZ         ,47   )
        ENUM(IMAGE_SAMPLE_O            ,48   )
        ENUM(IMAGE_SAMPLE_CL_O         ,49   )
        ENUM(IMAGE_SAMPLE_D_O          ,50   )
        ENUM(IMAGE_SAMPLE_D_CL_O       ,51   )
        ENUM(IMAGE_SAMPLE_L_O          ,52   )
        ENUM(IMAGE_SAMPLE_B_O          ,53   )
        ENUM(IMAGE_SAMPLE_B_CL_O       ,54   )
        ENUM(IMAGE_SAMPLE_LZ_O         ,55   )
        ENUM(IMAGE_SAMPLE_C_O          ,56   )
        ENUM(IMAGE_SAMPLE_C_CL_O       ,57   )
        ENUM(IMAGE_SAMPLE_C_D_O        ,58   )
        ENUM(IMAGE_SAMPLE_C_D_CL_O     ,59   )
        ENUM(IMAGE_SAMPLE_C_L_O        ,60   )
        ENUM(IMAGE_SAMPLE_C_B_O        ,61   )
        ENUM(IMAGE_SAMPLE_C_B_CL_O     ,62   )
        ENUM(IMAGE_SAMPLE_C_LZ_O       ,63   )
        ENUM(IMAGE_GATHER4             ,64   )
        ENUM(IMAGE_GATHER4_CL          ,65   )
        ENUM(IMAGE_GATHER4_L           ,66   )
        ENUM(IMAGE_GATHER4_B           ,67   )
        ENUM(IMAGE_GATHER4_B_CL        ,68   )
        ENUM(IMAGE_GATHER4_LZ          ,69   )
        ENUM(IMAGE_GATHER4_C           ,70   )
        ENUM(IMAGE_GATHER4_C_CL        ,71   )
        ENUM(IMAGE_GATHER4_C_L         ,76   )
        ENUM(IMAGE_GATHER4_C_B         ,77   )
        ENUM(IMAGE_GATHER4_C_B_CL      ,78   )
        ENUM(IMAGE_GATHER4_C_LZ        ,79   )
        ENUM(IMAGE_GATHER4_O           ,80   )
        ENUM(IMAGE_GATHER4_CL_O        ,81   )
        ENUM(IMAGE_GATHER4_L_O         ,84   )
        ENUM(IMAGE_GATHER4_B_O         ,85   )
        ENUM(IMAGE_GATHER4_B_CL_O      ,86   )
        ENUM(IMAGE_GATHER4_LZ_O        ,87   )
        ENUM(IMAGE_GATHER4_C_O         ,88   )
        ENUM(IMAGE_GATHER4_C_CL_O      ,89   )
        ENUM(IMAGE_GATHER4_C_L_O       ,92   )
        ENUM(IMAGE_GATHER4_C_B_O       ,93   )
        ENUM(IMAGE_GATHER4_C_B_CL_O    ,94   )
        ENUM(IMAGE_GATHER4_C_LZ_O      ,95   )
        ENUM(IMAGE_GET_LOD             ,96   )
        ENUM(IMAGE_SAMPLE_CD           ,104  )
        ENUM(IMAGE_SAMPLE_CD_CL        ,105  )
        ENUM(IMAGE_SAMPLE_C_CD         ,106  )
        ENUM(IMAGE_SAMPLE_C_CD_CL      ,107  )
        ENUM(IMAGE_SAMPLE_CD_O         ,108  )
        ENUM(IMAGE_SAMPLE_CD_CL_O      ,109  )
        ENUM(IMAGE_SAMPLE_C_CD_O       ,110  )
        ENUM(IMAGE_SAMPLE_C_CD_CL_O    ,111  )
    END_TRANSLATOR(ImageInstructions,MIMGOpcodes,IMAGE_INVALID)


    BEGIN_TRANSLATOR(TBufferNumberFormats,MTBUFF_NumberFormat)
        ENUM(NF_UNORM       , 0)
        ENUM(NF_SNORM       , 1)
        ENUM(NF_USCALED     , 2)
        ENUM(NF_SSCALED     , 3)
        ENUM(NF_UINT        , 4)
        ENUM(NF_SINT        , 5)
        ENUM(NF_SNORM_NZ    , 6)
        ENUM(NF_FLOAT       , 7)
        ENUM(NF_SRGB        , 9)
        ENUM(NF_UBNORM      , 10)
        ENUM(NF_UBNORM_NZ   , 11)
        ENUM(NF_UBINT       , 12)
        ENUM(NF_UBSCALED    , 13)
    END_TRANSLATOR(TBufferNumberFormats,MTBUFF_NumberFormat,NF_INVALID)

    BEGIN_TRANSLATOR(TBufferDataFormats,MTBUFF_DataFormat)
        ENUM( DF_8          , 1    )
        ENUM( DF_16         , 2    )
        ENUM( DF_8_8        , 3    )
        ENUM( DF_32         , 4    )
        ENUM( DF_16_16      , 5    )
        ENUM( DF_10_11_11   , 6    )
        ENUM( DF_11_11_10   , 7    )
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

    Dests Translate_SDest( uint n )
    {
        if( n <= 103 )
            return (Dests)(DEST_SGPR_FIRST+n);
        else
            return Translate_ScalarDestOddballs(n);
    }

    Sources Translate_SSrc( uint n )
    {
        if( n <= 103 )
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

    class SMRDInstruction : public Instruction
    {
    public:
        InstructionFormat GetFormat() const { return IF_SMEM; }
        uint GetLength()       const { return 4 + 4*(!IsOffsetIMM() && GetOffset() == SRC_LITERAL); }
        ScalarMemoryInstructions GetOpcode()  const { return Translate_SMRDOpcodes(ReadBits(26,22)); }
        Dests GetDest()                 const { return Translate_SDest(ReadBits( 21,15)); }
        Dests GetBase()                 const { return Translate_SDest(2*ReadBits( 14,9 )); } // base of SGPR pair containing address
        uint GetOffset()               const { return IsOffsetIMM() ? 4*ReadBits(7,0) : ReadBits(7,0); }
        bool IsOffsetIMM()             const { return ReadBits( 8, 8 )!=0; }
        // SI and CI use dword offsets, but VI uses byte.
    };

    class VOP2Instruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_VOP2; }
        bool HasLiteral() const { return (GetSrc0() == SRC_LITERAL || GetOpcode() == V_MADAK_F32 || GetOpcode() == V_MADMK_F32); }
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
        
        uint GetResultWidthInDWORDS() const;
        uint GetOperandWidthInDWORDS() const;
        

        const Instruction* GetBranchTarget() const { return 0; }
        bool IsBranch() const { return false; }
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
        
        VectorInstructions GetOpcode() const { return Translate_VOP3Opcodes(ReadBits(25,17));}
       
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
     
        bool IsCompare() const { return ReadBits(25,17) < 255; } // opcodes 0-255 are vcmps.

        bool IsVOP3bOp() const
        {
            // NOTE: ISA Doc, section 6, claims that vcmp is vop3b.  But AMD disassembler thinks its VOP3a
            //
            switch( GetOpcode() )
            {
            case V_ADD_I32      :   //  : D.u = S0.u + S1.u; VCC=carry-out (VOP3:sgpr=carry-out).
            case V_SUB_I32      :   //  : D.u = S0.u - S1.u; VCC=carry-out (VOP3:sgpr=carry-out).
            case V_SUBREV_I32   :   //  : D.u = S1.u - S0.u; VCC=carry-out (VOP3:sgpr=carry-out).
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
        bool GetClamp() const { return ReadBit(11); }

        // VOP3b only
        Dests GetSDst() const { return Translate_SDest(ReadBits(14,8)); }

    };

    class MUBUFFInstruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_MUBUFF; }
        BufferInstructions GetOpcode() const { return Translate_MUBUFFOpcodes(ReadBits(24,18)); }
        uint GetLength() const { return 8; }
        uint GetResultWidthInDWORDS() const ;

        uint GetOffset() const { return ReadBits(11,0); }
        Sources GetVAddr() const { return Translate_VSrc(ReadBits(39,32)); }
        Sources GetVData() const { return Translate_VSrc(ReadBits(47,40)); }
        Sources GetSResource() const { return (Sources)(SRC_SGPR_FIRST+4*ReadBits(52,48)); }
        Sources GetSOffset() const { return Translate_SSrc(ReadBits(63,56)); }
        
        bool IsOffN() const     { return ReadBit(12); }
        bool IsIdxN() const     { return ReadBit(13); }
        bool IsAddr64() const   { return ReadBit(15); }
        bool IsDirectToLDS() const { return ReadBit(16); }
        bool GetSLCBit() const { return ReadBit(54); }
        bool GetGLCBit() const { return ReadBit(14); }
        bool GetTFEBit() const { return ReadBit(55); }
        
    };

    class MTBUFFInstruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_MTBUFF; }
        BufferInstructions GetOpcode() const { return Translate_MTBUFFOpcodes(ReadBits(18,16)); }
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
        bool IsAddr64() const   { return ReadBit(15); }
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
       
        uint GetDMask() const { return ReadBits(11,8); }
        Sources GetSSampler() const  { return (Sources)(SRC_SGPR_FIRST+4*ReadBits(57,53)); }
        Sources GetSResource() const { return (Sources)(SRC_SGPR_FIRST+4*ReadBits(52,48)); }
        Sources GetVData() const { return Translate_VSrc( ReadBits(47,40) ); }
        Sources GetVAddr() const { return Translate_VSrc( ReadBits(39,32) ); }

        uint GetSamplerWidthInDWORDS() const;  // 0, 4
        uint GetResultWidthInDWORDS() const;
        

    };

    class DSInstruction : public Instruction
    {
    public:
        uint GetOffset0() const { return ReadBits(7,0); }  // Instructions can have one 16 bit offset field or two 8-bit offset fields
        uint GetOffset1() const { return ReadBits(15,8); }
        uint GetOffset16() const { return ReadBits(15,0); }
        DSInstructions GetOpcode() const { return Translate_DSOpcodes(ReadBits(25,18)); }
        Dests GetVDest()  const { return Translate_VDest(ReadBits(63,56)); }
        Sources GetVData0() const { return Translate_VSrc(ReadBits(47,40)); }
        Sources GetVData1() const { return Translate_VSrc(ReadBits(55,48)); }
        Sources GetVAddr()  const { return Translate_VSrc(ReadBits(39,32)); }
        bool IsGDS()     const { return ReadBit(17); }  
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
    InstructionFormat GCN1Decoder::ReadInstructionFormat( const uint8* pLocation )
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
                    case 0xC0000000: return IF_SMRD; // 1100 0... ....
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

    size_t GCN1Decoder::DetermineInstructionLength( const uint8* pLocation, InstructionFormat eEncoding )
    {
        switch( eEncoding )
        {
        case IF_VOP2:       return ((const _GCN1Decoder_INTERNAL::VOP2Instruction*)pLocation)->GetLength();
        case IF_VOP1:       return ((const _GCN1Decoder_INTERNAL::VOP1Instruction*)pLocation)->GetLength();
        case IF_VOPC:       return ((const _GCN1Decoder_INTERNAL::VOPCInstruction*)pLocation)->GetLength();
        case IF_SOP2:       return ((const _GCN1Decoder_INTERNAL::SOP2Instruction*)pLocation)->GetLength();
        case IF_SOPK:       return ((const _GCN1Decoder_INTERNAL::SOPKInstruction*)pLocation)->GetLength();
        case IF_SOP1:       return ((const _GCN1Decoder_INTERNAL::SOP1Instruction*)pLocation)->GetLength();
        case IF_SOPC:       return ((const _GCN1Decoder_INTERNAL::SOPCInstruction*)pLocation)->GetLength();
        case IF_SMRD:       return ((const _GCN1Decoder_INTERNAL::SMRDInstruction*)pLocation)->GetLength();

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
            return 8;

        case IF_UNKNOWN:  
        default:
            return 0;
        }
    }

    void GCN1Decoder::Decode( Instruction* pInst, const uint8* pLocation, InstructionFormat eEncoding ) 
    {
        memset(pInst,0,sizeof(*pInst));

        switch( eEncoding )
        {     
        case IF_SOP2:       
        case IF_SOPK:       
        case IF_SOP1:       
        case IF_SOPC:       
        case IF_SOPP:
            pInst->m_eClass = IC_SCALAR;
            break;
        case IF_SMRD:  
            pInst->m_eClass = IC_SCALAR_MEM;
            break;
            
        case IF_VOP2:       
        case IF_VOP1:       
        case IF_VOPC:  
        case IF_VOP3:  
            pInst->m_eClass = IC_VECTOR;
            break;
        case IF_VINTERP:  
            pInst->m_eClass = IC_VECTOR_INTERP;
            break;
        case IF_DS:   
            pInst->m_eClass = IC_DS;
            break;
        case IF_MTBUFF: 
        case IF_MUBUFF:  
            pInst->m_eClass = IC_BUFFER;
            break;

        case IF_MIMG:      
            pInst->m_eClass = IC_IMAGE;
            break;
        case IF_EXP:
            pInst->m_eClass = IC_EXPORT;
            break;

        case IF_FLAT:       
        case IF_UNKNOWN:  
        default:
            pInst->m_eClass = IC_INVALID;
        }
        



        using namespace _GCN1Decoder_INTERNAL;
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
                Fields.m_eOpcode = pIt->GetOpcode();
                Fields.m_Dest = pIt->GetDest();
                Fields.m_nSourceCount = 2;
                Fields.m_Sources[0] = pIt->GetSrc0();
                Fields.m_Sources[1] = pIt->GetSrc1();
                if( pIt->HasLiteral() )
                    Fields.m_Literal.UInt = pIt->ReadTrailingLiteral();
            }
            break;
        case IF_SOPP:    
            {
                auto* pIt = (const SOPPInstruction*) pLocation;
                auto& Fields = pInst->Fields.Scalar;
                Fields.m_eOpcode= pIt->GetOpcode();
                Fields.m_Dest = DEST_INVALID;
                Fields.m_nSourceCount = 0;
                Fields.m_nSIMM16 = pIt->ReadSIMM16();
                Fields.m_pBranchTarget = pIt->GetBranchTarget();
            }
            break;
        case IF_SMRD: 
            {
                auto* pIt = (const SMRDInstruction*) pLocation;
                auto& Fields = pInst->Fields.ScalarMem;
                Fields.m_bIsOffsetIMM = pIt->IsOffsetIMM();
                Fields.m_eOpcode      = pIt->GetOpcode();
                Fields.m_Dest         = pIt->GetDest();
                Fields.m_nBaseReg     = pIt->GetBase();
                Fields.m_nOffset      = pIt->GetOffset();
                Fields.m_bIsGLC       = false;
            }
            break;

        case IF_VOP2: 
            {
                auto* pIt = (const VOP2Instruction*) pLocation;
                auto& Fields = pInst->Fields.Vector;
                Fields.m_eOpcode = pIt->GetOpcode();
                Fields.m_nSrcCount  = 2;
                Fields.m_nDestCount = 1;
                Fields.m_Sources[0] = pIt->GetSrc0();
                Fields.m_Sources[1] = pIt->GetVSrc1();
                Fields.m_Dests[0] = pIt->GetVDst();
                if( pIt->HasLiteral() )
                    Fields.m_Literal.UInt = pIt->ReadTrailingLiteral();
            }
            break;
        case IF_VOP1:       
            {
                auto* pIt = (const VOP1Instruction*) pLocation;
                auto& Fields = pInst->Fields.Vector;
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
                    Fields.m_nClamp = pIt->GetClamp();
                    Fields.m_nSourceAbs = pIt->GetSourceAbsMask();
                }

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
            }
            break;

        case IF_MIMG:     
            {
                auto* pIt = (const MIMGInstruction*) pLocation;
                auto& Fields = pInst->Fields.Image;
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
                Fields.m_bIsD16 = 0;
            }
            break;

        case IF_FLAT:               
        default:
            return;
        }
    }




}