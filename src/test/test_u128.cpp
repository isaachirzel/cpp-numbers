#include "hirzel/numbers/U128.hpp"
#include "test/utils.hpp"
#include <iostream>

using namespace hirzel::numbers;

namespace test
{
	const uint32_t u32Max = uint32_t(0xFFFFFFFF);

	static void test_add()
	{
		expect(U128::zero + U128::zero, U128::zero);
		expect(U128::zero + U128::one, U128::one);
		expect(U128::max + U128::one, U128::zero);
		expect(U128(1, 0) + U128(1, 0), U128(2, 0));
		expect(U128(0, 0xFFFFFFFFFFFFFFFF) + U128::one, U128(1, 0));
	}

	static void test_subtract()
	{
		expect(U128::zero - U128::zero, U128::zero);
		expect(U128::one - U128::zero, U128::one);
		expect(U128::zero - U128::one, U128::max);
		expect(U128(1, 0) - U128(1, 0), U128::zero);
		expect(U128(1, 0) - U128::one, U128(0, 0xFFFFFFFFFFFFFFFF));
	}

	static void test_multiply()
	{
		expect(U128::zero * U128::zero, U128::zero);
		expect(U128::zero * U128::one, U128::zero);
		expect(U128::one * U128::zero, U128::zero);
		expect(U128::one * U128::one, U128::one);
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

	void test_u128()
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
