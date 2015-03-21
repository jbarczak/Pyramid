//
//    The decoder class is responsible for decoding GCN instructions into a standardized form
//
//     A decoder abstraction is necessary because AMD decided to change the 
//       microcode formats and opcode numbers for GCN3.   
//
//      Copyright 2015 Joshua Barczak, all rights reserved.
//
#ifndef _GCN_DECODER_H_
#define _GCN_DECODER_H_
#pragma once

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

    enum InstructionFormat;
    class Instruction;

    class IDecoder
    {
    public:

        enum GCNVersions
        {
            GCN1,
            GCN3,
        };

        /// Decoder factory
        static IDecoder* Create( GCNVersions eVersion );

        static void Destroy( IDecoder* pDecoder );

        /// Determine the format of the instruction at 'pLocation' by parsing the encoding bits
        virtual InstructionFormat ReadInstructionFormat( const uint8* pLocation ) = 0;

        /// Determine the length in bytes of the instruction at 'pLocation'
        virtual size_t DetermineInstructionLength( const uint8* pLocation, InstructionFormat eEncoding ) = 0;

        /// Decode the instruction at 'pLocation'
        virtual void Decode( Instruction* pInst, const uint8* pLocation, InstructionFormat eEncoding ) = 0;
    
    protected:

        // Utility methods which can be used by decoder implementations

        uint ReadBits( const uint8* pWhere, uint hi, uint lo ) const
        {
            uint shift = lo % 32;
            uint mask = (1<<(hi-lo+1))-1;
            return ( ((const uint32*)pWhere)[lo/32]>>shift)&mask;
        }
        bool ReadBit( const uint8* pWhere, uint bit ) const
        {
            uint shift = bit % 32;
            return (( ((const uint32*)pWhere)[bit/32]>>shift)&1) != 0;
        }

    };
}


#endif