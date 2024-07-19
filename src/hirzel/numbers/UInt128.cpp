#include "hirzel/numbers/UInt128.hpp"
#include <cassert>
#include <stdexcept>

#define notImplemented() (throw std::runtime_error(std::string(__func__) + " is not implemented."))


namespace hirzel::numbers
{
	using u64 = uint64_t;
	using u32 = uint32_t;

	const UInt128 UInt128::max = UInt128(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF);
	const UInt128 UInt128::zero = UInt128(0, 0);
	const UInt128 UInt128::one = UInt128(0, 1);

	static inline u32 multiply32Part(u32& part, u32 multiplier, u32 carry)
	{
		u64 product = part;

		product *= multiplier;
		product += carry;

		part = product & 0xFFFFFFFF;
		carry = product >> 32;

		return carry;
	}

	static u32 multiply128by32(u64& high, u64& low, u32 multiplier)
	{
		u32 carry = 0;
		u64 h = high;
		u64 l = low;
		u32 hh = high >> 32;
		u32 hl = high & 0xFFFFFFFF;
		u32 lh = low >> 32;
		u32 ll = low & 0xFFFFFFFF;

		// TODO: Check if conditionally dividing is faster or not

		carry = multiply32Part(ll, multiplier, carry);
		carry = multiply32Part(lh, multiplier, carry);
		carry = multiply32Part(hl, multiplier, carry);
		carry = multiply32Part(hh, multiplier, carry);

		l = lh;
		l <<= 32;
		l |= ll;

		h = hh;
		h <<= 32;
		h |= hl;

		low = l;
		high = h;		

		return carry;
	}

	static void divide128by128(u64& dividendHigh, u64& dividendLow, const u64 divisorHigh, const u64 divisorLow)
	{
		// assert((divisorHigh | divisorLow) != 0);

		// u64 quotientHigh = dividendHigh;
		// u64 quotientLow = dividendLow;

		// // if (divisor > dividend) {
		// // 	*quotient_ret = 0;
		// // 	*remainder_ret = dividend;
		// // 	return;
		// // }

		// // if (divisor == dividend) {
		// // 	*quotient_ret = 1;
		// // 	*remainder_ret = 0;
		// // 	return;
		// // }

		// UInt128 denominator = divisor;
		// UInt128 quotient = 0;

		// // Left aligns the MSB of the denominator and the dividend.
		// const int shift = Fls128(dividend) - Fls128(denominator);
		// denominator <<= shift;

		// // Uses shift-subtract algorithm to divide dividend by denominator. The
		// // remainder will be left in dividend.
		// for (int i = 0; i <= shift; ++i) {
		// 	quotient <<= 1;
		// 	if (dividend >= denominator) {
		// 	dividend -= denominator;
		// 	quotient |= 1;
		// 	}
		// 	denominator >>= 1;
		// }

		// *quotient_ret = quotient;
		// *remainder_ret = dividend;
	}

	static u64 divide64Part(u64& part, u64 divisor, u64 remainder)
	{

	}

	static u64 divide128by64(u64& high, u64& low, u64 divisor)
	{
		u64 remainder = 0;
		u64 h = high;
		u64 l = low;
		
		u64 a = h / divisor;
		u64 carry = h % divisor;


		// auto a = _high / divisor;
		// auto carry = _low / divisor;

		// auto b

		high = h;
		high = l;

		return remainder;
	}

	static u32 divide32Part(u32& part, u32 divisor, u32 remainder)
	{
		u64 dividend = remainder;

		dividend <<= 32;
		dividend |= part;

		part = u32(dividend / divisor);
		remainder = u32(dividend % divisor);

		return remainder;
	}

	static u32 divide128by32(u64& high, u64& low, u32 divisor)
	{
		u32 remainder = 0;
		u64 h = high;
		u64 l = low;
		u32 hh = h >> 32;
		u32 hl = h & 0xFFFFFFFF;
		u32 lh = l >> 32;
		u32 ll = l & 0xFFFFFFFF;

		// TODO: Check if conditionally dividing is faster or not

		remainder = divide32Part(hh, divisor, remainder);
		remainder = divide32Part(hl, divisor, remainder);
		remainder = divide32Part(lh, divisor, remainder);
		remainder = divide32Part(ll, divisor, remainder);

		l = lh;
		l <<= 32;
		l |= ll;

		h = hh;
		h <<= 32;
		h |= hl;

		high = h;
		low = l;

		return remainder;
	}

