#include "CommandLine.h"
#include <cstring>
#include <string>
#include <cctype>


namespace Real {
	
	
	TCHAR CommandLine::CmdLine[CommandLine::MaxCommandLineSize] {'\0'};

	bool CommandLine::bIsInitialized = false;

	CommandLineParser CommandLine::CmdParser = CommandLineParser();


	/**
	 * Parses an option from a stream. Supports options started with -, -- or /.
	 * 
	 * \param[in]  stream	character stream
	 * \param[out] type		parsed type
	 * \param[out] option	parsed option
	 * 
	 * \return const TCHAR* indicating result of parsing (nullptr - failure, valid ptr - first position after parsed value in a stream)
	 */
	const TCHAR* CommandLineParser::ParseOption(const TCHAR* stream, EOptionType& type, std::string& option)
	{
#ifdef REAL_DEBUG_MODE
		WARN_ASSERT(stream != nullptr, "CommandLineParser::ParseOption: stream is nullptr");
#endif

		if (!stream) return nullptr;

		// omit spaces
		while (*stream && std::isspace(*stream)) ++stream;

		switch (*stream)
		{
			case '-':
			{
				++stream;
			
				type = (*stream == '-') ? EOptionType::DOUBLE_HYPHEN : EOptionType::HYPHEN;
				if (type == EOptionType::DOUBLE_HYPHEN) ++stream;

				// parsing option
				while (*stream && !std::isspace(*stream) && *stream != '=')
					option += *stream++;

				if (!*stream) return nullptr;
				break;
			}

			case '/':
			{
				type = EOptionType::SWITCH;

				while (*++stream && !std::isspace(*stream))
					option += *stream;

				if (!*stream) return nullptr;
				break;
			}

			case '\0':
				return nullptr;

			default:
			{
#ifdef REAL_DEBUG_MODE
				WARN("CommandLineParser::ParseOption: Could not define option type");
#endif	
				//--stream;
				type = EOptionType::NONE;
				
				return stream;
			}

		}

		return stream;
	}

	/**
	 * Parses option value surrounded by spaces.
	 *
	 * \param[in]  stream character stream
	 * \param[out] value  parsed value
	 * 
	 * \return const TCHAR* indicating result of parsing (nullptr - failure, valid ptr - first position after parsed value in a stream)
	 */
	const TCHAR* CommandLineParser::ParseValue(const TCHAR* stream, std::string& value)
	{
#ifdef REAL_DEBUG_MODE
		WARN_ASSERT(stream != nullptr, "CommandLineParser::ParseValue: stream is nullptr");
#endif

		if (!stream) return nullptr;

		// skip spaces and '=' symbols
		while (*stream && (std::isspace(*stream) || *stream == '=')) ++stream;

		// do not parse options
		if (Char::IsOptionSign(*stream)) return nullptr;

		// parse the whole value
		while (*stream && !std::isspace(*stream)) value += *stream++;

		// if the end of the stream
		if (!*stream) return nullptr;

		return stream;
	}

	/**
	 * Parses token consisted of an option and/or its value.
	 *
	 * \param[in]  stream			character stream
	 * \param[out] token			token structure containing option name, option value and option type flag
	 * \param[in]  allowedOptions	allowed option flags
	 * 
	 * \return const TCHAR* indicating result of parsing (nullptr - failure, valid ptr - first position after parsed value in a stream)
	 */
	const TCHAR* CommandLineParser::ParseToken(const TCHAR* stream, CMDLN_TOKEN& token, EOptionType allowedOptions) {
#ifdef REAL_DEBUG_MODE
		WARN_ASSERT(stream != nullptr, "CommandLineParser::ParseToken: stream is invalid");
#endif
		const TCHAR* tempPointer = nullptr;
		const TCHAR* pointer = nullptr;
		
		tempPointer = ParseOption(stream, token.type, token.name);

		// if couldn't parse option this can be a value
		if (token.type == EOptionType::NONE && any(allowedOptions & EOptionType::SPACE))
		{
			token.type = EOptionType::SPACE;
			pointer = stream;
		}
		// successfully parsed an option
		else
		{
			pointer = tempPointer;
		}
		

		// if any option exists or we just have to parse a value
		if (pointer || any(allowedOptions & EOptionType::SPACE))
		{
			// successfully parsed a value
			if ((tempPointer = ParseValue(pointer, token.value)))
				pointer = tempPointer;
			// couldn't parse anything
			else if (token.type == EOptionType::SPACE)
			{
				token.type = EOptionType::NONE;
				pointer = nullptr;
			}
		}


		return pointer;
	}

