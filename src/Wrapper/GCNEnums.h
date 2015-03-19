//
//    GCN Opcodes, register types, and other things.  Based on a trawling of the ISA docs.
//      Copyright 2015 Joshua Barczak, all rights reserved.
//
#ifndef _GCN_ENUMS_H_
#define _GCN_ENUMS_H_
#pragma once

namespace GCN
{
 
    enum InstructionFormat
    {
        IF_SOP2,
        IF_SOPK,
        IF_SOP1,
        IF_SOPC,
        IF_SOPP,
        IF_SMEM,
        IF_VOP2,
        IF_VOP1,
        IF_VOPC,
        IF_VOP3, 
        IF_VINTERP,
        IF_DS,
        IF_MTBUFF,
        IF_MUBUFF,
        IF_MIMG,
        IF_EXP,
        IF_FLAT, // SEA ISLANDS
        IF_UNKNOWN,
    };

    enum Sources
    {
        SGPR_FIRST  = 0,   
        SGPR_LAST   = 103, 
        FSCR_LO     = 104, // flat scratch memory descriptor, low (SEA ISLANDS)
        FSCR_HI     = 105, // flat scratcy memory descriptor, hi (SEA ISLANDS)

        VCC_LO=106,     // Vector condition code
        VCC_HI,
        TBA_LO,         // Trap handler base address
        TBA_HI,
        TMA_LO,         // Pointer to data in memory used by trap handler
        TMA_HI,
        TTMP_FIRST ,      // Trap handler temporary regs (priviledged)
        TTMP_LAST = 123,
        M0 = 124,        // Magical memory register used for various things

        // 125 reserved

        EXEC_LO=126,     // Vector exec mask
        EXEC_HI,

        C_ZERO=128,
        CI_POSITIVE_FIRST= 129, // Signed integers 1 to 64
        CI_POSITIVE_LAST = 192,

        CI_NEGATIVE_FIRST=193, // Signed integers -1 to -16
        CI_NEGATIVE_LAST = 208,

        // 209-239 reserved

        CF_ONEHALF      =240, // float constants
        CF_MINUS_ONEHALF,
        CF_ONE,
        CF_MINUS_ONE,
        CF_TWO,
        CF_MINUS_TWO,
        CF_FOUR,
        CF_MINUS_FOUR,

        // 248-250 reserved

        VCCZ        = 251, // Vector condition code
        EXECZ       = 252, // Vector EXEC mask
        SCC         = 253, // Scalar condition code

        LDS_DIRECT  = 254, // direct LDS access, with address from 'M0' (vector only
        
        LITERAL= 255, // DWORD literal following instruction

        VGPR_FIRST = 256,
        VGPR_LAST  = 511,
    };



    enum SOP2_Opcodes
    {
        S_ADD_U32               = 0 ,   //: D.u = S0.u + S1.u. SCC = carry out.
        S_SUB_U32               = 1 ,   //: D.u = S0.u - S1.u. SCC = carry out.
        S_ADD_I32               = 2 ,   //: D.u = S0.i + S1.i. SCC = overflow.
        S_SUB_I32               = 3 ,   //: D.u = S0.i - S1.i. SCC = overflow.
        S_ADDC_U32              = 4 ,   //: D.u = S0.u + S1.u + SCC. SCC = carry-out.
        S_SUBB_U32              = 5 ,   //: D.u = S0.u - S1.u - SCC. SCC = carry-out.
        S_MIN_I32               = 6 ,   //: D.i = (S0.i < S1.i) ? S0.i : S1.i. SCC = 1 if S0 is min.
        S_MIN_U32               = 7 ,   //: D.u = (S0.u < S1.u) ? S0.u : S1.u. SCC = 1 if S0 is min.
        S_MAX_I32               = 8 ,   //: D.i = (S0.i > S1.i) ? S0.i : S1.i. SCC = 1 if S0 is max.
        S_MAX_U32               = 9 ,   //: D.u = (S0.u > S1.u) ? S0.u : S1.u. SCC = 1 if S0 is max.
        S_CSELECT_B32           = 10,   //: D.u = SCC ? S0.u : S1.u.
        S_CSELECT_B64           = 11,   //: D.u = SCC ? S0.u : S1.u.
                                        // 12,13 reserved
        S_AND_B32               = 14,   //: D.u = S0.u & S1.u. SCC = 1 if result is non-zero.
        S_AND_B64               = 15,   //: D.u = S0.u & S1.u. SCC = 1 if result is non-zero.
        S_OR_B32                = 16,   //: D.u = S0.u | S1.u. SCC = 1 if result is non-zero.
        S_OR_B64                = 17,   //: D.u = S0.u | S1.u. SCC = 1 if result is non-zero.
        S_XOR_B32               = 18,   //: D.u = S0.u ^ S1.u. SCC = 1 if result is non-zero.
        S_XOR_B64               = 19,   //: D.u = S0.u ^ S1.u. SCC = 1 if result is non-zero.
        S_ANDN2_B32             = 20,   //: D.u = S0.u & ~S1.u. SCC = 1 if result is non-zero.
        S_ANDN2_B64             = 21,   //: D.u = S0.u & ~S1.u. SCC = 1 if result is non-zero.
        S_ORN2_B32              = 22,   //: D.u = S0.u | ~S1.u. SCC = 1 if result is non-zero.
        S_ORN2_B64              = 23,   //: D.u = S0.u | ~S1.u. SCC = 1 if result is non-zero.
        S_NAND_B32              = 24,   //: D.u = ~(S0.u & S1.u). SCC = 1 if result is non-zero.
        S_NAND_B64              = 25,   //: D.u = ~(S0.u & S1.u). SCC = 1 if result is non-zero.
        S_NOR_B32               = 26,   //: D.u = ~(S0.u | S1.u). SCC = 1 if result is non-zero.
        S_NOR_B64               = 27,   //: D.u = ~(S0.u | S1.u). SCC = 1 if result is non-zero.
        S_XNOR_B32              = 28,   //: D.u = ~(S0.u ^ S1.u). SCC = 1 if result is non-zero.
        S_XNOR_B64              = 29,   //: D.u = ~(S0.u ^ S1.u). SCC = 1 if result is non-zero.
        S_LSHL_B32              = 30,   //: D.u = S0.u << S1.u[4:0]. SCC = 1 if result is non-zero.
        S_LSHL_B64              = 31,   //: D.u = S0.u << S1.u[5:0]. SCC = 1 if result is non-zero.
        S_LSHR_B32              = 32,   //: D.u = S0.u >> S1.u[4:0]. SCC = 1 if result is non-zero.
        S_LSHR_B64              = 33,   //: D.u = S0.u >> S1.u[5:0]. SCC = 1 if result is non-zero.
        S_ASHR_I32              = 34,   //: D.i = signtext(S0.i) >> S1.i[4:0]. SCC = 1 if result is non-zero.
        S_ASHR_I64              = 35,   //: D.i = signtext(S0.i) >> S1.i[5:0]. SCC = 1 if result is non-zero.
        S_BFM_B32               = 36,   //: D.u = ((1 << S0.u[4:0]) - 1) << S1.u[4:0]; bitfield mask.
        S_BFM_B64               = 37,   //: D.u = ((1 << S0.u[5:0]) - 1) << S1.u[5:0]; bitfield mask.
        S_MUL_I32               = 38,   //: D.i = S0.i * S1.i.
        S_BFE_U32               = 39,    // : Bit field extract. S0 is data, S1[4:0] is field offset, S1[22:16] is field width. D.u = (S0.u >> S1.u[4:0]) & ((1 << S1.u[22:16]) - 1). SCC = 1 if resultis non-zero.
        S_BFE_I32               = 40,
        S_BFE_U64               = 41,
        S_BFE_I64               = 42,
        S_CBRANCH_G_FORK        = 43, // Conditional branch using branch stack. Arg0 = compare mask (VCC or any SGPR), Arg1 = 64-bit byte address of target instruction.
        S_ABSDIFF_I32           = 44,  //  D.i = abs(S0.i >> S1.i). SCC = 1 if result is non-zero.
    
        SOP2_MIN = S_ADD_U32,
        SOP2_MAX = S_ABSDIFF_I32,
    };

    enum SOPK_Opcodes
    {
        S_MOVK_I32              = 0 ,  //: D.i = signext(SIMM16).
                               //1 reserved.
        S_CMOVK_I32             = 2 ,  //: if (SCC) D.i = signext(SIMM16); else NOP.
        S_CMPK_EQ_I32           = 3 ,  //: SCC = (D.i == signext(SIMM16).
        S_CMPK_LG_I32           = 4 ,  //: SCC = (D.i != signext(SIMM16).
        S_CMPK_GT_I32           = 5 ,  //: SCC = (D.i != signext(SIMM16)).
        S_CMPK_GE_I32           = 6 ,  //: SCC = (D.i >= signext(SIMM16)).
        S_CMPK_LT_I32           = 7 ,  //: SCC = (D.i < signext(SIMM16)).
        S_CMPK_LE_I32           = 8 ,  //: SCC = (D.i <= signext(SIMM16)).
        S_CMPK_EQ_U32           = 9 ,  //: SCC = (D.u == SIMM16).
        S_CMPK_LG_U32           = 10, //: SCC = (D.u != SIMM16).
        S_CMPK_GT_U32           = 11, //: SCC = (D.u > SIMM16).
        S_CMPK_GE_U32           = 12, //: SCC = (D.u >= SIMM16).
        S_CMPK_LT_U32           = 13, //: SCC = (D.u < SIMM16).
        S_CMPK_LE_U32           = 14, //: D.u = SCC = (D.u <= SIMM16).
        S_ADDK_I32              = 15, //: D.i = D.i + signext(SIMM16). SCC = overflow.
        S_MULK_I32              = 16, //: D.i = D.i * signext(SIMM16). SCC = overflow.
        S_CBRANCH_I_FORK        = 17, //: Conditional branch using branch-stack.
                                          // Arg0(sdst) = compare mask (VCC or any SGPR), SIMM16 = signed DWORD
                                          // branch offset relative to next instruction.
        S_GETREG_B32            = 18, // : D.u = hardware register. Read some or all of a hardware register
                                      //     into the LSBs of D. SIMM16 = {size[4:0], offset[4:0], hwRegId[5:0]}; offset
                                      //     is 0–31, size is 1–32.
        S_SETREG_B32            = 19, // : hardware register = D.u. Write some or all of the LSBs of D
                                         //     into a hardware register (note that D is a source SGPR).
                                         //     SIMM16 = {size[4:0], offset[4:0], hwRegId[5:0]}; offset is 0–31, size is 1–32.
                                //20 reserved.          //
        S_SETREG_IMM32_B32      = 21, //: This instruction uses a 32-bit literal constant. Write
                                         //    some or all of the LSBs of IMM32 into a hardware register.
                                         //    SIMM16 = {size[4:0], offset[4:0], hwRegId[5:0]}; offset is 0–31, size is 1–32.
        SOPK_MIN=0,
        SOPK_MAX=S_SETREG_IMM32_B32
    };

    enum SOP1_Opcodes
    {
        //  0 – 2 reserved.
        S_MOV_B32               = 3   ,//: D.u = S0.u.
        S_MOV_B64               = 4   ,//: D/u = S0.u.
        S_CMOV_B32              = 5   ,//: if(SCC) D.u = S0.u; else NOP.
        S_CMOV_B64              = 6   ,//: if(SCC) D.u = S0.u; else NOP.
        S_NOT_B32               = 7   ,//: D.u = ~S0.u SCC = 1 if result non-zero.
        S_NOT_B64               = 8   ,//: D.u = ~S0.u SCC = 1 if result non-zero.
        S_WQM_B32               = 9   ,//: D.u = WholeQuadMode(S0.u). SCC = 1 if result is non-zero.
        S_WQM_B64               = 10  ,//: D.u = WholeQuadMode(S0.u). SCC = 1 if result is non-zero.
        S_BREV_B32              = 11  ,//: D.u = S0.u[0:31] (reverse bits).
        S_BREV_B64              = 12  ,//: D.u = S0.u[0:63] (reverse bits).
        S_BCNT0_I32_B32         = 13  ,//: D.i = CountZeroBits(S0.u). SCC = 1 if result is non-zero.
        S_BCNT0_I32_B64         = 14  ,//: D.i = CountZeroBits(S0.u). SCC = 1 if result is non-zero.
        S_BCNT1_I32_B32         = 15  ,//: D.i = CountOneBits(S0.u). SCC = 1 if result is non-zero.
        S_BCNT1_I32_B64         = 16  ,//: D.i = CountOneBits(S0.u). SCC = 1 if result is non-zero.
        S_FF0_I32_B32           = 17  ,//: D.i = FindFirstZero(S0.u) from LSB; if no zeros, return -1.
        S_FF0_I32_B64           = 18  ,//: D.i = FindFirstZero(S0.u) from LSB; if no zeros, return -1.
        S_FF1_I32_B32           = 19  ,//: D.i = FindFirstOne(S0.u) from LSB; if no ones, return -1.
        S_FF1_I32_B64           = 20  ,//: D.i = FindFirstOne(S0.u) from LSB; if no ones, return -1.
        S_FLBIT_I32_B32         = 21  ,//: D.i = FindFirstOne(S0.u) from MSB; if no ones, return -1.
        S_FLBIT_I32_B64         = 22  ,//: D.i = FindFirstOne(S0.u) from MSB; if no ones, return -1.
        S_FLBIT_I32             = 23  ,//: D.i = Find first bit opposite of sign bit from MSB. If S0 == -1, return -1.
        S_FLBIT_I32_I64         = 24  ,//: D.i = Find first bit opposite of sign bit from MSB. If S0 == -1, return -1.
        S_SEXT_I32_I8           = 25  ,//: D.i = signext(S0.i[7:0]).
        S_SEXT_I32_I16          = 26  ,//: D.i = signext(S0.i[15:0]).
        S_BITSET0_B32           = 27  ,//: D.u[S0.u[4:0]] = 0.
        S_BITSET0_B64           = 28  ,//: D.u[S0.u[5:0]] = 0.
        S_BITSET1_B32           = 29  ,//: D.u[S0.u[4:0]] = 1.
        S_BITSET1_B64           = 30  ,//: D.u[S0.u[5:0]] = 1.
        S_GETPC_B64             = 31  ,//: D.u = PC + 4; destination receives the byte address of the next instruction.
        S_SETPC_B64             = 32  ,//: PC = S0.u; S0.u is a byte address of the instruction to jump to.
        S_SWAPPC_B64            = 33  ,//: D.u = PC + 4; PC = S0.u.
        S_RFE_B64               = 34  ,//: Return from Exception; PC = TTMP1,0.
                                //35 reserved.
        S_AND_SAVEEXEC_B64      = 36  ,//: D.u = EXEC, EXEC = S0.u & EXEC. SCC = 1 if the new value of EXEC is non-zero.
        S_OR_SAVEEXEC_B64       = 37  ,//: D.u = EXEC, EXEC = S0.u | EXEC. SCC = 1 if the newvalue of EXEC is non-zero.
        S_XOR_SAVEEXEC_B64      = 38  ,//: D.u = EXEC, EXEC = S0.u ^ EXEC. SCC = 1 if the new value of EXEC is non-zero.
        S_ANDN2_SAVEEXEC_B64    = 39  ,//: D.u = EXEC, EXEC = S0.u & ~EXEC. SCC =1 if the new value of EXEC is non-zero.
        S_ORN2_SAVEEXEC_B64     = 40  ,//: D.u = EXEC, EXEC = S0.u | ~EXEC. SCC = 1 if the new value of EXEC is non-zero.
        S_NAND_SAVEEXEC_B64     = 41  ,//: D.u = EXEC, EXEC = ~(S0.u & EXEC). SCC =1 if the new value of EXEC is non-zero.
        S_NOR_SAVEEXEC_B64      = 42 ,//: D.u = EXEC, EXEC = ~(S0.u | EXEC). SCC = 1 if the new value of EXEC is non-zero.
        S_XNOR_SAVEEXEC_B64     = 43 ,//: D.u = EXEC, EXEC = ~(S0.u ^ EXEC). SCC = 1 if the new value of EXEC is non-zero.
        S_QUADMASK_B32          = 44 ,//: D.u = QuadMask(S0.u). D[0] = OR(S0[3:0]), D[1] = OR(S0[7:4]) .... SCC = 1 if result is non-zero.
        S_QUADMASK_B64          = 45 ,//: D.u = QuadMask(S0.u). D[0] = OR(S0[3:0]),D[1] = OR(S0[7:4]) .... SCC = 1 if result is non-zero
        S_MOVRELS_B32           = 46 ,//: SGPR[D.u] = SGPR[S0.u + M0.u].
        S_MOVRELS_B64           = 47 ,//: SGPR[D.u] = SGPR[S0.u + M0.u].
        S_MOVRELD_B32           = 48 ,//: SGPR[D.u + M0.u] = SGPR[S0.u].
        S_MOVRELD_B64           = 49 ,//: SGPR[D.u + M0.u] = SGPR[S0.u].
        S_CBRANCH_JOIN          = 50 ,//: Conditional branch join point. Arg0 = saved CSP value. No dest.
                                //51 reserved.
        S_ABS_I32               =52 ,      //: D.i = abs(S0.i). SCC=1 if result is non-zero.
        S_MOV_FED_B32           =53 ,      //: D.u = S0.u, introduce edc double error upon write to dest sgpr.
        // All other values are reserved.

