#ifndef HIRZEL_NUMBERS_U128_HPP
#define HIRZEL_NUMBERS_U128_HPP

#include <cstdint>
#include <ostream>

namespace hirzel::numbers
{
	class U128
	{
		uint64_t _low;
		uint64_t _high;

	public:

		static const U128 zero;
		static const U128 one;
		static const U128 max;

	private:


	public:

		U128();
		U128(uint64_t high, uint64_t low);
		U128(uint64_t value);
		U128(uint32_t value);
		U128(U128&&) = default;
		U128(const U128&) = default;
		U128& operator=(U128&&) = default;
		U128& operator=(const U128&) = default;

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
		U128 operator%(const U128& divisor) const;
		U128 operator%(uint64_t divisor) const;
		U128 operator%(uint32_t divisor) const;

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
		U128& operator%=(const U128& divisor);
		U128& operator%=(uint64_t divisor);
		U128& operator%=(uint32_t divisor);

		U128& operator++();
		U128& operator--();

		bool operator==(const U128& other) const;
		bool operator==(uint64_t other) const;
		bool operator==(uint32_t other) const;
		bool operator!=(const U128& other) const;
		bool operator!=(uint64_t other) const;
		bool operator!=(uint32_t other) const;

		bool operator<(const U128& other) const;
		bool operator>(const U128& other) const;
		bool operator<=(const U128& other) const;
		bool operator>=(const U128& other) const;

		operator bool() const;

		auto& low() { return _low; }
		auto& high() { return _high; }
		const auto& low() const { return _low; }
		const auto& high() const { return _high; }

		friend std::ostream& operator<<(std::ostream& out, const U128& i);
	};

	using u128 = unsigned __int128;
}

#endif
