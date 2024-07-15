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
	const u128 Decimal::maxValue = ~(u128(1) << 127);
	const u64 Decimal::decimalPlaces = 19;
	const u64 Decimal::one = 10'000'000'000'000'000'000u;
	const u64 Decimal::maxIntegral = u64(maxValue / one);
	// u128 Decimal::multiply(const u128 l, const u128 r, u128 divisor)
	// {
	// 	constexpr u64 lowerMask = 0x00000000FFFFFFFF;
	// 	constexpr u64 upperMask = 0xEFFFFFFF00000000;

	// 	const u128 lHigh = l >> 32;
	// 	const u128 rHigh = r >> 32;
	// 	const u128 lLow = l & lowerMask;
	// 	const u128 rLow = r & lowerMask;
	// 	const auto low = lLow * rLow;

	// 	if (!(lHigh | rHigh))
	// 	{
	// 		return low;
	// 	}

	// 	const auto mid1 = lLow * rHigh;
	// 	const auto mid2 = lHigh * rLow;
	// 	const auto result = low + ((mid1 + mid2) << 32);
	// 	const auto overflowed = (lHigh && rHigh) ||
	// 		(result < low) ||
	// 		(mid1 >> 32) != 0 ||
	// 		(mid2 >> 32) != 0;

	// 	if (overflowed)
	// 	{
	// 		Log::warning("Decimal: Multiplication of $ and $ causes overflow.", of(l), of(r));
	// 	}

	// 	Log::debug("Result: $", result);

	// 	return result;
	// }

/*
	multiplY

	auto lIntegral = l / divisor;
		auto lMantissa = l % divisor;

		if (lMantissa == 0)
		{
			return lIntegral * r;
		}

		auto rMantissa = r % divisor;
		auto rIntegral = r / divisor;

		if (rMantissa == 0)
		{
			return l * rIntegral;
		}

		const auto integral = lIntegral * rIntegral; // TODO: Check with mantissa overflow

		// if (integral > maxIntegral)
		// {
		// 	Log::warning("Decimal: Multiplication of $ and $ causes overflow.", of(l), of(r));
		// }

		auto result = integral * one;

		result += lIntegral * rMantissa;
		result += lMantissa * rIntegral;
		// result += lMantissa * rMantissa / one;

		return result;
*/

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

	static u128 multiply(const u128 l, const u128 r, u128 divisor)
	{
		u128 result = l;

		result *= r;
		result /= divisor;

		return result;
	}

	Decimal::Decimal():
		_value(0)
	{}

	Decimal::Decimal(unsigned long long value):
		_value(0)
	{
		// NOTE: This is specifically for the literals as it's not expecting negative numbers

		if (value > maxIntegral)
		{
			throw std::invalid_argument("Decimal: Integer literal `" + std::to_string(value) + "` is too large to fit in decimal.");
		}

		_value = value;
		value *= one;
	}

	Decimal::Decimal(long double value):
		_value(0)
	{
		// NOTE: This is specifically for the literals as it's not expecting negative numbers

		auto integral = u128(value);

		if (integral > maxIntegral)
		{
			throw std::invalid_argument("Decimal: Double literal `" + std::to_string(value) + "` is too large to create decimal.");
		}

		auto floatMantissa = value - (long double)integral;
		auto mantissa = u64(floatMantissa * (long double)one);

		_value = integral * one;
		_value += mantissa;
	}

	Decimal::Decimal(int integral, unsigned mantissa):
		_value(0)
	{
		bool isNegative = integral < 0;
		u128 s = isNegative & 0x1;
		u128 i = isNegative
			? -integral
			: integral;

		s <<= 127;
		i *= one;

		u64 m = mantissa;

		if (mantissa)
		{
			while (true)
			{
				u64 step = mantissa * 10;

				if (step >= one)
					break;

				mantissa = step;
			}
		}

		_value = i;
		_value += m;
		_value |= s;
	}

	Decimal::Decimal(int integral):
		Decimal(integral, 0u)
	{}

	Decimal::Decimal(i64 integral, const u64 mantissa):
		_value(0)
	{
		if (mantissa >= one)
			throw std::invalid_argument("Decimal: Mantissa is too large to fit in deximal.");

		bool isNegative = integral < 0;
		
		if (isNegative)
			integral = -integral;

		if ((u64)integral > maxIntegral)
			throw std::invalid_argument("Decimal: Integral is too large to fit in decimal.");

		u128 s = isNegative & 0x1;
		u128 i = isNegative
			? -integral
			: integral;

		s <<= 127;
		i *= one;

		u64 m = mantissa;

		if (m)
		{
			while (true)
			{
				u64 step = m * 10;

				if (step >= one)
					break;

				m = step;
			}
		}

		_value = i;
		_value += m;
		_value |= s;
	}

	Decimal::Decimal(i64 integral):
		Decimal(integral, u64(0))
	{}

	Decimal::Decimal(const char* text, size_t length):
		Decimal()
	{
		if (length == 0)
		{
			throw std::invalid_argument("Decimal: String is empty.");
		}

		u32 partIndex = 0;
		u64 parts[2] = { 0, 0 };

		for (size_t i = 0; i < length; ++i)
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

		auto integral = parts[0];
		auto mantissa = parts[1];

		if (integral > maxIntegral)
		{
			throw std::invalid_argument("Decimal: Integral `" + std::to_string(integral) + "` is too large to fit in decimal.");
		}

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

		_value = integral;
		_value *= one;
		_value += mantissa;
	}

	Decimal::Decimal(const char* text):
		Decimal(text, std::strlen(text))
	{}

	Decimal::Decimal(const std::string_view& text):
		Decimal(text.data(), text.length())
	{}

	Decimal::Decimal(const std::string& text):
		Decimal(text.data(), text.length())
	{}

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

	double Decimal::toFloat() const
	{
		const auto integral = _value / Decimal::one;
		const auto mantissa = _value - integral * Decimal::one;
		const auto value = double(integral) + double(mantissa) / double(Decimal::one);

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

	Decimal Decimal::fromInt(const i128& value)
	{
		auto signBit = i32(value >> 127) & 0x1;
		auto coefficient = i32(1) - signBit - signBit;
		u128 abs = value * coefficient;
		auto sign = value & signBitMask;
		auto result = Decimal();

		result._value = abs | sign;
		
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
		auto value = fromInt(l + r);
		
		return value;
	}

	Decimal Decimal::operator-(const Decimal& other) const
	{
		auto l = toInt();
		auto r = other.toInt();
		auto value = fromInt(l - r);

		return value;
	}

	Decimal Decimal::operator*(const Decimal& other) const
	{
		auto result = Decimal();

		result._value = multiply(_value, other._value, Decimal::one);

		return result;
	}

	Decimal Decimal::operator/(const Decimal& other) const
	{
		auto result = Decimal();

		result._value = multiply(_value, Decimal::one, other._value);

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
		_value = multiply(_value, other._value, Decimal::one);

		return *this;
	}

	Decimal& Decimal::operator/=(const Decimal& other)
	{
		_value = multiply(_value, Decimal::one, other._value);

		return *this;
	}

	Decimal Decimal::operator-() const
	{
		auto result = Decimal();

		result._value = -_value;

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
		const auto integral = u64(value / Decimal::one);
		const auto integralValue = Decimal::one * integral;
		const auto mantissa = u64(value - integralValue);

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
		return Decimal(text, length);
	}

	Decimal operator""_dec(unsigned long long value)
	{
		return Decimal(value);
	}

	Decimal operator ""_dec(long double value)
	{
		return Decimal(value);
	}
}
