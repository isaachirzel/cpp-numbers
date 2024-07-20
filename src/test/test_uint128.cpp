#include "hirzel/numbers/UInt128.hpp"
#include "test/utils.hpp"
#include <iostream>

using namespace hirzel::numbers;

namespace test
{
	const uint32_t u32Max = uint32_t(0xFFFFFFFF);

	static void test_add()
	{
		expect(UInt128::zero + UInt128::zero, UInt128::zero);
		expect(UInt128::zero + UInt128::one, UInt128::one);
		expect(UInt128::max + UInt128::one, UInt128::zero);
		expect(UInt128(1, 0) + UInt128(1, 0), UInt128(2, 0));
		expect(UInt128(0, 0xFFFFFFFFFFFFFFFF) + UInt128::one, UInt128(1, 0));
	}

	static void test_subtract()
	{
		expect(UInt128::zero - UInt128::zero, UInt128::zero);
		expect(UInt128::one - UInt128::zero, UInt128::one);
		expect(UInt128::zero - UInt128::one, UInt128::max);
		expect(UInt128(1, 0) - UInt128(1, 0), UInt128::zero);
		expect(UInt128(1, 0) - UInt128::one, UInt128(0, 0xFFFFFFFFFFFFFFFF));
	}

	static void test_multiply()
	{
		expect(UInt128::zero * UInt128::zero, UInt128::zero);
		expect(UInt128::zero * UInt128::one, UInt128::zero);
		expect(UInt128::one * UInt128::zero, UInt128::zero);
		expect(UInt128::one * UInt128::one, UInt128::one);
	}

	static void test_divide()
	{
	}

	static void test_modulus()
	{
	}


	static void test_add_u64()
	{
	}

	static void test_subtract_u64()
	{
	}

	static void test_multiply_u64()
	{
	}

	static void test_divide_u64()
	{
	}

	static void test_modulus_u64()
	{
	}


	static void test_add_u32()
	{
	}

	static void test_subtract_u32()
	{
	}

	static void test_multiply_u32()
	{

	}

	static void test_divide_u32()
	{
	}

	static void test_modulus_u32()
	{
	}

	void test_uint128()
	{
		// TODO: add % operator

		run("Add", test_add);
		run("Subtract", test_subtract);
		run("Multiply", test_multiply);
		run("Divide", test_divide);
		run("Modulus", test_modulus);

		run("Add u64", test_add_u64);
		run("Subtract u64", test_subtract_u64);
		run("Multiply u64", test_multiply_u64);
		run("Divide u64", test_divide_u64);
		run("Modulus u64", test_modulus_u64);

		run("Add u32", test_add_u32);
		run("Subtract u32", test_subtract_u32);
		run("Multiply u32", test_multiply_u32);
		run("Divide u32", test_divide_u32);
		run("Modulus u32", test_modulus_u32);
	}
}
