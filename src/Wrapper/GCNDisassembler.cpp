//
//    GCN disassembler
//      Copyright 2015 Joshua Barczak, all rights reserved.
//
#pragma unmanaged
#include "GCNDisassembler.h"
#include "GCNIsa.h"

#define _CRT_SECURE_NO_WARNINGS
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include <algorithm>

typedef unsigned char uint8;
typedef unsigned int uint;





namespace GCN{
namespace Disassembler{
    namespace _INTERNAL
    {
        #define OPCODE_FORMAT "%22s"
        static const char* BINARY_OP_FORMAT = OPCODE_FORMAT " %6s,%6s,%6s\n";
        static const char* UNARY_OP_FORMAT  = OPCODE_FORMAT " %6s,%6s\n";   

        void Printf( IPrinter& printer, const char* Format, ... )
        {
            va_list vl;
            va_start(vl,Format);

            char scratch[2048];
            char* p = scratch;

            int n = _vscprintf( Format, vl );
            if( n+1 > sizeof(scratch) )
                p = (char*) malloc( n+1 );
            
            vsprintf(p,Format,vl);
            printer.Push(p);

            if( p != scratch )
                free(p);

            va_end(vl);
        }


        void FormatIndexedReg( char* buff, const char* pPrefix, uint nRegNum, uint nDWORDs )
        {
            if( nDWORDs>1)
                sprintf( buff, "%s%u[%u]",pPrefix, nRegNum, nDWORDs );
            else
                sprintf( buff, "%s%u", pPrefix, nRegNum );
        }
       
        void FormatReg64( char* buff, const char* pPrefix, uint nHiLo, uint nDWORDS )
        {
            if( nDWORDS == 1 ) // partial reg write
                sprintf( buff, "%s[%s]", pPrefix, nHiLo ? "HI" : "LO" );
            else // assumed full reg write
                strcpy(buff, pPrefix );
        }

        void FormatVReg( char* buff, uint nSrc, uint nDWORDS )
        {
            FormatIndexedReg(buff, "v", nSrc, nDWORDS );
        }
        void FormatSReg( char* buff, uint nSrc, uint nDWORDS )
        {
            FormatIndexedReg(buff, "s", nSrc, nDWORDS );
        }
       
        void FormatGenericSource( char* buff, uint nSrc, uint nDWORDs )
        {
            if( nSrc >= SGPR_FIRST && nSrc <= SGPR_LAST ) // sgpr
            {
                FormatSReg( buff,  nSrc-SGPR_FIRST, nDWORDs );
                return; // sgpr
            }
            if( nSrc >= TTMP_FIRST && nSrc <= TTMP_LAST ) // trap temporaries
            {
                FormatIndexedReg( buff, "TT", nSrc-TTMP_FIRST, nDWORDs );
                return; // sgpr
            }
            if( nSrc >= VGPR_FIRST && nSrc <= VGPR_LAST ) // vgpr
            {
                FormatVReg( buff, nSrc-VGPR_FIRST, nDWORDs );
                return; // sgpr
            }

            if( nSrc >= CI_POSITIVE_FIRST && nSrc <= CI_POSITIVE_LAST ) // positive int 1 to 64
            {
                sprintf( buff, "%d", (nSrc - CI_POSITIVE_FIRST)+1 );
                return;
            }
            if( nSrc >= CI_NEGATIVE_FIRST && nSrc <= CI_NEGATIVE_LAST ) // negative int, -1 to 16
            {
                sprintf( buff, "%d", (CI_NEGATIVE_FIRST-1)-nSrc);
                return;
            }
            
            // various 64-bit architectural regs
            switch( nSrc )
            {
            case FSCR_LO:
            case FSCR_HI:
                FormatReg64( buff, "FSCR", nSrc-FSCR_LO, nDWORDs );
                return ;
            case VCC_LO  : 
            case VCC_HI  : 
                FormatReg64( buff, "VCC", nSrc-VCC_LO, nDWORDs );
                return;
            case TBA_LO  : 
            case TBA_HI  : 
                FormatReg64( buff, "TBA", nSrc-TBA_LO, nDWORDs );
                return;
            case TMA_LO  : 
            case TMA_HI  :
                FormatReg64( buff, "TMA", nSrc-TMA_LO, nDWORDs );
                return;
            case EXEC_LO :
            case EXEC_HI :
                FormatReg64( buff, "EXEC", nSrc-EXEC_LO, nDWORDs );
                return;
            }

            switch( nSrc )
            {
            case M0:                strcpy( buff, "M0" );     break;
            case C_ZERO:            strcpy( buff, "0" );      break;
            case CF_ONEHALF:        strcpy( buff, "0.5");     break;
            case CF_MINUS_ONEHALF:  strcpy( buff, "-0.5");    break;
            case CF_ONE:            strcpy( buff, "1.0");     break;
            case CF_MINUS_ONE:      strcpy( buff, "-1.0");    break;
            case CF_TWO:            strcpy( buff, "2.0");     break;
            case CF_MINUS_TWO:      strcpy( buff, "-2.0");    break;
            case CF_FOUR:           strcpy( buff, "4.0");     break;
            case CF_MINUS_FOUR:     strcpy( buff, "-4.0");    break;
            case VCCZ       :       strcpy( buff, "VCCZ");    break;
            case EXECZ      :       strcpy( buff, "EXECZ");   break;
            case SCC        :       strcpy( buff, "SCC");     break;
            case LDS_DIRECT :       strcpy( buff, "LDS[M0]"); break;
            default:                strcpy( buff, "??????");  break;
            }
        }

        void FormatDest( char* buff, uint nSrc, uint nDWORDs )
        {
            FormatGenericSource(buff,nSrc,nDWORDs);
        }
       

        template< class TInstruction >
        void FormatTrailingLiteral( char* buff, const TInstruction* pI )
        {
            uint nLen = pI->GetLength();
            const uint8* pBytes = reinterpret_cast<const uint8*>(pI) + nLen-4;
            const uint32* pInt  = reinterpret_cast<const uint32*>( pBytes );
            sprintf( buff, "0x%08x", *pInt );
        }

        template< class TInstruction >
        void FormatSource( char* buff, uint nSrc, uint nDWORDs, const TInstruction* pI )
        {
            if( nSrc == LITERAL )
                FormatTrailingLiteral( buff, pI ) ; 
            else
                FormatGenericSource( buff, nSrc, nDWORDs );
        }

        template< class TInstruction >
        void PrintLabeledBranch( IPrinter& printer, const TInstruction* pInst, const std::vector<const Instruction*>* pLabels )
        {
            const Instruction* pTarget = pInst->GetBranchTarget();
            size_t i=0;
            while( pLabels && i < pLabels->size() )
            {
                if( pLabels->at(i) == pTarget )
                    break;
                i++;
            }

            if( pLabels && i < pLabels->size() )
            {
                Printf( printer, OPCODE_FORMAT" Label_%04u\n", EnumToString(pInst->GetOpcode()), i );
            }
            else if( pTarget )
            {
                Printf( printer, OPCODE_FORMAT" PC + %d\n", EnumToString(pInst->GetOpcode()), pTarget - static_cast<const Instruction*>(pInst) );
            }
            else
            {
                Printf( printer, OPCODE_FORMAT" ????\n", EnumToString(pInst->GetOpcode()) );
            }
        }

        

        const Instruction* Disassemble( IPrinter& printer, const SOP2Instruction* pInst )
        {
            const char* pOpcode = EnumToString(pInst->GetOpcode());
            
            if( pInst->GetOpcode() == S_CBRANCH_G_FORK )
            {
                char src0[64];
                char src1[64];
                FormatGenericSource(src0,pInst->GetSrc0(),2);
                FormatGenericSource(src1,pInst->GetSrc1(),2);
                Printf(printer,UNARY_OP_FORMAT, pOpcode, src0,src1 );
            }
            else
            {
                uint32 nDWORDS = pInst->GetResultWidthInDWORDS();
                char src0[64];
                char src1[64];
                char dst[64];
                FormatSource( src0, pInst->GetSrc0(), nDWORDS, pInst );
                FormatSource( src1, pInst->GetSrc1(), nDWORDS, pInst );
                FormatDest( dst, pInst->GetDest(), nDWORDS );

                Printf( printer, BINARY_OP_FORMAT, pOpcode, dst, src0, src1 );
            }

            return NextInstruction(pInst);
        }
        
