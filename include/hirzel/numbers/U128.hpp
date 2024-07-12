#ifndef HIRZEL_NUMBERS_U128_HPP
#define HIRZEL_NUMBERS_U128_HPP

#include <cstdint>
#include <functional>
#include <ostream>

namespace hirzel::numbers
{
	class U128
	{
		uint64_t _low;
		uint64_t _high;

		static std::function<void(const U128&, const U128&, char)> _onOverflow;
		static bool _hasOverflown;

	private:

		static uint32_t divide(U128& i, uint32_t divisor);
		static uint32_t multiply(U128& i, uint32_t mulitplier);

	public:

		U128();
		U128(uint64_t low, uint64_t high);
		U128(U128&&) = default;
		U128(const U128&) = default;
		U128& operator=(U128&&) = default;
		U128& operator=(const U128&) = default;

		static void setOverflowCallback(std::function<void(const U128&, const U128&, char)>&& callback);
		static void clearOverflow();
		static U128 fromMul(uint64_t l, uint64_t r);

		U128 operator+(const U128& addend) const;
		U128 operator+(uint64_t addend) const;
		U128 operator+(uint32_t addend) const;
		U128 operator-(const U128& subtrahend) const;
		U128 operator-(uint64_t subtrahend) const;
		U128 operator-(uint32_t subtrahend) const;
		U128 operator*(const U128& multiplier) const;
		U128 operator*(uint64_t multiplier) const;
		U128 operator*(uint32_t multiplier) const;
		U128 operator/(const U128& divisor) const;
		U128 operator/(uint64_t divisor) const;
		U128 operator/(uint32_t divisor) const;

		U128& operator+=(const U128& addend);
		U128& operator+=(uint64_t addend);
		U128& operator+=(uint32_t addend);
		U128& operator-=(const U128& subtrahend);
		U128& operator-=(uint64_t subtrahend);
		U128& operator-=(uint32_t subtrahend);
		U128& operator*=(const U128& multiplier);
		U128& operator*=(uint64_t multiplier);
		U128& operator*=(uint32_t multiplier);
		U128& operator/=(const U128& divisor);
		U128& operator/=(uint64_t divisor);
		U128& operator/=(uint32_t divisor);

		U128& operator++();
		U128& operator--();

		// U128 operator+(const U128& other);
		// U128 operator-(const U128& other);
		// U128 operator*(const U128& other);
		// U128 operator/(const U128& other);


		friend std::ostream& operator<<(std::ostream& out, const U128& i);
	};
}

#endif
