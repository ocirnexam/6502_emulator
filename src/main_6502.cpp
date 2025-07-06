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
    mem[0x1717] = 0x11;
    mem[0xFFFC] = INS_JSR;
    mem.WriteWord(0xFFFD, 0x4242);
    mem[0x4242] = INS_LDA_ABSOLUTE;
    mem[0x4243] = 0x17;
    mem[0x4244] = 0x17;
    cpu.Execute(8, mem);
    cpu.PrintRegisters();
    // inline program
    return 0;
}