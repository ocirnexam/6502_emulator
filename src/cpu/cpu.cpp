#include "cpu/cpu.hpp"

void CPU::Reset()
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

Byte CPU::FetchByte()
{
    Byte Data = memory[PC];
    PC++;
    IncreaseSysTicks(1);
    return Data;
}

Word CPU::FetchWord()
{
    Byte DataLow = memory[PC++];
    Byte DataHigh = memory[PC++];
    IncreaseSysTicks(2);
    Word Data = DataHigh << 8 | DataLow;
    return Data;
}

Byte CPU::ReadByte(Byte Address)
{
    Byte Data = memory[Address];
    IncreaseSysTicks(1);
    return Data;
}

Byte CPU::ReadByte(Word Address)
{
    Byte Data = memory[Address];
    IncreaseSysTicks(1);
    return Data;
}

Word CPU::ReadWord(Word Address)
{
    Word Data = memory[Address];
    Data = memory[Address + 1] << 8;
    IncreaseSysTicks(2);
    return Data;
}

void CPU::SetStatus(RegisterType regType)
{
    // Zero Flag has to be set if reg == 0
    Byte reg;
    switch (regType)
    {
    case RegisterType::A:
        reg = A;
        break;
    case RegisterType::X:
        reg = X;
        break;
    case RegisterType::Y:
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

void CPU::InstructionLDA(Byte Instruction)
{
    switch (Instruction)
    {
    // begin - LDA
    case INS_LDA_IMMEDIATE:
    {
        A = FetchByte();
        SetStatus(RegisterType::A);
    }
    break;
    case INS_LDA_ZEROPAGE:
    {
        Byte ZeroPageAddress = FetchByte();
        A = ReadByte(ZeroPageAddress);
        SetStatus(RegisterType::A);
    }
    break;
    case INS_LDA_ZEROPAGE_X:
    {
        Byte ZeroPageAddress = FetchByte();
        ZeroPageAddress += X;
        IncreaseSysTicks(1); // Adding X is an instruction with one tick
        A = ReadByte(ZeroPageAddress);
        SetStatus(RegisterType::A);
    }
    break;
    case INS_LDA_ABSOLUTE:
    {
        Word Address = FetchWord();
        A = ReadByte(Address);
        SetStatus(RegisterType::A);
    }
    break;
    case INS_LDA_ABSOLUTE_X:
    {
        Word Address = FetchWord();
        Address += X;
        IncreaseSysTicks(1);
        A = ReadByte(Address);
        SetStatus(RegisterType::A);
    }
    break;
    case INS_LDA_ABSOLUTE_Y:
    {
        Word Address = FetchWord();
        Address += Y;
        IncreaseSysTicks(1);
        A = ReadByte(Address);
        SetStatus(RegisterType::A);
    }
    break;
    case INS_LDA_INDIRECT_X:
    {
        Byte Address = FetchByte();
        Address += X;
        IncreaseSysTicks(1);
        Byte LSB = ReadByte(Address);
        Byte MSB = ReadByte((Byte)(Address + 1));
        A = ReadByte((Word)(MSB << 8 | LSB));
        SetStatus(RegisterType::A);
    }
    break;
    case INS_LDA_INDIRECT_Y:
    {
        Byte Address = FetchByte();
        Byte LSB = ReadByte(Address);
        Byte MSB = ReadByte((Byte)(Address + 1));
        Word TargetAddress = (MSB << 8 | LSB) + Y;
        IncreaseSysTicks(1);
        A = ReadByte(TargetAddress);
        SetStatus(RegisterType::A);
    }
    break;
    default:
        break;
    }
}

void CPU::InstructionLDX(Byte Instruction)
{
    switch (Instruction)
    {
    case INS_LDX_IMMEDIATE:
    {
        X = FetchByte();
        SetStatus(RegisterType::X);
    }
    break;
    case INS_LDX_ZEROPAGE:
    {
        Byte ZeroPageAddress = FetchByte();
        X = ReadByte(ZeroPageAddress);
        SetStatus(RegisterType::X);
    }
    break;
    case INS_LDX_ZEROPAGE_Y:
    {
        Byte ZeroPageAddress = FetchByte();
        ZeroPageAddress += Y;
        IncreaseSysTicks(1);
        X = ReadByte(ZeroPageAddress);
        SetStatus(RegisterType::X);
    }
    break;
    case INS_LDX_ABSOLUTE:
    {
        Word Address = FetchWord();
        X = ReadByte(Address);
        SetStatus(RegisterType::X);
    }
    break;
    case INS_LDX_ABSOLUTE_Y:
    {
        Word Address = FetchWord();
        Address += Y;
        IncreaseSysTicks(1);
        X = ReadByte(Address);
        SetStatus(RegisterType::X);
    }
    break;
    default:
        break;
    }
}

void CPU::InstructionLDY(Byte Instruction)
{
    switch (Instruction)
    {
    case INS_LDY_IMMEDIATE:
    {
        Y = FetchByte();
        SetStatus(RegisterType::Y);
    }
    break;
    case INS_LDY_ZEROPAGE:
    {
        Byte ZeroPageAddress = FetchByte();
        Y = ReadByte(ZeroPageAddress);
        SetStatus(RegisterType::Y);
    }
    break;
    case INS_LDY_ZEROPAGE_X:
    {
        Byte ZeroPageAddress = FetchByte();
        ZeroPageAddress += X;
        IncreaseSysTicks(1);
        Y = ReadByte(ZeroPageAddress);
        SetStatus(RegisterType::Y);
    }
    break;
    case INS_LDY_ABSOLUTE:
    {
        Word Address = FetchWord();
        Y = ReadByte(Address);
        SetStatus(RegisterType::Y);
    }
    break;
    case INS_LDY_ABSOLUTE_X:
    {
        Word Address = FetchWord();
        Address += X;
        IncreaseSysTicks(1);
        Y = ReadByte(Address);
        SetStatus(RegisterType::Y);
    }
    break;
    default:
        break;
    }
}

void CPU::InstructionLSR(Byte Instruction)
{
    switch (Instruction)
    {
    case INS_LSR_A:
    {
        PS = 0;
        PS |= (A & 1);
        A >>= 1;
        IncreaseSysTicks(1);
        SetStatus(RegisterType::A);
    }
    break;
    case INS_LSR_ZEROPAGE:
    {
        PS = 0;
        Byte ZeroPageAddress = FetchByte();
        Byte ShiftContent = ReadByte(ZeroPageAddress);
        PS |= (ShiftContent & 1);
        ShiftContent >>= 1;
        IncreaseSysTicks(1);
        memory[ZeroPageAddress] = ShiftContent;
        IncreaseSysTicks(1);
    }
    break;
    case INS_LSR_ZEROPAGE_X:
    {
        PS = 0;
        Byte ZeroPageAddress = FetchByte() + X;
        IncreaseSysTicks(1);
        Byte ShiftContent = ReadByte(ZeroPageAddress);
        PS |= (ShiftContent & 1);
        ShiftContent >>= 1;
        IncreaseSysTicks(1);
        memory[ZeroPageAddress] = ShiftContent;
        IncreaseSysTicks(1);
    }
    break;
    case INS_LSR_ABSOLUTE:
    {
        PS = 0;
        Byte Address = FetchWord();
        Byte ShiftContent = ReadByte(Address);
        PS |= (ShiftContent & 1);
        ShiftContent >>= 1;
        IncreaseSysTicks(1);
        memory.WriteWord(Address, ShiftContent);
        IncreaseSysTicks(1);
    }
    break;
    case INS_LSR_ABSOLUTE_X:
    {
        PS = 0;
        Byte Address = FetchWord() + X;
        IncreaseSysTicks(1);
        Byte ShiftContent = ReadByte(Address);
        PS |= (ShiftContent & 1);
        ShiftContent >>= 1;
        IncreaseSysTicks(1);
        memory.WriteWord(Address, ShiftContent);
        IncreaseSysTicks(1);
    }
    break;
    default:
        break;
    }
}

void CPU::Execute()
{
    char command = 0;
    Byte Instruction = 0;
    while (1)
    {
        system("clear");
        if (command == 's')
        {
            std::cout << "Step instruction: 0x" << std::hex << (int)Instruction << "\n";
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

        Instruction = FetchByte();

        switch (Instruction)
        {
        // begin - LDA
        case INS_LDA_IMMEDIATE:
        case INS_LDA_ZEROPAGE:
        case INS_LDA_ZEROPAGE_X:
        case INS_LDA_ABSOLUTE:
        case INS_LDA_ABSOLUTE_X:
        case INS_LDA_ABSOLUTE_Y:
        case INS_LDA_INDIRECT_X:
        case INS_LDA_INDIRECT_Y:
        {
            InstructionLDA(Instruction);
        }
        break;
        // end - LDA
        // begin - LDX
        case INS_LDX_IMMEDIATE:
        case INS_LDX_ZEROPAGE:
        case INS_LDX_ZEROPAGE_Y:
        case INS_LDX_ABSOLUTE:
        case INS_LDX_ABSOLUTE_Y:
        {
            InstructionLDX(Instruction);
        }
        break;
        // end - LDX
        // begin - LDY
        case INS_LDY_IMMEDIATE:
        case INS_LDY_ZEROPAGE:
        case INS_LDY_ZEROPAGE_X:
        case INS_LDY_ABSOLUTE:
        case INS_LDY_ABSOLUTE_X:
        {
            InstructionLDY(Instruction);
        }
        break;
        // end - LDY
        // begin - LSR
        case INS_LSR_A:
        case INS_LSR_ZEROPAGE:
        case INS_LSR_ZEROPAGE_X:
        case INS_LSR_ABSOLUTE:
        case INS_LSR_ABSOLUTE_X:
        {
            InstructionLSR(Instruction);
        }
        break;
        // end - LSR
        // begin - JUMP
        case INS_JSR:
        {
            Word Address = FetchWord();
            memory.WriteWord(SP, PC - 1);
            SP++;
            IncreaseSysTicks(3);
            PC = Address;
        }
        break;
        case INS_JMP_ABSOLUTE:
        {
            PC = FetchWord();
        }
        break;
        case INS_JMP_INDIRECT:
        {
            Word Address = FetchWord();
            PC = ReadWord(Address);
        }
        break;
        // end - JUMP
        default:
            if (Instruction == 0)
            {
                system("clear");
                PrintRegisters();
                std::cout << "Program Ended!" << std::endl;
            }
            else
            {
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