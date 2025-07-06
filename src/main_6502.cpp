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
    cpu.X = 1;
    mem[0x1718] = 0x69;
    mem[0xFFFC] = INS_JSR;
    mem[0xFFFD] = 0x42;
    mem[0xFFFE] = 0x42;
    mem[0x4242] = INS_LDA_ABSOLUTE_X;
    mem[0x4243] = 0x17;
    mem[0x4244] = 0x17;
    cpu.Execute(11, mem);
    cpu.PrintRegisters();
    // inline program
    return 0;
}