        SOP1_MIN = 3,
        SOP1_MAX = S_MOV_FED_B32,
    };
    
    enum SOPC_Opcodes
    {
        S_CMP_EQ_I32            =0   ,     //: SCC = (S0.i == S1.i).
        S_CMP_LG_I32            =1   ,     //: SCC = (S0.i != S1.i).
        S_CMP_GT_I32            =2   ,     //: SCC = (S0.i > S1.i).
        S_CMP_GE_I32            =3   ,     //: SCC = (S0.i >= S1.i).
        S_CMP_LT_I32            =4   ,     //: SCC = (S0.i < S1.i).
        S_CMP_LE_I32            =5   ,     //: SCC = (S0.i <= S1.i).
        S_CMP_EQ_U32            =6   ,     //: SCC = (S0.u == S1.u).
        S_CMP_LG_U32            =7   ,     //: SCC = (S0.u != S1.u).
        S_CMP_GT_U32            =8   ,     //: SCC = (S0.u > S1.u).
        S_CMP_GE_U32            =9   ,     //: SCC = (S0.u >= S1.u).
        S_CMP_LT_U32            =10  ,     //: SCC = (S0.u < S1.u).
        S_CMP_LE_U32            =11  ,     //: SCC = (S0.u <= S1.u).
        S_BITCMP0_B32           =12  ,     //: SCC = (S0.u[S1.u[4:0]] == 0).
        S_BITCMP1_B32           =13  ,     //: SCC = (S0.u[S1.u[4:0]] == 1).
        S_BITCMP0_B64           =14  ,     //: SCC = (S0.u[S1.u[5:0]] == 0).
        S_BITCMP1_B64           =15  ,     //: SCC = (S0.u[S1.u[5:0]] == 1).
        S_SETVSKIP              =16 ,      //: VSKIP = S0.u[S1.u[4:0]].

        SOPC_MIN = 0,
        SOPC_MAX = 16,
    };

    // Special scalar instructions
    enum SOPP_Opcodes
    {
        //////////////////////////////////////////////////////
        //  SOPP
        //////////////////////////////////////////////////////

        S_NOP                      =0  ,  //: do nothing. Repeat NOP 1..8 times based on SIMM16[2:0]. 0 = 1 time, 7 = 8 times.
        S_ENDPGM                   =1  ,  //: end of program; terminate wavefront.
        S_BRANCH                   =2  ,  //: PC = PC + signext(SIMM16 * 4) + 4.
                                          // 3 reserved.
        S_CBRANCH_SCC0             =4  ,  //: if(SCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        S_CBRANCH_SCC1             =5  ,  //: if(SCC == 1) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        S_CBRANCH_VCCZ             =6  ,  //: if(VCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        S_CBRANCH_VCCNZ            =7  ,  //: if(VCC != 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        S_CBRANCH_EXECZ            =8  ,  //: if(EXEC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        S_CBRANCH_EXECNZ           =9  ,  //: if(EXEC != 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
        S_BARRIER                  =10 ,  //: Sync waves within a thread group.
                         
        // 11 unused.          
                                
        S_WAITCNT                  =12 ,  //: Wait for count of outstanding lds, vector-memory and
                                          //  export/vmem-write-data to be at or below the specified levels. simm16[3:0] =
                                          //  vmcount, simm16[6:4] = export/mem-write-data count, simm16[12:8] =
                                          //  LGKM_cnt (scalar-mem/GDS/LDS count).
        S_SETHALT                  =13 ,  //: set HALT bit to value of SIMM16[0]. 1=halt, 0=resume. Halt is ignored while priv=1.
        S_SLEEP                    =14 ,  //: Cause a wave to sleep for approximately 64*SIMM16[2:0] clocks.
        S_SETPRIO                  =15 ,  //: User settable wave priority. 0 = lowest, 3 = highest.
        S_SENDMSG                  =16 ,  //: Send a message.
        S_SENDMSGHALT              =17 ,  //: Send a message and then HALT.
        S_TRAP                     =18 ,  //: Enter the trap handler. TrapID = SIMM16[7:0]. Wait for all instructions to complete, 
                                          //    save {pc_rewind,trapID,pc} into ttmp0,1; load TBA into PC,
                                          //    set PRIV=1 and continue.
        S_ICACHE_INV               =19 ,  //: Invalidate entire L1 I cache.
        S_INCPERFLEVEL             =20 ,  //: Increment performance counter specified in SIMM16[3:0] by 1.
        S_DECPERFLEVEL             =21 ,  //: Decrement performance counter specified in SIMM16[3:0] by 1.
        S_TTRACEDATA               =22 ,  //: Send M0 as user data to thread-trace.


        // Sea islands only...
        S_CBRANCH_CDBGSYS          = 23,// : If (conditional_debug_system != 0) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
        S_CBRANCH_CDBGUSER         = 24,// : If (conditional_debug_user != 0) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
        S_CBRANCH_CDBGSYS_OR_USER  = 25,// : If (conditional_debug_system || conditional_debug_user) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
        S_CBRANCH_CDBGSYS_AND_USER = 26,// : If (conditional_debug_system && conditional_debug_user) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
    
        SOPP_MIN=0,
        SOPP_MAX=26
    };

    enum SMEM_Opcodes
    {
        S_LOAD_DWORD               = 0 ,// : Read from read-only constant memory.
        S_LOAD_DWORDX2             = 1 ,// : Read from read-only constant memory.
        S_LOAD_DWORDX4             = 2 ,// : Read from read-only constant memory.
        S_LOAD_DWORDX8             = 3 ,// : Read from read-only constant memory.
        S_LOAD_DWORDX16            = 4 ,// : Read from read-only constant memory.
        S_BUFFER_LOAD_DWORD        = 8 ,// : Read from read-only constant memory.
        S_BUFFER_LOAD_DWORDX2      = 9 ,// : Read from read-only constant memory.
        S_BUFFER_LOAD_DWORDX4      = 10,// : Read from read-only constant memory.
        S_BUFFER_LOAD_DWORDX8      = 11,// : Read from read-only constant memory.
        S_BUFFER_LOAD_DWORDX16     = 12,// : Read from read-only constant memory.
        S_DCACHE_INV_VOL           = 29,// : Invalidate all volatile lines in L1 constant cache.
        S_MEMTIME                  = 30,// : Return current 64-bit timestamp.
        S_DCACHE_INV               = 31,// : Invalidate entire L1 K cache.
    };


    enum VOP2_Opcodes
    {
        V_CNDMASK_B32              = 0 ,  //: D.u = VCC[i] ? S1.u : S0.u (i = threadID in wave).
        V_READLANE_B32             = 1 ,  //: copy one VGPR value to one SGPR. Dst = SGPR-dest,
                                          //     Src0 = Source Data (VGPR# or M0(lds-direct)), Src1 = Lane Select (SGPR
                                          //     or M0). Ignores exec mask.
        V_WRITELANE_B32            = 2 ,  //: Write value into one VGPR one one lane. Dst = VGPRdest,
                                          //  Src0 = Source Data (sgpr, m0, exec or constants), Src1 = Lane Select
                                          // (SGPR or M0). Ignores exec mask.
        V_ADD_F32                  = 3 ,  //: D.f = S0.f + S1.f.
        V_SUB_F32                  = 4 ,  //: D.f = S0.f - S1.f.
        V_SUBREV_F32               = 5 ,  //: D.f = S1.f - S0.f.
        V_MAC_LEGACY_F32           = 6 ,  //: D.f = S0.F * S1.f + D.f.
        V_MUL_LEGACY_F32           = 7 ,  //: D.f = S0.f * S1.f (DX9 rules, 0.0*x = 0.0).
        V_MUL_F32                  = 8 ,  //: D.f = S0.f * S1.f.
        V_MUL_I32_I24              = 9 ,  //: D.i = S0.i[23:0] * S1.i[23:0].
        V_MUL_HI_I32_I24           = 10,  //: D.i = (S0.i[23:0] * S1.i[23:0])>>32.
        V_MUL_U32_U24              = 11,  //: D.u = S0.u[23:0] * S1.u[23:0].
        V_MUL_HI_U32_U24           = 12,  //: D.i = (S0.u[23:0] * S1.u[23:0])>>32.
        V_MIN_LEGACY_F32           = 13,  //: D.f = min(S0.f, S1.f) (DX9 rules for NaN).
        V_MAX_LEGACY_F32           = 14,  //: D.f = max(S0.f, S1.f) (DX9 rules for NaN).
        V_MIN_F32                  = 15,  //: D.f = min(S0.f, S1.f).
        V_MAX_F32                  = 16,  //: D.f = max(S0.f, S1.f).
        V_MIN_I32                  = 17,  //: D.i = min(S0.i, S1.i).
        V_MAX_I32                  = 18,  //: D.i = max(S0.i, S1.i).
        V_MIN_U32                  = 19,  //: D.u = min(S0.u, S1.u).
        V_MAX_U32                  = 20,  //: D.u = max(S0.u, S1.u).
        V_LSHR_B32                 = 21,  //: D.u = S0.u >> S1.u[4:0].
        V_LSHRREV_B32              = 22,  //: D.u = S1.u >> S0.u[4:0].
        V_ASHR_I32                 = 23,  //: D.i = S0.i >> S1.i[4:0].
        V_ASHRREV_I32              = 24,  //: D.i = S1.i >> S0.i[4:0].
        V_LSHL_B32                 = 25,  //: D.u = S0.u << S1.u[4:0].
        V_LSHLREV_B32              = 26,  //: D.u = S1.u << S0.u[4:0].
        V_AND_B32                  = 27,  //: D.u = S0.u & S1.u.
        V_OR_B32                   = 28,  //: D.u = S0.u | S1.u.
        V_XOR_B32                  = 29,  //: D.u = S0.u ^ S1.u.
        V_BFM_B32                  = 30,  //: D.u = ((1<<S0.u[4:0])-1) << S1.u[4:0]; S0=bitfield_width,S1=bitfield_offset.
        V_MAC_F32                  = 31,  //: D.f = S0.f * S1.f + D.f.
        V_MADMK_F32                = 32,  //: D.f = S0.f * K + S1.f; K is a 32-bit inline constant.
        V_MADAK_F32                = 33,  //: D.f = S0.f * S1.f + K; K is a 32-bit inline constant.
        V_BCNT_U32_B32             = 34,  //: D.u = CountOneBits(S0.u) + S1.u. Bit count.
        V_MBCNT_LO_U32_B32         = 35,  //: ThreadMask = (1 << ThreadPosition) - 1; 
                                          //    D.u = CountOneBits(S0.u & ThreadMask[31:0]) + S1.u. Masked bit count, ThreadPosition
                                          //    is the position of this thread in the wavefront (in 0 63).
        V_MBCNT_HI_U32_B32         = 36,  //: ThreadMask = (1 << ThreadPosition) - 1; 
                                          //    D.u = CountOneBits(S0.u & ThreadMask[63:32]) + S1.u. Masked bit count,
                                          //   ThreadPosition is the position of this thread in the wavefront (in 0..63).
                                   
        V_ADD_I32                  = 37, // : D.u = S0.u + S1.u; VCC=carry-out (VOP3:sgpr=carry-out).
        V_SUB_I32                  = 38, // : D.u = S0.u - S1.u; VCC=carry-out (VOP3:sgpr=carry-out).
        V_SUBREV_I32               = 39, // : D.u = S1.u - S0.u; VCC=carry-out (VOP3:sgpr=carry-out).
        V_ADDC_U32                 = 40, // : D.u = S0.u + S1.u + VCC; VCC=carry-out (VOP3:sgpr=carryout, S2.u=carry-in).
        V_SUBB_U32                 = 41, // : D.u = S0.u - S1.u - VCC; VCC=carry-out (VOP3:sgpr=carry-out,S2.u=carry-in).
        V_SUBBREV_U32              = 42, // : D.u = S1.u - S0.u - VCC; VCC=carry-out (VOP3:sgpr=carryout,S2.u=carry-in).
        V_LDEXP_F32                = 43, // : D.d = pow(S0.f, S1.i).
        V_CVT_PKACCUM_U8_F32       = 44, // : f32->u8(s0.f), pack into byte(s1.u), of dst.
        V_CVT_PKNORM_I16_F32       = 45, // : D = {(snorm)S1.f, (snorm)S0.f}.
        V_CVT_PKNORM_U16_F32       = 46, // : D = {(unorm)S1.f, (unorm)S0.f}.
        V_CVT_PKRTZ_F16_F32        = 47, // : D = {flt32_to_flt16(S1.f),flt32_to_flt16(S0.f)}, with round-toward-zero.
        V_CVT_PK_U16_U32           = 48, // : D = {(u32->u16)S1.u, (u32->u16)S0.u}.
        V_CVT_PK_I16_I32           = 49, // : D = {(i32->i16)S1.i, (i32->i16)S0.i}.
                                   // All other values are reserved.
                                   // 62 – 63 Do not use (maps to VOP1 and VOPC).
    };

    enum VOP1_Opcodes
    {     
        V_NOP                      =0  ,     //: do nothing.
        V_MOV_B32                  =1  ,     //: D.u = S0.u.
        V_READFIRSTLANE_B32        =2  ,     //: copy one VGPR value to one SGPR. Dst = SGPRdest,
                                             //         Src0 = Source Data (VGPR# or M0(lds-direct)), Lane# =
                                             //         FindFirst1fromLSB(exec) (lane = 0 if exec is zero). Ignores exec mask.
        V_CVT_I32_F64              =3  ,     //: D.i = (int)S0.d.
        V_CVT_F64_I32              =4  ,     //: D.f = (float)S0.i.
        V_CVT_F32_I32              =5  ,     //: D.f = (float)S0.i.
        V_CVT_F32_U32              =6  ,     //: D.f = (float)S0.u.
        V_CVT_U32_F32              =7  ,     //: D.u = (unsigned)S0.f.
        V_CVT_I32_F32              =8  ,     //: D.i = (int)S0.f.
        V_MOV_FED_B32              =9  ,     //: D.u = S0.u, introduce edc double error upon write to dest vgpr without causing an exception.
        V_CVT_F16_F32              =10 ,    //: D.f16 = flt32_to_flt16(S0.f).
        V_CVT_F32_F16              =11 ,    //: D.f = flt16_to_flt32(S0.f16).
        V_CVT_RPI_I32_F32          =12 ,    //: D.i = (int)floor(S0.f + 0.5).
        V_CVT_FLR_I32_F32          =13 ,    //: D.i = (int)floor(S0.f).
        V_CVT_OFF_F32_I4           =14 ,    //: 4-bit signed int to 32-bit float. For interpolation in shader.
        V_CVT_F32_F64              =15 ,    //: D.f = (float)S0.d.
        V_CVT_F64_F32              =16 ,    //: D.d = (double)S0.f.
        V_CVT_F32_UBYTE0           =17 ,    //: D.f = UINT2FLT(S0.u[7:0]).
        V_CVT_F32_UBYTE1           =18 ,    //: D.f = UINT2FLT(S0.u[15:8]).
        V_CVT_F32_UBYTE2           =19 ,    //: D.f = UINT2FLT(S0.u[23:16]).
        V_CVT_F32_UBYTE3           =20 ,    //: D.f = UINT2FLT(S0.u[31:24]).
        V_CVT_U32_F64              =21 ,    //: D.u = (uint)S0.d.
        V_CVT_F64_U32              =22 ,    //: D.d = (double)S0.u.
        // 23-26 Sea islands only
        V_TRUNC_F64                = 23 , //: D.d = trunc(S0.d), return integer part of S0.d.
        V_CEIL_F64                 = 24 , //: D.d = trunc(S0.d); if (S0.d > 0.0 && S0.d != D.d), D.d += 1.0.
        V_RNDNE_F64                = 25 , //: D.d = round_nearest_even(S0.d).
        V_FLOOR_F64                = 26 , //: D.d = trunc(S0.d); if (S0.d < 0.0 && S0.d != D.d), D.d += -1.0.
        //    27 – 31 reserved.
        V_FRACT_F32                =32 ,    //: D.f = S0.f - floor(S0.f).
        V_TRUNC_F32                =33 ,    //: D.f = trunc(S0.f), return integer part of S0.
        V_CEIL_F32                 =34 ,    //: D.f = ceil(S0.f). Implemented as: D.f = trunc(S0.f);  if (S0 > 0.0 && S0 != D), D += 1.0.
        V_RNDNE_F32                =35 ,    //: D.f = round_nearest_even(S0.f).
        V_FLOOR_F32                =36 ,    //: D.f = trunc(S0); if ((S0 < 0.0) && (S0 != D)) D += -1.0.
        V_EXP_F32                  =37 ,    //: D.f = pow(2.0, S0.f).
        V_LOG_CLAMP_F32            =38 ,    //: D.f = log2(S0.f), clamp -infinity to -max_float.
        V_LOG_F32                  =39 ,    //: D.f = log2(S0.f).
        V_RCP_CLAMP_F32            =40 ,    //: D.f = 1.0 / S0.f, result clamped to +-max_float.
        V_RCP_LEGACY_F32           =41 ,    //: D.f = 1.0 / S0.f, +-infinity result clamped to +-0.0.
        V_RCP_F32                  =42 ,    //: D.f = 1.0 / S0.f.
        V_RCP_IFLAG_F32            =43 ,    //: D.f = 1.0 / S0.f, only integer div_by_zero flag can be raised.
        V_RSQ_CLAMP_F32            =44 ,    //: D.f = 1.0 / sqrt(S0.f), result clamped to +-max_float.
        V_RSQ_LEGACY_F32           =45 ,    //: D.f = 1.0 / sqrt(S0.f).
        V_RSQ_F32                  =46 ,    //: D.f = 1.0 / sqrt(S0.f).
        V_RCP_F64                  =47 ,    //: D.d = 1.0 / (S0.d).
        V_RCP_CLAMP_F64            =48 ,    //: D.f = 1.0 / (S0.f), result clamped to +-max_float.
        V_RSQ_F64                  =49 ,    //: D.f = 1.0 / sqrt(S0.f).
        V_RSQ_CLAMP_F64            =50 ,    //: D.d = 1.0 / sqrt(S0.d), result clamped to +-max_float.
        V_SQRT_F32                 =51 ,    //: D.f = sqrt(S0.f).
        V_SQRT_F64                 =52 ,    //: D.d = sqrt(S0.d).
        V_SIN_F32                  =53 ,    //: D.f = sin(S0.f).
        V_COS_F32                  =54 ,    //: D.f = cos(S0.f).
        V_NOT_B32                  =55 ,    //: D.u = ~S0.u.
        V_BFREV_B32                =56 ,    //: D.u[31:0] = S0.u[0:31], bitfield reverse.
        V_FFBH_U32                 =57 ,    //: D.u = position of first 1 in S0 from MSB; D=0xFFFFFFFF if S0==0.
        V_FFBL_B32                 =58 ,    //: D.u = position of first 1 in S0 from LSB; D=0xFFFFFFFF if S0==0.
        V_FFBH_I32                 =59 ,    //: D.u = position of first bit different from sign bit in S0 from MSB;D=0xFFFFFFFF if S0==0 or 0xFFFFFFFF.
        V_FREXP_EXP_I32_F64        =60 ,    //: See V_FREXP_EXP_I32_F32.
        V_FREXP_MANT_F64           =61 ,    //: See V_FREXP_MANT_F32.
        V_FRACT_F64                =62 ,    //: S0.d - floor(S0.d).
        V_FREXP_EXP_I32_F32        =63 ,    //: If (S0.f == INF || S0.f == NAN), then D.i = 0; else D.i
                                             //    = TwosComplement(Exponent(S0.f) - 127 + 1). Returns exponent of single
                                             //    precision float input, such that S0.f = significand * (2 ** exponent). See also
                                             //    FREXP_MANT_F32, which returns the significand.
        V_FREXP_MANT_F32           =64 ,    //: if (S0.f == INF || S0.f == NAN) then D.f = S0.f; else D.f =
                                               // Mantissa(S0.f). Result range is in (-1.0,-0.5][0.5,1.0) in normal cases.
                                               // Returns binary significand of single precision float input, such that S0.f = significand
                                               // * (2 ** exponent). See also FREXP_EXP_I32_F32, which returns integer
                                               // exponent.
        V_CLREXCP                  =65 ,    //: Clear wave's exception state in SIMD(SP).
        V_MOVRELD_B32              =66 ,    //: VGPR[D.u + M0.u] = VGPR[S0.u].
        V_MOVRELS_B32              =67 ,    //: VGPR[D.u] = VGPR[S0.u + M0.u].
        V_MOVRELSD_B32             =68 ,    //: VGPR[D.u + M0.u] = VGPR[S0.u + M0.u].
        // Sea Islands only..
        V_LOG_LEGACY_F32           = 69,    //: D.f = log2(S0.f). Base 2 logarithm. Same as Southern Islands. (CI Doc says "..same as Sea Islands" typo?)
        V_EXP_LEGACY_F32           = 70,    //: D.f = pow(2.0, S0.f). Same as Southern Islands.

