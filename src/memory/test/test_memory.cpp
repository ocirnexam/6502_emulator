#include <unity.h>

#include "memory/memory.hpp"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_memory_init(void)
{
    Memory mem;
    mem.Initialize();
    for(int i = 0; i < MAX_MEM; i++)
    {
        TEST_ASSERT_EQUAL(0, mem[i]);
    }
}

void test_memory_set_byte(void)
{
    Memory mem;
    mem.Initialize();
    mem[0x100] = 0x7;
    TEST_ASSERT_EQUAL(0x7, mem[0x100]);

    mem[0x101] = 0x1111; // implicit cast wanted!
    TEST_ASSERT_NOT_EQUAL_UINT16(0x1111, mem[0x101]);
}

void test_memory_set_word(void)
{
    Memory mem;
    mem.Initialize();
    mem.WriteWord(0x100, 0x1122);
    TEST_ASSERT_EQUAL(0x1122, mem[0x100] | (mem[0x101] << 8));

    mem.WriteWord(0x101, 0x10);
    TEST_ASSERT_EQUAL(0x10, mem[0x101] | (mem[0x102] << 8));
}

int main(void)
{
	UNITY_BEGIN();

	// TESTS
	RUN_TEST(test_memory_init);
	RUN_TEST(test_memory_set_byte);
	RUN_TEST(test_memory_set_word);

	UNITY_END();

	return 0;
}