	static void multiply64by64(u64& high, u64& low, u64 l, u64 r)
	{
		u64 u1 = (l & 0xffffffff);
		u64 v1 = (r & 0xffffffff);
		u64 t = (u1 * v1);
		u64 w3 = (t & 0xffffffff);
		u64 k = (t >> 32);

		l >>= 32;
		t = (l * v1) + k;
		k = (t & 0xffffffff);
		u64 w1 = (t >> 32);

		r >>= 32;
		t = (u1 * r) + k;
		k = (t >> 32);

		high = (l * r) + w1 + k;
		low = (t << 32) + w3;
	}

	UInt128::UInt128(u64 high, u64 low):
		_low(low),
		_high(high)
	{}

	UInt128::UInt128():
		UInt128(0, 0)
	{}

	UInt128::UInt128(u64 value):
		UInt128(0, value)
	{}

	UInt128::UInt128(u32 value):
		UInt128(0, value)
	{}

	UInt128 UInt128::operator+(const UInt128& addend) const
	{
		auto result = UInt128(*this);

		result._low += addend._low;
		result._high += addend._high;

		u64 lowCarry = result._low < _low;

		result._high += lowCarry;

		return result;
	}

	UInt128 UInt128::operator+(u64 addend) const
	{
		auto result = UInt128(*this);

		result._low += addend;

		u32 lowCarry = result._low < _low;

		result._high += lowCarry;

		return result;
	}

	UInt128 UInt128::operator+(u32 addend) const
	{
		return *this + u64(addend);
	}

	UInt128 UInt128::operator-(const UInt128& subtrahend) const
	{
		auto result = UInt128(*this);

		result._low -= subtrahend._low;
		result._high -= subtrahend._high;

		u64 lowCarry = result._low > _low;

		result._high -= lowCarry;
		
		return result;
	}

	UInt128 UInt128::operator-(u64 subtrahend) const
	{
		auto result = UInt128(*this);

		result._low -= subtrahend;

		u64 lowCarry = result._low > _low;

		result._high -= lowCarry;
		
		return result;
	}

	UInt128 UInt128::operator-(u32 subtrahend) const
	{
		return *this - u64(subtrahend);
	}

	UInt128 UInt128::operator*(const UInt128& multiplier) const
	{
		auto result = UInt128();

		multiply64by64(result._high, result._low, _low, multiplier._low);

		result._high += _high * multiplier._low;
		result._high += _low * multiplier._high;

		return result;
	}

	UInt128 UInt128::operator*(u64 multiplier) const
	{
		auto result = UInt128(*this);

		multiply64by64(result._high, result._low, _low, multiplier);

		result._high += _high * multiplier;

		return result;
	}

	UInt128 UInt128::operator*(u32 multiplier) const
	{
		auto result = UInt128(*this);

		multiply128by32(result._high, result._low, multiplier);

		return result;
	}

	UInt128 UInt128::operator/(const UInt128& divisor) const
	{
		auto result = UInt128(*this);

		notImplemented();
		
		return result;
	}

	UInt128 UInt128::operator/(u64 divisor) const
	{
		auto result = UInt128(*this);

		divide128by64(result._high, result._low, divisor);
		
		return result;
	}

	UInt128 UInt128::operator/(u32 divisor) const
	{
		auto result = UInt128(*this);
		
		divide128by32(result._high, result._low, divisor);

		return result;
	}

	UInt128 UInt128::operator%(const UInt128& divisor) const
	{
		auto result = UInt128(*this);
		
		notImplemented();
		
		return result;
	}

	UInt128 UInt128::operator%(u64 divisor) const
	{
		auto result = UInt128(*this);
		auto remainder = divide128by64(result._high, result._low, divisor);
		
		return remainder;
	}

	UInt128 UInt128::operator%(u32 divisor) const
	{
		auto dividend = UInt128(*this);
		auto remainder = divide128by32(dividend._high, dividend._low, divisor);

		return UInt128(0, remainder);
	}

