#include "test/utils.hpp"
#include <iomanip>
#include <iostream>

namespace test
{
	void section(const char *subject, std::function<void()>&& function)
	{
		std::cout << "Running tests for `" << subject << "`...\n";

		function();

		std::cout << "Testing for `" << subject << "` completed.\n";
	}

	static inline void result(const char *text)
	{
		std::cout << "\b\b\b" << text;
	}

	void run(const char* name, std::function<void()>&& function)
	{
		std::cout << std::left << std::setfill(' ') << std::setw(40) << name;
		std::cout << "...";
		std::cout.flush();

		try
		{
			function();
			result("PASS");
		}
		catch (const std::exception& e)
		{
			result("FAIL");
			std::cout << "\n\tException: " << e.what();
		}

		std::cout << std::endl;
	}
}
