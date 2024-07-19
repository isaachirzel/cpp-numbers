#ifndef HIRZEL_NUMBERS_U128_HPP
#define HIRZEL_NUMBERS_U128_HPP

#include <cstdint>
#include <ostream>

namespace hirzel::numbers
{
	class UInt128
	{
		uint64_t _low;
		uint64_t _high;

	public:

		static const UInt128 zero;
		static const UInt128 one;
		static const UInt128 max;

	private:


	public:

		UInt128();
		UInt128(uint64_t high, uint64_t low);
		UInt128(uint64_t value);
		UInt128(uint32_t value);
		UInt128(UInt128&&) = default;
		UInt128(const UInt128&) = default;
		UInt128& operator=(UInt128&&) = default;
		UInt128& operator=(const UInt128&) = default;

		UInt128 operator+(const UInt128& addend) const;
		UInt128 operator+(uint64_t addend) const;
		UInt128 operator+(uint32_t addend) const;
		UInt128 operator-(const UInt128& subtrahend) const;
		UInt128 operator-(uint64_t subtrahend) const;
		UInt128 operator-(uint32_t subtrahend) const;
		UInt128 operator*(const UInt128& multiplier) const;
		UInt128 operator*(uint64_t multiplier) const;
		UInt128 operator*(uint32_t multiplier) const;
		UInt128 operator/(const UInt128& divisor) const;
		UInt128 operator/(uint64_t divisor) const;
		UInt128 operator/(uint32_t divisor) const;
		UInt128 operator%(const UInt128& divisor) const;
		UInt128 operator%(uint64_t divisor) const;
		UInt128 operator%(uint32_t divisor) const;

		UInt128& operator+=(const UInt128& addend);
		UInt128& operator+=(uint64_t addend);
		UInt128& operator+=(uint32_t addend);
		UInt128& operator-=(const UInt128& subtrahend);
		UInt128& operator-=(uint64_t subtrahend);
		UInt128& operator-=(uint32_t subtrahend);
		UInt128& operator*=(const UInt128& multiplier);
		UInt128& operator*=(uint64_t multiplier);
		UInt128& operator*=(uint32_t multiplier);
		UInt128& operator/=(const UInt128& divisor);
		UInt128& operator/=(uint64_t divisor);
		UInt128& operator/=(uint32_t divisor);
		UInt128& operator%=(const UInt128& divisor);
		UInt128& operator%=(uint64_t divisor);
		UInt128& operator%=(uint32_t divisor);

		UInt128& operator++();
		UInt128& operator--();

		bool operator==(const UInt128& other) const;
		bool operator==(uint64_t other) const;
		bool operator==(uint32_t other) const;
		bool operator!=(const UInt128& other) const;
		bool operator!=(uint64_t other) const;
		bool operator!=(uint32_t other) const;

		bool operator<(const UInt128& other) const;
		bool operator>(const UInt128& other) const;
		bool operator<=(const UInt128& other) const;
		bool operator>=(const UInt128& other) const;

		operator bool() const;

		auto& low() { return _low; }
		auto& high() { return _high; }
		const auto& low() const { return _low; }
		const auto& high() const { return _high; }

		friend std::ostream& operator<<(std::ostream& out, const UInt128& i);
	};

	using u128 = unsigned __int128;
}

#endif
