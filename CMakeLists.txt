cmake_minimum_required(VERSION 3.20)

SET(CXX_STANDARD 23)

project(emulator_6502)

add_subdirectory(src/Unity)

add_executable(emulator_6502 src/main_6502.cpp src/memory/memory.cpp src/cpu/cpu.cpp)
target_include_directories(emulator_6502 PUBLIC src)
target_compile_definitions(emulator_6502 PUBLIC VISUAL_DEBUG)

# Tests
add_executable(test_memory src/memory/test/test_memory.cpp
			               src/memory/memory.cpp)
target_link_libraries(test_memory unity)
target_include_directories(test_memory PUBLIC src
						                      src/memory
						                      src/Unity)

add_executable(test_cpu src/cpu/test/test_cpu.cpp
			            src/cpu/cpu.cpp
		       	        src/memory/memory.cpp)
target_link_libraries(test_cpu unity)
target_include_directories(test_cpu PUBLIC src
                                           src/cpu
                                           src/memory
                                           src/Unity)