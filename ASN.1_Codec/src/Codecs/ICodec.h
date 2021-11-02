#ifndef __REAL_ICODEC__
#define __REAL_ICODEC__

#include <exception>
#include "../Core.h"



namespace Real { namespace Codecs {


	/** /************************************************************************/
	/*							Common Codec Interface                          */
	/************************************************************************ / */
	class Codec
	{
	public:

		Codec() { }
		virtual ~Codec() { }


		/**
			* Encodes length bytes of sequence and writes them to the destination.
			* By default just copies the sequence of bytes.
			*
			* \param[in]  sequence 		source to get bytes from
			* \param[out] destination	target
			* \param[in]  length		number of bytes to encode
			*/
		virtual void Encode(void* sequence, void* destination, int32 length)
		{
			for (int i = 0; i < length; ++i)
				*((BYTE*)destination + i) = *((BYTE*)sequence + i);
		};

		/**
			* Decodes sequence of bytes.
			* By default just copies the sequence of bytes.
			* 
			* \param[in]  from		source to get bytes from
			* \param[out] to		destination to write decoded bytes to
			* \param[in]  length	number of bytes to decode
			*/
		virtual void Decode(void* from, void* to, int32 length)
		{
			for (int i = 0; i < length; ++i)
				*((BYTE*)to + i) = *((BYTE*)from + i);
		}

		/// Returns this codec's name
		virtual FORCEINLINE const TCHAR* GetCodecName() const { return "Base Codec"; }

		// Exceptions 

		/**
		 * This exception indicates that given sequence is inappropriate for given purposes.
		 */
		class bad_sequence : public std::exception
		{
		public:

			bad_sequence() : exception() { }

			explicit bad_sequence(const TCHAR* const _Message) NOEXCEPT
				: exception(_Message) { }

			NODISCARD virtual const TCHAR* what() const override
			{
				return "CodecError: could not encode given sequence";
			}
		

		};


		/**
		 * Indicates that token cannot be processed by this codec.
		 */
		class unsupported_token : public std::exception
		{
		public:

			unsupported_token() : exception() { }

			explicit unsupported_token(const TCHAR* const _Message) NOEXCEPT
				: exception(_Message) { }

			NODISCARD virtual const TCHAR* what() const override
			{
				return "CodecError: could not handle this type of token.";
			}

		};


	};


} }



#endif
