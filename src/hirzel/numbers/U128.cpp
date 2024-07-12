#include "hirzel/numbers/U128.hpp"

#define handleOverflow(a, b, op) do\
{\
	_hasOverflown = true;\
	if (_onOverflow) _onOverflow(a, b, op);\
} while(false)

namespace hirzel::numbers
{
	using u64 = uint64_t;
	using u32 = uint32_t;

	std::function<void(const U128&, const U128&, char op)> U128::_onOverflow;
	thread_local bool U128::_hasOverflown;

	const U128 U128::max = U128(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF);
	const U128 U128::zero = U128(0, 0);
	const U128 U128::one = U128(0, 1);

	U128::U128(u64 high, u64 low):
		_low(low),
		_high(high)
	{}

	U128::U128():
		_low(0),
		_high(0)
	{}

	void U128::setOverflowCallback(std::function<void(const U128&, const U128&, char)>&& callback)
	{
		_onOverflow = std::move(callback);
	}

	bool U128::hasOverflown()
	{
		return _hasOverflown;
	}

	void U128::clearOverflow()
	{
		_hasOverflown = false;
	}

	static inline void dividePart(u32& part, u32 divisor, u32& remainder)
	{
		u64 dividend = remainder;

		dividend <<= 32;
		dividend |= part;

		part = u32(dividend / divisor);
		remainder = u32(dividend % divisor);
	}

	u32 U128::divide(U128& i, u32 divisor)
	{
		u32 remainder = 0;
		u32 hh = i._high >> 32;
		u32 hl = i._high & 0xFFFFFFFF;
		u32 lh = i._low >> 32;
		u32 ll = i._low & 0xFFFFFFFF;

		// TODO: Check if conditionally dividing is faster or not

		dividePart(hh, divisor, remainder);
		dividePart(hl, divisor, remainder);
		dividePart(lh, divisor, remainder);
		dividePart(ll, divisor, remainder);

		i._low = lh;
		i._low <<= 32;
		i._low |= ll;

		i._high = hh;
		i._high <<= 32;
		i._high |= hl;

		return remainder;
	}

	static inline u32 multiplyPart(u32& part, u32 multiplier, u32 carry)
	{
		u64 product = part;

		product *= multiplier;
		product += carry;

		part = product & 0xFFFFFFFF;
		carry = product >> 32;

		return carry;
	}

	u32 U128::multiply(U128& i, u32 multiplier)
	{
		u32 carry = 0;
		u32 hh = i._high >> 32;
		u32 hl = i._high & 0xFFFFFFFF;
		u32 lh = i._low >> 32;
		u32 ll = i._low & 0xFFFFFFFF;

		// TODO: Check if conditionally dividing is faster or not

		carry = multiplyPart(ll, multiplier, carry);
		carry = multiplyPart(lh, multiplier, carry);
		carry = multiplyPart(hl, multiplier, carry);
		carry = multiplyPart(hh, multiplier, carry);

		i._low = lh;
		i._low <<= 32;
		i._low |= ll;

		i._high = hh;
		i._high <<= 32;
		i._high |= hl;

		return carry;
	}

	U128 U128::fromMul(u64 l, u64 r)
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

		u64 high = (l * r) + w1 + k;
		u64 low = (t << 32) + w3;

