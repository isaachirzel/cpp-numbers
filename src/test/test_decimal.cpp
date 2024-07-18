#include "test/test_decimal.hpp"
#include "hirzel/numbers/Decimal.hpp"
#include "test/utils.hpp"

using namespace hirzel::numbers;

namespace test
{
	void test_addition()
	{
		expect(0_dec + 0_dec, 0_dec);
		expect(0_dec + 1_dec, 1_dec);
		expect(0_dec + -1_dec, -1_dec);
		expect(-1_dec + 1_dec, 0_dec);
		expect(3_dec + 2_dec, 5_dec);
		expect(-3_dec + 2_dec, -1_dec);
	}

	void test_subtraction()
	{
		expect(0_dec - 0_dec, 0_dec);
		expect(0_dec - 1_dec, -1_dec);
		expect(0_dec - -1_dec, 1_dec);
		expect(-1_dec - -1_dec, 0_dec);
		expect(1_dec - 1_dec, 0_dec);
		expect(3_dec - 2_dec, 1_dec);
		expect(-3_dec - 2_dec, -5_dec);
	}

	void test_multiplication()
	{
		expect(0_dec * 0_dec, 0_dec);
		expect(1_dec * 0_dec, 0_dec);
		expect(1_dec * 1_dec, 1_dec);
		expect(1_dec * -1_dec, -1_dec);
		expect(-1_dec * -1_dec, 1_dec);
		expect(-3_dec * 5_dec, -15_dec);
		expect(-3_dec * -5_dec, 15_dec);
		expect(200000000000000000_dec * -10_dec, -2000000000000000000_dec);
	}

	void test_division()
	{
		expect(0_dec / 1_dec, 0_dec);
		expect(1_dec / 1_dec, 1_dec);
		expect(2_dec / 1_dec, 2_dec);
		expect(2_dec / 2_dec, 1_dec);
		expect(2_dec / -2_dec, -1_dec);
		expect(2.5_dec / 2_dec, 1.25_dec);
		expect(22_dec / 7_dec, "3.1428571428571428571"_dec);
		expect(3.75_dec / 3_dec, 1.25_dec);
		expect(3.75_dec / 1.25_dec, 3_dec);
		expect(3.75_dec / 0.01_dec, 375_dec);
	}

	void test_decimal()
	{
		// TODO: Test literals
		// TODO: Test constructors
		run("Addition", test_addition);
		run("Subtraction", test_subtraction);
		run("Multiplication", test_multiplication);
		run("Division", test_division);
	}
}
