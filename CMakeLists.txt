cmake_minimum_required(VERSION 3.20)

project(emulator_6502)

include_directories(src)

add_executable(emulator_6502 src/main_6502.cpp src/memory/memory.cpp src/cpu/cpu.cpp)