		return U128(low, high);
	}

	U128 U128::operator+(const U128& addend) const
	{
		auto result = U128(*this);

		result._low += addend._low;
		result._high += addend._high;

		u64 carry = result._low < _low;

		result._high += carry;

		if (result._high < _high)
		{
			handleOverflow(*this, addend, '+');
		}

		return result;
	}

	U128 U128::operator+(u64 addend) const
	{
		auto result = U128(*this);

		result._low += addend;

		u64 carry = result._low < _low;

		result._high += carry;

		if (result._high < _high)
		{
			handleOverflow(*this, U128(0, addend), '+');
		}
		
		return result;
	}

	U128 U128::operator+(u32 addend) const
	{
		return *this + (u64)addend;
	}

	U128 U128::operator-(const U128& subtrahend) const
	{
		auto result = U128(*this);

		result._low -= subtrahend._low;
		result._high -= subtrahend._high;

		u64 carry = result._low > _low;

		result._high -= carry;

		if (result._high > _high)
		{
			handleOverflow(*this, subtrahend, '-');
		}
		
		return result;
	}

	U128 U128::operator-(u64 subtrahend) const
	{
		auto result = U128(*this);

		result._low -= subtrahend;

		u64 carry = result._low > _low;

		result._high -= carry;

		if (result._high > _high)
		{
			handleOverflow(*this, U128(0, subtrahend), '-');
		}
		
		return result;
	}

	U128 U128::operator-(u32 subtrahend) const
	{
		return *this - (u64)subtrahend;
	}

	U128 U128::operator*(const U128& multiplier) const
	{
		auto result = fromMul(_low, multiplier._low);

		result._high += (result._high * multiplier._low) + (result._low * multiplier._high);

		return result;
	}

	U128 U128::operator*(u64 multiplier) const
	{
		auto result = U128(*this);
		// TODO: Implement this
		return result;
	}

	U128 U128::operator*(u32 multiplier) const
	{
		auto result = U128(*this);
		auto carry = multiply(result, multiplier);

		if (carry)
		{
			handleOverflow(*this, U128(0, multiplier), '*');
		}

		return result;
	}

	U128 U128::operator/(const U128& divisor) const
	{
		if ((divisor._high | divisor._low) == 0)
		{
			handleOverflow(*this, divisor, '/');
			
			return U128::max;
		}

		auto result = U128(*this);
		
		return result;
	}

	U128 U128::operator/(u64 divisor) const
	{
		if (divisor == 0)
		{
			handleOverflow(*this, U128(0, divisor), '/');
			
			return U128::max;
		}

		auto result = U128(*this);
		
		return result;
	}

	U128 U128::operator/(u32 divisor) const
	{
		if (divisor == 0)
		{
			handleOverflow(*this, U128(0, divisor), '/');

			return U128::max;
		}

		auto result = U128(*this);
		
		divide(result, divisor);

		return result;
	}

	U128& U128::operator+=(const U128& addend)
	{
		auto result = *this + addend;

		*this = result;
		
		return *this;
	}

	U128& U128::operator+=(u64 addend)
	{
		auto result = *this + addend;
		
		*this = result;
		
		return *this;
	}

	U128& U128::operator+=(u32 addend)
	{
		auto result = *this + addend;
		
		*this = result;
		
		return *this;
	}

	U128& U128::operator-=(const U128& subtrahend)
	{
		auto result = *this - subtrahend;
		
		*this = result;
		
		return *this;
	}

	U128& U128::operator-=(u64 subtrahend)
	{
		auto result = *this - subtrahend;
		
		*this = result;
		
		return *this;
	}

	U128& U128::operator-=(u32 subtrahend)
	{
		auto result = *this - subtrahend;
		
		*this = result;
		
		return *this;
	}

	U128& U128::operator*=(const U128& multiplier)
	{
		auto result = *this * multiplier;

		*this = result;

		return *this;
	}

	U128& U128::operator*=(u64 multiplier)
	{
		auto result = *this * multiplier;

		*this = result;

		return *this;
	}

	U128& U128::operator*=(u32 multiplier)
	{
		auto result = *this * multiplier;

		*this = result;

		return *this;
	}

	U128& U128::operator/=(const U128& divisor)
	{
		auto result = *this / divisor;

		*this = result;

		return *this;
	}

	U128& U128::operator/=(u64 divisor)
	{
		auto result = *this / divisor;

		*this = result;

		return *this;
	}

	U128& U128::operator/=(u32 divisor)
	{
		auto result = *this / divisor;

		*this = result;

		return *this;
	}

	U128& U128::operator++()
	{
		auto T = _low + 1;
		auto carry = ((T ^ _low) & T) >> 63;

    	_high += carry;
    	_low = T;

		return *this;
	}

	U128& U128::operator--()
	{
		auto T = _low - 1;
		auto carry = ((T ^ _low) & T) >> 63;

		// TODO: Check overflow

    	_high -= carry;
    	_low = T;

		return *this;
	}

	std::ostream& operator<<(std::ostream& out, const U128& i)
	{
		char buffer[41];

		auto value = i;
		char* iter = &buffer[sizeof(buffer) - 1];

		*iter = '\0';

		do
		{
			iter -= 1;

			auto remainder = U128::divide(value, 10);
			auto c = remainder + '0';
			
			*iter = c;
		}
		while (value._low | value._high);

		out << iter;

		return out;
	}
}
