# Emulator for the 6502 Microprocessor

## Description

This project is a complete emulator for the classic MOS Technology 6502 8-bit microprocessor, implemented in modern C++. The emulator supports the full official 6502 instruction set, including all addressing modes and status flag behaviors, and accurately replicates the original CPU's timing and logic.
Key Features

    ✅ Full implementation of all 56 (planned) official 6502 instructions

    🧠 Accurate simulation of registers: A, X, Y, SP, PC, and PS (status flags)

    📦 Emulation of memory access and memory-mapped I/O

    🛠 Written in C++

    🧪 Unit testing with the Unity Test Framework (planned)

    🔧 Designed for debugging and having fun deep-diving into CPU execution

6502 Architecture Highlights

    - 8-bit CPU with a 16-bit address bus (64KB memory address space)

    - Three general-purpose 8-bit registers: A (accumulator), X, and Y

    - Stack implemented via a fixed page (page 1: addresses $0100–$01FF) and 8-bit stack pointer

    - Rich set of addressing modes: immediate, zero-page, absolute, indexed, indirect, relative, etc.

    - Simplicity and efficiency made it a cornerstone of early computers (e.g., Apple II, Commodore 64, NES)

## Current State

Implemented Functions:
- LDA (all addressing types)
- LDX (all addressing types)
- LDY (all addressing types)
- LSR (all addressing types)
- JMP (direct & indirect)
- JSR (no return instruction yet)

## Usage

When the emulation runs, a command promt is shown below the current register status.
You can choose between:
- s (step) to step one instruction
- r (run) to finish the program
- q (quit) to exit the program
