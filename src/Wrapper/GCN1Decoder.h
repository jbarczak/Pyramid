//
//    Decoder for early GCN chips (SI and CI)
//
//      Copyright 2015 Joshua Barczak, all rights reserved.
//
#ifndef _GCN1_DECODER_H_
#define _GCN1_DECODER_H_
#pragma once

#include "GCNDecoder.h"

namespace GCN
{
    class GCN1Decoder : public IDecoder
    {
    public:
        virtual InstructionFormat ReadInstructionFormat( const uint8* pLocation );
        virtual size_t DetermineInstructionLength( const uint8* pLocation, InstructionFormat eEncoding );
        virtual void Decode( Instruction* pInst, const uint8* pLocation, InstructionFormat eEncoding ) ;
    };
}

#endif