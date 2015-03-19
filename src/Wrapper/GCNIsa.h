//
//    Wrapper classes around GCN instructions
//      Copyright 2015 Joshua Barczak, all rights reserved.
//
#ifndef _GCN_ISA_H_
#define _GCN_ISA_H_
#pragma once

#include "GCNEnums.h"

namespace GCN
{
    typedef unsigned char  uint8;
    typedef unsigned short uint16;
    typedef unsigned int   uint32;
    typedef unsigned int   uint;

    typedef short int16;

    static_assert( sizeof(uint8)  == 1, "Fix typedefs" );
    static_assert( sizeof(int16)  == 2, "Fix typedefs" );
    static_assert( sizeof(uint16) == 2, "Fix typedefs" );
    static_assert( sizeof(uint32) == 4, "Fix typedefs" );

    /// A helper class for interpretting GCN instructions
    ///   To use, simply cast an arbitrary pointer to the appropriate 'Instruction' type
    ///  
    class Instruction
    {
    public:

        /// Infer the type of instruction by searching for the appropriate bit pattern
        InstructionFormat GetFormat() const; 

        
        /// Return instruction length in bytes, including any literal costant
        uint GetLength() const;

        /// Determine whether this instruction alters control flow
        bool IsBranch() const;

        /// Derive the branch target for this instruction.  Return NULL if not a branch,
        ///   or if the branch target is data-dependent
        const Instruction* GetBranchTarget() const;

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
        InstructionFormat GetFormat() const { return IF_SOP2; }
        uint   GetLength()            const { return 4 + 4*((GetSrc0() == LITERAL || GetSrc1() == LITERAL)); };
        SOP2_Opcodes   GetOpcode()    const { return (SOP2_Opcodes)ReadBits( 29, 23 ); }
        uint   GetSrc0()              const { return ReadBits( 7,0 ); }
        uint   GetSrc1()              const { return ReadBits( 15,8); }
        uint   GetDest()              const { return ReadBits( 22,16); }

        uint GetResultWidthInDWORDS() const;

        bool IsBranch() const;
        const Instruction* GetBranchTarget() const { return 0; } // only one is S_CBRANCH_G_FORK, and its data-dependent

    };

    class SOP1Instruction : public Instruction
    {
    public:
        InstructionFormat GetFormat() const { return IF_SOP1; }
        uint   GetLength()            const { return 4 + 4*(GetSrc0() == LITERAL); };

        SOP1_Opcodes   GetOpcode()    const { return (SOP1_Opcodes)ReadBits( 15,8 ); }
        uint   GetSrc0()              const { return ReadBits(7,0); }
        uint   GetDest()              const { return ReadBits(22,16); }

        
        uint GetResultWidthInDWORDS() const;
        uint GetOperandWidthInDWORDS() const;
        
        bool IsBranch() const;
        const Instruction* GetBranchTarget() const { return 0; } // the PC manipulation instructions are all data-dependent


    };

    class SOPKInstruction : public Instruction
    {
    public:
        InstructionFormat GetFormat() const { return IF_SOPK; }
        uint GetLength() const { return 4 + 4*(GetOpcode() == S_SETREG_IMM32_B32); };

        SOPK_Opcodes GetOpcode() const { return (SOPK_Opcodes) ReadBits( 27,23 ); }
        uint GetDest()           const { return ReadBits(22,16); }

        int ReadSIMM16() const { return *((const int16*)this); }
        uint ReadSIMMBits( uint hi, uint lo) const { return ReadBits(hi,lo); }
        uint32 ReadIMM32() const { return ReadBits(63,32); }
        
        bool IsBranch() const;
        const Instruction* GetBranchTarget() const;

    };

    class SOPCInstruction : public Instruction
    {
    public:
        InstructionFormat GetFormat() const { return IF_SOPC; }
        uint GetLength()  const { return 4 + 4*(GetSrc0() == LITERAL || GetSrc1() == LITERAL); };

