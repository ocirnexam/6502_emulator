#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include <iostream>
#include <cassert>

#include "datatypes.hpp"

struct Memory
{
    static constexpr uint32_t MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];
    void Initialize();
    void WriteWord(uint32_t Address, Word Date);

    Byte operator[]( uint32_t Address ) const
    {
        assert(Address < MAX_MEM);
        return Data[Address];
    }
    Byte& operator[](uint32_t Address)
    {
        assert(Address < MAX_MEM);
        return Data[Address];
    }
};

#endif