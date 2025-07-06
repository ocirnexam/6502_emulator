#include "memory/memory.hpp"

void Memory::Initialize()
{
    for (u32 i = 0; i < MAX_MEM; i++)
    {
        Data[i] = 0;
    }
}

void Memory::WriteWord(u32 Address, Word Date)
{
    Data[Address] = Date & 0xFF;
    Data[Address + 1] = (Date >> 8) & 0xFF;
}