#include "cpu/cpu.hpp"

void CPU::Reset(Memory &memory)
{
    PC = 0xFFFC;
    SP = 0x0100;
    PS &= 0xFF; // Clear Processor Status register
    A = X = Y = 0;
    memory.Initialize();
}

Byte CPU::FetchByte(u32 &SysTicks, Memory &memory)
{
    Byte Data = memory[PC];
    PC++;
    SysTicks--;
    return Data;
}

Word CPU::FetchWord(u32 &SysTicks, Memory&memory)
{
    Byte DataLow = memory[PC++];
    Byte DataHigh = memory[PC++];
    SysTicks -= 2;
    Word Data = DataHigh << 8 | DataLow;
    return Data;

}

Byte CPU::ReadByte(u32 &SysTicks, Memory &memory, Byte Address)
{
    Byte Data = memory[Address];
    SysTicks--;
    return Data;
}

Byte CPU::ReadByte(u32 &SysTicks, Memory &memory, Word Address)
{
    Byte Data = memory[Address];
    SysTicks--;
    return Data;
}

void CPU::LDSetStatus(LDRegisterType regType)
{
    // Zero Flag has to be set if reg == 0
    Byte reg;
    switch(regType) {
        case LDRegisterType::A:
            reg = A;
            break;
        case LDRegisterType::X:
            reg = X;
            break;
        case LDRegisterType::Y:
            reg = Y;
            break;
    }
    if (reg == 0)
    {
        PS |= ZERO_FLAG;
    }
    else
    {
        PS &= ~ZERO_FLAG;
    }
    // Negative Flag has to be set if Bit 7 of reg is 1
    if (reg & 0x80) // 0b10000000
    {
        PS |= NEGATIVE_FLAG;
    }
    else
    {
        PS &= ~NEGATIVE_FLAG;
    }
}

void CPU::Execute(u32 SysTicks, Memory &memory)
{
    while (SysTicks > 0)
    {
        Byte Instruction = FetchByte(SysTicks, memory);
        switch (Instruction)
        {
        // begin - LDA
        case INS_LDA_IMMEDIATE:
        {
            A = FetchByte(SysTicks, memory);
            LDSetStatus(LDRegisterType::A);
        }
        break;
        case INS_LDA_ZEROPAGE:
        {
            Byte ZeroPageAddress = FetchByte(SysTicks, memory);
            A = ReadByte(SysTicks, memory, ZeroPageAddress);
            LDSetStatus(LDRegisterType::A);
        }
        break;
        case INS_LDA_ZEROPAGE_X:
        {
            Byte ZeroPageAddress = FetchByte(SysTicks, memory);
            ZeroPageAddress += X;
            SysTicks--; // Adding X is an instruction with one tick
            A = ReadByte(SysTicks, memory, ZeroPageAddress);
            LDSetStatus(LDRegisterType::A);
        }
        break;
        case INS_LDA_ABSOLUTE:
        {
            Word Address = FetchWord(SysTicks, memory);
            A = ReadByte(SysTicks, memory, Address);
            LDSetStatus(LDRegisterType::A);
        }
        break;
        case INS_LDA_ABSOLUTE_X:
        {
            Word Address = FetchWord(SysTicks, memory);
            Address += X;
            SysTicks--;
            A = ReadByte(SysTicks, memory, Address);
            LDSetStatus(LDRegisterType::A);
        }
        break;
        case INS_LDA_ABSOLUTE_Y:
        {
            Word Address = FetchWord(SysTicks, memory);
            Address += Y;
            SysTicks--;
            A = ReadByte(SysTicks, memory, Address);
            LDSetStatus(LDRegisterType::A);
        }
        break;
        case INS_LDA_INDIRECT_X:
        {
            Byte Address = FetchByte(SysTicks, memory);
            Address += X;
            SysTicks--;
            Byte LSB = ReadByte(SysTicks, memory, Address);
            Byte MSB = ReadByte(SysTicks, memory, (Byte) (Address + 1));
            A = ReadByte(SysTicks, memory, (Word) (MSB << 8 | LSB));
            LDSetStatus(LDRegisterType::A);
        }
        break;
        case INS_LDA_INDIRECT_Y:
        {
            Byte Address = FetchByte(SysTicks, memory);
            Byte LSB = ReadByte(SysTicks, memory, Address);
            Byte MSB = ReadByte(SysTicks, memory, (Byte) (Address + 1));
            Word TargetAddress = (MSB << 8 | LSB) + Y;
            SysTicks--;
            std::cout << std::hex << TargetAddress << std::endl;
            A = ReadByte(SysTicks, memory, TargetAddress);
            LDSetStatus(LDRegisterType::A);
        }
        break;
        // end - LDA
        // begin - LDX
        case INS_LDX_IMMEDIATE:
        {
            X = FetchByte(SysTicks, memory);
            LDSetStatus(LDRegisterType::X);
        }
        break;
        // end - LDX
        // begin - LDY
        case INS_LDY_IMMEDIATE:
        {
            Y = FetchByte(SysTicks, memory);
            LDSetStatus(LDRegisterType::Y);
        }
        break;
        // end - LDY
        case INS_JSR:
        {
            Word JumpAddress = FetchWord(SysTicks, memory);
            memory.WriteWord(SP, PC - 1);
            SP++;
            SysTicks -= 2;
            PC = JumpAddress;
            SysTicks--;
        }
        break;
        default:
            std::cout << "Instruction is not handled: " << std::hex << Instruction << "\n";
            return;
        }
    }
}

void CPU::PrintRegisters()
{
    std::cout << "------------------------------------\n";
    std::cout << "REGISTERS\n";
    std::cout << "------------------------------------\n";
    std::cout << "PC: 0x" << std::hex << (int)PC << "\n";
    std::cout << "SP: 0x" << std::hex << (int)SP << "\n";
    std::cout << "PS: 0x" << std::hex << (int)PS << "\n";
    std::cout << "A : 0x" << std::hex << (int)A << "\n";
    std::cout << "X : 0x" << std::hex << (int)X << "\n";
    std::cout << "Y : 0x" << std::hex << (int)Y << "\n";
    std::cout << "------------------------------------\n";
}