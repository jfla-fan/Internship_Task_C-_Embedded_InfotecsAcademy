#ifndef __REAL_ANY__
#define __REAL_ANY__

#include "../Core.h"
#include <any>


/**
 * Could just use boost::any, but creating your own design is more challenging.
 */
namespace Real
{
	/**
	 * Base structure to keep any type.
	 * Used to keep const char* values of command line tokens.
	 */
	struct Any
	{
		template<typename _Ty>
		explicit Any(const _Ty& element) : content(element) { }

		Any() { }

		template<typename _Ty>
		FORCEINLINE decltype(auto) As() { return std::any_cast<_Ty&>(content); }

		template<typename _Ty>
		FORCEINLINE const _Ty& As() const { return std::any_cast<const _Ty&>(content); }

		template<typename _Ty>
		FORCEINLINE _Ty& As() { return std::any_cast<_Ty&>(content); }


	private:

		std::any content;

	};

	/**
	 * Structure used to keep command line option values and easily convert them to different types.
	 */
	struct AnyChar : public Any
	{
	public:

		using str_type = const TCHAR*;

		template<typename _Ty>
		explicit AnyChar(const _Ty& element) : Any(element) { }

		AnyChar() { }

		/// Returns original string pointer
		FORCEINLINE str_type Get() const { return Any::As<str_type>(); }

		/// Converts original string to given integer type
		template<typename _Integer, std::enable_if_t<std::is_integral<_Integer>::value, bool> = true>
		FORCEINLINE _Integer ToInteger() { return static_cast<_Integer>(std::atoi(Get())); }

		/// Converts original string to given floating type
		template<typename _Floating, std::enable_if_t<std::is_floating_point<_Floating>::value, bool> = true>
		FORCEINLINE _Floating ToFloat() { return static_cast<_Floating>(std::atof(Get())); }

		/// Converts original string to std::string object
		FORCEINLINE std::string ToString() { return std::string(Get()); }

		/// Returns original ANSI string
		FORCEINLINE const TCHAR* ToANSI() const { return Get(); }


	};

}



#endif

