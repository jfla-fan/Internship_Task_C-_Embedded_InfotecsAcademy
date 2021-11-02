#ifndef __REAL_ENDIAN__
#define __REAL_ENDIAN__

#include "../Core.h"


#define IS_BIG_ENDIAN Real::Endian::Private::__is_big_endian
#define IS_LITTLE_ENDIAN !(IS_BIG_ENDIAN)


/**
 * Real::Endian functions are used to change endianness of integer values.
 * I could use Boost.Endian, but decided to implement this by myself.
 */
namespace Real { namespace Endian {

	/**
	 * Used for internal purposes.
	 */
	namespace Private
	{
		constexpr static int32 __x = 0x1;
		constexpr static bool __is_big_endian = ((__x & 0xff000000) == 0x1);
	}

	enum class ENDIANNESS
	{
		big,
		little,
		ACTUAL = ((IS_BIG_ENDIAN) ? big : little),
	};

	/**
	 * Actual implementation functions.
	 */
	namespace Implementation
	{

		FORCEINLINE uint8 endian_reverse_impl(uint8 x) NOEXCEPT
		{
			return x;
		}

		FORCEINLINE uint16 endian_reverse_impl(uint16 x) NOEXCEPT
		{
			return (x << 8) | (x >> 8);
		}

		FORCEINLINE uint32 endian_reverse_impl(uint32 x) NOEXCEPT
		{
			int32 half = x << 16 | x >> 16;
			return ((half << 8) & 0xff00ff00) | ((half >> 8) & 0x00ff00ff);
		}

		FORCEINLINE uint64 endian_reverse_impl(uint64 x) NOEXCEPT
		{
			uint64 step32 = x << 32 | x >> 32;
			uint64 step16 = (step32 & 0x0000FFFF0000FFFFULL) << 16 | (step32 & 0xFFFF0000FFFF0000ULL) >> 16;
			return (step16 & 0x00FF00FF00FF00FFULL) << 8 | (step16 & 0xFF00FF00FF00FF00ULL) >> 8;
		}

	}

	template<typename _EndianReversible>
	inline _EndianReversible native_to_big(_EndianReversible x) NOEXCEPT
	{
		if (ENDIANNESS::ACTUAL == ENDIANNESS::big)
			return x;

		return Implementation::endian_reverse_impl(x);
	}

	template<typename _EndianReversible>
	inline _EndianReversible big_to_native(_EndianReversible x) NOEXCEPT
	{
		if (ENDIANNESS::ACTUAL == ENDIANNESS::big)
			return x;

		return Implementation::endian_reverse_impl(x);
	}

	template<typename _EndianReversible>
	inline _EndianReversible native_to_little(_EndianReversible x) NOEXCEPT
	{
		if (ENDIANNESS::ACTUAL == ENDIANNESS::little)
			return x;

		return Implementation::endian_reverse_impl(x);
	}

	template<typename _EndianReversible>
	inline _EndianReversible little_to_native(_EndianReversible x) NOEXCEPT
	{
		if (ENDIANNESS::ACTUAL == ENDIANNESS::little)
			return x;

		return Implementation::endian_reverse_impl(x);
	}

} }





#endif
