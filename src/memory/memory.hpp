#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include <iostream>
#include <cassert>

#include "datatypes.hpp"

struct Memory
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];
    void Initialize();
    void WriteWord(u32 Address, Word Date);

    Byte operator[]( u32 Address ) const
    {
        assert(Address < MAX_MEM);
        return Data[Address];
    }
    Byte& operator[](u32 Address)
    {
        assert(Address < MAX_MEM);
        return Data[Address];
    }
};

#endif