        const Instruction* Disassemble( IPrinter& printer, const SOPCInstruction* pInst )
        {
            char src0[64];
            char src1[64];
            FormatSource( src0, pInst->GetSrc0(), 1, pInst );
            FormatSource( src1, pInst->GetSrc1(), 1, pInst );
            Printf(printer, UNARY_OP_FORMAT, EnumToString(pInst->GetOpcode()), src0, src1 );
            return NextInstruction(pInst);
        }

  

        const Instruction* Disassemble( IPrinter& printer, const SOPPInstruction* pInst, const std::vector<const Instruction*>* pLabels )
        {
            SOPP_Opcodes eOpcode = pInst->GetOpcode();
            switch( eOpcode )
            {
            default:
            case S_BARRIER:
            case S_ENDPGM:
            case S_ICACHE_INV:
                Printf( printer, OPCODE_FORMAT"\n", EnumToString(eOpcode)); 
                break;
            case S_NOP:     
                Printf( printer, OPCODE_FORMAT"(%u)\n","S_NOP",pInst->ReadSIMMBits(2,0)+1); 
                break;
            case S_SETHALT:
                Printf( printer, OPCODE_FORMAT"(%u)\n", "S_SETHALT", pInst->ReadSIMMBits(0,0) );
                break;
            case S_SLEEP:
                Printf( printer, OPCODE_FORMAT"(%u)\n", "S_SLEEP", 64*pInst->ReadSIMMBits(2,0) );
                break;
            case S_SETPRIO:
                Printf( printer, OPCODE_FORMAT"(%u)\n", "S_SETPRIO", pInst->ReadSIMMBits(1,0) );
                break;
            case S_TRAP:
                Printf( printer, OPCODE_FORMAT"(%u)\n", "S_TRAP", pInst->ReadSIMMBits(7,0) );
                break;
            case S_INCPERFLEVEL:
            case S_DECPERFLEVEL:
                Printf( printer, OPCODE_FORMAT"(%u)\n", EnumToString(eOpcode), pInst->ReadSIMMBits(3,0) );
                break;

            case S_SENDMSG:
            case S_SENDMSGHALT:
                {
                    uint nMsg = pInst->ReadSIMMBits(3,0);
                    switch( nMsg )
                    {
                    case 1: // interrupt
                        Printf(printer,OPCODE_FORMAT"  INTERRUPT(M0)\n",EnumToString(eOpcode));
                        break; 
                    case 2: // geometry shader goodness
                    case 3:
                        {
                            // 2 is GS.  3 is GS_DONE
                            // the GS op is NOP(0), CUT(1), EMIT(2), EMIT-CUT(3)
                            uint nGSOp = pInst->ReadSIMMBits(5,4);                            
                            Printf( printer, OPCODE_FORMAT"  GS: %s%s%s\n",
                                   EnumToString(eOpcode),
                                   (nGSOp&2) ? " EMIT " : "",
                                   (nGSOp&1) ? " CUT " : "",
                                   (nMsg == 3) ? "DONE" : "");
                        }                       
                        break; 

                    default: // something mysterious
                        Printf(printer,OPCODE_FORMAT"  0x%04x\n",EnumToString(eOpcode),pInst->ReadSIMMBits(15,0) );
                        break; 
                    }

                }
                break;

            case S_WAITCNT:
                {
                    uint VMCNT   = pInst->ReadSIMMBits(3,0);
                    uint EXPCNT  = pInst->ReadSIMMBits(6,4);
                    uint LKGMCNT = pInst->ReadSIMMBits(12,8);
                    
                    Printf( printer, OPCODE_FORMAT"     vmcnt(%u) expcnt(%u) lkgmcnt(%u)\n", "S_WAITCNT", VMCNT,EXPCNT,LKGMCNT );
                }
                break;

            case S_BRANCH         :         //: PC = PC + signext(SIMM16 * 4) + 4.
            case S_CBRANCH_SCC0   :         //: if(SCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
            case S_CBRANCH_SCC1   :         //: if(SCC == 1) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
            case S_CBRANCH_VCCZ   :         //: if(VCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
            case S_CBRANCH_VCCNZ  :         //: if(VCC != 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
            case S_CBRANCH_EXECZ  :         //: if(EXEC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
            case S_CBRANCH_EXECNZ :         //: if(EXEC != 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
            case S_CBRANCH_CDBGSYS :        // : If (conditional_debug_system != 0) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
            case S_CBRANCH_CDBGUSER:        // : If (conditional_debug_user != 0) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
            case S_CBRANCH_CDBGSYS_OR_USER: // : I f (conditional_debug_system || conditional_debug_user) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
            case S_CBRANCH_CDBGSYS_AND_USER:// : If (conditional_debug_system && conditional_debug_user) then PC = PC + signext(SIMM16 * 4) + 4; else NOP.
                PrintLabeledBranch(printer,pInst,pLabels);
                break;
            }
    
            return NextInstruction(pInst);
        }

        const Instruction* Disassemble( IPrinter& printer, const VOP2Instruction* pInst )
        {
            const char* pOpcode = EnumToString(pInst->GetOpcode());
            char src0[64];
            char src1[64];
            char dst[64];
            FormatSource( src0, pInst->GetSrc0(),  1,  pInst );
            FormatVReg( src1,   pInst->GetVSrc1(), 1 );
            FormatVReg( dst,    pInst->GetVDst(),  1 );
                    
            switch( pInst->GetOpcode() )
            {
            case V_MADAK_F32: 
                Printf(printer, OPCODE_FORMAT " %6s=%6s*%s + %f\n", pOpcode, dst, src0, src1, pInst->ReadFloatLiteral() );
                break;
            case V_MADMK_F32:
                Printf(printer, OPCODE_FORMAT " %6s=%6s *%10f + %s\n", pOpcode, dst, src0, pInst->ReadFloatLiteral(), src1 );
                break;
            default:
                Printf( printer, BINARY_OP_FORMAT, pOpcode,dst,src0,src1);
                break;
            }

            return NextInstruction(pInst);
        }

        const Instruction* Disassemble( IPrinter& printer, const VOP1Instruction* pInst )
        {
            char src0[64];
            char dst[64];
                
            switch( pInst->GetOpcode() )
            {
            case V_NOP:     Printf( printer, OPCODE_FORMAT"\n", "V_NOP"    ); break; 
            case V_CLREXCP: Printf( printer, OPCODE_FORMAT"\n", "V_CLREXCP"); break;
            case V_MOVRELD_B32:
                {
                    FormatVReg(dst,pInst->GetVDst(),1);
                    FormatSource(src0,pInst->GetSrc0(),1,pInst);
                    Printf(printer,OPCODE_FORMAT " v[M0+%s] = v[%s]\n", "V_MOVRELD", dst, src0 );
                }
                break;
            case V_MOVRELS_B32:
                {
                    FormatVReg(dst,pInst->GetVDst(),1);
                    FormatSource(src0,pInst->GetSrc0(),1,pInst);
                    Printf(printer,OPCODE_FORMAT " v[%s] = v[M0+%s]\n", "V_MOVRELS", dst, src0 );
                }
                break;
            case V_MOVRELSD_B32:
                {
                    FormatVReg(dst,pInst->GetVDst(),1);
                    FormatSource(src0,pInst->GetSrc0(),1,pInst);
                    Printf(printer,OPCODE_FORMAT " v[M0+%s] = v[M0+%s]\n", "V_MOVRELSD", dst, src0 );
                }
                break;      

            default:
                {
                    uint nDstWidth = pInst->GetResultWidthInDWORDS();
                    uint nSrcWidth = pInst->GetOperandWidthInDWORDS();
                    const char* pOpcode = EnumToString(pInst->GetOpcode());
                    FormatSource( src0, pInst->GetSrc0(),  nSrcWidth, pInst );
                    FormatVReg( dst,    pInst->GetVDst(),  nDstWidth );
                    Printf( printer, UNARY_OP_FORMAT, pOpcode,dst,src0);
                }
                break;
            }

            return NextInstruction(pInst);
        }

