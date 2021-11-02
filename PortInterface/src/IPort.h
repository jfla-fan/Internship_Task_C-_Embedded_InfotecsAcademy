#ifndef __REAL_PORT_INTERFACE__
#define __REAL_PORT_INTERFACE__

#include "Core.h"
#include "Internal.hpp"


#define HIGH Real::EVoltageLevel::HIGH_LEVEL
#define LOW	 Real::EVoltageLevel::LOW_LEVEL




namespace Real {
	using namespace Internal;

	class Board;

	/**
	 * General interface for controlling GPIO with ports.
	 */
	class IPort
	{
	public:

		friend class Board;

	public:

		/// Returns the amount of pins in this port.
		virtual uint8				GetPinsCount() const = 0;

		/**
		 * Returns the pin mode of a particular pin.
		 *
		 * \param pinNumber pin which mode should be returned.
		 * \return EPinMode pin mode.
		 */
		virtual EPinMode			GetPinMode(uint8 pinNumber) const = 0;

		/**
		 * Sets a particular pin's mode.
		 *
		 * \param pinNumber pin which mode should be set.
		 * \param mode		pin mode.
		 */
		virtual void				SetPinMode(uint8 pinNumber, EPinMode mode) = 0;

		/**
		 * Returns a pin value.
		 *
		 * \param pinNumber			pin which value should be returned.
		 * \return EVoltageLevel	value.
		 */
		virtual EVoltageLevel		GetPinValue(uint8 pinNumber) const = 0;

		/**
		 * Sets a particular pin's value.
		 *
		 * \param pinNumber pin which value should be set.
		 * \param level		value that should be set.
		 */
		virtual void				SetPinValue(uint8 pinNumber, EVoltageLevel level) = 0;

		/**
		 * Returns value representing all the alternate functions that a particular pin supports.
		 *
		 * \param pinNumber				pin which state should be returned.
		 * \return EAlternateFunction	object representing alternate functions.
		 */
		virtual EAlternateFunction	GetAlternateFunctions(uint8 pinNumber) const = 0;

		/**
		 * Sets a particular pin's function flag.
		 *
		 * \param pinNumber pin which state should be modified.
		 * \param functions flags.
		 */
		virtual void				SetAlternateFunctions(uint8 pinNumber, EAlternateFunction functions) = 0;

		/**
		 * Sets specified flags to zero.
		 *
		 * \param pinNumber pin which state should be updated.
		 * \param functions flags.
		 */
		virtual void				DisableAlternateFunctions(uint8 pinNumber, EAlternateFunction functions) = 0;

		/**
		 * Returns a voltage level on the input contacts of specified pin.
		 * 
		 * \param pinNumber pin which state should be checked.
		 * \return EVoltageLevel voltage level on the pin.
		 */
		virtual EVoltageLevel		ReadInputValue(uint8 pinNumber) const = 0;

		/**
		 * Checks if specified functions are supported on a particular pin.
		 *
		 * \param pinNumber pin which state should be checked
		 * \param functions flags
		 * \return bool
		 */
		virtual bool				SupportFunctions(uint8 pinNumber, EAlternateFunction functions) const = 0;


	};

}



#endif