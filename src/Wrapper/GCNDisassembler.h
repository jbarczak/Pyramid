//
//    GCN disassembler
//      Copyright 2015 Joshua Barczak, all rights reserved.
//
#ifndef _GCN_DISASSEMBLER_H_
#define _GCN_DISASSEMBLER_H_
#pragma once

namespace GCN{
    class Instruction;
namespace Disassembler{

    /// Interface that receives a series of text strings
    class IPrinter
    {
    public:
        virtual void Push( const char* pText ) = 0;
    };

    /// Returns a pointer to the instruction following the input instruction
    ///   Returns NULL if the instruction encoding wasn't recognized
    const Instruction* Disassemble( IPrinter& printer, const Instruction* pIt );

    /// Disassemble a full program, returns false if an instruction is encountered whose encoding
    ///  wasn't recognized
    bool DisassembleProgram( IPrinter& printer, const void* pISA, size_t nISASize );

    /// List encodings and hex bytes for each instruction.  Returns false if an instruction is encountered
    ///  whose encoding wasn't recognized. In this case, returns early
    bool ListEncodings( IPrinter& printer, const void* pISA, size_t nISASize );
}}

#endif