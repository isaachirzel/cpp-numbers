#include "hirzel/numbers/U128.hpp"
#include "test/utils.hpp"

using namespace hirzel::numbers;

namespace test
{
	#define confirmOverflow(expr) do\
	{\
		U128::clearOverflow();\
		auto i = (expr);\
		auto overflow = i.hasOverflown();\
		U128::clearOverflow();\
		confirm(overflow, "Overflow did not occur in `" #expr "`");\
	} while(false)

	#define confirmNoOverflow(expr) do\
	{\
		U128::clearOverflow();\
		auto i = (expr);\
		auto overflow = i.hasOverflown();\
		U128::clearOverflow();\
		confirm(!overflow, "Overflow occurred in `" #expr "`");\
	} while(false)

	const uint32_t u32Max = uint32_t(0xFFFFFFFF);

	static void test_add()
	{
		confirmNoOverflow(U128::zero + U128::zero);
		confirmNoOverflow(U128::max + U128::zero);
		confirmOverflow(U128::max + U128::one);
		confirmOverflow(U128::max + U128::max);
	}

	static void test_subtract()
	{
		confirmNoOverflow(U128::zero - U128::zero);
		confirmOverflow(U128::zero - U128::one);
		confirmNoOverflow(U128::max - U128::max);
		confirmOverflow(U128::zero - U128::max);
	}

	static void test_multiply()
	{
		confirmNoOverflow(U128::zero * U128::zero);
		confirmNoOverflow(U128::zero * U128::one);
		confirmNoOverflow(U128::zero * U128::max);

		confirmNoOverflow(U128::one * U128::zero);
		confirmNoOverflow(U128::one * U128::one);
		confirmNoOverflow(U128::one * U128::max);

		confirmNoOverflow(U128::max * U128::zero);
		confirmNoOverflow(U128::max * U128::one);
		confirmOverflow(U128::max * U128::max);
	}

	static void test_divide()
	{
		confirmOverflow(U128::zero / U128::zero);
		confirmNoOverflow(U128::zero / U128::one);
		confirmNoOverflow(U128::zero / U128::max);

		confirmOverflow(U128::one / U128::zero);
		confirmNoOverflow(U128::one / U128::one);
		confirmNoOverflow(U128::one / U128::max);

		confirmOverflow(U128::max / U128::zero);
		confirmNoOverflow(U128::max / U128::one);
		confirmNoOverflow(U128::max / U128::max);
	}

	static void test_add_u32()
	{
		confirmNoOverflow(U128::zero + uint32_t(0));
		confirmNoOverflow(U128::max + uint32_t(0));
		confirmOverflow(U128::max + uint32_t(1));
		confirmOverflow(U128::max + u32Max);
	}

	static void test_subtract_u32()
	{
		confirmNoOverflow(U128::zero - uint32_t(0));
		confirmOverflow(U128::zero - uint32_t(1));
		confirmNoOverflow(U128::max - uint32_t(1));
		confirmOverflow(U128::zero - u32Max);
	}

	static void test_multiply_u32()
	{
		confirmNoOverflow(U128::zero * uint32_t(0));
		confirmNoOverflow(U128::zero * uint32_t(1));
		confirmNoOverflow(U128::zero * u32Max);

		confirmNoOverflow(U128::one * uint32_t(0));
		confirmNoOverflow(U128::one * uint32_t(1));
		confirmNoOverflow(U128::one * u32Max);

		confirmNoOverflow(U128::max * uint32_t(0));
		confirmNoOverflow(U128::max * uint32_t(1));
		confirmOverflow(U128::max * u32Max);
	}

	static void test_divide_u32()
	{
	}


	void test_u128()
	{
		// TODO: add % operator

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
