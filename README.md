# Emulator for the 6502 Microprocessor

## Description

The Emulation of the 6502 can run all Assembly Instructions the real 6502 can execute. Currently, a program has to be loaded directly in the memory addresses and all data too.
This is quite inconvenient, so in the future, another method to load the assembly should be provided.

## Current State

Implemented Functions:
- LDA (all addressing types)
- LDX (all addressing types)
- LDY (all addressing types)
- LSR (Accumulator)
- JMP (direct & indirect)
- JSR (no return instruction yet)

## Usage

When the emulation runs, an command promt is shown below the current register status.
You can choose between:
- s (step) to step one instruction
- r (run) to finish the program
- q (quit) to exit the program
