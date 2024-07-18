#include "hirzel/numbers/Decimal.hpp"
#include <cmath>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace hirzel::numbers
{
	using u32 = uint32_t;
	using i32 = int32_t;
	using u64 = uint64_t;
	using i64 = int64_t;

	static constexpr u128 signBitMask = u128(1) << 127;
	static constexpr u128 valueBitMask = ~(u128(1) << 127);
	static constexpr u128 maxValue = ~(u128(1) << 127);
	static constexpr u64 one = 10'000'000'000'000'000'000u;
	static constexpr u64 maxIntegral = u64(maxValue / one);

	std::ostream& operator<<(std::ostream& out, const unsigned __int128 value)
	{
		const char *hex = "0123456789ABCDEF";

		// auto v = value;
		for (size_t i = 1; i <= sizeof(value); ++i)
		{
			unsigned c = (value >> ((sizeof(value) - i) * 8) & 0xFF);
			char h = hex[c >> 4];
			char l = hex[c & 0xF];

			out << h << l;
		}

		return out;
	}

	std::ostream& operator<<(std::ostream& out, const __int128 i)
	{
		char buffer[42];

		auto value = i;
		
		if (value < 0)
		{
			value = -value;
			out << '-';
		}

		char* iter = &buffer[sizeof(buffer) - 1];

		*iter = '\0';

		do
		{
			iter -= 1;

			auto remainder = value % 10;

			value /= 10;

			auto c = u32(remainder) + '0';
			
			*iter = c;
		}
		while (value != 0);

		out << iter;

		return out;
	}

	static u128 multiply(const u128& l, const u128& r)
	{
		// TODO: Optimize mantissa by taking the bit range it could be in first (bottom 64 bits)
		// Maybe this applies to integral as well
		
		const auto sign = (l & signBitMask) ^ (r & signBitMask);
		
		const auto lValue = l & valueBitMask;
		const auto lIntegral = lValue / one;
		const auto lMantissa = lValue % one;

		const auto rValue = r & valueBitMask;
		const auto rIntegral = rValue / one;
		const auto rMantissa = rValue % one;

		const auto a = lValue * rIntegral;
		const auto b = lIntegral * rMantissa;
		const auto c = lMantissa * rMantissa / one;

		const auto result = (a + b + c) | sign;

		return result;
	}

	static u128 divide(const u128& l, const u128& r)
	{
		const auto sign = (l & signBitMask) ^ (r & signBitMask);
		
		const auto lValue = l & valueBitMask;
		const auto rValue = r & valueBitMask;

		const auto lIntegral = lValue / rValue;
		const auto lMantissa = lValue % rValue;

		const auto oneIntegral = one / rValue;
		const auto oneMantissa = one % rValue;

		const auto a = lValue * oneIntegral;
		const auto b = lIntegral * oneMantissa;
		const auto c = lMantissa * oneMantissa / rValue;

		const auto result = (a + b + c) | sign;

		return result;
	}

	Decimal::Decimal():
		_value(0)
	{}


	Decimal::Decimal(const int integral):
		_value(0)
	{
		auto abs = integral < 0
			? -integral
			: integral;
		auto s = u128(integral < 0) << 127;
		auto v = u128(abs) * one;

		_value = s | v;
	}

	Decimal::Decimal(double value)
	{
		auto integral = u128(value);

		if (integral > maxIntegral)
		{
			throw std::invalid_argument("Decimal: double `" + std::to_string(value) + "` is too large to create decimal.");
		}

		auto floatMantissa = value - double(integral);
		auto mantissa = u64(floatMantissa * double(one));

		_value = integral * one;
		_value += mantissa;
	}

	Decimal Decimal::from(const i128& value)
	{
		auto signBit = i32(value >> 127) & 0x1;
		auto coefficient = i32(1) - signBit - signBit;
		u128 abs = value * coefficient;
		auto sign = value & signBitMask;
		auto result = Decimal();

		result._value = abs | sign;
		
		return result;
	}

	Decimal Decimal::from(const char* text, size_t length)
	{
		if (length == 0)
		{
			throw std::invalid_argument("Decimal: String is empty.");
		}

		size_t start = 0;
		auto s = u128(0);

		if (text[0] == '-')
		{
			s = signBitMask;
			start += 1;
		}

		u32 partIndex = 0;
		u64 parts[2] = { 0, 0 };

		for (size_t i = start; i < length; ++i)
		{
			auto c = text[i];

			if (c == '.')
			{
				if (partIndex == 1)
				{
					throw std::invalid_argument("Decimal: Duplicate `.` found in string: `" + std::string(text) + "`.");
				}

				partIndex = 1;

				continue;
			}

			auto& part = parts[partIndex];
			auto cValue = (unsigned char)(c - '0');

			if (cValue > 9)
			{
				throw std::invalid_argument("Decimal: Invalid character `" + std::string(1, cValue) + "` found in string: `" + std::string(text) + "`.");
			}

			part *= 10;
			part += cValue;
		}

		if (parts[0] > maxIntegral)
		{
			throw std::invalid_argument("Decimal: Integral `" + std::to_string(parts[0]) + "` is too large to fit in decimal.");
		}

		const auto integral = u128(parts[0]) * one;
		auto mantissa = parts[1];

		if (mantissa >= one)
		{
			throw std::invalid_argument("Decimal: Mantissa `." + std::to_string(mantissa) + "` is too large to fit in decimal.");
		}

		while (true)
		{
			u64 step = mantissa * 10;

			if (step >= one)
				break;

			mantissa = step;
		}

		auto result = Decimal();

		result._value = integral + mantissa;
		result._value |= s;

		return result;
	}

	Decimal Decimal::from(const char *text)
	{
		return from(text, std::strlen(text));
	}

	Decimal Decimal::from(const std::string_view& text)
	{
		return from(text.data(), text.length());
	}

	Decimal Decimal::from(const std::string& text)
	{
		return from(text.c_str(), text.length());
	}

	double Decimal::toFloat() const
	{
		const auto integral = _value / one;
		const auto mantissa = _value % one;
		const auto value = double(integral) + double(mantissa) / double(one);

		return value;
	}

	i128 Decimal::toInt() const
	{
		auto signBit = i32(_value >> 127);
		auto coefficient = 1 - signBit - signBit;
		i128 abs = _value & maxValue;
		i128 result = abs * coefficient;

		return result;
	}

	Decimal Decimal::abs() const
	{
		auto result = Decimal();

		result._value = _value & maxValue;

		return result;
	}

	bool Decimal::isNegative() const
	{
		return !!(_value & signBitMask);
	}

	bool Decimal::isPositive() const
	{
		return !(_value & signBitMask);
	}

	Decimal Decimal::operator+(const Decimal& other) const
	{
		auto l = toInt();
		auto r = other.toInt();
		auto value = from(l + r);

		return value;
	}

	Decimal Decimal::operator-(const Decimal& other) const
	{
		auto l = toInt();
		auto r = other.toInt();
		auto value = from(l - r);

		return value;
	}

	Decimal Decimal::operator*(const Decimal& other) const
	{
		auto result = Decimal();

		result._value = multiply(_value, other._value);

		return result;
	}

	Decimal Decimal::operator/(const Decimal& other) const
	{
		auto result = Decimal();

		result._value = divide(_value, other._value);

		return result;
	}


	Decimal& Decimal::operator+=(const Decimal& other)
	{
		_value += other._value;

		return *this;
	}

	Decimal& Decimal::operator-=(const Decimal& other)
	{
		_value -= other._value;

		return *this;
	}

	Decimal& Decimal::operator*=(const Decimal& other)
	{
		_value = multiply(_value, other._value);

		return *this;
	}

	Decimal& Decimal::operator/=(const Decimal& other)
	{
		_value = divide(_value, other._value);

		return *this;
	}

	Decimal Decimal::operator-() const
	{
		auto s = (_value & signBitMask) ^ signBitMask;
		auto v = _value & valueBitMask;
		auto result = Decimal();

		result._value = s | v;

		return result;
	}

	Decimal::operator bool() const
	{
		return _value > 0;
	}

	bool Decimal::operator==(const Decimal& other) const
	{
		return _value == other._value;
	}

	bool Decimal::operator!=(const Decimal& other) const
	{
		return _value != other._value;
	}

	static void outputIntegral(std::ostream& out, u64 integral)
	{
		char buffer[32];
		constexpr u32 end = sizeof(buffer) - 1;
		char* iter = &buffer[end];
		auto groupSize = 0;

		*iter = '\0';

		do
		{
			if (groupSize == 3)
			{
				iter -= 1;
				*iter = ',';
				groupSize = 0;
			}

			auto remainder = integral % 10;
			auto c = (char)remainder + '0';

			integral /= 10;
			iter -= 1;
			*iter = c; 
			groupSize += 1;
		}
		while (integral);

		out << iter;
	}

	static void outputMantissa(std::ostream& out, u64 mantissa)
	{
		char buffer[20] = "00";
		constexpr u32 end = sizeof(buffer) - 1;
		char *iter = &buffer[end];

		for (; mantissa; mantissa /= 10)
		{
			auto remainder = mantissa % 10;

			if (remainder)
			{
				*iter = '\0';
				break;
			}

			iter -= 1;
		}

		for (; mantissa; mantissa /= 10)
		{
			auto remainder = char(mantissa % 10);
			auto c = '0' + remainder;

			iter -= 1;
			*iter =  c;	
		}

		out << buffer;
	}

	std::ostream& operator<<(std::ostream& out, const Decimal& decimal)
	{
		const auto value = decimal._value & valueBitMask;
		const auto integral = u64(value / one);
		const auto mantissa = u64(value % one);

		if (decimal.isNegative())
		{
			out << "-";
		}

		out << "$";

		outputIntegral(out, integral);

		out << ".";

		outputMantissa(out, mantissa);

		return out;
	}

	Decimal operator""_dec(const char *text, size_t length)
	{
		return Decimal::from(text, length);
	}

	Decimal operator""_dec(unsigned long long value)
	{
		if (value > maxIntegral)
		{
			throw std::invalid_argument("Decimal: Integer literal `" + std::to_string(value) + "` is too large to fit in decimal.");
		}

		auto result = Decimal();

		result._value = u128(value) * one;

		return result;
	}

	Decimal operator ""_dec(long double value)
	{
		auto integral = u128(value);

		if (integral > maxIntegral)
		{
			throw std::invalid_argument("Decimal: Double literal `" + std::to_string(value) + "` is too large to create decimal.");
		}

		auto floatMantissa = value - (long double)integral;
		auto mantissa = u64(floatMantissa * (long double)one);
		auto result = Decimal();

		result._value = integral * one + mantissa;

		return result;
	}
}
