#ifndef __REAL_CORE__
#define __REAL_CORE__

#include <iostream>

#define LOG(_message_) std::cout << _message_ << std::endl

#define FATAL_ASSERT(_cond_, _message_)	\
				 do {					\
					if (!(_cond_))		\
				{						\
					LOG(_message_);		\
					std::exit(1);		\
				} } while (0)



#include "Platform/Platform.h"

// compiler type
#if defined(_MSC_VER)
#define REAL_MSVC_COMPILER
#elif defined(__GNUC__)
#define REAL_GNUC_COMPILER
#else
#define REAL_UNKNOWN_COMPILER
#endif


// force inline
#if defined(REAL_MSVC_COMPILER)
#define FORCEINLINE __forceinline
#elif defined(REAL_GNUC_COMPILER)
#define FORCEINLINE __attribute__(always_inline)
#else
#define FORCEINLINE
#endif


#define NODISCARD [[nodiscard]]
#define NOEXCEPT noexcept

#define DECLARE_ENUM_FLAG_OPERATIONS(__flag__, __flag_type__)	\
		FORCEINLINE __flag__ operator & (const __flag__& lhs, const __flag__& rhs) { return static_cast<__flag__>(static_cast<__flag_type__>(lhs) & static_cast<__flag_type__>(rhs)); }\
		template<typename _Integer, std::enable_if_t<std::is_integral<_Integer>::value, bool> = true> __flag__ operator & (const _Integer& lhs, const __flag__& rhs) { return static_cast<__flag__>(static_cast<__flag_type__>(lhs) & static_cast<__flag_type__>(rhs)); }\
		template<typename _Integer, std::enable_if_t<std::is_integral<_Integer>::value, bool> = true> __flag__ operator & (const __flag__& lhs, const _Integer& rhs) { return static_cast<__flag__>(static_cast<__flag_type__>(lhs) & static_cast<__flag_type__>(rhs)); }\
		FORCEINLINE __flag__ operator | (const __flag__& lhs, const __flag__& rhs) { return static_cast<__flag__>(static_cast<__flag_type__>(lhs) | static_cast<__flag_type__>(rhs)); }


#define BIT8(n)  (uint8) 1 << (n)
#define BIT16(n) (uint16)1 << (n) 
#define BIT32(n) (uint32)1 << (n)
#define BIT64(b) (uint64)1 << (n)

#endif
