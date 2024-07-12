#include "hirzel/numbers/U128.hpp"
#include "test/utils.hpp"
#include <limits>

using namespace hirzel::numbers;

namespace test
{
	static void test_add()
	{
		U128::clearOverflow();

		auto i2 = U128::max + U128::max;

		confirm(i2.hasOverflown(), "Overflow did not occur when adding max to max.");
	}

	static void test_subtract()
	{
	}

	static void test_multiply()
	{
	}

	static void test_divide()
	{
	}

	static void test_add_u32()
	{
		U128::clearOverflow();

		auto a = U128::max + uint32_t(0);

		confirm(!a.hasOverflown(), "Overflow occurred when adding 0 to max.");

		auto b = U128::max + uint32_t(1);

		confirm(b.hasOverflown(), "Overflow did not occur when adding 1 to max.");

		U128::clearOverflow();
	}

	static void test_subtract_u32()
	{
		U128::clearOverflow();

		auto a = U128() - uint32_t(0);
	}

	static void test_multiply_u32()
	{
	}

	static void test_divide_u32()
	{
	}

	void test_u128()
	{
		run("Add", test_add);
		run("Subtract", test_subtract);
		run("Multiply", test_multiply);
		run("Divide", test_divide);

		run("Add u32", test_add_u32);
		run("Subtract u32", test_subtract_u32);
		run("Multiply u32", test_multiply_u32);
		run("Divide u32", test_divide_u32);
	}
}
