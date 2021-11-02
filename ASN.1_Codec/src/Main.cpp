#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

#include "Misc/CommandLine.h"
#include "Codecs/ASN1_Codec.h"


/// Returns text with instructions.
extern const TCHAR* GetReference();


int main(int32 argc, TCHAR** argv)
{
	using namespace Real;
	using namespace Real::Codecs;
	using namespace Real::Codecs::ASN1CodecOptions;

	CommandLine::BuildFromArgc(argc, argv);

	ParsedArguments parsed = CommandLine::Parse(CommandLine::GetOriginal(), Real::EOptionType::ALL);

	if (parsed.Count() == 2)
	{
		const auto& [InputFileName, Value1] = *parsed.GetVariableMap().begin();
		const auto& [OutputFileName, Value2] = *(++parsed.GetVariableMap().begin());

		std::ifstream ifs(InputFileName, std::ios::in | std::ios::binary);
		
		if (!ifs.good())
		{
			LOG("Cannot open " << InputFileName << " file. Something went wrong.\n");
			LOG("Reference: \n" << GetReference());
			return 1;
		}

		std::ofstream ofs(OutputFileName, std::ios::out | std::ios::binary);

		if (!ofs.good())
		{
			LOG("Cannot open " << OutputFileName << " file. Something went wrong.\n");
			LOG("Reference: \n" << GetReference());
			return 1;
		}


		std::string sequence((std::istreambuf_iterator<TCHAR>(ifs)),
								std::istreambuf_iterator<TCHAR>());


		auto token = ASN1_Codec::EncodeToken(EASN1ValueType::OctetString, EASN1ClassTagType::UNIVERSAL, EASN1PCType::PRIMITIVE, sequence.c_str(), sequence.size());

		ofs << token;

		ifs.close();
		ofs.close();

	}

	else if (parsed.Count() == 1)
	{
		const auto& [Command, Value] = *parsed.GetVariableMap().begin();

		if (Command != "-")
		{
			LOG("This is not '-' sign.\n");
			LOG("Reference: \n" << GetReference());
			return 1;
		}

		std::string input_sequence; 
		std::getline(std::cin, input_sequence);
		
		auto token = ASN1_Codec::EncodeToken(EASN1ValueType::OctetString, EASN1ClassTagType::UNIVERSAL, EASN1PCType::PRIMITIVE, input_sequence.c_str(), input_sequence.size());

		std::stringstream output;
		output << token;

		BYTE c;
		while (output.get(c))
		{
			std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)c << " ";
		}

	}

	else
	{
		LOG("You did not enter allowed options.\nSee reference:\n" << GetReference());
		return 1;
	}

	return 0;
}


const TCHAR* GetReference()
{
	return
		"This application encodes given sequence of bytes into octet string with universal tag utilizing ASN.1 standard.\n"
		"You can enter 2 file names or '-' sign.\n"
		"Examples:\n"
		"\"input.txt output.txt\" - original sequence of bytes will be taken from input.txt and encoded sequence will be written to output.txt\n"
		"\"-\" - original sequence of bytes is taken from standard input and encoded sequence will be written to standard output."
		;
}