#include "cpu/cpu.hpp"

void CPU::Reset(Memory &memory)
{
    PC = 0xFFFC;
    SP = 0x0100;
    PS = 0x00; // Clear Processor Status register
    A = X = Y = 0;
    memory.Initialize();
}

void CPU::IncreaseSysTicks(uint32_t amount)
{
    SysTicks += amount;
}

Byte CPU::FetchByte(Memory &memory)
{
    Byte Data = memory[PC];
    PC++;
    IncreaseSysTicks(1);
    return Data;
}

Word CPU::FetchWord(Memory&memory)
{
    Byte DataLow = memory[PC++];
    Byte DataHigh = memory[PC++];
    IncreaseSysTicks(2);
    Word Data = DataHigh << 8 | DataLow;
    return Data;

}

Byte CPU::ReadByte(Memory &memory, Byte Address)
{
    Byte Data = memory[Address];
    IncreaseSysTicks(1);
    return Data;
}

Byte CPU::ReadByte(Memory &memory, Word Address)
{
    Byte Data = memory[Address];
    IncreaseSysTicks(1);
    return Data;
}

Word CPU::ReadWord(Memory &memory, Word Address)
{
    Word Data = memory[Address];
    Data = memory[Address + 1] << 8;
    IncreaseSysTicks(2);
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

void CPU::Execute(Memory &memory)
{
    char command = 0;
    Byte Instruction = 0;
    while (1)
    {
        system("clear");
        if (command == 's')
        {
            std::cout << "Step instruction: 0x" << std::hex << (int) Instruction << "\n";
        }
        else if (command == 'q')
        {
            break;
        }
        else if (command == 'r')
        {
            std::cout << "Run\n";
        }
        if (command != 'r')
        {
            PrintRegisters();
            std::cout << ">> ";
            std::cin >> command;
        }

        Instruction = FetchByte(memory);

        switch (Instruction)
        {
        // begin - LDA
        case INS_LDA_IMMEDIATE:
        {
            A = FetchByte(memory);
            LDSetStatus(LDRegisterType::A);
        }
        break;
        case INS_LDA_ZEROPAGE:
        {
            Byte ZeroPageAddress = FetchByte(memory);
            A = ReadByte(memory, ZeroPageAddress);
            LDSetStatus(LDRegisterType::A);
        }
        break;
        case INS_LDA_ZEROPAGE_X:
        {
            Byte ZeroPageAddress = FetchByte(memory);
            ZeroPageAddress += X;
            IncreaseSysTicks(1); // Adding X is an instruction with one tick
            A = ReadByte(memory, ZeroPageAddress);
            LDSetStatus(LDRegisterType::A);
        }
        break;
        case INS_LDA_ABSOLUTE:
        {
            Word Address = FetchWord(memory);
            A = ReadByte(memory, Address);
            LDSetStatus(LDRegisterType::A);
        }
        break;
        case INS_LDA_ABSOLUTE_X:
        {
            Word Address = FetchWord(memory);
            Address += X;
            IncreaseSysTicks(1);
            A = ReadByte(memory, Address);
            LDSetStatus(LDRegisterType::A);
        }
        break;
        case INS_LDA_ABSOLUTE_Y:
        {
            Word Address = FetchWord(memory);
            Address += Y;
            IncreaseSysTicks(1);
            A = ReadByte(memory, Address);
            LDSetStatus(LDRegisterType::A);
        }
        break;
        case INS_LDA_INDIRECT_X:
        {
            Byte Address = FetchByte(memory);
            Address += X;
            IncreaseSysTicks(1);
            Byte LSB = ReadByte(memory, Address);
            Byte MSB = ReadByte(memory, (Byte) (Address + 1));
            A = ReadByte(memory, (Word) (MSB << 8 | LSB));
            LDSetStatus(LDRegisterType::A);
        }
        break;
        case INS_LDA_INDIRECT_Y:
        {
            Byte Address = FetchByte(memory);
            Byte LSB = ReadByte(memory, Address);
            Byte MSB = ReadByte(memory, (Byte) (Address + 1));
            Word TargetAddress = (MSB << 8 | LSB) + Y;
            IncreaseSysTicks(1);
            A = ReadByte(memory, TargetAddress);
            LDSetStatus(LDRegisterType::A);
        }
        break;
        // end - LDA
        // begin - LDX
        case INS_LDX_IMMEDIATE:
        {
            X = FetchByte(memory);
            LDSetStatus(LDRegisterType::X);
        }
        break;
        case INS_LDX_ZEROPAGE:
        {
            Byte ZeroPageAddress = FetchByte(memory);
            X = ReadByte(memory, ZeroPageAddress);
            LDSetStatus(LDRegisterType::X);
        }
        break;
        // end - LDX
        // begin - LDY
        case INS_LDY_IMMEDIATE:
        {
            Y = FetchByte(memory);
            LDSetStatus(LDRegisterType::Y);
        }
        break;
        case INS_LDY_ZEROPAGE:
        {
            Byte ZeroPageAddress = FetchByte(memory);
            Y = ReadByte(memory, ZeroPageAddress);
            LDSetStatus(LDRegisterType::Y);
        }
        break;
        // end - LDY
        // begin - JUMP
        case INS_JSR:
        {
            Word Address = FetchWord(memory);
            memory.WriteWord(SP, PC - 1);
            SP++;
            IncreaseSysTicks(3);
            PC = Address;
        }
        break;
        case INS_JMP_ABSOLUTE:
        {
            PC = FetchWord(memory);
        }
        break;
        case INS_JMP_INDIRECT:
        {
            Word Address = FetchWord(memory);
            PC = ReadWord(memory, Address);

        }
        break;
        // end - JUMP
        default:
        if (Instruction == 0) {
            system("clear");
            PrintRegisters();
            std::cout << "Program Ended!" << std::endl;
        }
        else {
            std::cout << "Instruction is not handled: " << std::hex << Instruction << "\n";
        }
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
    std::cout << "PS: 0b" << std::dec << std::bitset<8>(PS) << ", Register Bits: [N V 1 B D I Z C]\n";
    std::cout << "A : 0x" << std::hex << (int)A << "\n";
    std::cout << "X : 0x" << std::hex << (int)X << "\n";
    std::cout << "Y : 0x" << std::hex << (int)Y << "\n";
    std::cout << "------------------------------------\n";
    std::cout << "SysTicks: " << std::dec << SysTicks << "\n";
    std::cout << "------------------------------------\n";
}