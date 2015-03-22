
#pragma unmanaged
#include "GCNEnums.h"
#include "GCN3Decoder.h"

// TODO: Special cases:
//   - V_MADAK_F16 and friends should convert literal from F16 to F32 so disassembler can grok it
//
//   - S_SET_GPR_IDX_ON (vopc) is a special little fella.  His Src1 field gets ripped out and used as an immediate
//       * Move to the SIMM16 field in the scalar instruction class
//  - BUFFER_STORE_LDS_DWORD
//          set the LDS bit.  Not sure if this is supposed to use M0 or a vgpr and of course the doc don't say

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
        switch( eEncoding )
        {
        //case IF_VOP2:       return ((const _GCN1Decoder_INTERNAL::VOP2Instruction*)this)->GetLength();
        //case IF_VOP1:       return ((const _GCN1Decoder_INTERNAL::VOP1Instruction*)this)->GetLength();
        //case IF_VOPC:       return ((const _GCN1Decoder_INTERNAL::VOPCInstruction*)this)->GetLength();
        //case IF_SOP2:       return ((const _GCN1Decoder_INTERNAL::SOP2Instruction*)this)->GetLength();
        //case IF_SOPK:       return ((const _GCN1Decoder_INTERNAL::SOPKInstruction*)this)->GetLength();
        //case IF_SOP1:       return ((const _GCN1Decoder_INTERNAL::SOP1Instruction*)this)->GetLength();
        //case IF_SOPC:       return ((const _GCN1Decoder_INTERNAL::SOPCInstruction*)this)->GetLength();
        //
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
        case IF_UNKNOWN:
        case IF_SMEM:
            return 8;

        default:
            return 0;
        }
    }

    void GCN3Decoder::Decode( Instruction* pInst, const uint8* pLocation, InstructionFormat eEncoding ) 
    {

    }




}