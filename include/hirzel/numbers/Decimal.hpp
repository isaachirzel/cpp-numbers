#ifndef HIRZEL_NUMBERS_DECIMAL_HPP
#define HIRZEL_NUMBERS_DECIMAL_HPP

#include "hirzel/numbers/U128.hpp"
#include "hirzel/numbers/I128.hpp"
#include <ostream>

namespace hirzel::numbers
{
	class Decimal
	{
		/*
			Layout:
				bit 1-127:	unsigned integer indicating number
				bit 128:	sign bit
		*/

		u128 _value;

	private:

		Decimal(const char* text, size_t length);
		i128 toInt() const;
		static Decimal from(const i128& value);
		static Decimal from(const char* text, size_t length);

	public:

		Decimal();
		explicit Decimal(const int integral);
		explicit Decimal(double value);
		Decimal(Decimal&&) = default;
		Decimal(const Decimal&) = default;
		Decimal& operator=(Decimal&&) = default;
		Decimal& operator=(const Decimal&) = default;

		static Decimal from(const char *text);
		static Decimal from(const std::string_view& text);
		static Decimal from(const std::string& text);

		double toFloat() const;
		Decimal abs() const;
		bool isNegative() const;
		bool isPositive() const;
		const auto& value() const { return _value; }

		Decimal operator+(const Decimal& other) const;
		Decimal operator-(const Decimal& other) const;
		Decimal operator*(const Decimal& other) const;
		Decimal operator/(const Decimal& other) const;

		Decimal& operator+=(const Decimal& other);
		Decimal& operator-=(const Decimal& other);
		Decimal& operator*=(const Decimal& other);
		Decimal& operator/=(const Decimal& other);

		Decimal operator-() const;

		operator bool() const;

		bool operator==(const Decimal& other) const;
		bool operator!=(const Decimal& other) const;

		friend std::ostream& operator<<(std::ostream& out, const Decimal& decimal);
		friend Decimal operator ""_dec(const char *text, size_t length);
		friend Decimal operator ""_dec(unsigned long long value);
		friend Decimal operator ""_dec(long double value);
	};

	Decimal operator ""_dec(const char *text, size_t length);
	Decimal operator ""_dec(unsigned long long value);
	Decimal operator ""_dec(long double value);
}

#endif
