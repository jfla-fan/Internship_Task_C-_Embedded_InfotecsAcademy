#ifndef __REAL_PARSED_ARGUMENTS__
#define __REAL_PARSED_ARGUMENTS__

#include "../Core.h"

#include "Any.hpp"
#include <unordered_map>
#include <map>


namespace Real {

	/**
	 * A structure that allows access parsed options.
	 */
	class CORE_API ParsedArguments
	{
	public:

		typedef std::unordered_map<std::string, AnyChar> var_map_type;

		typedef var_map_type::iterator iterator;
		typedef var_map_type::const_iterator const_iterator;

	public:

		/**
		 * Safely retrieves corresponding option value.
		 * 
		 * \param[in] option option name used for retrieving option value
		 * \return AnyChar object
		 */
		inline AnyChar Get(const std::string& option) 
		{
			if (VariableMap.count(option) > 0)
				return VariableMap[option];
			return AnyChar();
		}
		
		/// Returns the number of parsed objects.
		FORCEINLINE uint32 Count() const { return VariableMap.size(); }

		/// Checks if given option was passed correctly
		FORCEINLINE bool Exists(const std::string& option) const { return VariableMap.count(option) > 0; }

		/// Adds another option
		FORCEINLINE void Add(const std::string& option, const AnyChar& value) { VariableMap[option] = value; }
		
		/// Returns internal variable map
		FORCEINLINE var_map_type& GetVariableMap() { return VariableMap; }

	private:

		var_map_type VariableMap;

	};



}

#endif
