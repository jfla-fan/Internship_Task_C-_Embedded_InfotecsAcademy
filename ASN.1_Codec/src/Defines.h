#ifndef __REAL_DEFINES__
#define __REAL_DEFINES__

#include <iostream>

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

#define LOG(__message__) std::cout << __message__ << '\n'

#define CORE_LOG(__message__) std::cout << "Core : " << __message__ << '\n'

#define FATAL_ASSERT(__cond__, __message__)					\
				do {										\
				if (!(__cond__))								\
				{											\
					std::cout << (__message__) << '\n';		\
					std::exit(EXIT_FAILURE);				\
				} } while(0)	


#define WARN(__message__) do {												\
					std::cout << "!Warning! -> " << __message__ << '\n';	\
				} while (0)										


#define WARN_ASSERT(__cond__, __message__)					\
				do {										\
					if (!(__cond__)) WARN(__message__);		\
				} while (0)									

template<typename _EnumType> FORCEINLINE bool any(const _EnumType & en) { return en != _EnumType::NONE; }

#define REAL_ARRAY_COUNT(__arr__)  (sizeof(__arr__) / sizeof(__arr__[0]))
#define REAL_ARRAY_COUNT_TYPED(__arr__, __type__) (sizeof(__arr__) / sizeof(__type__))

#define BIT8(n) (uint8) 1 << (n)
#define BIT16(n) (uint16)1 << (n)
#define BIT32(n) (uint32)1 << (n)
#define BIT64(n) (uint64)1 << (n)


#ifdef REAL_USE_WIDECHAR_TYPE
#define TEXT(__text__) L##__text__
#else
#define TEXT(__text__) __text__
#endif

#define DECLARE_ENUM_FLAG_OPERATIONS(__flag__, __flag_type__)	\
		FORCEINLINE __flag__ operator & (const __flag__& lhs, const __flag__& rhs) { return static_cast<__flag__>(static_cast<__flag_type__>(lhs) & static_cast<__flag_type__>(rhs)); }\
		FORCEINLINE __flag__ operator | (const __flag__& lhs, const __flag__& rhs) { return static_cast<__flag__>(static_cast<__flag_type__>(lhs) | static_cast<__flag_type__>(rhs)); }

#endif
