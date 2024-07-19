#ifndef HIRZEL_NUMBERS_DECIMAL_HPP
#define HIRZEL_NUMBERS_DECIMAL_HPP

#include "hirzel/numbers/UInt128.hpp"
#include "hirzel/numbers/Int128.hpp"
#include <ostream>

namespace hirzel::numbers
{
	class Fixed128
	{
		/*
			Layout:
				bit 1-127:	unsigned integer indicating number
				bit 128:	sign bit
		*/

		u128 _value;

	private:

		Fixed128(const char* text, size_t length);
		i128 toInt() const;
		static Fixed128 from(const i128& value);
		static Fixed128 from(const char* text, size_t length);

	public:

		Fixed128();
		explicit Fixed128(const int integral);
		explicit Fixed128(double value);
		Fixed128(Fixed128&&) = default;
		Fixed128(const Fixed128&) = default;
		Fixed128& operator=(Fixed128&&) = default;
		Fixed128& operator=(const Fixed128&) = default;

		static Fixed128 from(const char *text);
		static Fixed128 from(const std::string_view& text);
		static Fixed128 from(const std::string& text);

		double toFloat() const;
		Fixed128 abs() const;
		bool isNegative() const;
		bool isPositive() const;
		const auto& value() const { return _value; }

		Fixed128 operator+(const Fixed128& other) const;
		Fixed128 operator-(const Fixed128& other) const;
		Fixed128 operator*(const Fixed128& other) const;
		Fixed128 operator/(const Fixed128& other) const;
		Fixed128 operator^(int exponent) const;

		Fixed128& operator+=(const Fixed128& other);
		Fixed128& operator-=(const Fixed128& other);
		Fixed128& operator*=(const Fixed128& other);
		Fixed128& operator/=(const Fixed128& other);
		Fixed128& operator^=(int exponent);

		Fixed128 operator-() const;

		operator bool() const;

		bool operator==(const Fixed128& other) const;
		bool operator!=(const Fixed128& other) const;

		friend std::ostream& operator<<(std::ostream& out, const Fixed128& decimal);
		friend Fixed128 operator ""_dec(const char *text, size_t length);
		friend Fixed128 operator ""_dec(unsigned long long value);
		friend Fixed128 operator ""_dec(long double value);
	};

	Fixed128 operator ""_dec(const char *text, size_t length);
	Fixed128 operator ""_dec(unsigned long long value);
	Fixed128 operator ""_dec(long double value);
}

#endif
