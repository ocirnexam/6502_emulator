#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <iostream>

#include "datatypes.hpp"
#include "memory/memory.hpp"

#define CARRY_FLAG        ((Byte) 1 << 7)
#define ZERO_FLAG         ((Byte) 1 << 6)
#define INTERRUPT_DISABLE ((Byte) 1 << 5)
#define DECIMAL_MODE      ((Byte) 1 << 4)
#define BREAK_COMMAND     ((Byte) 1 << 3)
#define OVERFLOW_FLAG     ((Byte) 1 << 1)
#define NEGATIVE_FLAG     ((Byte) 1 << 0)

// OPCODES
static constexpr Byte 
    // LDA
    INS_LDA_IMMEDIATE = 0xA9,
    INS_LDA_ZEROPAGE = 0xA5,
    INS_LDA_ZEROPAGE_X = 0xB5,
    INS_LDA_ABSOLUTE = 0xAD,
    INS_LDA_ABSOLUTE_X = 0xBD,
    INS_LDA_ABSOLUTE_Y = 0xB9,
    INS_LDA_INDIRECT_X = 0xA1,
    INS_LDA_INDIRECT_Y = 0xB1,
    // LDX
    INS_LDX_IMMEDIATE = 0xA2,
    // LDY
    INS_LDY_IMMEDIATE = 0xA0,
    // Jumps
    INS_JSR = 0x20, // Jump to subroutine
    INS_JMP_ABSOLUTE = 0x4C, // jump
    INS_JMP_INDIRECT = 0x6C; // jump
    

enum LDRegisterType {
    X,
    Y,
    A
};

struct CPU
{
    Word PC;
    Word SP;

    Byte A, X, Y;  // Accumulator, Index X, Index Y registers
    Byte PS; // Processor Status register

    uint32_t SysTicks = 0;

    void Reset(Memory& memory);
    Byte FetchByte(Memory& memory);
    Word FetchWord(Memory& memory);
    Byte ReadByte(Memory& memory, Byte Address);
    Byte ReadByte(Memory& memory, Word Address);
    Word ReadWord(Memory& memory, Word Address);

    void IncreaseSysTicks(uint32_t amount);
    void Execute(Memory& memory);

    void LDSetStatus(LDRegisterType reg);

    void PrintRegisters();
};

#endif