        const Instruction* Disassemble( IPrinter& printer, const EXPInstruction* pInst  )
        {
            uint nTarget = pInst->GetTarget();
            uint nRegMask = pInst->GetExportMask();

            char r[4][8];
            sprintf( r[0], "v%u", pInst->GetVSrc0() );
            sprintf( r[1], "v%u", pInst->GetVSrc1() );
            sprintf( r[2], "v%u", pInst->GetVSrc2() );
            sprintf( r[3], "v%u", pInst->GetVSrc3() );

            const char* pOpcode = pInst->GetCompressBit() ? "EXP_FP16" : "EXP";
                
            char flags[256];
            sprintf(flags, "(%s%s%s)", 
                    EnumToString(pInst->GetTarget()),
                    pInst->GetValidMaskBit() ? ",VM" : "",
                    pInst->GetDoneBit() ? ",DONE" : "" );

            Printf( printer, OPCODE_FORMAT " %6s,%4s,%4s,%4s %s\n",
                    pOpcode,
                    nRegMask&1 ? r[0] : "____",
                    nRegMask&2 ? r[1] : "____",
                    nRegMask&4 ? r[2] : "____",
                    nRegMask&8 ? r[3] : "____",
                    flags );


            return NextInstruction(pInst);
        }

        const Instruction* Disassemble( IPrinter& printer, const VINTERPInstruction* pInst  )
        {
            uint nSrc = pInst->GetVSrc0();
            char dst[8];
            sprintf( dst, "v%u", pInst->GetVDst() );

            static const char* CHANNELS[] = { "X","Y","Z","W"};
            static const char* SOURCES[]  = { "P10","P20","P0" };

            char op[256];
            sprintf( op, "%s(a%0u.%s)",
                    EnumToString(pInst->GetOpcode()),
                    pInst->GetAttributeIndex(),
                    CHANNELS[pInst->GetAttributeChannel()] );

            switch(pInst->GetOpcode())
            {
            case V_INTERP_P1_F32: Printf( printer ,OPCODE_FORMAT " %6s =  P10*v%u + P0\n", op, dst, nSrc ); break;
            case V_INTERP_P2_F32: Printf( printer, OPCODE_FORMAT " %6s += P20*v%u\n", op, dst, nSrc ); break;
            case V_INTERP_MOV_F32:Printf( printer, OPCODE_FORMAT " %6s = %s\n", op, dst, SOURCES[nSrc&3] ); break;
            }

            return NextInstruction(pInst);
        }

        const Instruction* Disassemble( IPrinter& printer, const SOP1Instruction* pInst ) 
        {
            char dst[64];
            char src0[64];

            switch( pInst->GetOpcode() )
            {
            case S_MOVRELD_B32:
                {
                    FormatSReg(dst,pInst->GetDest(),1);
                    FormatSource(src0,pInst->GetSrc0(),1,pInst);
                    Printf(printer,OPCODE_FORMAT " s[M0+%s] = s[%s]\n", "S_MOVRELD_B32", dst, src0 );
                }
                break;
            case S_MOVRELD_B64:
                {
                    FormatSReg(dst,pInst->GetDest(),1);
                    FormatSource(src0,pInst->GetSrc0(),1,pInst);
                    Printf(printer,OPCODE_FORMAT " s[M0+%s][2] = s[%s][2]\n", "S_MOVRELD_B64", dst, src0 );
                }
                break;
            case S_MOVRELS_B32:
                {
                    FormatSReg(dst,pInst->GetDest(),1);
                    FormatSource(src0,pInst->GetSrc0(),1,pInst);
                    Printf(printer,OPCODE_FORMAT " s[%s] = s[M0+%s]\n", "S_MOVRELS_B32", dst, src0 );
                }
                break;
            case S_MOVRELS_B64:
                {
                    FormatSReg(dst,pInst->GetDest(),1);
                    FormatSource(src0,pInst->GetSrc0(),1,pInst);
                    Printf(printer,OPCODE_FORMAT " s[%s][2] = s[M0+%s][2]\n", "S_MOVRELS_B64", dst, src0 );
                }
                break;
           
            default:
                {
                    uint nDstWidth = pInst->GetResultWidthInDWORDS();
                    uint nSrcWidth = pInst->GetOperandWidthInDWORDS();
                    const char* pOpcode = EnumToString(pInst->GetOpcode());
                    FormatSource( src0, pInst->GetSrc0(),  nSrcWidth, pInst );
                    FormatDest( dst,    pInst->GetDest(),  nDstWidth );
                    Printf( printer, UNARY_OP_FORMAT, pOpcode,dst,src0);
                }
                break;
            }

            return NextInstruction(pInst);
        }

        
        const Instruction* Disassemble( IPrinter& printer, const SOPKInstruction* pInst, const std::vector<const Instruction*>* pLabels ) 
        {
            switch( pInst->GetOpcode() )
            {
            default:
            case S_MOVK_I32          :
            case S_CMOVK_I32         :
            case S_CMPK_EQ_I32       :
            case S_CMPK_LG_I32       :
            case S_CMPK_GT_I32       :
            case S_CMPK_GE_I32       :
            case S_CMPK_LT_I32       :
            case S_CMPK_LE_I32       :
            case S_CMPK_EQ_U32       :
            case S_CMPK_LG_U32       :
            case S_CMPK_GT_U32       :
            case S_CMPK_GE_U32       :
            case S_CMPK_LT_U32       :
            case S_CMPK_LE_U32       :
            case S_ADDK_I32          :
            case S_MULK_I32          :
                {
                    char dst[64];
                    FormatDest(dst,pInst->GetDest(),1);
                    Printf( printer, OPCODE_FORMAT " %6s,%6d\n", EnumToString(pInst->GetOpcode()), dst, pInst->ReadSIMM16());
                }
                break;

            case S_CBRANCH_I_FORK:
                PrintLabeledBranch(printer,pInst,pLabels);
                break;

            case S_GETREG_B32:
            case S_SETREG_B32:
            case S_SETREG_IMM32_B32:
                {
                    char dst[64];
                    FormatDest(dst,pInst->GetDest(),1);
                    uint nSize  = pInst->ReadSIMMBits(10,6);
                    uint nStart = pInst->ReadSIMMBits(15,11);
                    uint nEnd   = nSize+nStart+1;
                    uint nReg   = pInst->ReadSIMMBits(5,0);

                    char HWReg[128];
                    sprintf( HWReg, "HWREG%u(%u:%u)", nReg,nEnd,nStart );

                    switch( pInst->GetOpcode() )
                    {
                    case S_GETREG_B32:
                        Printf(printer, OPCODE_FORMAT " %s = %s\n", EnumToString(pInst->GetOpcode()), dst, HWReg );
                        break;

                    case S_SETREG_B32:
                        Printf(printer, OPCODE_FORMAT " %s = %s\n", EnumToString(pInst->GetOpcode()), HWReg, dst);
                        break;

                    case S_SETREG_IMM32_B32:
                        Printf(printer, OPCODE_FORMAT " %s = %x\n", EnumToString(pInst->GetOpcode()), 
                               HWReg, 
                               pInst->ReadIMM32() & ((1<<nSize)-1));
                        break;
                    }

                    
                }
                break;
            }

            return NextInstruction(pInst);
        }


        const Instruction* Disassemble( IPrinter& printer, const SMEMInstruction* pInst )
        {
            switch( pInst->GetOpcode() )
            {
            case S_LOAD_DWORD          : 
            case S_LOAD_DWORDX2        : 
            case S_LOAD_DWORDX4        : 
            case S_LOAD_DWORDX8        : 
            case S_LOAD_DWORDX16       : 
            case S_BUFFER_LOAD_DWORD   : 
            case S_BUFFER_LOAD_DWORDX2 : 
            case S_BUFFER_LOAD_DWORDX4 : 
            case S_BUFFER_LOAD_DWORDX8 : 
            case S_BUFFER_LOAD_DWORDX16: 
                {
                    char base[64];
                    char dest[64];
                    char offs[64];
                    uint nBaseReg = pInst->GetBase()<<1;
                    FormatDest( dest, pInst->GetDest(), pInst->GetResultWidthInDWORDs() );
                    FormatSReg( base, nBaseReg, 2 );

                    uint nOffset = pInst->GetOffset();
                    if( pInst->IsOffsetIMM() )
                        sprintf( offs, "%u", nOffset*4 );
                    else
                        FormatSReg( offs, nOffset, 1 );

                    Printf( printer, OPCODE_FORMAT " %6s, %6s, %6s\n", EnumToString(pInst->GetOpcode()), dest, base, offs );
                }
                break;
            case S_MEMTIME             : 
                {
                    char dst[64];
                    FormatDest(dst,pInst->GetDest(), 2 );
                    Printf( printer, OPCODE_FORMAT " %6s\n", EnumToString(pInst->GetOpcode()),dst);
                }
                break;

            case S_DCACHE_INV_VOL      : 
            case S_DCACHE_INV          :   
                Printf( printer, OPCODE_FORMAT "\n", EnumToString(pInst->GetOpcode()));
                break;
            default:
                Printf( printer, OPCODE_FORMAT "\n", "???SMEM???");
                break;
            }

           
            return NextInstruction(pInst);
        }

