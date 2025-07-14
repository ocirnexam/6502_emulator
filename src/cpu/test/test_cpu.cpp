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

int main(void)
{
	UNITY_BEGIN();

	// TESTS
	RUN_TEST(test_cpu_reset);
	RUN_TEST(test_cpu_lda_immediate);
	RUN_TEST(test_cpu_lda_zeropage);
	RUN_TEST(test_cpu_lda_zeropage_x);

	UNITY_END();

	return 0;
}