#include "PortA.h"



namespace Real
{

	PortA* PortA::Instance = nullptr;

	PortA::RegistersT& PortA::Registers = *RegistersT::GetInstance();

	PortA* PortA::GetInstance()
	{
		if (!Instance)
			_Initialize();

		return Instance;
	}

	PortA::PortA()
	{
		_Initialize(this);
	}

	PortA::PortA(uint8 DDR_register_scheme, uint8 PORT_register_scheme)
	{
		Registers.Clear();

		Registers.DDR.pins  = DDR_register_scheme;
		Registers.PORT.pins = PORT_register_scheme;

		_Initialize(this);
	}

	PortA::PortA(uint8 DDR_register_scheme, uint8 PORT_register_scheme, uint16 AF_register_scheme[5])
	{
		Registers.Clear();

		Registers.DDR.pins = DDR_register_scheme;
		Registers.PORT.pins = PORT_register_scheme;

		std::memcpy(Registers.AF.pins, AF_register_scheme, sizeof(AF_register_scheme));
	
		_Initialize(this);
	}

	/**
	 * Returns the pin mode of a particular pin.
	 *
	 * \param pinNumber pin which mode should be returned
	 * \return EPinMode pin mode
	 */
	EPinMode PortA::GetPinMode(uint8 pinNumber) const 
	{
		FATAL_ASSERT(pinNumber >= 1 && pinNumber <= 5, "PortA: pin number is too high or too low.");

		return EPinMode(Registers.GetPinMode(pinNumber));
	}

	/**
	 * Sets a particular pin's mode.
	 *
	 * \param pinNumber pin which mode should be set
	 * \param mode pin mode
	 */
	void PortA::SetPinMode(uint8 pinNumber, EPinMode mode)
	{
		FATAL_ASSERT(pinNumber >= 1 && pinNumber <= 5, "PortA: pin number is too high or too low.");
		
		if (mode == EPinMode::INPUT)
			Registers.SetInputMode(pinNumber);
		else
			Registers.SetOutputMode(pinNumber);
	}

	/**
	 * Returns a pin value.
	 *
	 * \param pinNumber pin which value should be returned
	 * \return EVoltageLevel value
	 */
	EVoltageLevel PortA::GetPinValue(uint8 pinNumber) const
	{
		FATAL_ASSERT(pinNumber >= 1 && pinNumber <= 5, "PortA: pin number is too high or too low.");

		return EVoltageLevel(Registers.GetPinValue(pinNumber));
	}

	/**
	 * Sets a particular pin's value.
	 *
	 * \param pinNumber pin which value should be set
	 * \param level value that should be set
	 */
	void PortA::SetPinValue(uint8 pinNumber, EVoltageLevel level) 	
	{
		FATAL_ASSERT(pinNumber >= 1 && pinNumber <= 5, "PortA: pin number is too high or too low.");

		if (level == HIGH)
			Registers.SetHighPinValue(pinNumber);
		else
			Registers.SetLowPinValue(pinNumber);
	}

	/**
	 * Returns value representing all the alternate functions that a particular pin supports.
	 *
	 * \param pinNumber				pin which state should be returned.
	 * \return EAlternateFunction	object representing alternate functions.
	 */
	EAlternateFunction PortA::GetAlternateFunctions(uint8 pinNumber) const
	{
		FATAL_ASSERT(pinNumber >= 1 && pinNumber <= 5, "PortA: pin number is too high or too low.");

		return EAlternateFunction(Registers.GetAlternateFunctions(pinNumber));
	}

	/**
	 * Sets a particular pin's function flag.
	 *
	 * \param pinNumber pin which state should be modified.
	 * \param functions flags.
	 */
	void PortA::SetAlternateFunctions(uint8 pinNumber, EAlternateFunction functions)
	{
		FATAL_ASSERT(pinNumber >= 1 && pinNumber <= 5, "PortA: pin number is too high or too low.");

		Registers.SetAlternateFunctions(pinNumber, static_cast<uint16>(functions));
	}

	/**
	 * Sets specified flags to zero.
	 *
	 * \param pinNumber pin which state should be updated.
	 * \param functions flags.
	 */
	void PortA::DisableAlternateFunctions(uint8 pinNumber, EAlternateFunction functions)
	{
		FATAL_ASSERT(pinNumber >= 1 && pinNumber <= 5, "PortA: pin number is too high or too low.");

		Registers.DisableAlternateFunctions(pinNumber, static_cast<uint16>(functions));
	}

	/**
	 * Returns a voltage level on the input contacts of specified pin.
	 *
	 * \param pinNumber pin which state should be checked.
	 * \return EVoltageLevel voltage level on the pin.
	 */
	EVoltageLevel PortA::ReadInputValue(uint8 pinNumber) const
	{
		FATAL_ASSERT(pinNumber >= 1 && pinNumber <= 5, "PortA: pin number is too high or too low.");

		return EVoltageLevel(Registers.ReadInputValue(pinNumber));
	}

	/**
	 * Checks if specified functions are supported on a particular pin.
	 *
	 * \param pinNumber pin which state should be checked
	 * \param functions flags
	 * \return bool
	 */
	bool PortA::SupportFunctions(uint8 pinNumber, EAlternateFunction functions) const
	{
		FATAL_ASSERT(pinNumber >= 1 && pinNumber <= 5, "PortA: pin number is too high or too low.");

		return EAlternateFunction(Registers.GetAlternateFunctions(pinNumber) & functions) != EAlternateFunction::NONE;
	}

}