        const Instruction* Disassemble( IPrinter& printer, const VOPCInstruction* pInst )
        {
            char s0[64];
            char s1[64];
            FormatSource(s0,pInst->GetSrc0(),pInst->GetOperandWidthInDWORDs(), pInst );
            FormatVReg(s1,pInst->GetVSrc1(), pInst->GetOperandWidthInDWORDs() );
            Printf(printer,UNARY_OP_FORMAT, EnumToString(pInst->GetOpcode()),s0,s1);
            return NextInstruction(pInst);
        }

        void FormatVOP3Source( char* buff, uint nDWORDS, uint nSrc, bool abs, bool negate )
        {
            char tmp0[64];
            char tmp1[64];
            FormatGenericSource( tmp0, nSrc, nDWORDS );
            if( abs )
            {
                sprintf( tmp1, "|%s|", tmp0 );
                strcpy( tmp0, tmp1 );
            }
            if( negate )
            {
                sprintf( tmp1, "-%s", tmp0 );
                strcpy(tmp0,tmp1 );
            }
            strcpy(buff,tmp0);
        }

        const Instruction* Disassemble( IPrinter& printer, const VOP3Instruction* pInst )
        {
            
            char src0[64];
            char dst[64];
                
            switch( pInst->GetOpcode() )
            {
            case V3_NOP:     Printf( printer, OPCODE_FORMAT"\n", "V_NOP"    ); break; 
            case V3_CLREXCP: Printf( printer, OPCODE_FORMAT"\n", "V_CLREXCP"); break;
            case V3_MOVRELD_B32:
                {
                    FormatVReg(dst,pInst->GetVDst(),1);
                    FormatSource(src0,pInst->GetSrc0(),1,pInst);
                    Printf(printer,OPCODE_FORMAT " v[M0+%s] = v[%s]\n", "V_MOVRELD", dst, src0 );
                }
                break;
            case V3_MOVRELS_B32:
                {
                    FormatVReg(dst,pInst->GetVDst(),1);
                    FormatSource(src0,pInst->GetSrc0(),1,pInst);
                    Printf(printer,OPCODE_FORMAT " v[%s] = v[M0+%s]\n", "V_MOVRELS", dst, src0 );
                }
                break;
            case V3_MOVRELSD_B32:
                {
                    FormatVReg(dst,pInst->GetVDst(),1);
                    FormatSource(src0,pInst->GetSrc0(),1,pInst);
                    Printf(printer,OPCODE_FORMAT " v[M0+%s] = v[M0+%s]\n", "V_MOVRELSD", dst, src0 );
                }
                break;  

            case V3_MAD_I64_I32:
            case V3_MAD_U64_U32:
                {
                    // These are unusual in that the operands have different widths
                    //   V3_MAD_U64_U32     = 374 ,   // : {vcc_out,D.u64} = S0.u32 * S1.u32 + S2.u64.
                    //   V3_MAD_I64_I32     = 375 ,   // : {vcc_out,D.i64} = S0.i32 * S1.i32 + S2.i64.

                    char vdst[64];
                    char src0[64];
                    char src1[64];
                    char src2[64];
                    char omod[64];

                    uint abs    = pInst->GetSourceAbsMask() & ((pInst->IsVOP3bOp()) ? 0 : 0x7);
                    uint negate = pInst->GetSourceNegateMask();
            
                    FormatVReg(vdst,pInst->GetVDst(), 2 );
                    FormatVOP3Source( src0, 1, pInst->GetSrc0(), (abs & 1) != 0, (negate & 1) != 0 );
                    FormatVOP3Source( src1, 1, pInst->GetSrc1(), (abs & 2) != 0, (negate & 2) != 0 );
                    FormatVOP3Source( src2, 2, pInst->GetSrc2(), (abs & 4) != 0, (negate & 4) != 0 );

                    // TODO: DXX doesn't seem to use 'omod' and I can't tell what the values are supposed to map to
                    if( pInst->GetOMod() )
                        sprintf( omod, "OMOD(%u)", pInst->GetOMod());
                    else
                        sprintf( omod, "" );

                    bool clamp  = pInst->GetClamp();
                    Printf( printer, OPCODE_FORMAT " %6s,%6s,%6s,%6s   %s %s\n",
                            EnumToString(pInst->GetOpcode()),
                            vdst, src0,src1,src2,clamp?"CLAMP":"",omod );
                }
                break;

            default:
                {
                    char vdst[64];
                    char src0[64];
                    char src1[64];
                    char src2[64];
                    char omod[64];

                    uint abs    = pInst->GetSourceAbsMask() & ((pInst->IsVOP3bOp()) ? 0 : 0x7);
                    uint negate = pInst->GetSourceNegateMask();
            
                    uint nDWORDS = pInst->GetOperandSizeInDWORDS();

                    // compares are weird.  
                    //  according to chapter 6 of the isa doc, they are encoded as vop3b
                    //  according to chapter 12 (microcode formats), they are vop3a and the vdst field
                    //    is actually an sgpr/vcc
                    //  AMD's disassembler seems to believe the second interprettation, so we'll go with that...
                    // 
                    if( !pInst->IsCompare() )
                        FormatVReg(vdst,pInst->GetVDst(), pInst->GetResultSizeInDWORDS() );
                    else
                        FormatGenericSource(vdst,pInst->GetVDst(), 1 );

                    FormatVOP3Source( src0, nDWORDS, pInst->GetSrc0(), (abs & 1) != 0, (negate & 1) != 0 );
                    FormatVOP3Source( src1, nDWORDS, pInst->GetSrc1(), (abs & 2) != 0, (negate & 2) != 0 );
                    FormatVOP3Source( src2, nDWORDS, pInst->GetSrc2(), (abs & 4) != 0, (negate & 4) != 0 );

                    // TODO: DXX doesn't seem to use 'omod' and I can't tell what the values are supposed to map to
                    if( pInst->GetOMod() )
                        sprintf( omod, "OMOD(%u)", pInst->GetOMod());
                    else
                        sprintf( omod, "" );

                    if( pInst->IsVOP3bOp() )
                    {
                        // VOPC opcodes, or a few other oddballs
                        char sdst[64];
                        char out[64];
                        sprintf( out, "(%s,%s)", vdst, sdst );
                        FormatDest( sdst, pInst->GetSDst(), 1 );
                        Printf( printer, OPCODE_FORMAT " %10s,%6s,%6s,%6s  %s\n",
                                EnumToString(pInst->GetOpcode()),
                                out, src0,src1,src2,omod );
                    }
                    else
                    {
                        // everything else. 
                        bool clamp  = pInst->GetClamp();
                        Printf( printer, OPCODE_FORMAT " %6s,%6s,%6s,%6s   %s %s\n",
                                EnumToString(pInst->GetOpcode()),
                                vdst, src0,src1,src2,clamp?"CLAMP":"",omod );
                    }
                }
                break;
            }

            return NextInstruction(pInst);
        }

        void addprintf( char* buff, const char* Format, ... )
        {
            va_list vl;
            va_start(vl,Format);
            size_t n = strlen(buff);
            if( n )
            {
                strcat( buff, "+");
                vsprintf( buff+n+1, Format, vl );
            }
            else
            {
                vsprintf( buff, Format, vl );
            }

            va_end(vl);
        }

        
        void catprintf( char* buff, const char* Format, ... )
        {
            va_list vl;
            va_start(vl,Format);
            
            vsprintf( buff+strlen(buff), Format, vl );
           
            va_end(vl);
        }


