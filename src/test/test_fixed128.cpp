#include "test/test_fixed128.hpp"
#include "hirzel/numbers/Fixed128.hpp"
#include "test/utils.hpp"

using namespace hirzel::numbers;

namespace test
{
	static void test_addition()
	{
		expect(0_dec + 0_dec, 0_dec);
		expect(0_dec + 1_dec, 1_dec);
		expect(0_dec + -1_dec, -1_dec);
		expect(-1_dec + 1_dec, 0_dec);
		expect(3_dec + 2_dec, 5_dec);
		expect(-3_dec + 2_dec, -1_dec);
		
		expect(0_dec += 0_dec, 0_dec);
		expect(0_dec += 1_dec, 1_dec);
		expect(0_dec += -1_dec, -1_dec);
		expect(-1_dec += 1_dec, 0_dec);
		expect(3_dec += 2_dec, 5_dec);
		expect(-3_dec += 2_dec, -1_dec);
	}

	static void test_subtraction()
	{
		expect(0_dec - 0_dec, 0_dec);
		expect(0_dec - 1_dec, -1_dec);
		expect(0_dec - -1_dec, 1_dec);
		expect(-1_dec - -1_dec, 0_dec);
		expect(1_dec - 1_dec, 0_dec);
		expect(3_dec - 2_dec, 1_dec);
		expect(-3_dec - 2_dec, -5_dec);

		expect(0_dec -= 0_dec, 0_dec);
		expect(0_dec -= 1_dec, -1_dec);
		expect(0_dec -= -1_dec, 1_dec);
		expect(-1_dec -= -1_dec, 0_dec);
		expect(1_dec -= 1_dec, 0_dec);
		expect(3_dec -= 2_dec, 1_dec);
		expect(-3_dec -= 2_dec, -5_dec);
	}

	static void test_multiplication()
	{
		expect(0_dec * 0_dec, 0_dec);
		expect(1_dec * 0_dec, 0_dec);
		expect(1_dec * 1_dec, 1_dec);
		expect(1_dec * -1_dec, -1_dec);
		expect(-1_dec * -1_dec, 1_dec);
		expect(-3_dec * 5_dec, -15_dec);
		expect(-3_dec * -5_dec, 15_dec);
		expect(200000000000000000_dec * -10_dec, -2000000000000000000_dec);
		expect(1_dec * 0.01_dec, "0.01"_dec);
		expect(100_dec * 1.07_dec, 107_dec);

		expect(0_dec *= 0_dec, 0_dec);
		expect(1_dec *= 0_dec, 0_dec);
		expect(1_dec *= 1_dec, 1_dec);
		expect(1_dec *= -1_dec, -1_dec);
		expect(-1_dec *= -1_dec, 1_dec);
		expect(-3_dec *= 5_dec, -15_dec);
		expect(-3_dec *= -5_dec, 15_dec);
		expect(200000000000000000_dec *= -10_dec, -2000000000000000000_dec);
		expect(1_dec *= 0.01_dec, "0.01"_dec);
		expect(100_dec * 1.07_dec, 107_dec);
	}

	static void test_division()
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
		expect(10_dec / 0.01_dec, 1000_dec);
		expect(10000000000_dec / "0.000000001"_dec, 10000000000000000000_dec);
		
		expect(0_dec /= 1_dec, 0_dec);
		expect(1_dec /= 1_dec, 1_dec);
		expect(2_dec /= 1_dec, 2_dec);
		expect(2_dec /= 2_dec, 1_dec);
		expect(2_dec /= -2_dec, -1_dec);
		expect(2.5_dec /= 2_dec, 1.25_dec);
		expect(22_dec /= 7_dec, "3.1428571428571428571"_dec);
		expect(3.75_dec /= 3_dec, 1.25_dec);
		expect(3.75_dec /= 1.25_dec, 3_dec);
		expect(3.75_dec /= 0.01_dec, 375_dec);
		expect(10_dec /= 0.01_dec, 1000_dec);
		expect(10000000000_dec /= "0.000000001"_dec, 10000000000000000000_dec);
	}

	static void test_exponent()
	{
		expect(1_dec ^ 2, 1_dec);
		expect(2_dec ^ 2, 4_dec);
		expect(10_dec ^ 2, 100_dec);
		expect(10_dec ^ 19, 10000000000000000000_dec);
		expect(1_dec ^ -1, 1_dec);

		expect(1_dec ^= 2, 1_dec);
		expect(2_dec ^= 2, 4_dec);
		expect(10_dec ^= 2, 100_dec);
		expect(10_dec ^= 19, 10000000000000000000_dec);
		expect(1_dec ^= -1, 1_dec);
	}

	void test_fixed128()
	{
		// TODO: Test literals
		// TODO: Test constructors
		run("Addition", test_addition);
		run("Subtraction", test_subtraction);
		run("Multiplication", test_multiplication);
		run("Division", test_division);
		run("Exponent", test_exponent);
	}
}
