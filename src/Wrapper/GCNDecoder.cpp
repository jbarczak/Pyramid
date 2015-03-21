//
//    The decoder class is responsible for decoding GCN instructions into a standardized form
//
//     A decoder abstraction is necessary because AMD decided to change the 
//       microcode formats and opcode numbers for GCN3.   
//
//      Copyright 2015 Joshua Barczak, all rights reserved.
//

#pragma unmanaged
#include "GCNDecoder.h"
#include "GCN1Decoder.h"
#include "GCN3Decoder.h"

namespace GCN
{
    IDecoder* IDecoder::Create( GCNVersions eVersion )
    {
        switch( eVersion )
        {
        case GCN1: return new GCN1Decoder();
        case GCN3: return new GCN3Decoder();
        default:
            return 0;
        }
    }

    void IDecoder::Destroy( IDecoder* p )
    {
        delete p;
    }
}