        //All other values are reserved.
    };
    
    enum VOPC_Opcodes
    {
        V_CMP_F_F32         =  0x00    , // D.u = 0    //   Signal on sNaN input only.
        V_CMP_LT_F32        , // D.u = (S0 < S1)
        V_CMP_EQ_F32        , // D.u = (S0 == S1)
        V_CMP_LE_F32        , // D.u = (S0 <= S1)
        V_CMP_GT_F32        , // D.u = (S0 > S1)
        V_CMP_LG_F32        , // D.u = (S0 <> S1)
        V_CMP_GE_F32        , // D.u = (S0 >= S1)
        V_CMP_O_F32         , // D.u = (!isNaN(S0) && !isNaN(S1))
        V_CMP_U_F32         , // D.u = (!isNaN(S0) || !isNaN(S1))
        V_CMP_NGE_F32       , // D.u = !(S0 >= S1)
        V_CMP_NLG_F32       , // D.u = !(S0 <> S1)
        V_CMP_NGT_F32       , // D.u = !(S0 > S1)
        V_CMP_NLE_F32       , // D.u = !(S0 <= S1)
        V_CMP_NEQ_F32       , // D.u = !(S0 == S1)
        V_CMP_NLT_F32       , // D.u = !(S0 < S1)
        V_CMP_TRU_F32       , // D.u = 1
        V_CMPX_F_F32        =  0x10    , //   Signal on sNaN input only. Also write EXEC.
        V_CMPX_LT_F32       ,  // D.u = (S0 < S1)            
        V_CMPX_EQ_F32       ,  // D.u = (S0 == S1) 
        V_CMPX_LE_F32       ,  // D.u = (S0 <= S1) 
        V_CMPX_GT_F32       ,  // D.u = (S0 > S1) 
        V_CMPX_LG_F32       ,  // D.u = (S0 <> S1) 
        V_CMPX_GE_F32       ,  // D.u = (S0 >= S1) 
        V_CMPX_O_F32        ,  // D.u = (!isNaN(S0) && !isNaN(S1))  
        V_CMPX_U_F32        ,  // D.u = (!isNaN(S0) || !isNaN(S1))  
        V_CMPX_NGE_F32      ,  // D.u = !(S0 >= S1)
        V_CMPX_NLG_F32      ,  // D.u = !(S0 <> S1)
        V_CMPX_NGT_F32      ,  // D.u = !(S0 > S1)
        V_CMPX_NLE_F32      ,  // D.u = !(S0 <= S1)
        V_CMPX_NEQ_F32      ,  // D.u = !(S0 == S1)
        V_CMPX_NLT_F32      ,  // D.u = !(S0 < S1)
        V_CMPX_TRU_F32      ,  // D.u = 1
        V_CMP_F_F64         =  0x20    , //   Signal on sNaN input only.
        V_CMP_LT_F64        ,   // D.u = (S0 < S1)
        V_CMP_EQ_F64        ,   // D.u = (S0 == S1)
        V_CMP_LE_F64        ,   // D.u = (S0 <= S1)
        V_CMP_GT_F64        ,   // D.u = (S0 > S1)
        V_CMP_LG_F64        ,   // D.u = (S0 <> S1)
        V_CMP_GE_F64        ,   // D.u = (S0 >= S1)
        V_CMP_O_F64         ,   // D.u = (!isNaN(S0) && !isNaN(S1)) 
        V_CMP_U_F64         ,   // D.u = (!isNaN(S0) || !isNaN(S1)) 
        V_CMP_NGE_F64       ,   // D.u = !(S0 >= S1)
        V_CMP_NLG_F64       ,   // D.u = !(S0 <> S1)
        V_CMP_NGT_F64       ,   // D.u = !(S0 > S1)
        V_CMP_NLE_F64       ,   // D.u = !(S0 <= S1)
        V_CMP_NEQ_F64       ,   // D.u = !(S0 == S1)
        V_CMP_NLT_F64       ,   // D.u = !(S0 < S1)
        V_CMP_TRU_F64       ,   // D.u = 1
        V_CMPX_F_F64        =  0x30    , //   Signal on sNaN input only. Also write EXEC.
        V_CMPX_LT_F64       ,   // D.u = (S0 < S1)
        V_CMPX_EQ_F64       ,   // D.u = (S0 == S1)
        V_CMPX_LE_F64       ,   // D.u = (S0 <= S1)
        V_CMPX_GT_F64       ,   // D.u = (S0 > S1)
        V_CMPX_LG_F64       ,   // D.u = (S0 <> S1)
        V_CMPX_GE_F64       ,   // D.u = (S0 >= S1)
        V_CMPX_O_F64        ,   // D.u = (!isNaN(S0) && !isNaN(S1)) 
        V_CMPX_U_F64        ,   // D.u = (!isNaN(S0) || !isNaN(S1)) 
        V_CMPX_NGE_F64      ,   // D.u = !(S0 >= S1)
        V_CMPX_NLG_F64      ,   // D.u = !(S0 <> S1)
        V_CMPX_NGT_F64      ,   // D.u = !(S0 > S1)
        V_CMPX_NLE_F64      ,   // D.u = !(S0 <= S1)
        V_CMPX_NEQ_F64      ,   // D.u = !(S0 == S1)
        V_CMPX_NLT_F64      ,   // D.u = !(S0 < S1)
        V_CMPX_TRU_F64      ,   // D.u = 1
        V_CMPS_F_F32        =  0x40    , //   Signal on any NaN.
        V_CMPS_LT_F32       ,  // D.u = (S0 < S1)
        V_CMPS_EQ_F32       ,  // D.u = (S0 == S1)
        V_CMPS_LE_F32       ,  // D.u = (S0 <= S1)
        V_CMPS_GT_F32       ,  // D.u = (S0 > S1)
        V_CMPS_LG_F32       ,  // D.u = (S0 <> S1)
        V_CMPS_GE_F32       ,  // D.u = (S0 >= S1)
        V_CMPS_O_F32        ,  // D.u = (!isNaN(S0) && !isNaN(S1)) 
        V_CMPS_U_F32        ,  // D.u = (!isNaN(S0) || !isNaN(S1)) 
        V_CMPS_NGE_F32      ,  // D.u = !(S0 >= S1)
        V_CMPS_NLG_F32      ,  // D.u = !(S0 <> S1)
        V_CMPS_NGT_F32      ,  // D.u = !(S0 > S1)
        V_CMPS_NLE_F32      ,  // D.u = !(S0 <= S1)
        V_CMPS_NEQ_F32      ,  // D.u = !(S0 == S1)
        V_CMPS_NLT_F32      ,  // D.u = !(S0 < S1)
        V_CMPS_TRU_F32      ,  // D.u = 1
        V_CMPSX_F_F32       =  0x50    , //   Signal on any NaN. Also write EXEC.
        V_CMPSX_LT_F32      ,  // D.u = (S0 < S1)
        V_CMPSX_EQ_F32      ,  // D.u = (S0 == S1)
        V_CMPSX_LE_F32      ,  // D.u = (S0 <= S1)
        V_CMPSX_GT_F32      ,  // D.u = (S0 > S1)
        V_CMPSX_LG_F32      ,  // D.u = (S0 <> S1)
        V_CMPSX_GE_F32      ,  // D.u = (S0 >= S1)
        V_CMPSX_O_F32       ,  // D.u = (!isNaN(S0) && !isNaN(S1)) 
        V_CMPSX_U_F32       ,  // D.u = (!isNaN(S0) || !isNaN(S1)) 
        V_CMPSX_NGE_F32     ,  // D.u = !(S0 >= S1)
        V_CMPSX_NLG_F32     ,  // D.u = !(S0 <> S1)
        V_CMPSX_NGT_F32     ,  // D.u = !(S0 > S1)
        V_CMPSX_NLE_F32     ,  // D.u = !(S0 <= S1)
        V_CMPSX_NEQ_F32     ,  // D.u = !(S0 == S1)
        V_CMPSX_NLT_F32     ,  // D.u = !(S0 < S1)
        V_CMPSX_TRU_F32     ,  // D.u = 1
        V_CMPS_F_F64        =  0x60    , //   Signal on any NaN.
        V_CMPS_LT_F64       ,   // D.u = (S0 < S1)
        V_CMPS_EQ_F64       ,   // D.u = (S0 == S1)
        V_CMPS_LE_F64       ,   // D.u = (S0 <= S1)
        V_CMPS_GT_F64       ,   // D.u = (S0 > S1)
        V_CMPS_LG_F64       ,   // D.u = (S0 <> S1)
        V_CMPS_GE_F64       ,   // D.u = (S0 >= S1)
        V_CMPS_O_F64        ,   // D.u = (!isNaN(S0) && !isNaN(S1)) 
        V_CMPS_U_F64        ,   // D.u = (!isNaN(S0) || !isNaN(S1)) 
        V_CMPS_NGE_F64      ,   // D.u = !(S0 >= S1)
        V_CMPS_NLG_F64      ,   // D.u = !(S0 <> S1)
        V_CMPS_NGT_F64      ,   // D.u = !(S0 > S1)
        V_CMPS_NLE_F64      ,   // D.u = !(S0 <= S1)
        V_CMPS_NEQ_F64      ,   // D.u = !(S0 == S1)
        V_CMPS_NLT_F64      ,   // D.u = !(S0 < S1)
        V_CMPS_TRU_F64      ,   // D.u = 1
        V_CMPSX_F_F64        =  0x70    , //   Signal on any NaN. Also write EXEC.
        V_CMPSX_LT_F64      ,   // D.u = (S0 < S1)
        V_CMPSX_EQ_F64      ,   // D.u = (S0 == S1)
        V_CMPSX_LE_F64      ,   // D.u = (S0 <= S1)
        V_CMPSX_GT_F64      ,   // D.u = (S0 > S1)
        V_CMPSX_LG_F64      ,   // D.u = (S0 <> S1)
        V_CMPSX_GE_F64      ,   // D.u = (S0 >= S1)
        V_CMPSX_O_F64       ,   // D.u = (!isNaN(S0) && !isNaN(S1)) 
        V_CMPSX_U_F64       ,   // D.u = (!isNaN(S0) || !isNaN(S1)) 
        V_CMPSX_NGE_F64     ,   // D.u = !(S0 >= S1)
        V_CMPSX_NLG_F64     ,   // D.u = !(S0 <> S1)
        V_CMPSX_NGT_F64     ,   // D.u = !(S0 > S1)
        V_CMPSX_NLE_F64     ,   // D.u = !(S0 <= S1)
        V_CMPSX_NEQ_F64     ,   // D.u = !(S0 == S1)
        V_CMPSX_NLT_F64     ,   // D.u = !(S0 < S1)
        V_CMPSX_TRU_F64     ,   // D.u = 1
        V_CMP_F_I32         =  0x80, //D.u = 0  , //   On 32-bit integers.
        V_CMP_LT_I32        , //D.u = (S0 < S1)
        V_CMP_EQ_I32        , //D.u = (S0 == S1)
        V_CMP_LE_I32        , //D.u = (S0 <= S1)
        V_CMP_GT_I32        , //D.u = (S0 > S1)
        V_CMP_LG_I32        , //D.u = (S0 <> S1)
        V_CMP_GE_I32        , //D.u = (S0 >= S1)
        V_CMP_TRU_I32       , //D.u = 1
        V_CMPX_F_I32        =  0x90    , //   Also write EXEC.
        V_CMPX_LT_I32       ,//D.u = (S0 < S1)
        V_CMPX_EQ_I32       ,//D.u = (S0 == S1)
        V_CMPX_LE_I32       ,//D.u = (S0 <= S1)
        V_CMPX_GT_I32       ,//D.u = (S0 > S1)
        V_CMPX_LG_I32       ,//D.u = (S0 <> S1)
        V_CMPX_GE_I32       ,//D.u = (S0 >= S1)
        V_CMPX_TRU_I32      ,//D.u = 1
        V_CMP_F_I64           =  0xA0    , //    On 64-bit integers.
        V_CMP_LT_I64        ,//D.u = (S0 < S1)
        V_CMP_EQ_I64        ,//D.u = (S0 == S1)
        V_CMP_LE_I64        ,//D.u = (S0 <= S1)
        V_CMP_GT_I64        ,//D.u = (S0 > S1)
        V_CMP_LG_I64        ,//D.u = (S0 <> S1)
        V_CMP_GE_I64        ,//D.u = (S0 >= S1)
        V_CMP_TRU_I64       ,//D.u = 1
        V_CMPX_F_I64          =  0xB0    , //    Also write EXEC.
        V_CMPX_LT_I64       ,//D.u = (S0 < S1)
        V_CMPX_EQ_I64       ,//D.u = (S0 == S1)
        V_CMPX_LE_I64       ,//D.u = (S0 <= S1)
        V_CMPX_GT_I64       ,//D.u = (S0 > S1)
        V_CMPX_LG_I64       ,//D.u = (S0 <> S1)
        V_CMPX_GE_I64       ,//D.u = (S0 >= S1)
        V_CMPX_TRU_I64      ,//D.u = 1
        V_CMP_F_U32          =  0xC0    , //    On unsigned 32-bit integers.
        V_CMP_LT_U32        ,//D.u = (S0 < S1)
        V_CMP_EQ_U32        ,//D.u = (S0 == S1)
        V_CMP_LE_U32        ,//D.u = (S0 <= S1)
        V_CMP_GT_U32        ,//D.u = (S0 > S1)
        V_CMP_LG_U32        ,//D.u = (S0 <> S1)
        V_CMP_GE_U32        ,//D.u = (S0 >= S1)
        V_CMP_TRU_U32       ,//D.u = 1
        V_CMPX_F_U32            =  0xD0    , //    Also write EXEC.
        V_CMPX_LT_U32       ,//D.u = (S0 < S1)
        V_CMPX_EQ_U32       ,//D.u = (S0 == S1)
        V_CMPX_LE_U32       ,//D.u = (S0 <= S1)
        V_CMPX_GT_U32       ,//D.u = (S0 > S1)
        V_CMPX_LG_U32       ,//D.u = (S0 <> S1)
        V_CMPX_GE_U32       ,//D.u = (S0 >= S1)
        V_CMPX_TRU_U32      ,//D.u = 1
        V_CMP_F_U64           =  0xE0    , //    On unsigned 64-bit integers.
        V_CMP_LT_U64        ,//D.u = (S0 < S1)
        V_CMP_EQ_U64        ,//D.u = (S0 == S1)
        V_CMP_LE_U64        ,//D.u = (S0 <= S1)
        V_CMP_GT_U64        ,//D.u = (S0 > S1)
        V_CMP_LG_U64        ,//D.u = (S0 <> S1)
        V_CMP_GE_U64        ,//D.u = (S0 >= S1)
        V_CMP_TRU_U64       ,//D.u = 1
        V_CMPx_F_U64        =  0xF0    , //    Also write EXEC.
        V_CMPx_LT_U64       ,//D.u = (S0 < S1)
        V_CMPx_EQ_U64       ,//D.u = (S0 == S1)
        V_CMPx_LE_U64       ,//D.u = (S0 <= S1)
        V_CMPx_GT_U64       ,//D.u = (S0 > S1)
        V_CMPx_LG_U64       ,//D.u = (S0 <> S1)
        V_CMPx_GE_U64       ,//D.u = (S0 >= S1)
        V_CMPx_TRU_U64      ,//D.u = 1
        V_CMP_CLASS_F32     =  0x88   , //  D = IEEE numeric class function specified in S1.u, performed on S0.f.
        V_CMPX_CLASS_F32    =  0x98   , //  D = IEEE numeric class function specified in S1.u, performed on S0.f. Also write EXEC.
        V_CMP_CLASS_F64     =  0xA8   , //  D = IEEE numeric class function specified in S1.u, performed on S0.d.
        V_CMPX_CLASS_F64    =  0xB8   , //  D = IEEE numeric class function specified inS1.u, performed on S0.d. Also write EXEC.
        // TODO:   These opcodes are overloaded. e.g.  in reality 0x88-0x98 are all V_CMP_CLASS_F32 
    };


