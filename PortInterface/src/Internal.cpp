#include "Internal.hpp"


namespace Real { namespace Internal {

	typedef _PORTA_REGISTERS_STRUCT_ ARegistersT;
	

	ARegistersT* ARegistersT::Instance = nullptr;

	ARegistersT* ARegistersT::GetInstance()
	{
		if (!Instance)
		{
			Instance = new ARegistersT();
			Instance->Clear();
		}

		return Instance;
	}

	inline void ARegistersT::Clear()
	{
		DDR.pins = 0x00;
		PORT.pins = 0x00;
		PIN.pins = 0x00;
		AF = { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };
	}

} }
