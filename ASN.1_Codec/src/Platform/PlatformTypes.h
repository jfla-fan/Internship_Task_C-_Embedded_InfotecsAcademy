#ifndef __REAL_PLATFORM_TYPES__
#define __REAL_PLATFORM_TYPES__

#include "../Defines.h"


namespace Real { namespace System {



	// signed types

	// 8-bit signed integer
	typedef std::int8_t		int8;

	// 16-bit signed integer
	typedef std::int16_t    int16;

	// 32-bit signed integer
	typedef std::int32_t	int32;

	// 64-bit signed integer
	typedef std::int64_t	int64;


	// unsigned types

	// 8-bit unsigned integer
	typedef std::uint8_t	uint8;

	// 16-bit unsigned integer
	typedef std::uint16_t	uint16;

	// 32-bit unsigned integer
	typedef std::uint32_t	uint32;

	// 64-bit unsigned integer
	typedef std::uint64_t	uint64;

	// others types

	// unsigned int
	typedef std::size_t		SIZE_T;

	// should be at least 8 bytes
	typedef char			BYTE;

	// An ANSI character. 8-bit fixed-width representation of 7-bit character
	typedef char			ANSICHAR;

	// A wide character. ?-bit fixed-width representation of the platform's natural wide character set. Could be different sizes on different platforms. 
	typedef wchar_t			WIDECHAR;

	// unsigned char
	typedef unsigned char	UCHAR;

	// 8-bit character type
	typedef unsigned char	CHAR8;
	
	// 16-bit character type
	typedef char16_t		CHAR16;
	
	// 32-bit character type
	typedef char32_t		CHAR32;

	// Switchable
#if defined(REAL_USE_WIDECHAR_TYPE)
	typedef WIDECHAR		TCHAR;
#else
	typedef ANSICHAR		TCHAR;
#endif

} }



#endif