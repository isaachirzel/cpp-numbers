#include "test/test_decimal.hpp"
#include "hirzel/numbers/Decimal.hpp"
#include "test/utils.hpp"

using namespace hirzel::numbers;

namespace test
{
	void test_addition()
	{
		expect(Decimal(0) + Decimal(0), Decimal(0));
		expect(Decimal(0) + Decimal(1), Decimal(1));
		expect(Decimal(0) + Decimal(-1), Decimal(-1));
		expect(Decimal(-1) + Decimal(1), Decimal(0));
		expect(Decimal(3) + Decimal(2), Decimal(5));
		expect(Decimal(-3) + Decimal(2), Decimal(-1));
	}

	void test_subtraction()
	{
		expect(Decimal(0) - Decimal(0), Decimal(0));
		expect(Decimal(0) - Decimal(1), Decimal(-1));
		expect(Decimal(0) - Decimal(-1), Decimal(1));
		expect(Decimal(-1) - Decimal(-1), Decimal(0));
		expect(Decimal(1) - Decimal(1), Decimal(0));
		expect(Decimal(3) - Decimal(2), Decimal(1));
		expect(Decimal(-3) - Decimal(2), Decimal(-5));
	}

	void test_multiplication()
	{
		expect(Decimal(0) * Decimal(0), Decimal(0));
		expect(Decimal(1) * Decimal(0), Decimal(0));
		expect(Decimal(1) * Decimal(1), Decimal(1));
		expect(Decimal(1) * Decimal(-1), Decimal(-1));
		expect(Decimal(-1) * Decimal(-1), Decimal(1));
		expect(Decimal(-3) * Decimal(5), Decimal(-15));
		expect(Decimal(-3) * Decimal(-5), Decimal(15));
		expect(Decimal(200000000000000000) * Decimal(-10), Decimal(-2000000000000000000));
	}

	void test_division()
	{
		
	}

	void test_decimal()
	{
		run("Addition", test_addition);
		run("Subtraction", test_subtraction);
		run("Multiplication", test_multiplication);
		run("Division", test_division);
	}
}
