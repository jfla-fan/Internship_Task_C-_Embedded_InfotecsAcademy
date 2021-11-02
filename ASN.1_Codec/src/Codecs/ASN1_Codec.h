#ifndef __REAL_ASN1_CODEC__
#define __REAL_ASN1_CODEC__

#include "../Core.h"
#include "ICodec.h"

#define ASN1_CODEC_USED


namespace Real { namespace Codecs {

	
	/**
	 *	\todo implement encoding and decoding for other types of data utilizing ASN.1 standard in future.
	 */

	namespace ASN1CodecOptions
	{

		constexpr uint8 _UNIVERSAL_CLASS_TAG_BITS_			= 0b00000000u;
		constexpr uint8 _APPLICATION_CLASS_TAG_BITS_		= 0b01000000u;
		constexpr uint8 _CONTEXTSPECIFIC_CLASS_TAG_BITS_	= 0b10000000u;
		constexpr uint8 _PRIVATE_CLASS_TAG_BITS_			= 0b11000000u;

		constexpr uint8 _PRIMITIVE_PC_TAG_BITS_				= 0b00000000u;
		constexpr uint8 _CONSTRUCTED_PC_TAG_BITS_			= 0b00100000u;


		/**
		 * Types that ASN.1 codec can process. Only supports an OctetString with universal tag now.
		*/
		enum class EASN1ValueType : uint8
		{
			Boolean = 1,
			Integer,
			BitString,
			OctetString,
			Null,
			ObjectIdentifier,
			ObjectDescriptor,
			InstanceOfExternal,
			Real,
			Enumerated,
			EmbeddedPDV,
			// ... lots of other types, no support for them now
			MaxASN1Values
		};

		/**
		 * Represents ASN.1 classes of tag.
		 */
		enum class EASN1ClassTagType : uint8
		{
			UNIVERSAL = _UNIVERSAL_CLASS_TAG_BITS_,
			APPLICATION = _APPLICATION_CLASS_TAG_BITS_,
			CONTEXT_SPECIFIC = _CONTEXTSPECIFIC_CLASS_TAG_BITS_,
			PRIVATE = _PRIVATE_CLASS_TAG_BITS_,
			ASN1ClassTagCount = 4
		};

		/**
		 * Enum that allows to choose between Primitive and Constructed options.
		 */
		enum class EASN1PCType : uint8
		{
			PRIMITIVE = _PRIMITIVE_PC_TAG_BITS_,
			CONSTRUCTED = _CONSTRUCTED_PC_TAG_BITS_,
			ASN1PCCount = 2
		};


		/// Union that allows operations on identifier octet
		union IDENTIFIER_OCTET
		{
			uint8 Content;	///< identifier byte
			
			uint8 bit8 : 1;
			uint8 bit7 : 1;

			uint8 bit6 : 1; 

			uint8 bit5 : 1;
			uint8 bit4 : 1;
			uint8 bit3 : 1;
			uint8 bit2 : 1;
			uint8 bit1 : 1;


			uint8 CLASS() const { return Content & 0b11000000; }
			uint8 PC() const { return Content & 0b00100000; }
			uint8 TAG_NUMBER() const { return Content & 0b00011111; }

			void SetClass(EASN1ClassTagType type) { Content = (Content & 0b00111111u) | static_cast<uint8>(type); }
			void SetPC(EASN1PCType type) { Content = (Content & 0b11011111) & static_cast<uint8>(type); }
			void SetTagNumber(uint8 number) { Content = (Content & 0b11100000) & number; }
		};

		/// Returns string representation of a token value type
		std::string GetASN1ValueTypeString(EASN1ValueType type);

	}


	/**
	 * Encodes and decodes data utilizing ASN.1 standard    
	 */
	class ASN1_Codec : public Codec
	{
	public:

		typedef uint64 SIZE_TYPE;
		class ASN1EncodedToken;

	public:
		

		ASN1_Codec() { };
		~ASN1_Codec() override { };


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
		static ASN1EncodedToken EncodeToken(ASN1CodecOptions::EASN1ValueType value_type, ASN1CodecOptions::EASN1ClassTagType class_type, ASN1CodecOptions::EASN1PCType pc_type, const void* source, SIZE_TYPE length);

		FORCEINLINE const TCHAR* GetCodecName() const override { return "ASN.1 Codec"; }

	protected:

		class asn1_unsupported_token : public unsupported_token
		{
		public:

			asn1_unsupported_token() : unsupported_token() { }

			explicit asn1_unsupported_token(const TCHAR* const _Message) NOEXCEPT
				: unsupported_token(_Message) { }

			NODISCARD const TCHAR* what() const override 
			{
				return "asn.1 codec error: unsupported token.";
			}
		};

		class asn1_bad_sequence : public bad_sequence
		{
			asn1_bad_sequence() : bad_sequence() { }

			explicit asn1_bad_sequence(const TCHAR* const _Message) NOEXCEPT
				: bad_sequence(_Message) { }