        SOPC_Opcodes GetOpcode()  const { return (SOPC_Opcodes) ReadBits( 22,16 ); }
        uint GetSrc0()            const { return ReadBits(7,0); }
        uint GetSrc1()            const { return ReadBits(15,8 ); }
        
        const Instruction* GetBranchTarget() const { return 0; }
        bool IsBranch() const { return false; }
    };

    class SOPPInstruction : public Instruction
    {
    public:
        InstructionFormat GetFormat() const { return IF_SOPP; }
        uint GetLength() const  { return 4; }

        
        int ReadSIMM16() const { return *((const int16*)this); }
        uint ReadSIMMBits( uint hi, uint lo ) const { return ReadBits(hi,lo); }

        SOPP_Opcodes GetOpcode() const { return (SOPP_Opcodes) ReadBits( 22,16 ); }
        const Instruction* GetBranchTarget() const;
        
        bool IsBranch() const;
    };

    class SMEMInstruction : public Instruction
    {
    public:
        InstructionFormat GetFormat() const { return IF_SMEM; }
        uint GetLength()       const { return 4 + 4*(!IsOffsetIMM() && GetOffset() == LITERAL); }

        SMEM_Opcodes GetOpcode()       const { return (SMEM_Opcodes) ReadBits(26,22); }
        uint GetDest()                 const { return ReadBits( 21,15); }
        uint GetBase()                 const { return ReadBits( 14,9 ); } // base of SGPR pair containing address
        uint GetOffset()               const { return ReadBits( 7, 0 ); }
        bool IsOffsetIMM()             const { return ReadBits( 8, 8 )!=0; }

        uint GetResultWidthInDWORDs() const;
        

        const Instruction* GetBranchTarget() const { return 0; }
        bool IsBranch() const { return false; }
    };

