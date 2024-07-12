#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <functional>

namespace test
{
	void confirm(bool condition, const char *error);
	void section(const char *subject, std::function<void()>&& function);
	void run(const char *name, std::function<void()>&& function);
}

#endif
