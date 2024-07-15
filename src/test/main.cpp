#include "test/test_u128.hpp"
#include "test/test_i128.hpp"
#include "test/test_decimal.hpp"
#include "test/utils.hpp"

int main()
{
	/*
		take the top bit and turn it into a bool
		so 
			1111 (-7)
			becomes
			0111 (7) and (true)

			generate 2s complement

			~value + 1

			ooor 

			multiply flag by 2 then subtract 1 to make it
			true or false to -1 or 1
			take 2s complement of the flag?

			multiply value by coefficient
	*/
	// test::section("U128", test::test_u128);
	// test::section("I128", test::test_i128);
	test::section("Decimal", test::test_decimal);

	if (!test::isTestingSuccessful())
		return 1;

	return 0;
}
