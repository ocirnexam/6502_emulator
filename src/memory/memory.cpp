#include "memory/memory.hpp"

void Memory::Initialize()
{
    for (uint32_t i = 0; i < MAX_MEM; i++)
    {
        Data[i] = 0;
    }
}

void Memory::WriteWord(uint32_t Address, Word Date)
{
    Data[Address] = Date & 0xFF;
    Data[Address + 1] = (Date >> 8) & 0xFF;
}