    enum VOP3_Opcodes
    {
         
        // 0-255 are the VOPC instructions in VOP3a form
        V3_CMP_F_F32         =  0x00    , // D.u = 0    //   Signal on sNaN input only.
        V3_CMP_LT_F32        , // D.u = (S0 < S1)
        V3_CMP_EQ_F32        , // D.u = (S0 == S1)
        V3_CMP_LE_F32        , // D.u = (S0 <= S1)
        V3_CMP_GT_F32        , // D.u = (S0 > S1)
        V3_CMP_LG_F32        , // D.u = (S0 <> S1)
        V3_CMP_GE_F32        , // D.u = (S0 >= S1)
        V3_CMP_O_F32         , // D.u = (!isNaN(S0) && !isNaN(S1))
        V3_CMP_U_F32         , // D.u = (!isNaN(S0) || !isNaN(S1))
        V3_CMP_NGE_F32       , // D.u = !(S0 >= S1)
        V3_CMP_NLG_F32       , // D.u = !(S0 <> S1)
        V3_CMP_NGT_F32       , // D.u = !(S0 > S1)
        V3_CMP_NLE_F32       , // D.u = !(S0 <= S1)
        V3_CMP_NEQ_F32       , // D.u = !(S0 == S1)
        V3_CMP_NLT_F32       , // D.u = !(S0 < S1)
        V3_CMP_TRU_F32       , // D.u = 1
        V3_CMPX_F_F32        =  0x10    , //   Signal on sNaN input only. Also write EXEC.
        V3_CMPX_LT_F32       ,  // D.u = (S0 < S1)            
        V3_CMPX_EQ_F32       ,  // D.u = (S0 == S1) 
        V3_CMPX_LE_F32       ,  // D.u = (S0 <= S1) 
        V3_CMPX_GT_F32       ,  // D.u = (S0 > S1) 
        V3_CMPX_LG_F32       ,  // D.u = (S0 <> S1) 
        V3_CMPX_GE_F32       ,  // D.u = (S0 >= S1) 
        V3_CMPX_O_F32        ,  // D.u = (!isNaN(S0) && !isNaN(S1))  
        V3_CMPX_U_F32        ,  // D.u = (!isNaN(S0) || !isNaN(S1))  
        V3_CMPX_NGE_F32      ,  // D.u = !(S0 >= S1)
        V3_CMPX_NLG_F32      ,  // D.u = !(S0 <> S1)
        V3_CMPX_NGT_F32      ,  // D.u = !(S0 > S1)
        V3_CMPX_NLE_F32      ,  // D.u = !(S0 <= S1)
        V3_CMPX_NEQ_F32      ,  // D.u = !(S0 == S1)
        V3_CMPX_NLT_F32      ,  // D.u = !(S0 < S1)
        V3_CMPX_TRU_F32      ,  // D.u = 1
        V3_CMP_F_F64         =  0x20    , //   Signal on sNaN input only.
        V3_CMP_LT_F64        ,   // D.u = (S0 < S1)
        V3_CMP_EQ_F64        ,   // D.u = (S0 == S1)
        V3_CMP_LE_F64        ,   // D.u = (S0 <= S1)
        V3_CMP_GT_F64        ,   // D.u = (S0 > S1)
        V3_CMP_LG_F64        ,   // D.u = (S0 <> S1)
        V3_CMP_GE_F64        ,   // D.u = (S0 >= S1)
        V3_CMP_O_F64         ,   // D.u = (!isNaN(S0) && !isNaN(S1)) 
        V3_CMP_U_F64         ,   // D.u = (!isNaN(S0) || !isNaN(S1)) 
        V3_CMP_NGE_F64       ,   // D.u = !(S0 >= S1)
        V3_CMP_NLG_F64       ,   // D.u = !(S0 <> S1)
        V3_CMP_NGT_F64       ,   // D.u = !(S0 > S1)
        V3_CMP_NLE_F64       ,   // D.u = !(S0 <= S1)
        V3_CMP_NEQ_F64       ,   // D.u = !(S0 == S1)
        V3_CMP_NLT_F64       ,   // D.u = !(S0 < S1)
        V3_CMP_TRU_F64       ,   // D.u = 1
        V3_CMPX_F_F64        =  0x30    , //   Signal on sNaN input only. Also write EXEC.
        V3_CMPX_LT_F64       ,   // D.u = (S0 < S1)
        V3_CMPX_EQ_F64       ,   // D.u = (S0 == S1)
        V3_CMPX_LE_F64       ,   // D.u = (S0 <= S1)
        V3_CMPX_GT_F64       ,   // D.u = (S0 > S1)
        V3_CMPX_LG_F64       ,   // D.u = (S0 <> S1)
        V3_CMPX_GE_F64       ,   // D.u = (S0 >= S1)
        V3_CMPX_O_F64        ,   // D.u = (!isNaN(S0) && !isNaN(S1)) 
        V3_CMPX_U_F64        ,   // D.u = (!isNaN(S0) || !isNaN(S1)) 
        V3_CMPX_NGE_F64      ,   // D.u = !(S0 >= S1)
        V3_CMPX_NLG_F64      ,   // D.u = !(S0 <> S1)
        V3_CMPX_NGT_F64      ,   // D.u = !(S0 > S1)
        V3_CMPX_NLE_F64      ,   // D.u = !(S0 <= S1)
        V3_CMPX_NEQ_F64      ,   // D.u = !(S0 == S1)
        V3_CMPX_NLT_F64      ,   // D.u = !(S0 < S1)
        V3_CMPX_TRU_F64      ,   // D.u = 1
        V3_CMPS_F_F32        =  0x40    , //   Signal on any NaN.
        V3_CMPS_LT_F32       ,  // D.u = (S0 < S1)
        V3_CMPS_EQ_F32       ,  // D.u = (S0 == S1)
        V3_CMPS_LE_F32       ,  // D.u = (S0 <= S1)
        V3_CMPS_GT_F32       ,  // D.u = (S0 > S1)
        V3_CMPS_LG_F32       ,  // D.u = (S0 <> S1)
        V3_CMPS_GE_F32       ,  // D.u = (S0 >= S1)
        V3_CMPS_O_F32        ,  // D.u = (!isNaN(S0) && !isNaN(S1)) 
        V3_CMPS_U_F32        ,  // D.u = (!isNaN(S0) || !isNaN(S1)) 
        V3_CMPS_NGE_F32      ,  // D.u = !(S0 >= S1)
        V3_CMPS_NLG_F32      ,  // D.u = !(S0 <> S1)
        V3_CMPS_NGT_F32      ,  // D.u = !(S0 > S1)
        V3_CMPS_NLE_F32      ,  // D.u = !(S0 <= S1)
        V3_CMPS_NEQ_F32      ,  // D.u = !(S0 == S1)
        V3_CMPS_NLT_F32      ,  // D.u = !(S0 < S1)
        V3_CMPS_TRU_F32      ,  // D.u = 1
        V3_CMPSX_F_F32       =  0x50    , //   Signal on any NaN. Also write EXEC.
        V3_CMPSX_LT_F32      ,  // D.u = (S0 < S1)
        V3_CMPSX_EQ_F32      ,  // D.u = (S0 == S1)
        V3_CMPSX_LE_F32      ,  // D.u = (S0 <= S1)
        V3_CMPSX_GT_F32      ,  // D.u = (S0 > S1)
        V3_CMPSX_LG_F32      ,  // D.u = (S0 <> S1)
        V3_CMPSX_GE_F32      ,  // D.u = (S0 >= S1)
        V3_CMPSX_O_F32       ,  // D.u = (!isNaN(S0) && !isNaN(S1)) 
        V3_CMPSX_U_F32       ,  // D.u = (!isNaN(S0) || !isNaN(S1)) 
        V3_CMPSX_NGE_F32     ,  // D.u = !(S0 >= S1)
        V3_CMPSX_NLG_F32     ,  // D.u = !(S0 <> S1)
        V3_CMPSX_NGT_F32     ,  // D.u = !(S0 > S1)
        V3_CMPSX_NLE_F32     ,  // D.u = !(S0 <= S1)
        V3_CMPSX_NEQ_F32     ,  // D.u = !(S0 == S1)
        V3_CMPSX_NLT_F32     ,  // D.u = !(S0 < S1)
        V3_CMPSX_TRU_F32     ,  // D.u = 1
        V3_CMPS_F_F64        =  0x60    , //   Signal on any NaN.
        V3_CMPS_LT_F64       ,   // D.u = (S0 < S1)
        V3_CMPS_EQ_F64       ,   // D.u = (S0 == S1)
        V3_CMPS_LE_F64       ,   // D.u = (S0 <= S1)
        V3_CMPS_GT_F64       ,   // D.u = (S0 > S1)
        V3_CMPS_LG_F64       ,   // D.u = (S0 <> S1)
        V3_CMPS_GE_F64       ,   // D.u = (S0 >= S1)
        V3_CMPS_O_F64        ,   // D.u = (!isNaN(S0) && !isNaN(S1)) 
        V3_CMPS_U_F64        ,   // D.u = (!isNaN(S0) || !isNaN(S1)) 
        V3_CMPS_NGE_F64      ,   // D.u = !(S0 >= S1)
        V3_CMPS_NLG_F64      ,   // D.u = !(S0 <> S1)
        V3_CMPS_NGT_F64      ,   // D.u = !(S0 > S1)
        V3_CMPS_NLE_F64      ,   // D.u = !(S0 <= S1)
        V3_CMPS_NEQ_F64      ,   // D.u = !(S0 == S1)
        V3_CMPS_NLT_F64      ,   // D.u = !(S0 < S1)
        V3_CMPS_TRU_F64      ,   // D.u = 1
        V3_CMPSX_F_F64        =  0x70    , //   Signal on any NaN. Also write EXEC.
        V3_CMPSX_LT_F64      ,   // D.u = (S0 < S1)
        V3_CMPSX_EQ_F64      ,   // D.u = (S0 == S1)
        V3_CMPSX_LE_F64      ,   // D.u = (S0 <= S1)
        V3_CMPSX_GT_F64      ,   // D.u = (S0 > S1)
        V3_CMPSX_LG_F64      ,   // D.u = (S0 <> S1)
        V3_CMPSX_GE_F64      ,   // D.u = (S0 >= S1)
        V3_CMPSX_O_F64       ,   // D.u = (!isNaN(S0) && !isNaN(S1)) 
        V3_CMPSX_U_F64       ,   // D.u = (!isNaN(S0) || !isNaN(S1)) 
        V3_CMPSX_NGE_F64     ,   // D.u = !(S0 >= S1)
        V3_CMPSX_NLG_F64     ,   // D.u = !(S0 <> S1)
        V3_CMPSX_NGT_F64     ,   // D.u = !(S0 > S1)
        V3_CMPSX_NLE_F64     ,   // D.u = !(S0 <= S1)
        V3_CMPSX_NEQ_F64     ,   // D.u = !(S0 == S1)
        V3_CMPSX_NLT_F64     ,   // D.u = !(S0 < S1)
        V3_CMPSX_TRU_F64     ,   // D.u = 1
        V3_CMP_F_I32         =  0x80, //D.u = 0  , //   On 32-bit integers.
        V3_CMP_LT_I32        , //D.u = (S0 < S1)
        V3_CMP_EQ_I32        , //D.u = (S0 == S1)
        V3_CMP_LE_I32        , //D.u = (S0 <= S1)
        V3_CMP_GT_I32        , //D.u = (S0 > S1)
        V3_CMP_LG_I32        , //D.u = (S0 <> S1)
        V3_CMP_GE_I32        , //D.u = (S0 >= S1)
        V3_CMP_TRU_I32       , //D.u = 1
        V3_CMPX_F_I32        =  0x90    , //   Also write EXEC.
        V3_CMPX_LT_I32       ,//D.u = (S0 < S1)
        V3_CMPX_EQ_I32       ,//D.u = (S0 == S1)
        V3_CMPX_LE_I32       ,//D.u = (S0 <= S1)
        V3_CMPX_GT_I32       ,//D.u = (S0 > S1)
        V3_CMPX_LG_I32       ,//D.u = (S0 <> S1)
        V3_CMPX_GE_I32       ,//D.u = (S0 >= S1)
        V3_CMPX_TRU_I32      ,//D.u = 1
        V3_CMP_F_I64           =  0xA0    , //    On 64-bit integers.
        V3_CMP_LT_I64        ,//D.u = (S0 < S1)
        V3_CMP_EQ_I64        ,//D.u = (S0 == S1)
        V3_CMP_LE_I64        ,//D.u = (S0 <= S1)
        V3_CMP_GT_I64        ,//D.u = (S0 > S1)
        V3_CMP_LG_I64        ,//D.u = (S0 <> S1)
        V3_CMP_GE_I64        ,//D.u = (S0 >= S1)
        V3_CMP_TRU_I64       ,//D.u = 1
        V3_CMPX_F_I64          =  0xB0    , //    Also write EXEC.
        V3_CMPX_LT_I64       ,//D.u = (S0 < S1)
        V3_CMPX_EQ_I64       ,//D.u = (S0 == S1)
        V3_CMPX_LE_I64       ,//D.u = (S0 <= S1)
        V3_CMPX_GT_I64       ,//D.u = (S0 > S1)
        V3_CMPX_LG_I64       ,//D.u = (S0 <> S1)
        V3_CMPX_GE_I64       ,//D.u = (S0 >= S1)
        V3_CMPX_TRU_I64      ,//D.u = 1
        V3_CMP_F_U32          =  0xC0    , //    On unsigned 32-bit integers.
        V3_CMP_LT_U32        ,//D.u = (S0 < S1)
        V3_CMP_EQ_U32        ,//D.u = (S0 == S1)
        V3_CMP_LE_U32        ,//D.u = (S0 <= S1)
        V3_CMP_GT_U32        ,//D.u = (S0 > S1)
        V3_CMP_LG_U32        ,//D.u = (S0 <> S1)
        V3_CMP_GE_U32        ,//D.u = (S0 >= S1)
        V3_CMP_TRU_U32       ,//D.u = 1
        V3_CMPX_F_U32            =  0xD0    , //    Also write EXEC.
        V3_CMPX_LT_U32       ,//D.u = (S0 < S1)
        V3_CMPX_EQ_U32       ,//D.u = (S0 == S1)
        V3_CMPX_LE_U32       ,//D.u = (S0 <= S1)
        V3_CMPX_GT_U32       ,//D.u = (S0 > S1)
        V3_CMPX_LG_U32       ,//D.u = (S0 <> S1)
        V3_CMPX_GE_U32       ,//D.u = (S0 >= S1)
        V3_CMPX_TRU_U32      ,//D.u = 1
        V3_CMP_F_U64           =  0xE0    , //    On unsigned 64-bit integers.
        V3_CMP_LT_U64        ,//D.u = (S0 < S1)
        V3_CMP_EQ_U64        ,//D.u = (S0 == S1)
        V3_CMP_LE_U64        ,//D.u = (S0 <= S1)
        V3_CMP_GT_U64        ,//D.u = (S0 > S1)
        V3_CMP_LG_U64        ,//D.u = (S0 <> S1)
        V3_CMP_GE_U64        ,//D.u = (S0 >= S1)
        V3_CMP_TRU_U64       ,//D.u = 1
        V3_CMPx_F_U64        =  0xF0    , //    Also write EXEC.
        V3_CMPx_LT_U64       ,//D.u = (S0 < S1)
        V3_CMPx_EQ_U64       ,//D.u = (S0 == S1)
        V3_CMPx_LE_U64       ,//D.u = (S0 <= S1)
        V3_CMPx_GT_U64       ,//D.u = (S0 > S1)
        V3_CMPx_LG_U64       ,//D.u = (S0 <> S1)
        V3_CMPx_GE_U64       ,//D.u = (S0 >= S1)
        V3_CMPx_TRU_U64      ,//D.u = 1
        V3_CMP_CLASS_F32     =  0x88   , //  D = IEEE numeric class function specified in S1.u, performed on S0.f.
        V3_CMPX_CLASS_F32    =  0x98   , //  D = IEEE numeric class function specified in S1.u, performed on S0.f. Also write EXEC.
        V3_CMP_CLASS_F64     =  0xA8   , //  D = IEEE numeric class function specified in S1.u, performed on S0.d.
        V3_CMPX_CLASS_F64    =  0xB8   , //  D = IEEE numeric class function specified inS1.u, performed on S0.d. Also write EXEC.
        // TODO:   These opcodes are overloaded. e.g.  in reality 0x88-0x98 are all V_CMP_CLASS_F32 

