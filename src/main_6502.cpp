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
    mem[0x05] = 0x06;
    mem[0x15] = 0x63;
    mem[0x16] = 0x96;
    mem[0x9669] = 0x89;
    mem[0x1010] = 0x01;
    mem[0x1011] = 0x99;
    // program
    mem[0xFFFC] = INS_JMP_ABSOLUTE;
    mem[0xFFFD] = 0xAA;
    mem[0xFFFE] = 0xAA;
    mem[0xAAAA] = INS_LDY_ZEROPAGE;
    mem[0xAAAB] = 0x05;
    mem[0xAAAC] = INS_LDA_INDIRECT_Y;
    mem[0xAAAD] = 0x15;
    mem[0xAAAE] = INS_LDX_ABSOLUTE;
    mem[0xAAAF] = 0x10;
    mem[0xAAB0] = 0x10;
    mem[0xAAB1] = INS_LDY_ABSOLUTE_X;
    mem[0xAAB2] = 0x10;
    mem[0xAAB3] = 0x10;
    mem[0xAAB4] = INS_LSR_A;
    // inline program
    cpu.Execute(mem);
    return 0;
}