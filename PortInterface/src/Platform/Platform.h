#ifndef __REAL_PLATFORM__
#define __REAL_PLATFORM__


#if defined(_WIN32) || defined(_WIN64)
#define REAL_PLATFORM_WINDOWS
#endif

#ifdef __linux__
#define REAL_PLATFORM_LINUX
#endif

#ifdef __APPLE__
#define REAL_PLATFORM_MAC
#endif

#include "PlatformTypes.h"

#define DECLARE_PLATFORM_TYPE(type) typedef Real::Platform::##type type

// signed integer types

DECLARE_PLATFORM_TYPE(int8);
DECLARE_PLATFORM_TYPE(int16);
DECLARE_PLATFORM_TYPE(int32);
DECLARE_PLATFORM_TYPE(int64);

// unsigned integer types

DECLARE_PLATFORM_TYPE(uint8);
DECLARE_PLATFORM_TYPE(uint16);
DECLARE_PLATFORM_TYPE(uint32);
DECLARE_PLATFORM_TYPE(uint64);

// other types

DECLARE_PLATFORM_TYPE(BYTE);

DECLARE_PLATFORM_TYPE(SIZE_T);

DECLARE_PLATFORM_TYPE(ANSICHAR);
DECLARE_PLATFORM_TYPE(WIDECHAR);

DECLARE_PLATFORM_TYPE(CHAR16);
DECLARE_PLATFORM_TYPE(CHAR32);

DECLARE_PLATFORM_TYPE(TCHAR);
DECLARE_PLATFORM_TYPE(UCHAR);

#endif