        //////////////////////////////////////////////////////
        //  VOP3a
        //////////////////////////////////////////////////////
  

        V3_CNDMASK_B32      = 256,  //: D.u = S2[i] ? S1.u : S0.u (i = threadID in wave).
        V3_READLANE_B32     = 257,  //: copy one VGPR value to one SGPR. Dst = SGPR-dest,
                                   // Src0 = Source Data (VGPR# or M0(lds-direct)), Src1 = Lane Select (SGPR
                                   //    or M0). Ignores exec mask.
        V3_WRITELANE_B32    = 258,  //: Write value into one VGPR one one lane. Dst = VGPRdest,
                                   //    Src0 = Source Data (sgpr, m0, exec or constants), Src1 = Lane Select
                                   //    (SGPR or M0). Ignores exec mask.
        V3_ADD_F32          = 259,  //: D.f = S0.f + S1.f.
        V3_SUB_F32          = 260,  //: D.f = S0.f - S1.f.
        V3_SUBREV_F32       = 261,  //: D.f = S1.f - S0.f.
        V3_MAC_LEGACY_F32   = 262,  //: D.f = S0.F * S1.f + D.f.
        V3_MUL_LEGACY_F32   = 263,  //: D.f = S0.f * S1.f (DX9 rules, 0.0*x = 0.0).
        V3_MUL_F32          = 264,  //: D.f = S0.f * S1.f.
        V3_MUL_I32_I24      = 265,  //: D.i = S0.i[23:0] * S1.i[23:0].
        V3_MUL_HI_I32_I24   = 266,  //: D.i = (S0.i[23:0] * S1.i[23:0])>>32.
        V3_MUL_U32_U24      = 267,  //: D.u = S0.u[23:0] * S1.u[23:0].
        V3_MUL_HI_U32_U24   = 268,  //: D.i = (S0.u[23:0] * S1.u[23:0])>>32.
        V3_MIN_LEGACY_F32   = 269,  //: D.f = min(S0.f, S1.f) (DX9 rules for NaN).
        V3_MAX_LEGACY_F32   = 270,  //: D.f = max(S0.f, S1.f) (DX9 rules for NaN).
        V3_MIN_F32          = 271,  //: D.f = min(S0.f, S1.f).
        V3_MAX_F32          = 272,  //: D.f = max(S0.f, S1.f).
        V3_MIN_I32          = 273,  //: D.i = min(S0.i, S1.i).
        V3_MAX_I32          = 274,  //: D.i = max(S0.i, S1.i).
        V3_MIN_U32          = 275,  //: D.u = min(S0.u, S1.u).
        V3_MAX_U32          = 276,  //: D.u = max(S0.u, S1.u).
        V3_LSHR_B32         = 277,  //: D.u = S0.u >> S1.u[4:0].
        V3_LSHRREV_B32      = 278,  //: D.u = S1.u >> S0.u[4:0].
        V3_ASHR_I32        = 279,  //: D.i = S0.i >> S1.i[4:0].
        V3_ASHRREV_I32     = 280,  //    : D.i = S1.i >> S0.i[4:0].
        V3_LSHL_B32        = 281,  //: D.u = S0.u << S1.u[4:0].
        V3_LSHLREV_B32     = 282,  //    : D.u = S1.u << S0.u[4:0].
        V3_AND_B32         = 283,  //: D.u = S0.u & S1.u.
        V3_OR_B32           = 284,  //: D.u = S0.u | S1.u.
        V3_XOR_B32          = 285,  //: D.u = S0.u ^ S1.u.
        V3_BFM_B32         = 286,  //: D.u = ((1<<S0.u[4:0])-1) << S1.u[4:0]; S0=bitfield_width,S1=bitfield_offset.
        V3_MAC_F32          = 287,  //: D.f = S0.f * S1.f + D.f.
        V3_MADMK_F32        = 288,  //: D.f = S0.f * K + S1.f; K is a 32-bit inline constant.
        V3_MADAK_F32        = 289,  //: D.f = S0.f * S1.f + K; K is a 32-bit inline constant.
        V3_BCNT_U32_B32     = 290,  //: D.u = CountOneBits(S0.u) + S1.u. Bit count.
        V3_MBCNT_LO_U32_B32 = 291,  //: ThreadMask = (1 << ThreadPosition) - 1;
                                   //     D.u = CountOneBits(S0.u & ThreadMask[31:0]) + S1.u. Masked bit count,
                                   //     ThreadPosition is the position of this thread in the wavefront (in 0..63).
        V3_MBCNT_HI_U32_B32 = 292,  //: ThreadMask = (1 << ThreadPosition) - 1;
                                    //    D.u = CountOneBits(S0.u & ThreadMask[63:32]) + S1.u. Masked bit count,
                                    //    ThreadPosition is the position of this thread in the wavefront (in 0..63).

                                    
        //////////////////////////////////////////////////////
        //  VOP3b
        //////////////////////////////////////////////////////

        V3_ADD_I32       = 293,  //  : D.u = S0.u + S1.u; VCC=carry-out (VOP3:sgpr=carry-out).
        V3_SUB_I32       = 294,  //  : D.u = S0.u - S1.u; VCC=carry-out (VOP3:sgpr=carry-out).
        V3_SUBREV_I32    = 295,  //  : D.u = S1.u - S0.u; VCC=carry-out (VOP3:sgpr=carry-out).
        V3_ADDC_U32      = 296,  //  : D.u = S0.u + S1.u + VCC; VCC=carry-out (VOP3:sgpr=carryout,S2.u=carry-in).
        V3_SUBB_U32      = 297,  //  : D.u = S0.u - S1.u - VCC; VCC=carry-out (VOP3:sgpr=carry-out,S2.u=carry-in).
        V3_SUBBREV_U32   = 298,  //  : D.u = S1.u - S0.u - VCC; VCC=carry-out (VOP3:sgpr=carryout,S2.u=carry-in).
        V3_DIV_SCALE_F32 = 365,  //  = D.f = Special case divide preop and flags(s0.f = Quotient,s1.f = Denominator, s2.f = Numerator) s0 must equal s1 or s2.
        V3_DIV_SCALE_F64 = 366,  //  = D.d = Special case divide preop and flags(s0.d = Quotient,s1.d = Denominator, s2.d = Numerator) s0 must equal s1 or s2.

        //////////////////////////////////////////////////////
        //  VOP3a again
        //////////////////////////////////////////////////////

        V3_LDEXP_F32           = 299, //  : D.d = pow(S0.f, S1.i).
        V3_CVT_PKACCUM_U8_F32  = 300, //  : f32->u8(s0.f), pack into byte(s1.u), of dst.
        V3_CVT_PKNORM_I16_F32  = 301, //  : D = {(snorm)S1.f, (snorm)S0.f}.
        V3_CVT_PKNORM_U16_F32  = 302, //  : D = {(unorm)S1.f, (unorm)S0.f}.
        V3_CVT_PKRTZ_F16_F32   = 303, //  : D = {flt32_to_flt16(S1.f),flt32_to_flt16(S0.f)}, with round-toward-zero.
        V3_CVT_PK_U16_U32      = 304, //  : D = {(u32->u16)S1.u, (u32->u16)S0.u}.
        V3_CVT_PK_I16_I32      = 305, //  : D = {(i32->i16)S1.i, (i32->i16)S0.i}.
        
       // 318 – 319 Do not use (maps to VOP1 and VOPC).
       // 320 – 372 Are VOP3a-only opcodes.
        
        V3_MAD_LEGACY_F32   = 320 ,   //   = D.f = S0.f * S1.f + S2.f (DX9 rules, 0.0*x = 0.0).
        V3_MAD_F32          = 321 ,   //   = D.f = S0.f * S1.f + S2.f.
        V3_MAD_I32_I24      = 322 ,   //   = D.i = S0.i * S1.i + S2.iD.i = S0.i * S1.i + S2.i.
        V3_MAD_U32_U24      = 323 ,   //   = D.u = S0.u * S1.u + S2.u.
        V3_CUBEID_F32       = 324 ,   //   = Rm.w <- Rn,x, Rn,y, Rn.z.
        V3_CUBESC_F32       = 325 ,   //   = Rm.y <- Rn,x, Rn,y, Rn.z.
        V3_CUBETC_F32       = 326 ,   //   = Rm.x <- Rn,x, Rn,y, Rn.z.
        V3_CUBEMA_F32       = 327 ,   //   = Rm.z <- Rn,x, Rn,y, Rn.z
        V3_BFE_U32          = 328 ,   //   = D.u = (S0.u>>S1.u[4:0]) & ((1<<S2.u[4:0])-1); bitfield extract,S0=data, S1=field_offset, S2=field_width.
        V3_BFE_I32          = 329 ,   //   = D.i = (S0.i>>S1.u[4:0]) & ((1<<S2.u[4:0])-1); bitfield extract, S0=data, S1=field_offset, S2=field_width.
        V3_BFI_B32          = 330 ,   //   = D.u = (S0.u & S1.u) | (~S0.u & S2.u); bitfield insert.
        V3_FMA_F32          = 331 ,   //   = D.f = S0.f * S1.f + S2.f
        V3_FMA_F64          = 332 ,   //   = D.d = S0.d * S1.d + S2.d.
                                     //
        V3_LERP_U8          = 333 ,   //  = D.u = ((S0.u[31:24] + S1.u[31:24] + S2.u[24]) >> 1) << 24 +
                                     //       S1.u[15:8] + S2.u[8]) >> 1) << 8 + ((S0.u[7:0] + S1.u[7:0] + S2.u[0]) >> 1).
                                     //       Unsigned eight-bit pixel average on packed unsigned bytes (linear interpolation).
                                     //       S2 acts as a round mode; if set, 0.5 rounds up; otherwise, 0.5 truncates.
        V3_ALIGNBIT_B32     = 334 ,   //  = D.u = ({S0,S1} >> S2.u[4:0]) & 0xFFFFFFFF.
        V3_ALIGNBYTE_B32    = 335 ,   //  = D.u = ({S0,S1} >> (8*S2.u[4:0])) & 0xFFFFFFFF.
        V3_MULLIT_F32       = 336 ,   //  = D.f = S0.f * S1.f, replicate result into 4 components (0.0 * x = 0.0; special INF, NaN, overflow rules).
        V3_MIN3_F32         = 337 ,   //  = D.f = min(S0.f, S1.f, S2.f).
        V3_MIN3_I32         = 338 ,   //  = D.i = min(S0.i, S1.i, S2.i).
        V3_MIN3_U32         = 339 ,   //  = 0x153 D.u = min(S0.u, S1.u, S2.u).
        V3_MAX3_F32         = 340 ,   //  = D.f = max(S0.f, S1.f, S2.f).
        V3_MAX3_I32         = 341 ,   //  = D.i = max(S0.i, S1.i, S2.i).
        V3_MAX3_U32         = 342 ,   //  = D.u = max(S0.u, S1.u, S2.u).
        V3_MED3_F32         = 343 ,   //  = D.f = median(S0.f, S1.f, S2.f).
        V3_MED3_I32         = 344 ,   //  = D.i = median(S0.i, S1.i, S2.i).
        V3_MED3_U32         = 345 ,   //  = D.u = median(S0.u, S1.u, S2.u).
        V3_SAD_U8           = 346 ,   //  = D.u = Byte SAD with accum_lo(S0.u, S1.u, S2.u).
        V3_SAD_HI_U8        = 347 ,   //  = D.u = Byte SAD with accum_hi(S0.u, S1.u, S2.u).
        V3_SAD_U16          = 348 ,   //  = D.u = Word SAD with accum(S0.u, S1.u, S2.u).
        V3_SAD_U32          = 349 ,   //  = D.u = Dword SAD with accum(S0.u, S1.u, S2.u).
        V3_CVT_PK_U8_F32    = 350 ,   //  = f32->u8(s0.f), pack into byte(s1.u), of dword(s2).
        V3_DIV_FIXUP_F32    = 351 ,   //  = D.f = Special case divide fixup and flags(s0.f = Quotient,   s1.f = Denominator, s2.f = Numerator).
        V3_DIV_FIXUP_F64    = 352 ,   //  = D.d = Special case divide fixup and flags(s0.d = Quotient,  s1.d = Denominator, s2.d = Numerator).
        V3_LSHL_B64         = 353 ,   //  = D = S0.u << S1.u[4:0].
        V3_LSHR_B64         = 354 ,   //  = D = S0.u >> S1.u[4:0].
        V3_ASHR_I64         = 355 ,   //  = D = S0.u >> S1.u[4:0].
        V3_ADD_F64          = 356 ,   //  = D.d = S0.d + S1.d.
        V3_MUL_F64          = 357 ,   //  = D.d = S0.d * S1.d.
        V3_MIN_F64          = 358 ,   //  = D.d = min(S0.d, S1.d).
        V3_MAX_F64          = 359 ,   //  = D.d = max(S0.d, S1.d).
        V3_LDEXP_F64        = 360 ,   //  = D.d = pow(S0.d, S1.i[31:0]).
        V3_MUL_LO_U32       = 361 ,   //  = D.u = S0.u * S1.u.
        V3_MUL_HI_U32       = 362 ,   //  = D.u = (S0.u * S1.u)>>32.
        V3_MUL_LO_I32       = 363 ,   //  = D.i = S0.i * S1.i.
        V3_MUL_HI_I32       = 364 ,   //  = D.i = (S0.i * S1.i)>>32.

        //365 – 366 See corresponding opcode numbers in VOP3 (3 in, 2 out), (VOP3b).

        V3_DIV_FMAS_F32    =  367 ,  // = D.f = Special case divide FMA with scale and flags(s0.f = Quotient, s1.f = Denominator, s2.f = Numerator).
        V3_DIV_FMAS_F64    =  368 ,  // = D.d = Special case divide FMA with scale and flags(s0.d= Quotient, s1.d = Denominator, s2.d = Numerator).
        V3_MSAD_U8         =  369 ,  // = D.u = Masked Byte SAD with accum_lo(S0.u, S1.u, S2.u).
        
        // Opcodes 370/371 were renamed between SI and CI.  Not sure if they behave differently or not
        V3_QSAD_U8         =  370 ,  // = D.u = Quad-Byte SAD with accum_lo/hiu(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        V3_MQSAD_U8        =  371 ,  // = D.u = Masked Quad-Byte SAD with accum_lo/hi(S0.u[63:0],S1.u[31:0], S2.u[63:0]).       
        V3_QSAD_PK_U16_U8  =  370 ,  // : D.u = Quad-Byte SAD with accum_lo/hiu(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        V3_MQSAD_PK_U16_U8 =  371 ,  // = D.u = Masked Quad-Byte SAD with accum_lo/hi(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        //

        V3_TRIG_PREOP_F64  =  372 ,  // = D.d = Look Up 2/PI (S0.d) with segment select  S1.u[4:0].

        // 373-375 are Sea Islands only
        V3_MQSAD_U32_U8    = 373 ,   // : D.u128 = Masked Quad-Byte SAD with 32-bit accum_lo/hi(S0.u[63:0], S1.u[31:0], S2.u[127:0]).
        V3_MAD_U64_U32     = 374 ,   // : {vcc_out,D.u64} = S0.u32 * S1.u32 + S2.u64.
        V3_MAD_I64_I32     = 375 ,   // : {vcc_out,D.i64} = S0.i32 * S1.i32 + S2.i64.

        //384 – 452 Are the VOP1 opcodes when VOP3 encoding is required. For example, V_OP1_OFFSET + V_MOV_B32 generates the VOP3 version of MOV.

