
//
//    GCN disassembler
//      Copyright 2015 Joshua Barczak, all rights reserved.
//
#ifndef _GCN_BUFFERED_PRINTER_H_
#define _GCN_BUFFERED_PRINTER_H_
#pragma once

#include "GCNDisassembler.h"
#include <vector>
#include <string.h>

namespace GCN{
namespace Disassembler{
    class BufferedPrinter : public GCN::Disassembler::IPrinter
    {
    public:
        virtual void Push( const char* p )
        {
            m_Bytes.insert( m_Bytes.end(), p, p + strlen(p) );
        }

        std::vector<char> m_Bytes;
    };

}}

#endif
