//
//    ASIC-independent data structure for manipulating GCN instructions
//
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

    uint GetSourceCountForInstruction( VectorInstructions eOp );
    uint GetSrc0WidthInDWORDs( VectorInstructions eOp );
    uint GetSrc1WidthInDWORDs( VectorInstructions eOp );
    uint GetSrc2WidthInDWORDs( VectorInstructions eOp );
    uint GetResultWidthInDWORDs( VectorInstructions eOp );
    uint GetDataWidthInDWORDs( DSInstructions eOp );
    uint GetResultWidthInDWORDs( BufferInstructions eOp );
    bool IsTBufferInstruction( BufferInstructions eOp );

    uint GetSamplerWidthInDWORDs( ImageInstructions eOp );

    /// The 'Instruction' base class contains all fields for all subclasses in a union
    ///   This is done to allow Instructions to be used in containers without fear of slicing
    ///
    ///   Code that manipulates instructions should examine the class field and then static_cast to one 
    ///    of the concrete classes
    ///
    class Instruction
    {
    public:

        InstructionClass GetClass() const { return m_eClass; }
        
    protected:

        friend class GCN1Decoder;
        friend class GCN3Decoder;

        InstructionClass m_eClass;
        
        union LiteralConstant
        {
            uint32 UInt;
            float  Float;
        };

        union
        {
            struct 
            {
                ScalarInstructions m_eOpcode;
                uint8   m_nSourceCount;
                int16   m_nSIMM16;
                Sources m_Sources[2];
                Dests   m_Dest;
                const uint8*  m_pBranchTarget;
                LiteralConstant m_Literal;
            } Scalar;

            struct
            {
                ScalarMemoryInstructions m_eOpcode;
                uint m_nBaseReg;
                uint m_nOffset;
                Dests m_Dest;
                bool m_bIsOffsetIMM; 
            } ScalarMem;

            struct
            {
                VectorInstructions m_eOpcode;
                uint8 m_nDestCount;
                uint8 m_nSrcCount;
                uint8 m_nOMOD;
                uint8 m_nSourceAbs;
                uint8 m_nSourceNegate;
                uint8 m_nClamp;
                Dests m_Dests[2];
                Sources m_Sources[3];
                LiteralConstant m_Literal;
            } Vector;

            struct
            {
                VectorInstructions m_eOpcode;
                Sources m_VSrc;
                Dests m_VDst;
                uint8 m_nAttributeIndex;
                uint8 m_nAttributeChannel;
            } Interp;
            struct
            {
                unsigned m_Done      : 1;
                unsigned m_Compress  : 1;
                unsigned m_ValidMask : 1;
                unsigned m_ChannelMask : 4;
                ExportTargets m_eTarget;
                Sources m_Sources[4];
            } Export;

            struct 
            {
                unsigned m_bGDS : 1;
                unsigned m_nOffset0 : 8;  // Instructions can have one 16 bit offset field or two 8-bit offset fields
                unsigned m_nOffset1 : 8;
                uint16   m_nOffset16;
                DSInstructions m_eOpcode;
                Dests    m_Dest;
                Sources  m_VAddr;
                Sources  m_VData0;
                Sources  m_VData1;
            } DS;
            
            struct
            {
                unsigned m_bTFE       : 1;
                unsigned m_bLDSDirect : 1;
                unsigned m_bGLC : 1;
                unsigned m_bSLC : 1;
                unsigned m_bOffN : 1;
                unsigned m_bIdxN : 1;
                unsigned m_bAddr64 : 1;
                uint16 m_nOffset;
                TBufferNumberFormats m_eNumberFormat;
                TBufferDataFormats m_eDataFormat;
                Sources m_VData;
                Sources m_VAddr;
                Sources m_SResource;
                Sources m_SOffset;
                BufferInstructions m_eOpcode;
            } Buffer;

            struct
            {
                unsigned m_bTFE       : 1;
                unsigned m_bGLC : 1;
                unsigned m_bSLC : 1;
                unsigned m_bArray : 1;
                unsigned m_bUnnormalized : 1;
                unsigned m_bLWE : 1;
                unsigned m_bRes256 : 1;
                unsigned m_nDMask : 4;
                Sources m_VData;
                Sources m_VAddr;
                Sources m_SResource;
                Sources m_SSampler;
                ImageInstructions m_eOpcode;
            } Image;
        
        }Fields;

    };


    class ScalarInstruction : public Instruction
    {
    public:

        bool IsBranch() const;

        const uint8* GetBranchTarget() const { return Fields.Scalar.m_pBranchTarget; }
        
        uint ReadSIMMBits( uint hi, uint lo ) const { return (Fields.Scalar.m_nSIMM16>>lo)&((1<<(1+hi-lo))-1); }
        int16 GetSIMM16( ) const { return Fields.Scalar.m_nSIMM16; };
        
        uint GetResultWidthInDWORDs() const;
        uint GetArg0WidthInDWORDs() const;
        uint GetArg1WidthInDWORDs() const;

        uint32 GetLiteralAsDWORD() const { return Fields.Scalar.m_Literal.UInt; }
        
        ScalarInstructions GetOpcode() const { return Fields.Scalar.m_eOpcode; }
        uint GetSourceCount() const { return Fields.Scalar.m_nSourceCount; }
        
        Sources GetSource(uint i) const { return Fields.Scalar.m_Sources[i]; }
        Dests GetDest() const { return Fields.Scalar.m_Dest; }

    };


    class ScalarMemoryInstruction : public Instruction
    {
    public:
        uint GetResultWidthInDWORDs() const;
        ScalarMemoryInstructions GetOpcode() const { return Fields.ScalarMem.m_eOpcode; }
        bool  IsOffsetIMM() const { return Fields.ScalarMem.m_bIsOffsetIMM; }
        uint  GetOffset() const { return Fields.ScalarMem.m_nOffset; }
        Dests GetDest() const { return Fields.ScalarMem.m_Dest; }
        uint  GetBase() const { return Fields.ScalarMem.m_nBaseReg; }
    };


    class VectorInstruction : public Instruction
    {
    public:
          
        VectorInstructions GetOpcode() const { return Fields.Vector.m_eOpcode; }
        float GetLiteralAsFloat() const { return Fields.Vector.m_Literal.Float; }
        uint32 GetLiteralAsDWORD() const { return Fields.Vector.m_Literal.UInt; }

        uint GetSourceAbsMask() const { return Fields.Vector.m_nSourceAbs; }
        uint GetSourceNegateMask() const { return Fields.Vector.m_nSourceNegate; }
        uint GetOMod() const { return Fields.Vector.m_nOMOD; }
        bool GetClamp() const { return Fields.Vector.m_nClamp != 0; }

        uint GetDestCount() const { return Fields.Vector.m_nDestCount; }
        uint GetSourceCount() const { return Fields.Vector.m_nSrcCount; }

        Dests GetSDst() const { return Fields.Vector.m_Dests[1]; }; // VOP3b only
        Dests GetVDst() const { return Fields.Vector.m_Dests[0]; }
        Sources GetSrc0() const  { return Fields.Vector.m_Sources[0]; }
        Sources GetVSrc1() const { return Fields.Vector.m_Sources[1]; }
        Sources GetVSrc2() const { return Fields.Vector.m_Sources[2]; }

        uint GetSrc0WidthInDWORDS()   const   { return GetSrc0WidthInDWORDs(GetOpcode()); }
        uint GetSrc1WidthInDWORDS()   const   { return GetSrc1WidthInDWORDs(GetOpcode()); }
        uint GetSrc2WidthInDWORDS()   const   { return GetSrc2WidthInDWORDs(GetOpcode()); }
        uint GetResultWidthInDWORDS() const   { return GetResultWidthInDWORDs(GetOpcode()); }
    };


    class InterpolationInstruction : public Instruction
    {
    public:
        VectorInstructions GetOpcode() const { return Fields.Interp.m_eOpcode; }
        Sources GetVSrc() const { return Fields.Interp.m_VSrc; }
        Dests GetVDst() const { return Fields.Interp.m_VDst; }
        uint GetAttributeIndex() const { return Fields.Interp.m_nAttributeIndex; }
        uint GetAttributeChannel() const { return Fields.Interp.m_nAttributeChannel; }
    };

    class ExportInstruction : public Instruction
    {
    public:
        bool GetCompressBit() const { return Fields.Export.m_Compress; }
        bool GetDoneBit() const { return Fields.Export.m_Done; }
        bool GetValidMaskBit() const { return Fields.Export.m_ValidMask; }
        ExportTargets GetTarget() const { return Fields.Export.m_eTarget; }
        uint GetExportMask() const { return Fields.Export.m_ChannelMask; }
        Sources GetVSrc0() const { return Fields.Export.m_Sources[0]; }
        Sources GetVSrc1() const { return Fields.Export.m_Sources[1]; }
        Sources GetVSrc2() const { return Fields.Export.m_Sources[2]; }
        Sources GetVSrc3() const { return Fields.Export.m_Sources[3]; }
    };

    class DataShareInstruction : public Instruction
    {
    public:
        DSInstructions GetOpcode() const { return Fields.DS.m_eOpcode; }
        uint GetOffset0()  const   { return Fields.DS.m_nOffset0; }  // Instructions can have one 16 bit offset field or two 8-bit offset fields
        uint GetOffset1()  const   { return Fields.DS.m_nOffset1; }
        uint GetOffset16() const   { return Fields.DS.m_nOffset16; }
        Dests   GetVDest()   const { return Fields.DS.m_Dest; }
        Sources GetVData0()  const { return Fields.DS.m_VData0; }
        Sources GetVData1()  const { return Fields.DS.m_VData1; }
        Sources GetVAddr()   const { return Fields.DS.m_VAddr; }
        bool IsGDS()       const   { return Fields.DS.m_bGDS!=0; }

        uint GetDataWidthInDWORDS() const { return GetDataWidthInDWORDs( GetOpcode() ); }
    };

    class BufferInstruction : public Instruction
    {
    public:

        BufferInstructions GetOpcode() const { return Fields.Buffer.m_eOpcode; }
        bool IsDirectToLDS() const { return Fields.Buffer.m_bLDSDirect; }
        bool GetTFEBit() const { return Fields.Buffer.m_bTFE; }
        bool GetGLCBit() const { return Fields.Buffer.m_bGLC; }
        bool GetSLCBit() const { return Fields.Buffer.m_bSLC; }
        bool IsTBuffer() const { return IsTBufferInstruction( GetOpcode()); }
        bool IsOffN() const    { return Fields.Buffer.m_bOffN; }
        bool IsIdxN() const    { return Fields.Buffer.m_bIdxN;}
        bool IsAddr64() const  { return Fields.Buffer.m_bAddr64; }
        uint GetOffset() const { return Fields.Buffer.m_nOffset; }
        TBufferNumberFormats GetNumberFormat() const { return Fields.Buffer.m_eNumberFormat; }
        TBufferDataFormats GetDataFormat() const { return Fields.Buffer.m_eDataFormat; }
        Sources GetVData() const { return Fields.Buffer.m_VData; }
        Sources GetVAddr() const { return Fields.Buffer.m_VAddr; }
        Sources GetSResource() const { return Fields.Buffer.m_SResource; }
        Sources GetSOffset() const { return Fields.Buffer.m_SOffset; }
        uint GetResultWidthInDWORDS() const { return GetResultWidthInDWORDs(GetOpcode()); }

    };


    
    class ImageInstruction : public Instruction
    {
    public:

        ImageInstructions GetOpcode() const   { return Fields.Image.m_eOpcode; }
        bool IsTFE() const                    { return Fields.Image.m_bTFE; }
        bool IsGLC() const                    { return Fields.Image.m_bGLC; }
        bool IsSLC() const                    { return Fields.Image.m_bSLC; }
        bool IsArray() const                  { return Fields.Image.m_bArray; }
        bool IsUnnormalized() const           { return Fields.Image.m_bUnnormalized; }
        bool IsLWE() const                    { return Fields.Image.m_bLWE; }
        uint GetDMask() const                 { return Fields.Image.m_nDMask; }
        Sources GetVData() const              { return Fields.Image.m_VData; }
        Sources GetVAddr() const              { return Fields.Image.m_VAddr; }
        Sources GetSResource() const          { return Fields.Image.m_SResource; }
        Sources GetSSampler() const           { return Fields.Image.m_SSampler; }
        uint GetSamplerWidthInDWORDS() const  { return GetSamplerWidthInDWORDs(GetOpcode()); }
        uint GetResourceWidthInDWORDS() const { return Fields.Image.m_bRes256*4 + 4; }
        uint GetResultWidthInDWORDS() const;
    };

};


#endif