	UInt128& UInt128::operator+=(const UInt128& addend)
	{
		auto result = *this + addend;

		*this = result;
		
		return *this;
	}

	UInt128& UInt128::operator+=(u64 addend)
	{
		auto result = *this + addend;
		
		*this = result;
		
		return *this;
	}

	UInt128& UInt128::operator+=(u32 addend)
	{
		auto result = *this + addend;
		
		*this = result;
		
		return *this;
	}

	UInt128& UInt128::operator-=(const UInt128& subtrahend)
	{
		auto result = *this - subtrahend;
		
		*this = result;
		
		return *this;
	}

	UInt128& UInt128::operator-=(u64 subtrahend)
	{
		auto result = *this - subtrahend;
		
		*this = result;
		
		return *this;
	}

	UInt128& UInt128::operator-=(u32 subtrahend)
	{
		auto result = *this - subtrahend;
		
		*this = result;
		
		return *this;
	}

	UInt128& UInt128::operator*=(const UInt128& multiplier)
	{
		auto result = *this * multiplier;

		*this = result;

		return *this;
	}

	UInt128& UInt128::operator*=(u64 multiplier)
	{
		auto result = *this * multiplier;

		*this = result;

		return *this;
	}

	UInt128& UInt128::operator*=(u32 multiplier)
	{
		auto result = *this * multiplier;

		*this = result;

		return *this;
	}

	UInt128& UInt128::operator/=(const UInt128& divisor)
	{
		auto result = *this / divisor;

		*this = result;

		return *this;
	}

	UInt128& UInt128::operator/=(u64 divisor)
	{
		auto result = *this / divisor;

		*this = result;

		return *this;
	}

	UInt128& UInt128::operator/=(u32 divisor)
	{
		auto result = *this / divisor;

		*this = result;

		return *this;
	}

	UInt128& UInt128::operator%=(const UInt128& divisor)
	{
		auto result = *this % divisor;

		*this = result;

		return *this;
	}

	UInt128& UInt128::operator%=(u64 divisor)
	{
		auto result = *this % divisor;

		*this = result;

		return *this;
	}

	UInt128& UInt128::operator%=(u32 divisor)
	{
		auto result = *this % divisor;

		*this = result;

		return *this;
	}

	UInt128& UInt128::operator++()
	{
		auto T = _low + 1;
		auto carry = ((T ^ _low) & T) >> 63;

    	_high += carry;
    	_low = T;

		return *this;
	}

	UInt128& UInt128::operator--()
	{
		auto T = _low - 1;
		auto carry = ((T ^ _low) & T) >> 63;

    	_high -= carry;
    	_low = T;

		return *this;
	}

	bool UInt128::operator==(const UInt128& other) const
	{
		return ((_low ^ other._low) | (_high ^ other._high)) == 0;
	}

	bool UInt128::operator==(u64 other) const
	{
		return (_high | (_low ^ other)) == 0;
	}

	bool UInt128::operator==(u32 other) const
	{
		return *this == u64(other);
	}

	bool UInt128::operator!=(const UInt128& other) const
	{
		return !(*this == other);
	}

	bool UInt128::operator!=(u64 other) const
	{
		return !(*this == other);
	}

	bool UInt128::operator!=(u32 other) const
	{
		return !(*this == u64(other));
	}

	bool UInt128::operator<(const UInt128& other) const
	{
		// TODO: Optimize this
		return _high == other._high
			? _low <= other._low
			: _high <= other._high;
	}

	bool UInt128::operator>(const UInt128& other) const
	{
		return other < *this;
	}

	bool UInt128::operator<=(const UInt128& other) const
	{
		return !(other < *this);
	}

	bool UInt128::operator>=(const UInt128& other) const
	{
		return !(*this < other);
	}

	UInt128::operator bool() const
	{
		return (_high | _low) != 0;
	}

	std::ostream& operator<<(std::ostream& out, const UInt128& i)
	{
		char buffer[42];

		auto value = i;
		char* iter = &buffer[sizeof(buffer) - 1];

		*iter = '\0';

		do
		{
			iter -= 1;

			auto remainder = divide128by32(value._high, value._low, 10);
			auto c = remainder + '0';
			
			*iter = c;
		}
		while (value._low | value._high);

		out << iter;

		return out;
	}
}