        V3_NOP               = 384,  //: do nothing.
        V3_MOV_B32           = 385,  //: D.u = S0.u.
        V3_READFIRSTLANE_B32 = 386,  //: copy one VGPR value to one SGPR. Dst = SGPRdest,
                                    //    Src0 = Source Data (VGPR# or M0(lds-direct)), Lane# =
                                    //    FindFirst1fromLSB(exec) (lane = 0 if exec is zero). Ignores exec mask.
        V3_CVT_I32_F64       = 387,  //: D.i = (int)S0.d.
        V3_CVT_F64_I32       = 388,  //: D.f = (float)S0.i.
        V3_CVT_F32_I32       = 389,  //: D.f = (float)S0.i.
        V3_CVT_F32_U32       = 390,  //: D.f = (float)S0.u.
        V3_CVT_U32_F32       = 391,  //: D.u = (unsigned)S0.f.
        V3_CVT_I32_F32       = 392,  //: D.i = (int)S0.f.
        V3_MOV_FED_B32       = 393,  //: D.u = S0.u, introduce edc double error upon write to dest vgpr without causing an exception.
        V3_CVT_F16_F32       = 394,  //: D.f16 = flt32_to_flt16(S0.f).
        V3_CVT_F32_F16       = 395,  //: D.f = flt16_to_flt32(S0.f16).
        V3_CVT_RPI_I32_F32   = 396,  //: D.i = (int)floor(S0.f + 0.5).
        V3_CVT_FLR_I32_F32   = 397,  //: D.i = (int)floor(S0.f).
        V3_CVT_OFF_F32_I4    = 398,  //: 4-bit signed int to 32-bit float. For interpolation in shader.
        V3_CVT_F32_F64       = 399,  //: D.f = (float)S0.d.
        V3_CVT_F64_F32       = 400,  //: D.d = (double)S0.f.
        V3_CVT_F32_UBYTE0    = 401,  //: D.f = UINT2FLT(S0.u[7:0]).
        V3_CVT_F32_UBYTE1    = 402,  //: D.f = UINT2FLT(S0.u[15:8]).
        V3_CVT_F32_UBYTE2    = 403,  //: D.f = UINT2FLT(S0.u[23:16]).
        V3_CVT_F32_UBYTE3    = 404,  //: D.f = UINT2FLT(S0.u[31:24]).
        V3_CVT_U32_F64       = 405,  //: D.u = (uint)S0.d.
        V3_CVT_F64_U32       = 406,  //: D.d = (double)S0.u.
        //407 – 415 reserved.    //
        V3_FRACT_F32         = 416,  //: D.f = S0.f - floor(S0.f).
        V3_TRUNC_F32         = 417,  //: D.f = trunc(S0.f), return integer part of S0.
        V3_CEIL_F32          = 418,  //: D.f = ceil(S0.f). Implemented as: D.f = trunc(S0.f); if (S0 > 0.0 && S0 != D), D += 1.0.
        V3_RNDNE_F32         = 419,  //: D.f = round_nearest_even(S0.f).
        V3_FLOOR_F32         = 420,  //: D.f = trunc(S0); if ((S0 < 0.0) && (S0 != D)) D += -1.0.
        V3_EXP_F32           = 421,  //: D.f = pow(2.0, S0.f).
        V3_LOG_CLAMP_F32     = 422,  //: D.f = log2(S0.f), clamp -infinity to -max_float.
        V3_LOG_F32           = 423,  //: D.f = log2(S0.f).
        V3_RCP_CLAMP_F32     = 424,  //: D.f = 1.0 / S0.f, result clamped to +-max_float.
        V3_RCP_LEGACY_F32    = 425,  //: D.f = 1.0 / S0.f, +-infinity result clamped to +-0.0.
        V3_RCP_F32           = 426,  //: D.f = 1.0 / S0.f.
        V3_RCP_IFLAG_F32     = 427,  //: D.f = 1.0 / S0.f, only integer div_by_zero flag can be raised.
        V3_RSQ_CLAMP_F32     = 428,  //: D.f = 1.0 / sqrt(S0.f), result clamped to +-max_float.
        V3_RSQ_LEGACY_F32    = 429,  //: D.f = 1.0 / sqrt(S0.f).
        V3_RSQ_F32           = 430,  //: D.f = 1.0 / sqrt(S0.f).
        V3_RCP_F64           = 431,  //: D.d = 1.0 / (S0.d).
        V3_RCP_CLAMP_F64     = 432,  //: D.f = 1.0 / (S0.f), result clamped to +-max_float.
        V3_RSQ_F64           = 433,  //: D.f = 1.0 / sqrt(S0.f).
        V3_RSQ_CLAMP_F64     = 434,  //: D.d = 1.0 / sqrt(S0.d), result clamped to +-max_float.
        V3_SQRT_F32          = 435,  //: D.f = sqrt(S0.f).
                                  //
                                  //
        V3_SQRT_F64           = 436,  //: D.d = sqrt(S0.d).
        V3_SIN_F32            = 437,  //: D.f = sin(S0.f).
        V3_COS_F32            = 438,  //: D.f = cos(S0.f).
        V3_NOT_B32            = 439,  //: D.u = ~S0.u.
        V3_BFREV_B32          = 440,  //: D.u[31:0] = S0.u[0:31], bitfield reverse.
        V3_FFBH_U32           = 441,  //: D.u = position of first 1 in S0 from MSB; D=0xFFFFFFFF if S0==0.
        V3_FFBL_B32           = 442,  //: D.u = position of first 1 in S0 from LSB; D=0xFFFFFFFF ifS0==0.
        V3_FFBH_I32           = 443,  //: D.u = position of first bit different from sign bit in S0 from MSB; D=0xFFFFFFFF if S0==0 or 0xFFFFFFFF.
        V3_FREXP_EXP_I32_F64  = 444,  //: See V_FREXP_EXP_I32_F32.
        V3_FREXP_MANT_F64     = 445,  //: See V_FREXP_MANT_F32.
        V3_FRACT_F64          = 446,  //: S0.d - floor(S0.d).
        V3_FREXP_EXP_I32_F32  = 447,  //: If (S0.f == INF || S0.f == NAN), then D.i = 0; else D.i
                                     //    = TwosComplement(Exponent(S0.f) - 127 + 1). Returns exponent of single
                                     //    precision float input, such that S0.f = significand * (2 ** exponent). See also
                                     //    FREXP_MANT_F32, which returns the significand.
        V3_FREXP_MANT_F32    = 448 ,  //: if (S0.f == INF || S0.f == NAN) then D.f = S0.f; else D.f =
                                     //    Mantissa(S0.f). Result range is in (-1.0,-0.5][0.5,1.0) in normal cases.
                                     //    Returns binary significand of single precision float input, such that S0.f = significand
                                     //    * (2 ** exponent). See also FREXP_EXP_I32_F32, which returns integer
                                     //    exponent.
        V3_CLREXCP        = 449  ,    //: Clear wave's exception state in shader processor SIMD.
        V3_MOVRELD_B32    = 450  ,    //: VGPR[D.u + M0.u] = VGPR[S0.u].
        V3_MOVRELS_B32    = 451  ,    //: VGPR[D.u] = VGPR[S0.u + M0.u].
        V3_MOVRELSD_B32   = 452  ,    //: VGPR[D.u + M0.u] = VGPR[S0.u + M0.u].
        
        //All other values are reserved.
    };

    enum VINTERP_Opcodes
    {
        //////////////////////////////////////////////////////
        //  VINTERP
        //////////////////////////////////////////////////////

        V_INTERP_P1_F32  = 0 , //  : D = P10 * S + P0; parameter interpolation.
        V_INTERP_P2_F32  = 1 , //  : D = P20 * S + D; parameter interpolation.
        V_INTERP_MOV_F32 = 2 , //  : D = {P10,P20,P0}[S]; parameter load.
        //3 reserved.
    };


