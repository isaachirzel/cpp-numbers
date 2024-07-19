#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <functional>
#include <sstream>

namespace test
{
	void _confirm(bool condition, const char *error);
	#define confirm(expr, message) _confirm(expr, #expr ": " message)
	#define expect(expr, expected) do\
	{\
		auto result = (expr);\
		if (result != (expected))\
		{\
			auto out = std::stringstream();\
			out << (#expr ": Expected `");\
			out << (expected);\
			out << "`, got `";\
			out << result;\
			out << "`.";\
			throw std::runtime_error(out.str());\
		}\
	} while(false)

	void section(const char *subject, std::function<void()>&& function);
	void run(const char *name, std::function<void()>&& function);
	bool isTestingSuccessful();
}

#endif
