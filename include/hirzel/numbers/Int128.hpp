#ifndef HIRZEL_NUMBERS_I128_HPP
#define HIRZEL_NUMBERS_I128_HPP

#include <cstdint>

namespace hirzel::numbers
{
	class Int128
	{
		using u32 = uint32_t;
		using u64 = uint64_t;

		u64 _low;
		u64 _high;

	};
	
	using i128 = __int128;
}

#endif
