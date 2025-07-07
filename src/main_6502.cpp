#include <stdio.h>
#include <stdlib.h>

#include "cpu/cpu.hpp"
#include "memory/memory.hpp"

int main()
{
    Memory mem;
    CPU cpu;
    cpu.Reset(mem);
    // inline program
    // data
    mem[0x15] = 0x64;
    mem[0x16] = 0x96;
    mem[0x9669] = 0x88;
    // program
    mem[0xFFFC] = INS_JMP_ABSOLUTE;
    mem[0xFFFD] = 0xAA;
    mem[0xFFFE] = 0xAA;
    mem[0xAAAA] = INS_LDY_IMMEDIATE;
    mem[0xAAAB] = 0x05;
    mem[0xAAAC] = INS_LDA_INDIRECT_Y;
    mem[0xAAAD] = 0x15;
    cpu.Execute(mem);
    cpu.PrintRegisters();
    // inline program
    return 0;
}