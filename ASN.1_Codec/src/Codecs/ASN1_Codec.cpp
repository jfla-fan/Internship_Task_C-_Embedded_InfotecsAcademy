#include "ASN1_Codec.h"
#include "../Platform/Limits.h"
#include "../Misc/Endian.hpp"



namespace Real { namespace Codecs {

	using namespace ASN1CodecOptions;

	/// Returns string representation of a token value type
	std::string GetASN1ValueTypeString(EASN1ValueType type)
	{
		switch (type)
		{
		case EASN1ValueType::Boolean:
			return std::string("BOOLEAN");
		case EASN1ValueType::Integer:
			return std::string("INTEGER");
		case EASN1ValueType::Enumerated:
			return std::string("ENUMERATED");
		case EASN1ValueType::Real:
			return std::string("REAL");
		case EASN1ValueType::BitString:
			return std::string("BIT_STRING");
		case EASN1ValueType::OctetString:
			return std::string("OCTET_STRING");
		case EASN1ValueType::Null:
			return std::string("NULL");
		default:
			return std::string("MaxASN1Values");
		}
	}

	/**
	 * Fully encodes token of a given value_type, class tag type and pc type.
	 * Returns a structure that represents any correct type of token.
	 *
	 * \param value_type type of value the token stores
	 * \param class_type class type
	 * \param pc_type	 primitive/constructed
	 * \param source	 content stream
	 * \param length	 length of the content
	 *
	 * \return ASN1_Codec::ASN1EncodedToken structure that represents the token
	 */
	ASN1_Codec::ASN1EncodedToken ASN1_Codec::EncodeToken(EASN1ValueType value_type, EASN1ClassTagType class_type, EASN1PCType pc_type, const void* source, SIZE_TYPE length)
	{
		ASN1EncodedToken goal(value_type, length);

		// constructing identifier octet, other data became valid in constructor
		ConstructIdentifierOctet(goal, value_type, class_type, pc_type);

		// constructing length field
		ConstructLengthField(goal, length);
		
		// saving the content
		switch (value_type)
		{

		case EASN1ValueType::OctetString:
			EncodeOctetString(goal, source, length);
			break;

		default:
			throw asn1_unsupported_token{};
		}


		return goal;
	}

	/**
	 * Takes a token, encode the source sequence of bytes and sets corresponding token field.
	 * To get fully encoded token should also construct identifier and length field.
	 *
	 * \param goal		token structure that contains token data
	 * \param source	input source
	 * \param length	length of the content
	 */
	void ASN1_Codec::EncodeOctetString(ASN1EncodedToken& goal, const void* source, SIZE_TYPE length)
	{
		goal.Content.NumberOfEncodedBytes = length;
		goal.Content.Value = new BYTE[length];
		std::memcpy(goal.Content.Value, source, length);
	}

	/**
	 * Takes a token and sets length field.
	 * To get fully encoded token should also construct identifier octet and encode value content.
	 *
	 * \param goal		token structure that contains token data
	 * \param length	length of the content in bytes
	 */
	void ASN1_Codec::ConstructLengthField(ASN1EncodedToken& goal, SIZE_TYPE length) 
	{
		goal.Length.Value = length;
		// 7 bits max unsigned int value
		if (length <= MAX_INT8)
		{
			goal.Length.NumberOfEncodedBytes = 1;
			goal.Length.EncodedLengthSequence = new BYTE();
			// safe casting because length fits in 1 byte
			*goal.Length.EncodedLengthSequence = static_cast<uint8>(length);
		}
		else if (length <= MAX_UINT16)
		{
			goal.Length.NumberOfEncodedBytes = 3;
			goal.Length.EncodedLengthSequence = new BYTE[3];
			goal.Length.EncodedLengthSequence[0] = 0b10000010;

			uint16 big_endian_length = Endian::native_to_big<uint16>(length);
			std::memcpy(goal.Length.EncodedLengthSequence + 1, &big_endian_length, goal.Length.NumberOfEncodedBytes - 1);
		}
		else if (length <= MAX_UINT32)
		{
			goal.Length.NumberOfEncodedBytes = 5;
			goal.Length.EncodedLengthSequence = new BYTE[5];
			goal.Length.EncodedLengthSequence[0] = 0b10000101;

			uint32 big_endian_length = Endian::native_to_big<uint32>(length);
			std::memcpy(goal.Length.EncodedLengthSequence + 1, &big_endian_length, goal.Length.NumberOfEncodedBytes - 1);
		}
		// does not support 128-bit integers and more
		else
		{
			goal.Length.NumberOfEncodedBytes = 9;
			goal.Length.EncodedLengthSequence = new BYTE[9];
			goal.Length.EncodedLengthSequence[0] = 0b100001000;

			uint64 big_endian_length = Endian::native_to_big<uint64>(length);
			std::memcpy(goal.Length.EncodedLengthSequence + 1, &big_endian_length, goal.Length.NumberOfEncodedBytes - 1);
		}
	}

	/// 
	/// Takes a token and sets identifier octet.
	/// To get fully encoded should also construct length field and encode value content.
	/// 
	/// \param goal			token structure that contains token data
	/// \param value_type	type of token value
	/// \param class_type	type of identifier octet class
	/// \param pc_type		type of pc field (primitive / constructed)
	void ASN1_Codec::ConstructIdentifierOctet(ASN1EncodedToken& goal, ASN1CodecOptions::EASN1ValueType value_type, ASN1CodecOptions::EASN1ClassTagType class_type, ASN1CodecOptions::EASN1PCType pc_type)
	{
		uint8 tag_number = static_cast<uint8>(value_type);

		// if size >= 31 last 5 bits should be equal to 1
		tag_number = (tag_number >= 31) ? 0b00011111 : static_cast<uint8>(tag_number);
		
		goal.Identifier.IdentifierOctet.Content = static_cast<uint8>(class_type) | static_cast<uint8>(pc_type) | tag_number;
	}


	/**
	* Allows easily write encoded tokens to streams.
	*
	* \param os	stream where token should be written to
	* \param token token
	*
	* \return std::ostream& link to the stream
	*/
	std::ostream& operator << (std::ostream& stream, const ASN1_Codec::ASN1EncodedToken& token)
	{
		stream << token.GetIdentifier().Content;

		for (ASN1_Codec::SIZE_TYPE i = 0; i < token.GetLengthBytesCount(); ++i)
		{
			stream << token.GetLengthBytes()[i];
		}

		for (ASN1_Codec::SIZE_TYPE i = 0; i < token.GetContentBytesCount(); ++i)
		{
			stream << token.GetContentBytes()[i];
		}

		return stream;
	}

} }