			NODISCARD virtual const TCHAR* what() const override
			{
				return "asn.1 codec error: bad sequence.";
			}

		};

		/// 
		/// Takes a token and sets identifier octet.
		/// To get fully encoded should also construct length field and encode value content.
		/// 
		/// \param goal			token structure that contains token data
		/// \param value_type	type of token value
		/// \param class_type	type of identifier octet class
		/// \param pc_type		type of pc field (primitive / constructed)
		static void ConstructIdentifierOctet(ASN1EncodedToken& goal, ASN1CodecOptions::EASN1ValueType value_type, ASN1CodecOptions::EASN1ClassTagType class_type, ASN1CodecOptions::EASN1PCType pc_type);
		
		/**
		 * Takes a token and sets length field.
		 * To get fully encoded token should also construct identifier octet and encode value content.
		 * 
		 * \param goal		token structure that contains token data
		 * \param length	length of the content in bytes
		 */
		static void ConstructLengthField(ASN1EncodedToken& goal, SIZE_TYPE length);

		/**
		 * Takes a token, encode the source sequence of bytes and sets corresponding token field.
		 * To get fully encoded token should also construct identifier and length field.
		 * 
		 * \param goal		token structure that contains token data
		 * \param source	input source
		 * \param length	length of the content
		 */
		static void EncodeOctetString(ASN1EncodedToken& goal, const void* source, SIZE_TYPE length);

	public:

		class ASN1EncodedToken
		{
		public:

			friend class ASN1_Codec;

		private:

			bool								bIsEncoded; ///< checks if this token has been already encoded

			ASN1CodecOptions::EASN1ValueType	ValueType;	///< type of token value


			struct {

				SIZE_TYPE			Value; ///< Actual length

				/**
				 * Fits in one byte if length is less than 128.
				 * Otherwise it is encoded as big endian number.
				 */
				BYTE*				EncodedLengthSequence;
				uint8				NumberOfEncodedBytes;

			} 
			Length;

			struct {
				
				ASN1CodecOptions::IDENTIFIER_OCTET	IdentifierOctet; ///< octet itself
				
				/**
				 * nullptr if tag number fits in 5 bits.
				 * Otherwise it is encoded in a special format.
				 */
				BYTE*	EncodedTagNumberBytes;

				/**
				 * Number of all the bytes including leading octet.
				 */
				int8	NumberOfEncodedBytes;

			} 
			Identifier;

			struct 
			{
				BYTE* Value;

				SIZE_TYPE NumberOfEncodedBytes;
			} 
			Content;

		public:

			ASN1EncodedToken(ASN1CodecOptions::EASN1ValueType type, SIZE_TYPE length)
				: bIsEncoded(false), ValueType(type)
			{
				Length.Value = length;
				Length.EncodedLengthSequence = nullptr;
				Length.NumberOfEncodedBytes = 0;

				Identifier.EncodedTagNumberBytes = nullptr;
				Identifier.NumberOfEncodedBytes = 0;

				Content.Value = nullptr;
				Content.NumberOfEncodedBytes = 0;
			}

			/// Checks if this token has been already encoded (preferably with ASN1_Codec::EncodeToken function).
			FORCEINLINE bool IsEncoded() const { return bIsEncoded; }

			/// Returns an identifier.
			FORCEINLINE ASN1CodecOptions::IDENTIFIER_OCTET GetIdentifier() const { return Identifier.IdentifierOctet; }

			/// Returns a pointer to the identifier encoded tag number sequence. Always nullptr.
			FORCEINLINE const BYTE* GetIndentifierBytes() const { return Identifier.EncodedTagNumberBytes; }

			/// Returns number of bytes that represent tag number. Always 0.
			FORCEINLINE SIZE_TYPE GetIdentifierBytesCount() const { return Identifier.NumberOfEncodedBytes; }

			/// Returns the length of the content.
			FORCEINLINE SIZE_TYPE GetLength() const { return Length.Value; }

			/// Returns pointer to encoded length bytes sequence.
			FORCEINLINE const BYTE* GetLengthBytes() const { return Length.EncodedLengthSequence; }

			/// Returns number of bytes in length bytes sequence.
			FORCEINLINE uint8 GetLengthBytesCount() const { return Length.NumberOfEncodedBytes; }

			/// Returns pointer to the content.
			FORCEINLINE const BYTE* GetContentBytes() const { return Content.Value; }

			/// Returns number of bytes in encoded content bytes sequence.
			FORCEINLINE SIZE_TYPE GetContentBytesCount() const { return Content.NumberOfEncodedBytes; }

			/// Returns the value type of this token.
			FORCEINLINE ASN1CodecOptions::EASN1ValueType GetValueType() const { return ValueType; }

			/**
			 * Allows easily write encoded tokens to streams.
			 * 
			 * \param os	stream where token should be written to
			 * \param token token
			 * 
			 * \return std::ostream& link to the stream
			 */
			friend std::ostream& operator << (std::ostream& os, const ASN1EncodedToken& token);


		};


	};



} }


#endif