    enum DS_Opcodes
    {
       DS_ADD_U32             =  0 ,   //  DS[A] = DS[A] + D0; uint add.
       DS_SUB_U32             =  1 ,   //  DS[A] = DS[A] - D0; uint subtract.
       DS_RSUB_U32            =  2 ,   //  DS[A] = D0 - DS[A]; uint reverse subtract.
       DS_INC_U32             =  3 ,   //  DS[A] = (DS[A] >= D0 ? 0 : DS[A] + 1); uint increment.
       DS_DEC_U32             =  4 ,   //  DS[A] = (DS[A] == 0 || DS[A] > D0 ? D0 : DS[A] - 1); uint decrement.
       DS_MIN_I32             =  5 ,   //  DS[A] = min(DS[A], D0); int min.
       DS_MAX_I32             =  6 ,   //  DS[A] = max(DS[A], D0); int max.
       DS_MIN_U32             =  7 ,   //  DS[A] = min(DS[A], D0); uint min.
       DS_MAX_U32             =  8 ,   //  DS[A] = max(DS[A], D0); uint max.
       DS_AND_B32             =  9 ,   //  DS[A] = DS[A] & D0; Dword AND.
       DS_OR_B32              = 10 ,   //  DS[A] = DS[A] | D0; Dword OR.
       DS_XOR_B32             = 11 ,   //  DS[A] = DS[A] ^ D0; Dword XOR.
       DS_MSKOR_B32           = 12 ,   //  DS[A] = (DS[A] ^ ~D0) | D1; masked Dword OR.
       DS_WRITE_B32           = 13 ,   //  DS[A] = D0; write a Dword.
       DS_WRITE2_B32          = 14 ,   //  DS[ADDR+offset0*4] = D0;DS[ADDR+offset1*4] = D1; write 2 Dwords.
       DS_WRITE2ST64_B32      = 15 ,   //  DS[ADDR+offset0*4*64] = D0;DS[ADDR+offset1*4*64] = D1; write 2 Dwords.
       DS_CMPST_B32           = 16 ,   //  DS[A] = (DS[A] == D0 ? D1 : DS[A]); compare store.
       DS_CMPST_F32           = 17 ,   //  DS[A] = (DS[A] == D0 ? D1 : DS[A]); compare store with float rules.
       DS_MIN_F32             = 18 ,   //  DS[A] = (DS[A] < D1) ? D0 : DS[A]; float compare swap (handles NaN/INF/denorm).
       DS_MAX_F32             = 19 ,   //  DS[A] = (D0 > DS[A]) ? D0 : DS[A]; float, handles NaN/INF/denorm.
       DS_NOP                  = 20,   // Do nothing.  SEA ISLANDS ONLY
       DS_GWS_SEMA_RELEASE_ALL = 24,   // GDS Only. Release all wavefronts waiting on this semaphore. ResourceID is in offset[4:0]. SEA ISLANDS ONLY
       DS_GWS_INIT            = 25 ,   //  GDS only.
       DS_GWS_SEMA_V          = 26 ,   //  GDS only.
       DS_GWS_SEMA_BR         = 27 ,   //  GDS only.
       DS_GWS_SEMA_P          = 28 ,   //  GDS only.
       DS_GWS_BARRIER         = 29 ,   //  GDS only.
       DS_WRITE_B8            = 30 ,   //  DS[A] = D0[7:0]; byte write.
       DS_WRITE_B16           = 31 ,   //  DS[A] = D0[15:0]; short write.
       DS_ADD_RTN_U32         = 32 ,   //  Uint add.
       DS_SUB_RTN_U32         = 33 ,   //  Uint subtract.
       DS_RSUB_RTN_U32        = 34 ,   //  Uint reverse subtract.
       DS_INC_RTN_U32         = 35 ,   //  Uint increment.
       DS_DEC_RTN_U32         = 36 ,   //  Uint decrement.
       DS_MIN_RTN_I32         = 37 ,   //  Int min.
       DS_MAX_RTN_I32         = 38 ,   //  Int max.
       DS_MIN_RTN_U32         = 39 ,   //  Uint min.
       DS_MAX_RTN_U32         = 40 ,   //  Uint max.
       DS_AND_RTN_B32         = 41 ,   //  Dword AND.
       DS_OR_RTN_B32          = 42 ,   //  Dword OR.
       DS_XOR_RTN_B32         = 43 ,   //  Dword XOR.
       DS_MSKOR_RTN_B32       = 44 ,   //  Masked Dword OR.
       DS_WRXCHG_RTN_B32      = 45 ,   //  Write exchange. Offset = {offset1,offset0}. A = ADDR+offset. D=DS[Addr]. DS[Addr]=D0.
       DS_WRXCHG2_RTN_B32     = 46 ,   //  Write exchange 2 separate Dwords.
       DS_WRXCHG2ST64_RTN_B32 = 47 ,   //  Write echange 2 Dwords, stride 64.
       DS_CMPST_RTN_B32       = 48 ,   //  Compare store.
       DS_CMPST_RTN_F32       = 49 ,   //  Compare store with float rules.
       DS_MIN_RTN_F32         = 50 ,   //  DS[A] = (DS[A] < D1) ? D0 : DS[A]; float compare swap (handles NaN/INF/denorm).
       DS_MAX_RTN_F32         = 51 ,   //  DS[A] = (D0 > DS[A]) ? D0 : DS[A]; float, handles NaN/INF/denorm .
       DS_WRAP_RTN_B32        = 52 ,   // DS[A] = (DS[A] >= D0) ? DS[A] - D0 : DS[A] + D1. SEA ISLANDS ONLY
       DS_SWIZZLE_B32         = 53 ,   //  R = swizzle(Data(vgpr), offset1:offset0). Dword swizzle. no data is written to LDS. see ds_opcodes.docx for details.
       DS_READ_B32            = 54 ,   //  R = DS[A]; Dword read.
       DS_READ2_B32           = 55 ,   //  R = DS[ADDR+offset0*4], R+1 = DS[ADDR+offset1*4]. Read 2 Dwords.
       DS_READ2ST64_B32       = 56 ,   //  R = DS[ADDR+offset0*4*64], R+1 = DS[ADDR+offset1*4*64]. Read 2 Dwords.
       DS_READ_I8             = 57 ,   //  R = signext(DS[A][7:0]}; signed byte read.
       DS_READ_U8             = 58 ,   //  R = {24’h0,DS[A][7:0]}; unsigned byte read.
       DS_READ_I16            = 59 ,   //  R = signext(DS[A][15:0]}; signed short read.
       DS_READ_U16            = 60 ,   //  R = {16’h0,DS[A][15:0]}; unsigned short read.
       DS_CONSUME             = 61 ,   //  Consume entries from a buffer.
       DS_APPEND              = 62 ,   //  Append one or more entries to a buffer.
       DS_ORDERED_COUNT       = 63 ,   //  Increment an append counter. Operation is done in order of wavefront creation.
       DS_ADD_U64             = 64 ,   //  Uint add.
       DS_SUB_U64             = 65 ,   //  Uint subtract.
       DS_RSUB_U64            = 66 ,   //  Uint reverse subtract.
       DS_INC_U64             = 67 ,   //  Uint increment.
       DS_DEC_U64             = 68 ,   //  Uint decrement.
       DS_MIN_I64             = 69 ,   //  Int min.
       DS_MAX_I64             = 70 ,   //  Int max.
       DS_MIN_U64             = 71 ,   //  Uint min.
       DS_MAX_U64             = 72 ,   //  Uint max.
       DS_AND_B64             = 73 ,   //  Dword AND.
       DS_OR_B64              = 74 ,   //  Dword OR.
       DS_XOR_B64             = 75 ,   //  Dword XOR.
       DS_MSKOR_B64           = 76 ,   //  Masked Dword XOR.
       DS_WRITE_B64           = 77 ,   //  Write.
       DS_WRITE2_B64          = 78 ,   //  DS[ADDR+offset0*8] = D0;DS[ADDR+offset1*8] = D1; write 2 Dwords.
       DS_WRITE2ST64_B64      = 79 ,   //  DS[ADDR+offset0*8*64] = D0; DS[ADDR+offset1*8*64] = D1; write 2 Dwords.
       DS_CMPST_B64           = 80 ,   //  Compare store.
       DS_CMPST_F64           = 81 ,   //  Compare store with float rules.
       DS_MIN_F64             = 82 ,   //  DS[A] = (D0 < DS[A]) ? D0 : DS[A]; float, handles NaN/INF/denorm.
       DS_MAX_F64             = 83 ,   //  DS[A] = (D0 > DS[A]) ? D0 : DS[A]; float, handles NaN/INF/denorm.
       DS_ADD_RTN_U64         = 96 ,   //  Uint add.
       DS_SUB_RTN_U64         = 97 ,   //  Uint subtract.
       DS_RSUB_RTN_U64        = 98 ,   //  Uint reverse subtract.
       DS_INC_RTN_U64         = 99 ,   //  Uint increment.
       DS_DEC_RTN_U64         = 100,   // Uint decrement.
       DS_MIN_RTN_I64         = 101,   // Int min.
       DS_MAX_RTN_I64         = 102,   // Int max.
       DS_MIN_RTN_U64         = 103,   // Uint min.
       DS_MAX_RTN_U64         = 104,   // Uint max.
       DS_AND_RTN_B64         = 105,   // Dword AND.
       DS_OR_RTN_B64          = 106,   // Dword OR.
       DS_XOR_RTN_B64         = 107,   // Dword XOR.
       DS_MSKOR_RTN_B64       = 108,   // Masked Dword XOR.
       DS_WRXCHG_RTN_B64      = 109,   // Write exchange.
       DS_WRXCHG2_RTN_B64     = 110,   // Write exchange relative.
       DS_WRXCHG2ST64_RTN_B64 = 111,   // Write echange 2 Dwords.
       DS_CMPST_RTN_B64       = 112,   // Compare store.
       DS_CMPST_RTN_F64       = 113,   // Compare store with float rules.
       DS_MIN_RTN_F64         = 114,   // DS[A] = (D0 < DS[A]) ? D0 : DS[A]; float, handles NaN/INF/denorm.
       DS_MAX_RTN_F64         = 115,   // DS[A] = (D0 > DS[A]) ? D0 : DS[A]; float, handles NaN/INF/denorm.
       DS_READ_B64            = 118,   // Dword read.
       DS_READ2_B64           = 119,   // R = DS[ADDR+offset0*8], R+1 = DS[ADDR+offset1*8]. Read 2 Dwords
       DS_READ2ST64_B64       = 120,   // R = DS[ADDR+offset0*8*64], R+1 = DS[ADDR+offset1*8*64]. Read 2 Dwords.
       DS_CONDXCHG32_RTN_B64  = 126,   // Conditional write exchange. SEA ISLANDS ONLY
       DS_ADD_SRC2_U32        = 128,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} : {offset1[6],offset1[6:0],offset0}). DS[A] = DS[A] + DS[B]; uint add.
       DS_SUB_SRC2_U32        = 129,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} : {offset1[6],offset1[6:0],offset0}). DS[A] = DS[A] - DS[B]; uint subtract.
       DS_RSUB_SRC2_U32       = 130,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} : {offset1[6],offset1[6:0],offset0}). DS[A] = DS[B]- DS[A]; uint reverse subtract.
       DS_INC_SRC2_U32        = 131,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] = (DS[A] >= DS[B] ? 0 : DS[A] + 1); uint increment.
       DS_DEC_SRC2_U32        = 132,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] = (DS[A]== 0 || DS[A] > DS[B] ? DS[B] : DS[A] - 1); uintdecrement.
       DS_MIN_SRC2_I32        = 133,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] =min(DS[A], DS[B]); int min.
       DS_MAX_SRC2_I32        = 134,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} : {offset1[6],offset1[6:0],offset0}). DS[A] =max(DS[A], DS[B]); int max.
       DS_MIN_SRC2_U32        = 135,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] = min(DS[A], DS[B]); uint min.
       DS_MAX_SRC2_U32        = 136,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} : {offset1[6],offset1[6:0],offset0}). DS[A] =max(DS[A], DS[B]); uint maxw
       DS_AND_SRC2_B32        = 137,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} : {offset1[6],offset1[6:0],offset0}). DS[A] = DS[A]& DS[B]; Dword AND.
       DS_OR_SRC2_B32         = 138,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :  {offset1[6],offset1[6:0],offset0}). DS[A] = DS[A] |DS[B]; Dword OR.
       DS_XOR_SRC2_B32        = 139,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] = DS[A] ^DS[B]; Dword XOR.
       DS_WRITE_SRC2_B32      = 140,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] = DS[B];write Dword.
       DS_MIN_SRC2_F32        = 146,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] = (DS[B] < DS[A]) ? DS[B] : DS[A]; float, handles NaN/INF/denorm.
       DS_MAX_SRC2_F32        = 147,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} : {offset1[6],offset1[6:0],offset0}). DS[A] = (DS[B] > DS[A]) ? DS[B] : DS[A]; float, handles NaN/INF/denorm.
       DS_ADD_SRC2_U64        = 192,   // Uint add.
       DS_SUB_SRC2_U64        = 193,   // Uint subtract.
       DS_RSUB_SRC2_U64       = 194,   // Uint reverse subtract.
       DS_INC_SRC2_U64        = 195,   // Uint increment.
       DS_DEC_SRC2_U64        = 196,   // Uint decrement.
       DS_MIN_SRC2_I64        = 197,   // Int min.
       DS_MAX_SRC2_I64        = 198,   // Int max.
       DS_MIN_SRC2_U64        = 199,   // Uint min.
       DS_MAX_SRC2_U64        = 200,   // Uint max.
       DS_AND_SRC2_B64        = 201,   // Dword AND.
       DS_OR_SRC2_B64         = 202,   // Dword OR.
       DS_XOR_SRC2_B64        = 203,   // Dword XOR.
       DS_WRITE_SRC2_B64      = 204,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] = DS[B]; write Qword.
       DS_MIN_SRC2_F64        = 210,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). [A] = (D0 < DS[A]) ? D0 : DS[A]; float, handlesNaN/INF/denorm.
       DS_MAX_SRC2_F64        = 211,   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). [A] = (D0 > DS[A]) ? D0 : DS[A]; float, handlesNaN/INF/denorm.
       // SEA ISLANDS ONLY 
       DS_WRITE_B96            = 222,   //{DS[A+2], DS[A+1], DS[A]} = D0[95:0]; tri-dword write.
       DS_WRITE_B128           = 223,   //{DS[A+3], DS[A+2], DS[A+1], DS[A]} = D0[127:0]; qword write.
       DS_CONDXCHG32_RTN_B128  = 253,   //Conditional write exchange.
       DS_READ_B96             = 254,   //Tri-Dword read.
       DS_READ_B128            = 255,   //Qword read.

       //All other values are reserved.
    };



    enum MUBUFF_Opcodes
    {
                                         // Memory Buffer Instructions 12-39
                                         // © 2012 Advanced Micro Devices, Inc. All rights reserved.
        BUFFER_LOAD_FORMAT_X     = 0 ,   // : Untyped buffer load 1 Dword with format conversion.
        BUFFER_LOAD_FORMAT_XY    = 1 ,   // : Untyped buffer load 2 Dwords with format conversion.
        BUFFER_LOAD_FORMAT_XYZ   = 2 ,   // : Untyped buffer load 3 Dwords with format conversion.
        BUFFER_LOAD_FORMAT_XYZW  = 3 ,   // : Untyped buffer load 4 Dwords with format conversion.
        BUFFER_STORE_FORMAT_X    = 4 ,   // : Untyped buffer store 1 Dword with format conversion.
        BUFFER_STORE_FORMAT_XY   = 5 ,   // : Untyped buffer store 2 Dwords with format conversion.
        BUFFER_STORE_FORMAT_XYZ  = 6 ,   // : Untyped buffer store 3 Dwords with format conversion.
        BUFFER_STORE_FORMAT_XYZW = 7 ,   // : Untyped buffer store 4 Dwords with format conversion.
        BUFFER_LOAD_UBYTE        = 8 ,   // : Untyped buffer load unsigned byte.
        BUFFER_LOAD_SBYTE        = 9 ,   // : Untyped buffer load signed byte.
        BUFFER_LOAD_USHORT       = 10,   // : Untyped buffer load unsigned short.
        BUFFER_LOAD_SSHORT       = 11,   // : Untyped buffer load signed short.
        BUFFER_LOAD_DWORD        = 12,   // : Untyped buffer load Dword.
        BUFFER_LOAD_DWORDX2      = 13,   // : Untyped buffer load 2 Dwords.
        BUFFER_LOAD_DWORDX4      = 14,   // : Untyped buffer load 4 Dwords.
        BUFFER_LOAD_DWORDX3      = 15,   // : Untyped buffer load 3 Dwords. SEA ISLANDS
         //16 – 23 reserved.             //
        BUFFER_STORE_BYTE        = 24,   // : Untyped buffer store byte.
        //25 reserved.
        BUFFER_STORE_SHORT       = 26 , //  : Untyped buffer store short.
        //27 reserved.
        BUFFER_STORE_DWORD      = 28 , //   : Untyped buffer store Dword.
        BUFFER_STORE_DWORDX2    = 29 , //   : Untyped buffer store 2 Dwords.
        BUFFER_STORE_DWORDX4    = 30 , //   : Untyped buffer store 4 Dwords.
        BUFFER_STORE_DWORDX3    = 31 , //   : Untyped buffer store 3 Dwords.  SEA ISLANDS
        //31 – 47 reserved.
        BUFFER_ATOMIC_SWAP      = 48, //    : 32b. dst=src, returns previous value if glc==1.
        BUFFER_ATOMIC_CMPSWAP   = 49, //    : 32b, dst = (dst==cmp) ? src : dst. Returns previous value if glc==1. src comes from the first data-vgpr, cmp from the second.
        BUFFER_ATOMIC_ADD       = 50, //    : 32b, dst += src. Returns previous value if glc==1.
        BUFFER_ATOMIC_SUB       = 51, //    : 32b, dst -= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_RSUB      = 52, //    : 32b, dst = src-dst. Returns previous value if glc==1.  SI ONLY
        BUFFER_ATOMIC_SMIN      = 53, //    : 32b, dst = (src < dst) ? src : dst (signed). Returns previous value if glc==1.
        BUFFER_ATOMIC_UMIN      = 54, //    : 32b, dst = (src < dst) ? src : dst (unsigned). Returns previous value if glc==1.
        BUFFER_ATOMIC_SMAX      = 55, //    : 32b, dst = (src > dst) ? src : dst (signed). Returns previousvalue if glc==1.
        BUFFER_ATOMIC_UMAX      = 56, //    : 32b, dst = (src > dst) ? src : dst (unsigned). Returns previous value if glc==1.
        BUFFER_ATOMIC_AND       = 57, //    : 32b, dst &= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_OR        = 58, //    : 32b, dst |= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_XOR       = 59, //    : 32b, dst ^= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_INC       = 60, //    : 32b, dst = (dst >= src) ? 0 : dst+1. Returns previous value if glc==1.
        BUFFER_ATOMIC_DEC       = 61, //    : 32b, dst = ((dst==0 || (dst > src)) ? src : dst-1. Returns previous value if glc==1.
        BUFFER_ATOMIC_FCMPSWAP  = 62, //    : 32b , dst = (dst == cmp) ? src : dst, returns previous  value if glc==1. Float compare swap (handles NaN/INF/denorm). srccomes from the first data-vgpr; cmp from the second.
        BUFFER_ATOMIC_FMIN      = 63, //    : 32b , dst = (src < dst) ? src : dst,. Returns previousvalue if glc==1. float, handles NaN/INF/denorm.
        BUFFER_ATOMIC_FMAX      = 64, //    : 32b , dst = (src > dst) ? src : dst, returns previous valueif glc==1. float, handles NaN/INF/denorm.
       // 65 – 79 reserved.
        BUFFER_ATOMIC_SWAP_X2    = 80, //     : 64b. dst=src, returns previous value if glc==1.
        BUFFER_ATOMIC_CMPSWAP_X2 = 81, //     : 64b, dst = (dst==cmp) ? src : dst. Returns previous value if glc==1. src comes from the first two data-vgprs, cmp from the second two.
        BUFFER_ATOMIC_ADD_X2     = 82, //     : 64b, dst += src. Returns previous value if glc==1.
        BUFFER_ATOMIC_SUB_X2     = 83, //     : 64b, dst -= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_RSUB_X2    = 84, //     : 64b, dst = src-dst. Returns previous value if glc==1.  SI ONLY
        BUFFER_ATOMIC_SMIN_X2    = 85, //     : 64b, dst = (src < dst) ? src : dst (signed). Returnsprevious value if glc==1.
        BUFFER_ATOMIC_UMIN_X2    = 86, //     : 64b, dst = (src < dst) ? src : dst (unsigned).Returns previous value if glc==1.
        BUFFER_ATOMIC_SMAX_X2    = 87, //     : 64b, dst = (src > dst) ? src : dst (signed). Returnsprevious value if glc==1.
        BUFFER_ATOMIC_UMAX_X2    = 88, //     : 64b, dst = (src > dst) ? src : dst (unsigned). Returns previous value if glc==1.

        // NOTE: BUFFER_ATOMIC_RSUB(53) and RSUB_X2(84) is SI only

        BUFFER_ATOMIC_AND_X2       = 89 , //     : 64b, dst &= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_OR_X2        = 90 , //     : 64b, dst |= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_XOR_X2       = 91 , //     : 64b, dst ^= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_INC_X2       = 92 , //     : 64b, dst = (dst >= src) ? 0 : dst+1. Returns previous value if glc==1.
        BUFFER_ATOMIC_DEC_X2       = 93 , //     : 64b, dst = ((dst==0 || (dst > src)) ? src : dst-1. Returns previous value if glc==1.
        BUFFER_ATOMIC_FCMPSWAP_X2  = 94 , //     : 64b , dst = (dst == cmp) ? src : dst, returns previous value if glc==1. Double compare swap (handles NaN/INF/denorm). src comes from the first two data-vgprs, cmp from the second two.
        BUFFER_ATOMIC_FMIN_X2      = 95 , //     : 64b , dst = (src < dst) ? src : dst, returns previous value if glc==1. Double, handles NaN/INF/denorm.
        BUFFER_ATOMIC_FMAX_X2      = 96 , //     : 64b , dst = (src > dst) ? src : dst, returns previous value if glc==1. Double, handles NaN/INF/denorm.
        //97 – 111 reserved.
        BUFFER_WBINVL1_SC      = 112 , //      : write back and invalidate the shader L1 only for lines of MTYPE SC and GC. Always returns ACK to shader.
        BUFFER_WBINVL1         = 113 , //      : write back and invalidate the shader L1. Always returns ACK to shader.
    };


    enum MTBUFF_Opcodes
    {
        TBUFFER_LOAD_FORMAT_X      = 0 , //  : Untyped buffer load 1 Dword with format conversion.
        TBUFFER_LOAD_FORMAT_XY     = 1 , //  : Untyped buffer load 2 Dwords with format conversion.
        TBUFFER_LOAD_FORMAT_XYZ    = 2 , //  : Untyped buffer load 3 Dwords with format conversion.
        TBUFFER_LOAD_FORMAT_XYZW   = 3 , //  : Untyped buffer load 4 Dwords with format conversion.
        TBUFFER_STORE_FORMAT_X     = 4 , //  : Untyped buffer store 1 Dword with format conversion.
        TBUFFER_STORE_FORMAT_XY    = 5 , //  : Untyped buffer store 2 Dwords with format conversion.
        TBUFFER_STORE_FORMAT_XYZ   = 6 , //  : Untyped buffer store 3 Dwords with format conversion.
        TBUFFER_STORE_FORMAT_XYZW  = 7 , //  : Untyped buffer store 4 Dwords with format conversion.
    };

    enum MTBUFF_NumberFormat
    {
        NF_UNORM       ,
        NF_SNORM       ,
        NF_USCALED     ,
        NF_SSCALED     ,
        NF_UINT        ,
        NF_SINT        ,
        NF_SNORM_NZ    ,
        NF_FLOAT       ,
         //8 reserved
        NF_SRGB       =9,
        NF_UBNORM      ,
        NF_UBNORM_NZ   ,
        NF_UBINT       ,
        NF_UBSCALED    ,
    };
    enum MTBUFF_DataFormat
    {
        DF_INVALID=0,
        DF_8,
        DF_16,
        DF_8_8,
        DF_32,
        DF_16_16,
        DF_10_11_11,
        DF_11_11_10,
        DF_10_10_10_2,
        DF_2_10_10_10,
        DF_8_8_8_8,
        DF_32_32,
        DF_16_16_16_16,
        DF_32_32_32,
        DF_32_32_32_32,
    };

    enum MIMG_Opcodes
    {
        IMAGE_LOAD             = 0,   // Image memory load with format conversion specified in T#. No sampler.
        IMAGE_LOAD_MIP         = 1,   // Image memory load with user-supplied mip level. No sampler.
        IMAGE_LOAD_PCK         = 2,   // Image memory load with no format conversion. No sampler.
        IMAGE_LOAD_PCK_SGN     = 3,   // Image memory load with with no format conversion and sign extension. No sampler.
        IMAGE_LOAD_MIP_PCK     = 4,   // Image memory load with user-supplied mip level, no format conversion. No sampler.
        IMAGE_LOAD_MIP_PCK_SGN = 5,   // Image memory load with user-supplied mip level, no format conversion and with sign extension. No sampler.
        //6 – 7 reserved.
        IMAGE_STORE          = 8 ,     //: Image memory store with format conversion specified in T#. No sampler.
        IMAGE_STORE_MIP      = 9 ,     //: Image memory store with format conversion specified in T# to user specified mip level. No sampler.
        IMAGE_STORE_PCK      = 10,     //: Image memory store of packed data without format conversion. No sampler.
        IMAGE_STORE_MIP_PCK  =  11,    //: Image memory store of packed data without format conversion to user-supplied mip level. No sampler.
        //12 – 13 reserved.
        IMAGE_GET_RESINFO     = 14,    //: return resource info. No sampler.
        IMAGE_ATOMIC_SWAP     = 15,    //: dst=src, returns previous value if glc==1.
        IMAGE_ATOMIC_CMPSWAP  = 16,    //: dst = (dst==cmp) ? src : dst. Returns previous value if glc==1.
        IMAGE_ATOMIC_ADD      = 17,    //: dst += src. Returns previous value if glc==1.
        IMAGE_ATOMIC_SUB      = 18,    //: dst -= src. Returns previous value if glc==1.
        IMAGE_ATOMIC_RSUB     = 19,    //: dst = src-dst. Returns previous value if glc==1.  SI ONLY
        IMAGE_ATOMIC_SMIN     = 20,    //: dst = (src < dst) ? src : dst (signed). Returns previous value if glc==1.
        IMAGE_ATOMIC_UMIN     = 21,    //: dst = (src < dst) ? src : dst (unsigned). Returns previous value if glc==1.
        IMAGE_ATOMIC_SMAX     = 22,    //: dst = (src > dst) ? src : dst (signed). Returns previous value if glc==1.
        IMAGE_ATOMIC_UMAX     = 23,    //: dst = (src > dst) ? src : dst (unsigned). Returns previous value if glc==1.
        IMAGE_ATOMIC_AND      = 24,    //: dst &= src. Returns previous value if glc==1.
        IMAGE_ATOMIC_OR       = 25,    //: dst |= src. Returns previous value if glc==1.
        IMAGE_ATOMIC_XOR      = 26,    //: dst ^= src. Returns previous value if glc==1.
        IMAGE_ATOMIC_INC      = 27,    //: dst = (dst >= src) ? 0 : dst+1. Returns previous value if glc==1.
        IMAGE_ATOMIC_DEC      = 28,    //: dst = ((dst==0 || (dst > src)) ? src : dst-1. Returns previous value if glc==1.
        IMAGE_ATOMIC_FCMPSWAP = 29,    //: dst = (dst == cmp) ? src : dst, returns previous value of dst if glc==1 - double and float atomic compare swap. Obeys floating point compare rules for special values.
        IMAGE_ATOMIC_FMIN     = 30,    //: dst = (src < dst) ? src : dst, returns previous value of dst if glc==1 - double and float atomic min (handles NaN/INF/denorm).
        IMAGE_ATOMIC_FMAX     = 31,    //: dst = (src > dst) ? src : dst, returns previous value of dst if glc==1 - double and float atomic min (handles NaN/INF/denorm).
        IMAGE_SAMPLE          = 32,    //: sample texture map.
        IMAGE_SAMPLE_CL       = 33,    //: sample texture map, with LOD clamp specified in shader.
        IMAGE_SAMPLE_D        = 34,    //: sample texture map, with user derivatives.
        IMAGE_SAMPLE_D_CL     = 35,    //: sample texture map, with LOD clamp specified in shader, with user derivatives.
        IMAGE_SAMPLE_L        = 36,    //: sample texture map, with user LOD.
        IMAGE_SAMPLE_B        = 37,    //: sample texture map, with lod bias.
        IMAGE_SAMPLE_B_CL     = 38,    //: sample texture map, with LOD clamp specified in shader, with lod bias.
        IMAGE_SAMPLE_LZ       = 39,    //: sample texture map, from level 0.
        IMAGE_SAMPLE_C        = 40,    //: sample texture map, with PCF.
        IMAGE_SAMPLE_C_CL     = 41,    //: SAMPLE_C, with LOD clamp specified in shader.
        IMAGE_SAMPLE_C_D      = 42,    //: SAMPLE_C, with user derivatives.
        IMAGE_SAMPLE_C_D_CL   = 43,    //: SAMPLE_C, with LOD clamp specified in shader,with user derivatives.
        IMAGE_SAMPLE_C_L      = 44,    //: SAMPLE_C, with user LOD.
        IMAGE_SAMPLE_C_B      = 45,    //: SAMPLE_C, with lod bias.
        IMAGE_SAMPLE_C_B_CL   = 46,    //: SAMPLE_C, with LOD clamp specified in shader, with lod bias.
        IMAGE_SAMPLE_C_LZ      = 47, //     : SAMPLE_C, from level 0.
        IMAGE_SAMPLE_O         = 48, // : sample texture map, with user offsets.
        IMAGE_SAMPLE_CL_O      = 49, //     : SAMPLE_O with LOD clamp specified in shader.
        IMAGE_SAMPLE_D_O       = 50, // : SAMPLE_O, with user derivatives.
        IMAGE_SAMPLE_D_CL_O    = 51, //     : SAMPLE_O, with LOD clamp specified in shader,with user derivatives.
        IMAGE_SAMPLE_L_O       = 52, // : SAMPLE_O, with user LOD.
        IMAGE_SAMPLE_B_O       = 53, // : SAMPLE_O, with lod bias.
        IMAGE_SAMPLE_B_CL_O    = 54, // : SAMPLE_O, with LOD clamp specified in shader,with lod bias.
        IMAGE_SAMPLE_LZ_O      = 55, // : SAMPLE_O, from level 0.
        IMAGE_SAMPLE_C_O       = 56, // : SAMPLE_C with user specified offsets.
        IMAGE_SAMPLE_C_CL_O    = 57, // : SAMPLE_C_O, with LOD clamp specified in shader.
        IMAGE_SAMPLE_C_D_O     = 58, // : SAMPLE_C_O, with user derivatives.
        IMAGE_SAMPLE_C_D_CL_O  = 59, // : SAMPLE_C_O, with LOD clamp specified in shader, with user derivatives.
        IMAGE_SAMPLE_C_L_O     = 60, // : SAMPLE_C_O, with user LOD.
        IMAGE_SAMPLE_C_B_O     = 61, // : SAMPLE_C_O, with lod bias.
        IMAGE_SAMPLE_C_B_CL_O  = 62, // : SAMPLE_C_O, with LOD clamp specified in shader, with lod bias.
        IMAGE_SAMPLE_C_LZ_O    = 63, // : SAMPLE_C_O, from level 0.
        IMAGE_GATHER4          = 64, // : gather 4 single component elements (2x2).
        IMAGE_GATHER4_CL       = 65, // : gather 4 single component elements (2x2) with user LOD clamp.
        IMAGE_GATHER4_L        = 66, // : gather 4 single component elements (2x2) with user LOD.
        IMAGE_GATHER4_B        = 67, // : gather 4 single component elements (2x2) with user bias.
        IMAGE_GATHER4_B_CL     = 68, // : gather 4 single component elements (2x2) with user bias and clamp.
        IMAGE_GATHER4_LZ       = 69, // : gather 4 single component elements (2x2) at level 0.
        IMAGE_GATHER4_C        = 70, // : gather 4 single component elements (2x2) with PCF.
        IMAGE_GATHER4_C_CL     = 71, // : gather 4 single component elements (2x2) with user LOD clamp and PCF.
        //72 – 75 reserved.          //
        IMAGE_GATHER4_C_L    = 76,  // : gather 4 single component elements (2x2) with user LOD and PCF.
        IMAGE_GATHER4_C_B    = 77,  // : gather 4 single component elements (2x2) with user bias and PCF.
        IMAGE_GATHER4_C_B_CL = 78,  // : gather 4 single component elements (2x2) with user bias, clamp and PCF.
        IMAGE_GATHER4_C_LZ   = 79,  // : gather 4 single component elements (2x2) at level 0, with PCF.
        IMAGE_GATHER4_O      = 80,  // : GATHER4, with user offsets.
        IMAGE_GATHER4_CL_O   = 81,  // : GATHER4_CL, with user offsets.
        //82 – 83 reserved.           //
        IMAGE_GATHER4_L_O     = 84,  // : GATHER4_L, with user offsets.
        IMAGE_GATHER4_B_O     = 85,  // : GATHER4_B, with user offsets.
        IMAGE_GATHER4_B_CL_O  = 86,  // : GATHER4_B_CL, with user offsets.
        IMAGE_GATHER4_LZ_O    = 87,  // : GATHER4_LZ, with user offsets.
        IMAGE_GATHER4_C_O     = 88,  // : GATHER4_C, with user offsets.
        IMAGE_GATHER4_C_CL_O  = 89,  // : GATHER4_C_CL, with user offsets.
        //90 – 91 reserved.          //
        IMAGE_GATHER4_C_L_O   = 92,  // : GATHER4_C_L, with user offsets.
        IMAGE_GATHER4_C_B_O   = 93,  // : GATHER4_B, with user offsets.
        IMAGE_GATHER4_C_B_CL_O= 94,  // : GATHER4_B_CL, with user offsets.
        IMAGE_GATHER4_C_LZ_O  = 95,  // : GATHER4_C_LZ, with user offsets.
        IMAGE_GET_LOD         = 96,  // : Return calculated LOD.
        //97 – 103 reserved.         //
        IMAGE_SAMPLE_CD        = 104, // : sample texture map, with user derivatives (LOD per quad)
        IMAGE_SAMPLE_CD_CL     = 105, // : sample texture map, with LOD clamp specified in  shader, with user derivatives (LOD per quad).
        IMAGE_SAMPLE_C_CD      = 106, // : SAMPLE_C, with user derivatives (LOD per quad).
        IMAGE_SAMPLE_C_CD_CL   = 107, // : SAMPLE_C, with LOD clamp specified in shader, with user derivatives (LOD per quad).
        IMAGE_SAMPLE_CD_O      = 108, // : SAMPLE_O, with user derivatives (LOD per quad).
        IMAGE_SAMPLE_CD_CL_O   = 109, // : SAMPLE_O, with LOD clamp specified in shader, with user derivatives (LOD per quad).
        IMAGE_SAMPLE_C_CD_O    = 110, // : SAMPLE_C_O, with user derivatives (LOD per quad).
        IMAGE_SAMPLE_C_CD_CL_O = 111, // : SAMPLE_C_O, with LOD clamp specified in shader, with user derivatives (LOD per quad).
        
       // All other values are reserved.
    };

    enum FLAT_Opcodes // Flat opcode family is SEA ISLANDS ONLY
    {
        //0 - 7 reserved.
        FLAT_LOAD_UBYTE       =  8,  //: Flat load unsigned byte. Zero extend to VGPR destination.
        FLAT_LOAD_SBYTE       =  9,  //: Flat load signed byte. Sign extend to VGPR destination.
        FLAT_LOAD_USHORT      = 10,  //: Flat load unsigned short. Zero extend to VGPR destination.
        FLAT_LOAD_SSHORT      = 11,  //: Flat load signed short. Sign extend to VGPR destination.
        FLAT_LOAD_DWORD       = 12,  //: Flat load Dword.
        FLAT_LOAD_DWORDX2     = 13,  //: Flat load 2 Dwords.
        FLAT_LOAD_DWORDX4     = 14,  //: Flat load 4 Dwords.
        FLAT_LOAD_DWORDX3     = 15,  //: Flat load 3 Dwords.
        //16 - 23 reserved.          //
        FLAT_STORE_BYTE       = 24,  //: Flat store byte.
        //25reserved.                //
        FLAT_STORE_SHORT      = 26,  //: Flat store short.
        //27reserved.                //
        FLAT_STORE_DWORD      = 28,  //: Flat store Dword.
        FLAT_STORE_DWORDX2    = 29,  //: Flat store 2 Dwords.
        FLAT_STORE_DWORDX4    = 30,  //: Flat store 4 Dwords.
        FLAT_STORE_DWORDX3    = 31,  //: Flat store 3 Dwords.
        //32 – 47 reserved.          //
        FLAT_ATOMIC_SWAP      = 48,  //: 32b. dst=src, returns previous value if rtn==1.
        FLAT_ATOMIC_CMPSWAP   = 49,  //: 32b, dst = (dst==cmp) ? src : dst. Returns previous value if rtn==1. src comes from the first data-VGPR, cmp from the second.
        FLAT_ATOMIC_ADD       = 50,  //: 32b, dst += src. Returns previous value if rtn==1.
        FLAT_ATOMIC_SUB       = 51,  //: 32b, dst -= src. Returns previous value if rtn==1.
        //52reserved.                //
        FLAT_ATOMIC_SMIN      = 53,  //: 32b, dst = (src < dst) ? src : dst (signed comparison). Returns previous value if rtn==1.
        FLAT_ATOMIC_UMIN      = 54,  //: 32b, dst = (src < dst) ? src : dst (unsigned comparison). Returns previous value if rtn==1.
        FLAT_ATOMIC_SMAX      = 55,  //: 32b, dst = (src > dst) ? src : dst (signed comparison). Returns previous value if rtn==1.
                                     //
        FLAT_ATOMIC_UMAX       = 56,  //: 32b, dst = (src > dst) ? src : dst (unsigned comparison). Returns previous value if rtn==1.
        FLAT_ATOMIC_AND        = 57,  //: 32b, dst &= src. Returns previous value if rtn==1.
        FLAT_ATOMIC_OR         = 58,  //: 32b, dst |= src. Returns previous value if rtn==1.
        FLAT_ATOMIC_XOR        = 59,  //: 32b, dst ^= src. Returns previous value if rtn==1.
        FLAT_ATOMIC_INC        = 60,  //: 32b, dst = (dst >= src) ? 0 : dst+1 (unsigned comparison). Returns previous value if rtn==1.
        FLAT_ATOMIC_DEC        = 61,  //: 32b, dst = ((dst==0 || (dst > src)) ? src : dst-1 (unsigned comparison). Returns previous value if rtn==1.
        FLAT_ATOMIC_FCMPSWAP   = 62,  //: 32b , dst = (dst == cmp) ? src : dst, returns previous value if rtn==1. Floating point compare-swap handles NaN/INF/denorm. src comes from the first data-VGPR; cmp from the second.
        FLAT_ATOMIC_FMIN       = 63,  //: 32b , dst = (src < dst) ? src : dst. Returns previous value if rtn==1. float, handles NaN/INF/denorm.
        FLAT_ATOMIC_FMAX       = 64,  //: 32b , dst = (src > dst) ? src : dst, returns previous value if rtn==1. Floating point compare handles NaN/INF/denorm.
        //65 – 79 reserved.           //
        FLAT_ATOMIC_SWAP_X2     = 80, //: 64b. dst=src, returns previous value if rtn==1.
        FLAT_ATOMIC_CMPSWAP_X2  = 81, //: 64b, dst = (dst==cmp) ? src : dst. Returns previous value if rtn==1. src comes from the first two data-VGPRs, cmp from the second two.
        FLAT_ATOMIC_ADD_X2      = 82, //: 64b, dst += src. Returns previous value if rtn==1.
        FLAT_ATOMIC_SUB_X2      = 83, //: 64b, dst -= src. Returns previous value if rtn==1.
        //84reserved.                 //
        FLAT_ATOMIC_SMIN_X2     = 85, //: 64b, dst = (src < dst) ? src : dst (signed comparison). Returns previous value if rtn==1.
        FLAT_ATOMIC_UMIN_X2     = 86, //: 64b, dst = (src < dst) ? src : dst (unsigned comparison). Returns previous value if rtn==1.
        FLAT_ATOMIC_SMAX_X2     = 87, //: 64b, dst = (src > dst) ? src : dst (signed comparison). Returns previous value if rtn==1.
        FLAT_ATOMIC_UMAX_X2     = 88, //: 64b, dst = (src > dst) ? src : dst (unsigned comparison). Returns previous value if rtn==1.
        FLAT_ATOMIC_AND_X2      = 89, //: 64b, dst &= src. Returns previous value if rtn==1.
        FLAT_ATOMIC_OR_X2       = 90, //: 64b, dst |= src. Returns previous value if rtn==1.
        FLAT_ATOMIC_XOR_X2      = 91, //: 64b, dst ^= src. Returns previous value if rtn==1.
        FLAT_ATOMIC_INC_X2      = 92, //: 64b, dst = (dst >= src) ? 0 : dst+1. Returns previous value if rtn==1.
        FLAT_ATOMIC_DEC_X2      = 93, //: 64b, dst = ((dst==0 || (dst > src)) ? src : dst - 1. Returns previous value if rtn==1.
        FLAT_ATOMIC_FCMPSWAP_X2 = 94, //: 64b , dst = (dst == cmp) ? src : dst, returns previous value if rtn==1. Double compare swap (handles NaN/INF/denorm). src comes from the first two data-VGPRs, cmp from the second two.
        FLAT_ATOMIC_FMIN_X2     = 95, //: 64b , dst = (src < dst) ? src : dst, returns previous value if rtn==1. Double, handles NaN/INF/denorm.
        FLAT_ATOMIC_FMAX_X2     = 96, //: 64b , dst = (src > dst) ? src : dst, returns previous value if rtn==1. Double, handles NaN/INF/denorm.
        
        //All other values are reserved.
    };


    enum ExportTargets
    {
        EXP_MRT0      = 0,
        EXP_MRT1      = 1,
        EXP_MRT2      = 2,
        EXP_MRT3      = 3,
        EXP_MRT4      = 4,
        EXP_MRT5      = 5,
        EXP_MRT6      = 6,
        EXP_MRT7      = 7,
        EXP_Z         = 8,
        EXP_NULL      = 9,
        EXP_POS0      = 12,
        EXP_POS1      = 13,
        EXP_POS2      = 14,
        EXP_POS3      = 15,
        EXP_PARAM0    = 32,
        EXP_PARAM1,
        EXP_PARAM2,
        EXP_PARAM3,
        EXP_PARAM4,
        EXP_PARAM5,
        EXP_PARAM6,
        EXP_PARAM7,
        EXP_PARAM8,
        EXP_PARAM9,
        EXP_PARAM10,
        EXP_PARAM11,
        EXP_PARAM12,
        EXP_PARAM13,
        EXP_PARAM14,
        EXP_PARAM15,
        EXP_PARAM16,
        EXP_PARAM17,
        EXP_PARAM18,
        EXP_PARAM19,
        EXP_PARAM20,
        EXP_PARAM21,
        EXP_PARAM22,
        EXP_PARAM23,
        EXP_PARAM24,
        EXP_PARAM25,
        EXP_PARAM26,
        EXP_PARAM27,
        EXP_PARAM28,
        EXP_PARAM29,
        EXP_PARAM30,
        EXP_PARAM31,
    };

    const char* EnumToString(SOP2_Opcodes);
    const char* EnumToString(SOP1_Opcodes);
    const char* EnumToString(SOPK_Opcodes);
    const char* EnumToString(SOPC_Opcodes);
    const char* EnumToString(SOPP_Opcodes);
    const char* EnumToString(SMEM_Opcodes);
    const char* EnumToString(VOP2_Opcodes);
    const char* EnumToString(VOP1_Opcodes);
    const char* EnumToString(VOPC_Opcodes);
    const char* EnumToString(VOP3_Opcodes);
    const char* EnumToString(VINTERP_Opcodes);
    const char* EnumToString(DS_Opcodes);
    const char* EnumToString(MUBUFF_Opcodes);
    const char* EnumToString(MTBUFF_Opcodes);
    const char* EnumToString(MIMG_Opcodes);
    const char* EnumToString(FLAT_Opcodes);
    const char* EnumToString(ExportTargets);
    
    const char* EnumToString(MTBUFF_NumberFormat);
    const char* EnumToString(MTBUFF_DataFormat);
}



#endif