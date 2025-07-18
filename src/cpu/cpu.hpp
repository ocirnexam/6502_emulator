#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <iostream>
#include <cstdint>
#include <bitset>

#include "datatypes.hpp"
#include "memory/memory.hpp"


#define CARRY_FLAG        ((Byte) 1 << 0)
#define ZERO_FLAG         ((Byte) 1 << 1)
#define INTERRUPT_DISABLE ((Byte) 1 << 2)
#define DECIMAL_MODE      ((Byte) 1 << 3)
#define BREAK_COMMAND     ((Byte) 1 << 4)
#define OVERFLOW_FLAG     ((Byte) 1 << 6)
#define NEGATIVE_FLAG     ((Byte) 1 << 7)

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
    INS_LDX_ZEROPAGE = 0xA6,
    INS_LDX_ZEROPAGE_Y = 0xB6,
    INS_LDX_ABSOLUTE = 0xAE,
    INS_LDX_ABSOLUTE_Y = 0xBE,
    // LDY
    INS_LDY_IMMEDIATE = 0xA0,
    INS_LDY_ZEROPAGE = 0xA4,
    INS_LDY_ZEROPAGE_X = 0xB4,
    INS_LDY_ABSOLUTE = 0xAC,
    INS_LDY_ABSOLUTE_X = 0xBC,
    // LSR
    INS_LSR_A = 0x4A,
    INS_LSR_ZEROPAGE = 0x46,
    INS_LSR_ZEROPAGE_X = 0x56,
    INS_LSR_ABSOLUTE = 0x4E,
    INS_LSR_ABSOLUTE_X = 0x5E,
    // Jumps
    INS_JSR = 0x20, // Jump to subroutine
    INS_JMP_ABSOLUTE = 0x4C, // jump
    INS_JMP_INDIRECT = 0x6C; // jump
    

enum RegisterType {
    X,
    Y,
    A
};

struct CPU
{
    Memory memory;
    Word PC;
    Word SP;

    Byte A, X, Y;  // Accumulator, Index X, Index Y registers
    Byte PS; // Processor Status register

    uint32_t SysTicks = 0;

    void Reset();
    Byte FetchByte();
    Word FetchWord();
    Byte ReadByte(Byte Address);
    Byte ReadByte(Word Address);
    Word ReadWord(Word Address);

    void IncreaseSysTicks(uint32_t amount);
    void Execute();

    void SetStatus(RegisterType reg);

    void PrintRegisters();

    // private functions for cpu instructions
private:
    void InstructionLDA(Byte Instruction);
    void InstructionLDX(Byte Instruction);
    void InstructionLDY(Byte Instruction);
    void InstructionLSR(Byte Instruction);
};

#endif