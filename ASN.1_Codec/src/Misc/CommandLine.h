#ifndef __REAL_COMMAND_LINE__
#define __REAL_COMMAND_LINE__

#include "../Core.h"
#include <vector>
#include "ParsedArguments.hpp"


namespace Real {

	/**
	 * Option Type.
	 */
	enum class EOptionType : uint8
	{
		NONE			= 0x0,		///< invalid option
		SPACE			= BIT8(1),	///< file.exe value
		HYPHEN			= BIT8(2),	///< file.exe -option value
		DOUBLE_HYPHEN	= BIT8(3),	///< file.exe --option value
		SWITCH			= BIT8(4),	///< file.exe /option value
		ALL				= SPACE | HYPHEN | DOUBLE_HYPHEN | SWITCH, ///< all the options allowed
	};

	DECLARE_ENUM_FLAG_OPERATIONS(EOptionType, uint8)

	// command line token structure
	// @name - option name
	// @value - option value
	// @type - option type
	struct CORE_API CMDLN_TOKEN { 
	
		CMDLN_TOKEN() : name(""), value(""), type(EOptionType::NONE) { }

		/// set all the fields to default values
		void Reset() { name = ""; value = ""; type = EOptionType::NONE; }

		std::string name, value; 
		EOptionType type; 
	};

	/**
	 * Char namespace contains functions for managing char and string type values.
	 */
	namespace Char
	{
		/// checks if character is an option sign
		FORCEINLINE bool IsOptionSign(char c) { return c == '-' || c == '/'; }

		/**
		 * Constructs a string which consists of an appropriate option sign 
		 * and a given value
		 * \param type option type
		 * \param value given string
		 * \return string: option type specifier + given string
		 */
		inline std::string ConstructOption(EOptionType type, const std::string& value)
		{
			switch (type)
			{
			case EOptionType::NONE: case EOptionType::SPACE:
				return value;
			case EOptionType::HYPHEN:
				return "-" + value;
			case EOptionType::DOUBLE_HYPHEN:
				return "--" + value;
			case EOptionType::SWITCH:
				return "/" + value;
			default:
				return value;
			}
		}
	}

	/**
	 * Structure that contains functions for parsing command line.
	 */
	struct CORE_API CommandLineParser
	{
		/**
		 * Parses an option from a stream. Supports options started with 
		 * -, --, /
		 * 
		 * \param[in]  stream	character stream
		 * \param[out] type		parsed type
		 * \param[out] option	parsed option
		 * 
		 * \return const TCHAR* indicating result of parsing (nullptr - failure, valid ptr - first position after parsed value in a stream)
		 */
		const TCHAR* ParseOption(const TCHAR* stream, EOptionType& type, std::string& option);

		/**
		 * Parses option value surrounded by spaces.
		 * 
		 * \param[in]	 stream character stream
		 * \param[out]	 value  parsed value
		 * 
		 * \return const TCHAR* indicating result of parsing (nullptr - failure, valid ptr - first position after parsed value in a stream)
		 */
		const TCHAR* ParseValue(const TCHAR* stream, std::string& value);

		/**
		 * Parses token consisted of an option and/or its value.
		 * 
		 * \param[in]  stream			character stream
		 * \param[out] token			token structure containing option name, option value and option type flag
		 * \param[in]  allowedOptions	allowed option flags
		 * 
		 * \return const TCHAR* indicating result of parsing (nullptr - failure, valid ptr - first position after parsed value in a stream)
		 */
		const TCHAR* ParseToken(const TCHAR* stream, CMDLN_TOKEN& token, EOptionType allowedOptions = EOptionType::ALL);
	};

	struct CORE_API CommandLine
	{
		/// maximum size of the command line
		static const uint32 MaxCommandLineSize = 16384;

		/// Reset state to uninitialized
		static void Reset();

		/// Constructs command line string and stores it into CmdLine
		static void BuildFromArgc(int32 argc, TCHAR* argv[]);

		/// Returns pointer to original command line string
		static inline const TCHAR* GetOriginal()
		{
			FATAL_ASSERT(bIsInitialized == true, "CommandLine: cmd was not initialized");
			return CmdLine;
		}

		/// Checks if the command line has been initialized
		static FORCEINLINE bool IsInitialized() { return bIsInitialized; }

		/// Sets a new command line
		static bool Set(const TCHAR* NewCommandLine);

		/// Appends a new String to existing command line as it is (without spaces)
		static void Append(const TCHAR* CharString);

		/**
		 * Parses command line (should be initialized) and returns parsed options.
		 * Returned options will satisfy allowedOptions flags.
		 * 
		 * \param[in] InCmdLine			input command line without a name of a program	
		 * \param[in] allowedOptions	allowed options flag
		 * 
		 * \return ParsedArguments object which maps to parsed values
		 */
		static ParsedArguments Parse(const TCHAR* InCmdLine, EOptionType allowedOptions);

	private:

		/// Bool value indicates command line initialization state
		static bool bIsInitialized;

		// CommandLine parser
		static CommandLineParser CmdParser;

		// null-terminated line that contains command-line options
		static TCHAR CmdLine[MaxCommandLineSize];

	};



}



#endif