    class VOP2Instruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_VOP2; }
        uint GetLength() const { return 4 + 4*(GetSrc0() == LITERAL || GetOpcode() == V_MADAK_F32 || GetOpcode() == V_MADMK_F32); }

        VOP2_Opcodes GetOpcode() const { return (VOP2_Opcodes)ReadBits(30,25); }
        uint GetSrc0()           const { return ReadBits(8,0); }
        uint GetVSrc1()          const { return ReadBits(16,9); }
        uint GetVDst()           const { return ReadBits(24,17); }
        
        uint GetResultWidthInDWORDS() const { return 1; }

        float ReadFloatLiteral() const { return *(const float*)( ((const uint32*)this) + 1); }
        

        const Instruction* GetBranchTarget() const { return 0; }
        bool IsBranch() const { return false; }
    };

    class VOP1Instruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_VOP1; }
        uint GetLength() const ;

        VOP1_Opcodes GetOpcode() const { return (VOP1_Opcodes) ReadBits(16,9); }
        uint GetVDst()           const { return ReadBits(24,17); }
        uint GetSrc0()           const { return ReadBits(8,0); }
        
        uint GetResultWidthInDWORDS() const;
        uint GetOperandWidthInDWORDS() const;
        

        const Instruction* GetBranchTarget() const { return 0; }
        bool IsBranch() const { return false; }
    };

    class VOPCInstruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_VOPC; }
        uint GetLength() const { return 4 + 4*(GetSrc0() == LITERAL); }

        VOPC_Opcodes GetOpcode() const { return (VOPC_Opcodes) ReadBits(24,17); }
        uint GetSrc0()           const { return ReadBits( 8,0 ); }
        uint GetVSrc1()          const { return ReadBits( 16,9 ); }
      
        uint GetOperandWidthInDWORDs() const;
        

        const Instruction* GetBranchTarget() const { return 0; }
        bool IsBranch() const { return false; }
    };

    class VINTERPInstruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_VINTERP; }
        uint GetLength() const { return 4; }

        VINTERP_Opcodes GetOpcode() const { return (VINTERP_Opcodes) ReadBits(17,16); }
        uint GetAttributeChannel() const { return ReadBits(9,8); };
        uint GetAttributeIndex() const { return ReadBits(15,10); };
        uint GetVSrc0() const { return ReadBits(7,0); }
        uint GetVDst() const { return ReadBits(25,18); }

        const Instruction* GetBranchTarget() const { return 0; }
        bool IsBranch() const { return false; }
    };

    class VOP3Instruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_VOP3; }
        uint GetLength() const { return 8; }
        
        VOP3_Opcodes GetOpcode() const { return (VOP3_Opcodes)ReadBits(25,17);}
        uint GetVDst() const { return ReadBits(7,0); }
        uint GetSrc0() const { return ReadBits(40,32); }
        uint GetSrc1() const { return ReadBits(49,41); }
        uint GetSrc2() const { return ReadBits(58,50); }
        uint GetSourceNegateMask() const { return ReadBits(63,61); };
        uint GetOMod() const { return ReadBits(60,59); }
     
        bool IsCompare() const { return GetOpcode() < 255; }

        bool IsVOP3bOp() const;

        // VOP3a only
        uint GetSourceAbsMask() const { return ReadBits(10,8); }
        bool GetClamp() const { return ReadBit(11); }

        // VOP3b only
        uint GetSDst() const { return ReadBits(14,8); }

        uint GetOperandSizeInDWORDS() const;
        uint GetResultSizeInDWORDS() const;
        

        const Instruction* GetBranchTarget() const { return 0; }
        bool IsBranch() const { return false; }

        
    };

    class MUBUFFInstruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_MUBUFF; }
        MUBUFF_Opcodes GetOpcode() const { return (MUBUFF_Opcodes)ReadBits(24,18); }
        uint GetLength() const { return 8; }
        uint GetResultWidthInDWORDS() const ;

        uint GetOffset() const { return ReadBits(11,0); }
        uint GetVAddr() const { return ReadBits(39,32); }
        uint GetVData() const { return ReadBits(47,40); }
        uint GetSResource() const { return ReadBits(52,48); }
        uint GetSOffset() const { return ReadBits(63,56); }
        
        bool IsOffN() const     { return ReadBit(12); }
        bool IsIdxN() const     { return ReadBit(13); }
        bool IsAddr64() const   { return ReadBit(15); }
        bool IsDirectToLDS() const { return ReadBit(16); }
        bool GetSLCBit() const { return ReadBit(54); }
        bool GetGLCBit() const { return ReadBit(14); }
        bool GetTFEBit() const { return ReadBit(55); }
        

        const Instruction* GetBranchTarget() const { return 0; }
        bool IsBranch() const { return false; }
    };

    class MTBUFFInstruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_MTBUFF; }
        MTBUFF_Opcodes GetOpcode() const { return (MTBUFF_Opcodes)ReadBits(18,16); }
        uint GetLength() const { return 8; }

        uint GetResultWidthInDWORDS() const ;

        MTBUFF_NumberFormat GetNumberFormat() const { return (MTBUFF_NumberFormat) ReadBits(25,23); }
        MTBUFF_DataFormat GetDataFormat() const { return (MTBUFF_DataFormat) ReadBits(22,19); }

        uint GetOffset() const    { return ReadBits(11,0); }
        uint GetVAddr() const     { return ReadBits(39,32); }
        uint GetVData() const     { return ReadBits(47,40); }
        uint GetSResource() const { return ReadBits(52,48); }
        uint GetSOffset() const   { return ReadBits(63,56); }
        
        bool IsOffN() const     { return ReadBit(12); }
        bool IsIdxN() const     { return ReadBit(13); }
        bool IsAddr64() const   { return ReadBit(15); }
        bool GetSLCBit() const  { return ReadBit(54); }
        bool GetGLCBit() const  { return ReadBit(14); }
        bool GetTFEBit() const  { return ReadBit(55); }
        

        const Instruction* GetBranchTarget() const { return 0; }
        bool IsBranch() const { return false; }
    };
    
   

    class MIMGInstruction : public Instruction
    {
    public:
        
        InstructionFormat GetFormat() const { return IF_MIMG; }
        MIMG_Opcodes GetOpcode() const { return (MIMG_Opcodes) ReadBits(24,18); }
        uint GetLength() const { return 8; }

        bool IsSLC() const { return ReadBit(25); }
        bool IsGLC() const { return ReadBit(13); }
        bool IsTFE() const { return ReadBit(16); }
        bool IsLWE() const { return ReadBit(17); }
        bool IsArray() const { return ReadBit(14); }
        bool IsUnormalized() const { return ReadBit(12); }

        uint GetDMask() const { return ReadBits(11,8); }
        uint GetSSampler() const { return ReadBits(57,53); }
        uint GetSResource() const { return ReadBits(52,48); }
        uint GetVData() const { return ReadBits(47,40); }
        uint GetVAddr() const { return ReadBits(39,32); }

        uint GetResourceWidthInDWORDS() const { return ReadBit(15) ? 4 : 8; }; // 4 or 8
        uint GetSamplerWidthInDWORDS() const;  // 0, 4
        uint GetResultWidthInDWORDS() const;
        

        const Instruction* GetBranchTarget() const { return 0; }
        bool IsBranch() const { return false; }
    };

    class DSInstruction : public Instruction
    {
    public:
        InstructionFormat GetFormat() const { return IF_DS; }
        uint GetLength() const { return 8; }

        uint GetOffset0() const { return ReadBits(7,0); }  // Instructions can have one 16 bit offset field or two 8-bit offset fields
        uint GetOffset1() const { return ReadBits(15,8); }
        uint GetOffset16() const { return ReadBits(15,0); }
        DS_Opcodes GetOpcode() const { return (DS_Opcodes) ReadBits(25,18); }
        uint GetVDest()  const { return ReadBits(63,56); }
        uint GetVData0() const { return ReadBits(47,40); }
        uint GetVData1() const { return ReadBits(55,48); }
        uint GetVAddr()  const { return ReadBits(39,32); }
        bool IsGDS()     const { return ReadBit(17); }

        uint GetDataWidthInDWORDS() const;
        

        const Instruction* GetBranchTarget() const { return 0; }
        bool IsBranch() const { return false; }

    };

    class EXPInstruction : public Instruction
    {
    public:
        InstructionFormat GetFormat() const { return IF_EXP; }
        uint GetLength() const { return 8; }
  
        bool GetCompressBit() const { return ReadBit(10); }
        bool GetDoneBit() const { return ReadBit(11); }
        bool GetValidMaskBit() const { return ReadBit(12); }

        ExportTargets GetTarget() const { return (ExportTargets)ReadBits(9,4); }
        uint GetExportMask() const { return ReadBits(3,0); }
        uint GetVSrc0() const { return ReadBits(39,32); }
        uint GetVSrc1() const { return ReadBits(47,40); }
        uint GetVSrc2() const { return ReadBits(55,48); }
        uint GetVSrc3() const { return ReadBits(63,56); }
        

        const Instruction* GetBranchTarget() const { return 0; }
        bool IsBranch() const { return false; }
    };

    class FLATInstruction : public Instruction
    {
    public:
        InstructionFormat GetFormat() const { return IF_FLAT; }
        FLAT_Opcodes GetOpcode() const;
        uint GetLength() const { return 8; }
        
        const Instruction* GetBranchTarget() const { return 0; }
        bool IsBranch() const { return false; }
  
    };


    
    template< class TInstruction >
    inline const TInstruction* NextInstruction( const TInstruction* pI )
    {
        return reinterpret_cast<const TInstruction*>( reinterpret_cast<const uint8*>(pI) + pI->GetLength() );
    }
};


#endif