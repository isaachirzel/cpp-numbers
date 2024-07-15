#ifndef HIRZEL_NUMBERS_DECIMAL_HPP
#define HIRZEL_NUMBERS_DECIMAL_HPP

#include "hirzel/numbers/U128.hpp"
#include "hirzel/numbers/I128.hpp"
#include <ostream>

namespace hirzel::numbers
{
	class Decimal
	{
		static const u128 maxValue;
		static const uint64_t decimalPlaces;
		static const uint64_t one;
		static const uint64_t maxIntegral;

		/*
			Layout:
				bit 1-127:	unsigned integer indicating number
				bit 128:	sign bit
		*/

		u128 _value;

	private:

		Decimal(unsigned long long value);
		Decimal(long double value);

		i128 toInt() const;
		static Decimal fromInt(const i128& value);

	public:

		Decimal();
		Decimal(int integral, unsigned mantissa);
		Decimal(int integral);
		Decimal(int64_t integral, uint64_t mantissa);
		Decimal(int64_t integral);
		Decimal(const char* text, size_t length);
		Decimal(const char* text);
		Decimal(const std::string_view& text);
		Decimal(const std::string& text);
		Decimal(double value);
		Decimal(Decimal&&) = default;
		Decimal(const Decimal&) = default;
		Decimal& operator=(Decimal&&) = default;
		Decimal& operator=(const Decimal&) = default;

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
