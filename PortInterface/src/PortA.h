#ifndef __REAL_PORT_A__
#define __REAL_PORT_A__

#include "IPort.h"



namespace Real
{

	/**
	 * \brief 5 bits port.
	 * 
	 * 5 bits port with pins counted from 1 to 5.
	 * Should be initialized only once (as there's no more than 1 port A on the board).
	 */
	class PortA : public IPort
	{
	public:

		typedef Internal::_PORTA_REGISTERS_STRUCT_ RegistersT;
		friend struct RegisterT;

	public:

		PortA();

		PortA(uint8 DDR_register_scheme, uint8 PORT_register_scheme);

		PortA(uint8 DDR_register_scheme, uint8 PORT_register_scheme, uint16 AF_register_scheme[5]);


		static PortA* GetInstance();

		/// Returns the amount of pins in this port.
		uint8				GetPinsCount() const override { return 5; }

		/**
		 * Returns the pin mode of a particular pin.
		 * 
		 * \param pinNumber pin which mode should be returned.
		 * \return EPinMode pin mode.
		 */
		EPinMode			GetPinMode(uint8 pinNumber) const override;
		
		/**
		 * Sets a particular pin's mode.
		 * 
		 * \param pinNumber pin which mode should be set.
		 * \param mode		pin mode.
		 */
		void				SetPinMode(uint8 pinNumber, EPinMode mode) override;

		/**
		 * Returns a pin value.
		 * 
		 * \param pinNumber			pin which value should be returned.
		 * \return EVoltageLevel	value.
		 */
		EVoltageLevel		GetPinValue(uint8 pinNumber) const override;
		
		/**
		 * Sets a particular pin's value.
		 * 
		 * \param pinNumber pin which value should be set.
		 * \param level		value that should be set.
		 */
		void				SetPinValue(uint8 pinNumber, EVoltageLevel level) override;

		/**
		 * Returns value representing all the alternate functions that a particular pin supports.
		 * 
		 * \param pinNumber				pin which state should be returned.
		 * \return EAlternateFunction	object representing alternate functions.
		 */
		EAlternateFunction	GetAlternateFunctions(uint8 pinNumber) const override;

		/**
		 * Sets a particular pin's function flag.
		 * 
		 * \param pinNumber pin which state should be modified.
		 * \param functions flags.
		 */
		void				SetAlternateFunctions(uint8 pinNumber, EAlternateFunction functions) override;

		/**
		 * Sets specified flags to zero.
		 * 
		 * \param pinNumber pin which state should be updated.
		 * \param functions flags.
		 */
		void				DisableAlternateFunctions(uint8 pinNumber, EAlternateFunction functions) override;

		/**
		 * Returns a voltage level on the input contacts of specified pin.
		 *
		 * \param pinNumber pin which state should be checked.
		 * \return EVoltageLevel voltage level on the pin.
		 */
		EVoltageLevel		ReadInputValue(uint8 pinNumber) const override;

		/**
		 * Checks if specified functions are supported on a particular pin.
		 * 
		 * \param pinNumber pin which state should be checked
		 * \param functions flags
		 * \return bool
		 */
		bool				SupportFunctions(uint8 pinNumber, EAlternateFunction functions) const override;



	private:
		
		static PortA* Instance;

		static RegistersT& Registers;

		static FORCEINLINE void _Initialize(PortA* what = nullptr) { Instance = (what) ? what : new PortA(); }

	};


}





#endif