        template< class BUFFInstruction >
        void FormatBuffAddress( char* offset, const BUFFInstruction* pInst )
        {
            if( pInst->IsAddr64() )
            {
                // address is:  resource_base + soffset+ioffset + vgpr[2]
                if( pInst->GetSOffset() != C_ZERO )
                    FormatGenericSource(offset,pInst->GetSOffset(), 1 );

                if( pInst->GetOffset() )
                    addprintf( offset, "%u", pInst->GetOffset() );
                
                // send voffset[2] as full address
                char reg[64];
                FormatVReg( reg, pInst->GetVAddr(), 2 );
                addprintf(offset, "%s", reg );
            }
            else
            {
                // address is: resource_base + soffset+(ioffset or vgpr) + resource_stride*(vgpr_idx)

                // byte offset
                if( pInst->GetSOffset() != C_ZERO )
                    FormatGenericSource(offset,pInst->GetSOffset(), 1 );
                
                if( !pInst->IsOffN() )
                {
                    // send literal offset from instruction
                    if( pInst->GetOffset() )
                        addprintf( offset, "%u", pInst->GetOffset() );  
                }
                else
                {                    
                    // send vaddr gpr as offset.  If both index and offset are used, offset is the second one
                    uint nOffsetReg = pInst->GetVAddr();
                    if( pInst->IsIdxN() )
                        nOffsetReg++;

                    char reg[64];
                    FormatVReg( reg, nOffsetReg, 1 );
                    addprintf( offset, "%s", reg );
                }

                if( pInst->IsIdxN() )
                {
                    char idx[64];
                    FormatVReg( idx, pInst->GetVAddr(), 1 );      // vaddr as an index
                    addprintf( offset, "%s*STRIDE", idx );
                }
            }
        }

        
        template< class BUFFInstruction >
        void FormatBuffFlags( char* flags, const BUFFInstruction* pInst )
        {
            if( pInst->GetTFEBit() )
                addprintf(flags,"TFE");
            if( pInst->GetGLCBit() )
                addprintf(flags,"GLC");
            if( pInst->GetSLCBit() )
                addprintf(flags,"SLC");
        }
        
        const Instruction* Disassemble( IPrinter& printer, const MUBUFFInstruction* pInst )
        {
            uint nResultWidth = pInst->GetResultWidthInDWORDS();
            if( !nResultWidth )
            {
                // oddballs like cache invalidation
                Printf( printer, OPCODE_FORMAT"\n", EnumToString(pInst->GetOpcode()) );
            }
            else
            {
                // destination
                char dst[64];
                if( pInst->IsDirectToLDS())
                    sprintf(dst,"LDS[M0]");
                else
                    FormatVReg( dst, pInst->GetVData(), nResultWidth );
            
                // resource constant
                char rsrc[64]="";
                FormatSReg(rsrc, pInst->GetSResource()*4, 4 );

                // address
                char address[256]="";
                FormatBuffAddress(address,pInst);

                // flags
                char flags[256]="";
                FormatBuffFlags(flags,pInst);

                Printf( printer, OPCODE_FORMAT " %6s, %8s [%s] %s\n",
                        EnumToString(pInst->GetOpcode()), dst, rsrc,address,flags);
            }
             
            return NextInstruction(pInst);
        }


               
        const Instruction* Disassemble( IPrinter& printer, const MTBUFFInstruction* pInst )
        {
            uint nResultWidth = pInst->GetResultWidthInDWORDS();
            if( !nResultWidth )
            {
                // oddballs like cache invalidation
                Printf( printer, OPCODE_FORMAT"\n", EnumToString(pInst->GetOpcode()) );
            }
            else
            {
                // destination
                char dst[64];
                FormatVReg( dst, pInst->GetVData(), nResultWidth );
            
                // resource constant
                char rsrc[64]="";
                FormatSReg(rsrc, pInst->GetSResource()*4, 4 );

                // address
                char address[256]="";
                FormatBuffAddress(address,pInst);

                // format
                char fmt[256];
                sprintf( fmt, "%s_%s", EnumToString(pInst->GetDataFormat()), EnumToString(pInst->GetNumberFormat()) );

                // flags
                char flags[256]="";
                FormatBuffFlags(flags,pInst);

                Printf( printer, OPCODE_FORMAT " %6s, %8s [%s] %s %s\n",
                        EnumToString(pInst->GetOpcode()), dst, rsrc,address,fmt, flags);
            }
             
            return NextInstruction(pInst);
        }


        const Instruction* Disassemble( IPrinter& printer, const MIMGInstruction* pInst )
        {
            char sampler[64]  = "";
            char resource[64] = "";
            uint nResourceWidth  = pInst->GetResourceWidthInDWORDS();
            uint nSamplerWidth   = pInst->GetSamplerWidthInDWORDS();
            if( nSamplerWidth )
                FormatSReg( sampler, pInst->GetSSampler()*4, nSamplerWidth );
            if( nResourceWidth )
                FormatSReg( resource, pInst->GetSResource()*4, nResourceWidth );

            char dmask[64]="";
            uint nDMask = pInst->GetDMask();
            if( nDMask & 1 ) catprintf(dmask, "R");
            if( nDMask & 2 ) catprintf(dmask, "G");
            if( nDMask & 4 ) catprintf(dmask, "B");
            if( nDMask & 8 ) catprintf(dmask, "A");

            char flags[256] = "";
            if( pInst->IsGLC() ) addprintf(flags,"GLC");
            if( pInst->IsSLC() ) addprintf(flags,"SLC");
            if( pInst->IsLWE() ) addprintf(flags,"LWE");
            if( pInst->IsTFE() ) addprintf(flags,"TFE");
            if( pInst->IsArray() ) addprintf(flags,"ARRAY");
            if( pInst->IsUnormalized() ) addprintf( flags,"UNNORM");

            // NOTE: There is no way to infer the address width, because
            //      it depends on the resource dimension, which in turn depends on the descriptor
            //
            char vaddr[64]="";
            char vdata[64]="";
            FormatVReg(vaddr,pInst->GetVAddr(),1); 
            FormatVReg(vdata,pInst->GetVData(),pInst->GetResultWidthInDWORDS());
            
            char opcode[512];
            sprintf( opcode, "%s(%s)", EnumToString(pInst->GetOpcode()),dmask);

            Printf(printer,OPCODE_FORMAT " %6s,%6s,    T:%6s", opcode,vdata,vaddr,resource);
            
            if( nSamplerWidth )
                Printf(printer,",    S:%6s",sampler);

            Printf(printer,"    %s\n", flags);

            return NextInstruction(pInst);
        }



      
        static void FormatDSAddress( char* pBuff, uint nReg, uint nOffset )
        {
            char A[64];
            FormatVReg(A,nReg,1);
            if( nOffset )
                sprintf(pBuff,"DS[%s+%u]", A,nOffset );
            else
                sprintf(pBuff,"DS[%s]", A );

        }
       