	/// Reset state to uninitialized
	void CommandLine::Reset()
	{
		bIsInitialized = false;
		CmdLine[0] = '\0';
	}

	/// Constructs command line string and stores it into CmdLine
	void CommandLine::BuildFromArgc(int32 argc, TCHAR* argv[])
	{
		uint32 cmdLength = std::strlen(CmdLine);
		uint32 length;
		for (int32 i = 1; i < argc; ++i)
		{
			length = std::strlen(argv[i]);

			FATAL_ASSERT(length + cmdLength + 1 <= MaxCommandLineSize, "CommandLine::BuildFromArgc: too many symbols");
			
			std::memcpy(CmdLine + cmdLength, argv[i], length);
			CmdLine[cmdLength + length] = ' ';
			cmdLength += (length + 1);
		}

		CmdLine[cmdLength] = '\0';
		bIsInitialized = true;
	}

	/// Sets a new command line
	bool CommandLine::Set(const TCHAR* NewCommandLine)
	{
		if (!bIsInitialized)
		{
			std::strncpy(CmdLine, NewCommandLine, REAL_ARRAY_COUNT_TYPED(NewCommandLine, TCHAR));
			return true;
		}
#ifdef REAL_DEBUG_MODE
		WARN("CommandLine::Set: cmd line has already been initialized");
#endif
		return false;
	}

	/// Appends a new String to an existing command line as it is (without spaces)
	void CommandLine::Append(const TCHAR* CharString)
	{
		SIZE_T LastIndex = std::strlen(CmdLine);
		SIZE_T ExtraSize = std::strlen(CharString);

		FATAL_ASSERT(LastIndex + ExtraSize + 2 <= MaxCommandLineSize, "too big size for cmd");
		
#ifdef REAL_DEBUG_MODE
		if (!bIsInitialized)
			WARN("CommandLine: better initialize before appending");
#endif

		CmdLine[LastIndex] = ' ';
		std::memcpy(CmdLine + LastIndex + 1, CharString, ExtraSize);
		CmdLine[LastIndex + ExtraSize + 2] = '\0';
	}

	/**
	 * Parses command line (should be initialized) and returns parsed options.
	 * Returned options will satisfy allowedOptions flags.
	 *
	 * \param[in] InCmdLine			input command line without a name of a program
	 * \param[in] allowedOptions	allowed options flag
	 *
	 * \return ParsedArguments object which maps to parsed values
	 */
	ParsedArguments CommandLine::Parse(const TCHAR* InCmdLine, EOptionType allowedOptions)
	{
		FATAL_ASSERT(InCmdLine, "CommandLine::Parse: InCmdLine is invalid");

		ParsedArguments args;

		CMDLN_TOKEN curr;
		TCHAR* string_pointer = nullptr;
		
		while ((InCmdLine = CmdParser.ParseToken(InCmdLine, curr)))
		{
			if (any(curr.type & allowedOptions))
			{
				std::string& value_string = curr.value;
				
				// if we don't have an option, but have a value, then
				// we can store it as an option without option signs (to easily find it in ParsedArguments object).
				if (curr.type == EOptionType::SPACE)
					std::swap(curr.name, curr.value);
				
				curr.name = Char::ConstructOption(curr.type, curr.name);

				string_pointer = new TCHAR[value_string.size() + 1];
				value_string.copy(string_pointer, value_string.size());
				string_pointer[value_string.size()] = '\0';


				args.Add(curr.name, AnyChar((const char*)string_pointer));
			}

			curr.Reset();
		}

		return args;
	}

}