#include "hirzel/numbers/U128.hpp"
#include "test/utils.hpp"
#include <chrono>
#include <stdexcept>
#include <thread>

namespace test
{
	static bool isCallbackCalled = true;

	static void test_subtract_overflow()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		throw std::runtime_error("Houston we got a problem (nerd emoji)");
	}

	static void test_add_overflow()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	void test_u128()
	{
		hirzel::numbers::U128::setOverflowCallback([] (const auto&, const auto&, char)
		{
			isCallbackCalled = true;
		});

		run("Add overflow", test_add_overflow);
		run("Subtract overflow", test_subtract_overflow);
	}
}
