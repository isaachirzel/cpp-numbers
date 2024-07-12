#include "test/utils.hpp"
#include <iomanip>
#include <iostream>
#include <stdexcept>

#define RED		"\033[1;31m"
#define GREEN	"\033[1;32m"
#define YELLOW	"\033[1;33m"
#define RESET	"\033[0m"

namespace test
{
	// TODO: Multi threading

	static bool isAllTestingPassed = true;
	static bool isSectionPassed = true;
	static bool isRunningSection = false;

	void confirm(bool condition, const char *message)
	{
		if (!condition)
			throw std::runtime_error(message);
	}

	void section(const char *subject, std::function<void()>&& function)
	{
		std::cout << "Running tests for `" YELLOW << subject << RESET "`...\n";

		isRunningSection = true;
		isSectionPassed = true;

		function();

		isRunningSection = false;

		const char *verb = isSectionPassed
			? "\033[1;32mpassed\033[0m"
			: "\033[1;31mfailed\033[0m";

		std::cout << "Testing for `" YELLOW << subject << RESET "` " << verb << ".\n";
	}

	static void pass()
	{
		std::cout << "\b\b\b" GREEN "PASS" RESET;
	}

	static void fail()
	{
		std::cout << "\b\b\b" RED "FAIL" RESET;

		isSectionPassed = false;
		isAllTestingPassed = false;
	}

	void run(const char* name, std::function<void()>&& function)
	{
		std::cout << std::left << std::setfill(' ') << std::setw(40) << name;
		std::cout << "...";
		std::cout.flush();

		try
		{
			function();
			pass();
		}
		catch (const std::exception& e)
		{
			fail();
			std::cout << "\n\t" RED "Exception" RESET ": " << e.what();
		}

		std::cout << std::endl;
	}
}
