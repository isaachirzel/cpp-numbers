#include "test/test_uint128.hpp"
#include "test/test_int128.hpp"
#include "test/test_fixed128.hpp"
#include "test/utils.hpp"

int main()
{
	test::section("Fixed128", test::test_fixed128);

	if (!test::isTestingSuccessful())
		return 1;

	return 0;
}