        const Instruction* Disassemble( IPrinter& printer, const DSInstruction* pInst ) 
        {
            char opcode[256];
            sprintf(opcode, "%s%s", EnumToString(pInst->GetOpcode()), pInst->IsGDS() ? "(GDS)":"");

            switch( pInst->GetOpcode() )
            {
            // oddballs
            default:     
            case DS_NOP:
                Printf( printer, OPCODE_FORMAT"\n", opcode ); 
                break;

            case DS_SWIZZLE_B32         :   //  R = swizzle(Data(vgpr), offset1:offset0). Dword swizzle. no data is written to LDS. see ds_opcodes.docx for details.
                {
                    char R[64];
                    char D[64];
                    FormatVReg(R,pInst->GetVData0(), pInst->GetVData1());
                    FormatVReg(D,pInst->GetVDest(), 1 );
                    Printf( printer, OPCODE_FORMAT" %6s,%6s,   0x%04x\n", opcode, R, D, pInst->GetOffset16() );
                }
                break;

            case DS_GWS_SEMA_RELEASE_ALL:   //  GDS Only. Release all wavefronts waiting on this semaphore. ResourceID is in offset[4:0]. SEA ISLANDS ONLY
            case DS_GWS_INIT            :   //  GDS only.
            case DS_GWS_SEMA_V          :   //  GDS only.
            case DS_GWS_SEMA_BR         :   //  GDS only.
            case DS_GWS_SEMA_P          :   //  GDS only.
            case DS_GWS_BARRIER         :   //  GDS only.
                {
                    uint nSemaphoreID = pInst->GetOffset16()& (0x1f);
                    Printf( printer, OPCODE_FORMAT" %u\n", EnumToString(pInst->GetOpcode()),nSemaphoreID);
                }
                break;

            // 1 address, 1 data
            case DS_ADD_U32             :  //  DS[A] = DS[A] + D0; uint add.
            case DS_SUB_U32             :  //  DS[A] = DS[A] - D0; uint subtract.
            case DS_RSUB_U32            :  //  DS[A] = D0 - DS[A]; uint reverse subtract.
            case DS_INC_U32             :  //  DS[A] = (DS[A] >= D0 ? 0 : DS[A] + 1); uint increment.
            case DS_DEC_U32             :  //  DS[A] = (DS[A] == 0 || DS[A] > D0 ? D0 : DS[A] - 1); uint decrement.
            case DS_MIN_I32             :  //  DS[A] = min(DS[A], D0); int min.
            case DS_MAX_I32             :  //  DS[A] = max(DS[A], D0); int max.
            case DS_MIN_U32             :  //  DS[A] = min(DS[A], D0); uint min.
            case DS_MAX_U32             :  //  DS[A] = max(DS[A], D0); uint max.
            case DS_AND_B32             :  //  DS[A] = DS[A] & D0; Dword AND.
            case DS_OR_B32              :  //  DS[A] = DS[A] | D0; Dword OR.
            case DS_XOR_B32             :  //  DS[A] = DS[A] ^ D0; Dword XOR.
            case DS_MSKOR_B32           :  //  DS[A] = (DS[A] ^ ~D0) | D1; masked Dword OR.
            case DS_WRITE_B32           :  //  DS[A] = D0; write a Dword.
            case DS_WRITE_B8            :  //  DS[A] = D0[7:0]; byte write.
            case DS_WRITE_B16           :  //  DS[A] = D0[15:0]; short write.
            case DS_ADD_U64             :  //  Uint add.
            case DS_SUB_U64             :  //  Uint subtract.
            case DS_RSUB_U64            :  //  Uint reverse subtract.
            case DS_INC_U64             :  //  Uint increment.
            case DS_DEC_U64             :  //  Uint decrement.
            case DS_MIN_I64             :  //  Int min.
            case DS_MAX_I64             :  //  Int max.
            case DS_MIN_U64             :  //  Uint min.
            case DS_MAX_U64             :  //  Uint max.
            case DS_AND_B64             :  //  Dword AND.
            case DS_OR_B64              :  //  Dword OR.
            case DS_XOR_B64             :  //  Dword XOR.
            case DS_MSKOR_B64           :  //  Masked Dword XOR.
            case DS_WRITE_B64           :  //  Write.
            case DS_MIN_F64             :  //  DS[A] = (D0 < DS[A]) ? D0 : DS[A]; float, handles NaN/INF/denorm.
            case DS_MAX_F64             :  //  DS[A] = (D0 > DS[A]) ? D0 : DS[A]; float, handles NaN/INF/denorm.
            case DS_MAX_F32             :  //  DS[A] = (D0 > DS[A]) ? D0 : DS[A]; float, handles NaN/INF/denorm.
            case DS_WRITE_B96           :   //{DS[A+2], DS[A+1], DS[A]} = D0[95:0]; tri-dword write.
            case DS_WRITE_B128          :   //{DS[A+3], DS[A+2], DS[A+1], DS[A]} = D0[127:0]; qword write.
                {
                    char D[64];
                    FormatVReg(D,pInst->GetVData0(),pInst->GetDataWidthInDWORDS() );

                    char A[64];
                    FormatDSAddress(A,pInst->GetVAddr(), pInst->GetOffset16());

                    Printf(printer,OPCODE_FORMAT" %6s, %s\n", opcode, D, A );
                }
                break;

                // address + result
            case DS_READ_B32            :  //  R = DS[A]; Dword read.
            case DS_READ_I8             :  //  R = signext(DS[A][7:0]}; signed byte read.
            case DS_READ_U8             :  //  R = {24’h0,DS[A][7:0]}; unsigned byte read.
            case DS_READ_I16            :  //  R = signext(DS[A][15:0]}; signed short read.
            case DS_READ_U16            :  //  R = {16’h0,DS[A][15:0]}; unsigned short read.
            case DS_READ_B64            :  // Dword read.
            case DS_READ_B96            :   //Tri-Dword read.
            case DS_READ_B128           :   //Qword read.
                {
                    char Dst[64];
                    FormatVReg(Dst,pInst->GetVDest(),pInst->GetDataWidthInDWORDS() );

                    char A[64];
                    FormatDSAddress(A,pInst->GetVAddr(), pInst->GetOffset16());

                    Printf(printer,OPCODE_FORMAT" %6s, %s\n", opcode, Dst, A );
                }
                break;


            // 1 address, 1 data, 1 return (return atomics, cas)
            // NOTE: Some of these I'm not sure about, but am erring on the side of caution
            //   Docs are not very explicit about some of these...
            case DS_MIN_F32             :  //  DS[A] = (DS[A] < D1) ? D0 : DS[A]; float compare swap (handles NaN/INF/denorm).  NOTE: Doc says this uses D1 and D0.  Typo??
            case DS_CMPST_B32           :  //  DS[A] = (DS[A] == D0 ? D1 : DS[A]); compare store.
            case DS_CMPST_F32           :  //  DS[A] = (DS[A] == D0 ? D1 : DS[A]); compare store with float rules.
            case DS_ADD_RTN_U32         :  //  Uint add.
            case DS_SUB_RTN_U32         :  //  Uint subtract.
            case DS_RSUB_RTN_U32        :  //  Uint reverse subtract.
            case DS_INC_RTN_U32         :  //  Uint increment.
            case DS_DEC_RTN_U32         :  //  Uint decrement.
            case DS_MIN_RTN_I32         :  //  Int min.
            case DS_MAX_RTN_I32         :  //  Int max.
            case DS_MIN_RTN_U32         :  //  Uint min.
            case DS_MAX_RTN_U32         :  //  Uint max.
            case DS_AND_RTN_B32         :  //  Dword AND.
            case DS_OR_RTN_B32          :  //  Dword OR.
            case DS_XOR_RTN_B32         :  //  Dword XOR.
            case DS_MSKOR_RTN_B32       :  //  Masked Dword OR.
            case DS_MIN_RTN_F32         :  //  DS[A] = (DS[A] < D1) ? D0 : DS[A]; float compare swap (handles NaN/INF/denorm).
            case DS_MAX_RTN_F32         :  //  DS[A] = (D0 > DS[A]) ? D0 : DS[A]; float, handles NaN/INF/denorm .
            case DS_WRAP_RTN_B32        :  //  DS[A] = (DS[A] >= D0) ? DS[A] - D0 : DS[A] + D1. SEA ISLANDS ONLY
            case DS_CMPST_B64           :  //  Compare store.
            case DS_CMPST_F64           :  //  Compare store with float rules.
            case DS_ADD_RTN_U64         :  //  Uint add.
            case DS_SUB_RTN_U64         :  //  Uint subtract.
            case DS_RSUB_RTN_U64        :  //  Uint reverse subtract.
            case DS_INC_RTN_U64         :  //  Uint increment.
            case DS_DEC_RTN_U64         :  // Uint decrement.
            case DS_MIN_RTN_I64         :  // Int min.
            case DS_MAX_RTN_I64         :  // Int max.
            case DS_MIN_RTN_U64         :  // Uint min.
            case DS_MAX_RTN_U64         :  // Uint max.
            case DS_AND_RTN_B64         :  // Dword AND.
            case DS_OR_RTN_B64          :  // Dword OR.
            case DS_XOR_RTN_B64         :  // Dword XOR.
            case DS_MSKOR_RTN_B64       :  // Masked Dword XOR.
            case DS_CMPST_RTN_B64       :  // Compare store.
            case DS_CMPST_RTN_F64       :  // Compare store with float rules.
            case DS_MIN_RTN_F64         :  // DS[A] = (D0 < DS[A]) ? D0 : DS[A]; float, handles NaN/INF/denorm.
            case DS_MAX_RTN_F64         :  // DS[A] = (D0 > DS[A]) ? D0 : DS[A]; float, handles NaN/INF/denorm.
            case DS_WRXCHG_RTN_B32      :  //  Write exchange. Offset = {offset1,offset0}. A = ADDR+offset. D=DS[Addr]. DS[Addr]=D0.
            case DS_WRXCHG_RTN_B64      :  //  Write exchange.
            case DS_CONSUME             :  //  Consume entries from a buffer.
            case DS_APPEND              :  //  Append one or more entries to a buffer.
            case DS_ORDERED_COUNT       :  //  Increment an append counter. Operation is done in order of wavefront creation.
                {
                    char Dst[64];
                    char Arg[64];
                    FormatVReg(Dst,pInst->GetVDest(),pInst->GetDataWidthInDWORDS() );
                    FormatVReg(Arg,pInst->GetVData0(),pInst->GetDataWidthInDWORDS() );
                    
                    char A[64];
                    FormatDSAddress(A,pInst->GetVAddr(),pInst->GetOffset16());

                    Printf(printer,OPCODE_FORMAT" %6s,%6s, %s\n", opcode, Dst,Arg, A );
                }
                break;

                // 1 address, 2 data, 1 return
            case DS_CMPST_RTN_B32       :  //  Compare store.
            case DS_CMPST_RTN_F32       :  //  Compare store with float rules.
            case DS_CONDXCHG32_RTN_B128 :  //  Conditional write exchange.
            case DS_CONDXCHG32_RTN_B64  :  //  Conditional write exchange. SEA ISLANDS ONLY
                {
                    char Dst[64];
                    char Arg0[64];
                    char Arg1[64];
                    FormatVReg(Dst,pInst->GetVDest(),pInst->GetDataWidthInDWORDS() );
                    FormatVReg(Arg0,pInst->GetVData0(),pInst->GetDataWidthInDWORDS() );
                    FormatVReg(Arg1,pInst->GetVData0(),1);
                    
                    char A[64];
                    FormatDSAddress(A,pInst->GetVAddr(),pInst->GetOffset16());

                    Printf(printer,OPCODE_FORMAT" %6s,%6s,%6s %s\n", opcode, Dst,Arg0, Arg1, A );
                }
                break;

            // 2 address, 2 data (2x write)
            case DS_WRITE2_B32          :   //  DS[ADDR+offset0*4] = D0;DS[ADDR+offset1*4] = D1; write 2 Dwords.
            case DS_WRITE2ST64_B32      :   //  DS[ADDR+offset0*4*64] = D0;DS[ADDR+offset1*4*64] = D1; write 2 Dwords.
            case DS_WRITE2_B64          :   //  DS[ADDR+offset0*8] = D0;DS[ADDR+offset1*8] = D1; write 2 Dwords.
            case DS_WRITE2ST64_B64      :   //  DS[ADDR+offset0*8*64] = D0; DS[ADDR+offset1*8*64] = D1; write 2 Dwords.
            case DS_WRXCHG2_RTN_B64     :   //  Write exchange 2 dwords.
            case DS_WRXCHG2_RTN_B32     :   //  Write exchange 2 separate Dwords.
                {
                    uint nAddressMultiplier=0;                    
                    switch( pInst->GetOpcode() )
                    {
                    case DS_WRXCHG2ST64_RTN_B64 : nAddressMultiplier=64;   break;
                    case DS_WRXCHG2ST64_RTN_B32 : nAddressMultiplier=64;   break;
                    case DS_WRITE2_B32          : nAddressMultiplier=4;    break;
                    case DS_WRITE2ST64_B32      : nAddressMultiplier=4*64; break;
                    case DS_WRITE2_B64          : nAddressMultiplier=8;    break;
                    case DS_WRITE2ST64_B64      : nAddressMultiplier=8*64; break;
                    }

                    uint nDataWidth = pInst->GetDataWidthInDWORDS();
                    
                    char D0[64];
                    char D1[64];
                    FormatVReg( D0, pInst->GetVData0(), nDataWidth );
                    FormatVReg( D1, pInst->GetVData1(), nDataWidth );

                    char A0[64];
                    char A1[64];
                    uint nOffs0 = pInst->GetOffset0()*nAddressMultiplier;
                    uint nOffs1 = pInst->GetOffset1()*nAddressMultiplier;
                    FormatDSAddress(A0,pInst->GetVAddr(),nOffs0);
                    FormatDSAddress(A1,pInst->GetVAddr(),nOffs1);

                    Printf(printer,OPCODE_FORMAT" %6s,%6s, %s  %s\n", opcode, D0,D1, A0,A1 );
                }
                break;

            // 2 address, 1 data (read2)
            case DS_READ2_B32           :   // R = DS[ADDR+offset0*4], R+1 = DS[ADDR+offset1*4]. Read 2 Dwords.
            case DS_READ2ST64_B32       :   // R = DS[ADDR+offset0*4*64], R+1 = DS[ADDR+offset1*4*64]. Read 2 Dwords.
            case DS_READ2_B64           :   // R = DS[ADDR+offset0*8], R+1 = DS[ADDR+offset1*8]. Read 2 Dwords
            case DS_READ2ST64_B64       :   // R = DS[ADDR+offset0*8*64], R+1 = DS[ADDR+offset1*8*64]. Read 2 Dwords.
                {
                    uint nAddressMultiplier=0;
                    switch( pInst->GetOpcode() )
                    {
                    case DS_READ2_B32      : nAddressMultiplier=4;    break;
                    case DS_READ2ST64_B32  : nAddressMultiplier=4*64; break;
                    case DS_READ2_B64      : nAddressMultiplier=8;    break;
                    case DS_READ2ST64_B64  : nAddressMultiplier=8*64; break;
                    }

                    uint nDataWidth =  pInst->GetDataWidthInDWORDS();
                    

                    char Dst[64];
                    FormatVReg(Dst,pInst->GetVDest(),nDataWidth );

                    char A0[64];
                    char A1[64];
                    uint nOffs0 = pInst->GetOffset0()*nAddressMultiplier;
                    uint nOffs1 = pInst->GetOffset1()*nAddressMultiplier;
                    FormatDSAddress(A0,pInst->GetVAddr(),nOffs0);
                    FormatDSAddress(A1,pInst->GetVAddr(),nOffs1);
                    Printf(printer,OPCODE_FORMAT" %6s, %s  %s\n", opcode, Dst, A0,A1 );
                
                }
                break;

            // weird stuff 
            case DS_ADD_SRC2_U32       :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} : {offset1[6],offset1[6:0],offset0}). DS[A] = DS[A] + DS[B]; uint add.
            case DS_SUB_SRC2_U32       :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} : {offset1[6],offset1[6:0],offset0}). DS[A] = DS[A] - DS[B]; uint subtract.
            case DS_RSUB_SRC2_U32      :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} : {offset1[6],offset1[6:0],offset0}). DS[A] = DS[B]- DS[A]; uint reverse subtract.
            case DS_INC_SRC2_U32       :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] = (DS[A] >= DS[B] ? 0 : DS[A] + 1); uint increment.
            case DS_DEC_SRC2_U32       :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] = (DS[A]== 0 || DS[A] > DS[B] ? DS[B] : DS[A] - 1); uintdecrement.
            case DS_MIN_SRC2_I32       :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] =min(DS[A], DS[B]); int min.
            case DS_MAX_SRC2_I32       :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} : {offset1[6],offset1[6:0],offset0}). DS[A] =max(DS[A], DS[B]); int max.
            case DS_MIN_SRC2_U32       :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] = min(DS[A], DS[B]); uint min.
            case DS_MAX_SRC2_U32       :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} : {offset1[6],offset1[6:0],offset0}). DS[A] =max(DS[A], DS[B]); uint maxw
            case DS_AND_SRC2_B32       :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} : {offset1[6],offset1[6:0],offset0}). DS[A] = DS[A]& DS[B]; Dword AND.
            case DS_OR_SRC2_B32        :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :  {offset1[6],offset1[6:0],offset0}). DS[A] = DS[A] |DS[B]; Dword OR.
            case DS_XOR_SRC2_B32       :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] = DS[A] ^DS[B]; Dword XOR.
            case DS_WRITE_SRC2_B32     :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] = DS[B];write Dword.
            case DS_MIN_SRC2_F32       :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] = (DS[B] < DS[A]) ? DS[B] : DS[A]; float, handles NaN/INF/denorm.
            case DS_MAX_SRC2_F32       :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} : {offset1[6],offset1[6:0],offset0}). DS[A] = (DS[B] > DS[A]) ? DS[B] : DS[A]; float, handles NaN/INF/denorm.
            case DS_ADD_SRC2_U64       :   // Uint add.
            case DS_SUB_SRC2_U64       :   // Uint subtract.
            case DS_RSUB_SRC2_U64      :   // Uint reverse subtract.
            case DS_INC_SRC2_U64       :   // Uint increment.
            case DS_DEC_SRC2_U64       :   // Uint decrement.
            case DS_MIN_SRC2_I64       :   // Int min.
            case DS_MAX_SRC2_I64       :   // Int max.
            case DS_MIN_SRC2_U64       :   // Uint min.
            case DS_MAX_SRC2_U64       :   // Uint max.
            case DS_AND_SRC2_B64       :   // Dword AND.
            case DS_OR_SRC2_B64        :   // Dword OR.
            case DS_XOR_SRC2_B64       :   // Dword XOR.
            case DS_WRITE_SRC2_B64     :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). DS[A] = DS[B]; write Qword.
            case DS_MIN_SRC2_F64       :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). [A] = (D0 < DS[A]) ? D0 : DS[A]; float, handlesNaN/INF/denorm.
            case DS_MAX_SRC2_F64       :   // B = A + 4*(offset1[7] ? {A[31],A[31:17]} :{offset1[6],offset1[6:0],offset0}). [A] = (D0 > DS[A]) ? D0 : DS[A]; float, handlesNaN/INF/denorm.
                {
                    char Addr[64];
                    FormatVReg(Addr,pInst->GetVAddr(),1);

                    Printf(printer,OPCODE_FORMAT" %6s, off0(0x%02x),off1(0x%02x)\n", Addr, pInst->GetOffset0(),pInst->GetOffset0());
                }
                break;
            }

            return NextInstruction(pInst);
        }

        
        static const Instruction* Disassemble( IPrinter& printer, const Instruction* pInst, const std::vector<const Instruction*>* pLabels )
        {
            switch( pInst->GetFormat() )
            {
            case IF_SOP2:       return _INTERNAL::Disassemble( printer, (const SOP2Instruction*) pInst ); break;
            case IF_VOP2:       return _INTERNAL::Disassemble( printer, (const VOP2Instruction*) pInst ); break;
            case IF_VOP1:       return _INTERNAL::Disassemble( printer, (const VOP1Instruction*) pInst ); break;
            case IF_SOPP:       return _INTERNAL::Disassemble( printer, (const SOPPInstruction*) pInst,pLabels ); break;
            case IF_EXP:        return _INTERNAL::Disassemble( printer, (const EXPInstruction*) pInst ); break;
            case IF_VINTERP:    return _INTERNAL::Disassemble( printer, (const VINTERPInstruction*) pInst ); break;
            case IF_SOPC:       return _INTERNAL::Disassemble( printer, (const SOPCInstruction*) pInst ); break;
            case IF_SOP1:       return _INTERNAL::Disassemble( printer, (const SOP1Instruction*) pInst ); break;
            case IF_SOPK:       return _INTERNAL::Disassemble( printer, (const SOPKInstruction*) pInst, pLabels); break;
            case IF_SMEM:       return _INTERNAL::Disassemble( printer, (const SMEMInstruction*)pInst ); break;
            case IF_VOPC:       return _INTERNAL::Disassemble( printer, (const VOPCInstruction*)pInst ); break;
            case IF_VOP3:       return _INTERNAL::Disassemble( printer, (const VOP3Instruction*)pInst ); break;
            case IF_MUBUFF:     return _INTERNAL::Disassemble( printer, (const MUBUFFInstruction*) pInst ); break;
            case IF_MTBUFF:     return _INTERNAL::Disassemble( printer, (const MTBUFFInstruction*) pInst ); break;
            case IF_MIMG:       return _INTERNAL::Disassemble( printer, (const MIMGInstruction*) pInst ); break;
            case IF_DS:         return _INTERNAL::Disassemble( printer, (const DSInstruction*) pInst ); break;
            case IF_FLAT:      
                _INTERNAL::Printf( printer, "Disassembler for instruction at 0x%08x has not been implemented yet.\n", pInst );
                return NextInstruction(pInst);
           
            default:
            case IF_UNKNOWN:
                _INTERNAL::Printf( printer, "ERROR: Can't determine encoding for instruction at: 0x%016x\n", pInst );
                return 0;
            }
        }



    }


    const Instruction* Disassemble( IPrinter& printer, const Instruction* pInst )
    {
        return _INTERNAL::Disassemble(printer,pInst,0);
    }


    bool DisassembleProgram( IPrinter& printer, const void* pISA, size_t nISASize )
    {
        const uint8* pStart   = (const uint8*)pISA;
        const uint8* pEnd     = ((const uint8*)pISA) + nISASize;

        std::vector< const Instruction* > labels;
        std::vector< const Instruction* > branches;

        // first pass to identify basic blocks and validate instruction encodings
        const uint8* pInst    = pStart;
        while( pInst < pEnd )
        {
            const Instruction* pI = (const Instruction*)pInst;
            uint nLength = pI->GetLength();
            if( nLength == 0 )
            {
                _INTERNAL::Printf( printer, "ERROR: Can't determine encoding for instruction at: 0x%016x\n", pInst );
                return false;
            }

            if( (pInst+nLength) > pEnd )
            {
                _INTERNAL::Printf( printer, "ERROR: Instruction at: 0x%016x overruns specified buffer\n", pInst );
                return false;
            }

            if( pI->IsBranch() )
            {
                branches.push_back(pI);

                const Instruction* pLabel = pI->GetBranchTarget();
                if( pLabel )
                {
                    const uint8* pAddr = (const uint8*)pLabel;
                    if( (pAddr-pStart) >= (ptrdiff_t)nISASize )
                    {
                        _INTERNAL::Printf(printer, "ERROR: Branch target for instruction at: 0x%016x overruns specified buffer\n",pInst);
                        return false;
                    }

                    labels.push_back(pLabel);
                }
            }

            pInst += nLength;
        }

        std::sort( labels.begin(), labels.end() );
        std::sort( branches.begin(), branches.end() );
        
        branches.push_back( (const Instruction*) pEnd ); // so there's always at least one
        labels.push_back( (const Instruction*) pEnd );

        uint nBranch=0;
        uint nLabel=0;

        // second pass to disassemble
        pInst = pStart;
        while( pInst < pEnd )
        {
            const Instruction* pI = (const Instruction*)pInst;
            
            // print labels
            if( pI == labels[nLabel] )
            {
                _INTERNAL::Printf(printer, "Label_%04u:\n", nLabel );
                nLabel++;
            }
           
            // disassemble instruction
            pInst = (const uint8*) _INTERNAL::Disassemble(printer,pI,&labels);

            // extra newline after a branch or before a label
            if( pI == branches[nBranch] ) 
            {
                printer.Push("\n");
                nBranch++;
            }
            else if( ((const Instruction*)pInst) == labels[nLabel] )
            {
                printer.Push("\n");
            }
        }

        return true;
    }


    bool ListEncodings( IPrinter& printer, const void* pISA, size_t nISASize )
    {
        const uint8* pStart   = (const uint8*)pISA;
        const uint8* pEnd     = ((const uint8*)pISA) + nISASize;

        const uint8* pInst    = pStart;
        while( pInst < pEnd )
        {
            const uint32* pDWORDS = reinterpret_cast<const uint32*>(pInst);
           
            const Instruction* pI = (const Instruction*)pInst;
            uint nLength = pI->GetLength();
            if( nLength == 0 )
            {
                _INTERNAL::Printf( printer, "ERROR: Can't determine encoding for instruction at: 0x%016x\n", pInst );
                _INTERNAL::Printf( printer, "First DWORD is: 0x%08x\n", pDWORDS[0] );
                return false;
            }

            if( (pInst+nLength) > pEnd )
            {
                _INTERNAL::Printf( printer, "ERROR: Instruction at: 0x%016x overruns specified buffer\n", pInst );
                return false;
            }

            switch( pI->GetFormat() )
            {
            case IF_SOP2:       printer.Push("SOP2    "); break;
            case IF_SOPK:       printer.Push("SOPK    "); break;
            case IF_SOP1:       printer.Push("SOP1    "); break;
            case IF_SOPC:       printer.Push("SOPC    "); break;
            case IF_SOPP:       printer.Push("SOPP    "); break;
            case IF_SMEM:       printer.Push("SMEM    "); break;
            case IF_VOP2:       printer.Push("VOP2    "); break;
            case IF_VOP1:       printer.Push("VOP1    "); break;
            case IF_VOPC:       printer.Push("VOPC    "); break;
            case IF_VOP3:       printer.Push("VOP3    "); break;
            case IF_VINTERP:    printer.Push("VINTERP "); break;
            case IF_DS:         printer.Push("DS      "); break;
            case IF_MTBUFF:     printer.Push("MTBUFF  "); break;
            case IF_MUBUFF:     printer.Push("MUBUFF  "); break;
            case IF_MIMG:       printer.Push("MIMG    "); break;
            case IF_EXP:        printer.Push("EXP     "); break;
            case IF_FLAT:       printer.Push("FLAT    "); break;
            }

            for( uint i=0; i<nLength; i+= 4 )
                _INTERNAL::Printf( printer, "0x%08x ", pDWORDS[i]);
            printer.Push("\n");

            pInst += nLength;
        }

        return true;
    }
}}
