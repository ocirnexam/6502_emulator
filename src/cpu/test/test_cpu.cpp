#include <unity.h>

#include "cpu/cpu.hpp"
#include "memory/memory.hpp"

CPU cpu;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_cpu_reset(void)
{
    cpu.Reset();
    // Registers set to 0
    TEST_ASSERT_EQUAL(0, cpu.A);
    TEST_ASSERT_EQUAL(0, cpu.X);
    TEST_ASSERT_EQUAL(0, cpu.Y);
    TEST_ASSERT_EQUAL(0, cpu.PS);
    TEST_ASSERT_EQUAL(0, cpu.SysTicks);

    // PC should be 0xFFFC
    TEST_ASSERT_EQUAL(0xFFFC, cpu.PC);

    // Memory should be initialized with 0
    for(int i = 0; i < MAX_MEM; i++)
    {
        TEST_ASSERT_EQUAL(0, cpu.memory[i]);
    }
}

void test_cpu_lda_immediate(void)
{
    cpu.Reset();
    Memory& memory = cpu.memory;
    memory[0xFFFC] = INS_LDA_IMMEDIATE;
    memory[0xFFFD] = 0x42;
    cpu.Execute();

    TEST_ASSERT_EQUAL(0x42, cpu.A);
    TEST_ASSERT_EQUAL(2, cpu.SysTicks);
}

void test_cpu_lda_zeropage(void)
{
    cpu.Reset();
    Memory& memory = cpu.memory;
    memory[0xFFFC] = INS_LDA_ZEROPAGE;
    memory[0xFFFD] = 0x42;
    memory[0x42] = 0x69;
    cpu.Execute();

    TEST_ASSERT_EQUAL(0x69, cpu.A);
    TEST_ASSERT_EQUAL(3, cpu.SysTicks);
}

void test_cpu_lda_zeropage_x(void)
{
    cpu.Reset();
    Memory& memory = cpu.memory;
    cpu.X = 5;
    memory[0xFFFC] = INS_LDA_ZEROPAGE_X;
    memory[0xFFFD] = 0x42;
    memory[0x47] = 0x11; // address will be 0x42 + X
    cpu.Execute();

    TEST_ASSERT_EQUAL(0x11, cpu.A);
    TEST_ASSERT_EQUAL(4, cpu.SysTicks);
}

void test_cpu_lda_absolute(void)
{
    cpu.Reset();
    Memory& memory = cpu.memory;
    memory[0xFFFC] = INS_LDA_ABSOLUTE;
    memory[0xFFFD] = 0x42;
    memory[0xFFFE] = 0x42;
    memory[0x4242] = 0x12;
    cpu.Execute();

    TEST_ASSERT_EQUAL(0x12, cpu.A);
    TEST_ASSERT_EQUAL(4, cpu.SysTicks);
}

void test_cpu_lda_absolute_x(void)
{
    cpu.Reset();
    Memory& memory = cpu.memory;
    cpu.X = 1;
    memory[0xFFFC] = INS_LDA_ABSOLUTE_X;
    memory[0xFFFD] = 0x42;
    memory[0xFFFE] = 0x42;
    memory[0x4243] = 0x15; // address will be 0x4242 + X
    cpu.Execute();

    TEST_ASSERT_EQUAL(0x15, cpu.A);
    TEST_ASSERT_EQUAL(5, cpu.SysTicks);
}

void test_cpu_lda_absolute_y(void)
{
    cpu.Reset();
    Memory& memory = cpu.memory;
    cpu.Y = 2;
    memory[0xFFFC] = INS_LDA_ABSOLUTE_Y;
    memory[0xFFFD] = 0x42;
    memory[0xFFFE] = 0x42;
    memory[0x4244] = 0x16; // address will be 0x4242 + Y
    cpu.Execute();

    TEST_ASSERT_EQUAL(0x16, cpu.A);
    TEST_ASSERT_EQUAL(5, cpu.SysTicks);
}

void test_cpu_lda_indirect_x(void)
{
    cpu.Reset();
    Memory& memory = cpu.memory;
    cpu.X = 5;
    memory[0xFFFC] = INS_LDA_INDIRECT_X;
    memory[0xFFFD] = 0x50; // read full address at 0x50 + X and 0x51 + X
    memory[0x55] = 0x17;
    memory[0x56] = 0x18;
    memory[0x1817] = 0x19;
    cpu.Execute();

    TEST_ASSERT_EQUAL(0x19, cpu.A);
    TEST_ASSERT_EQUAL(6, cpu.SysTicks);
}

void test_cpu_lda_indirect_y(void)
{
    cpu.Reset();
    Memory& memory = cpu.memory;
    cpu.Y = 7;
    memory[0xFFFC] = INS_LDA_INDIRECT_Y;
    memory[0xFFFD] = 0x51; // read full address at 0x51 and 0x52
    memory[0x51] = 0x20;
    memory[0x52] = 0x21;
    memory[0x2127] = 0x22; // full address + Y
    cpu.Execute();

    TEST_ASSERT_EQUAL(0x22, cpu.A);
    TEST_ASSERT_EQUAL(6, cpu.SysTicks);
}

int main(void)
{
	UNITY_BEGIN();

	// TESTS
	RUN_TEST(test_cpu_reset);
    // LDA Tests
	RUN_TEST(test_cpu_lda_immediate);
	RUN_TEST(test_cpu_lda_zeropage);
	RUN_TEST(test_cpu_lda_zeropage_x);
    RUN_TEST(test_cpu_lda_absolute);
    RUN_TEST(test_cpu_lda_absolute_x);
    RUN_TEST(test_cpu_lda_absolute_y);
    RUN_TEST(test_cpu_lda_indirect_x);
    RUN_TEST(test_cpu_lda_indirect_y);

	UNITY_END();

	return 0;
}