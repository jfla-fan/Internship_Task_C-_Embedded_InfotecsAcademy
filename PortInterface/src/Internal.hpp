#ifndef __REAL_INTERNAL__
#define __REAL_INTERNAL__

#include "Core.h"




namespace Real { namespace Internal {

	/**
	 * Physical value representing low digital voltage level (V).
	 * If voltage level is lower or equal to this value, digital input value gets read as LOW.
	 */
	static constexpr float _LOW_VOLTAGE_LEVEL_	= 0.8f;
	
	/**
	 * Physical value representing high digital voltage level (V).
	 * If voltage level is higher or equal to this value, digital input value gets read as HIGH.
	 */
	static constexpr float _HIGH_VOLTAGE_LEVEL_ = 2.0f;

	/**
	 * Name of the board.
	 * This name is just an example, does not exist in real life.
	 */
	static constexpr const TCHAR* const _BOARD_NAME_ = "InfoTECH2021";

	/**
	 * Amount of ports on the board.
	 */
	static constexpr uint16 _PORT_COUNT_ = 1;

	/**
	 * Direction of port lines or just a mode.
	 */
	enum class EPinMode : uint8
	{
		INPUT = 0x00,
		OUTPUT = 0x01,
	};

	/**
	 * Enum for representing digital pins values.
	 */
	enum class EVoltageLevel : uint8
	{
		LOW_LEVEL = 0x00,
		HIGH_LEVEL = 0x01,
	};
	
	/**
	 * Set of possible alternate functions for pins.
	 */
	enum class EAlternateFunction : uint16
	{
		NONE = 0x0000,

		/// Pulse-Width-Modulation
		PWM  = BIT16(0),
		
		/// Serial Data Line		- I2C
		SDA  = PWM << 1,
		
		/// Serial Clock Line		- I2C
		SCL  = SDA << 1,

		/// Serial Clock			- SPI
		SCK  = SCL << 1,

		/// Master In Slave Out		- SPI
		MISO = SCK << 1,

		/// Master Out Slave In		- SPI
		MOSI = MISO << 1,

		/// Slave Select			- SPI
		SS	 = MOSI << 1,

		/// Interrupt
		INT  = SS << 1,

		/// Transmitter				- Serial
		TX   = INT << 1,

		/// Receiver				- Serial
		RX   = TX << 1,

		/// Analog input
		ANALOG_IN = RX << 1,
		

		EAlternateFunctionsCount = 11,
	};

	DECLARE_ENUM_FLAG_OPERATIONS(EAlternateFunction, uint16)


	/**
	 * This structure represents states of port A registers.
	 * Allows to change pins modes, read or write digital values or customize alternate functions of pins.
	 */
	struct _PORTA_REGISTERS_STRUCT_
	{
	public:

		/**
		 * Data Direction Register.
		 * Used to get/set pin modes.
		 * 0 - INPUT mode, 1 - OUTPUT mode.
		 */
		union
		{
			uint8 pins;

			uint8 pin1 : 1;
			uint8 pin2 : 1;
			uint8 pin3 : 1;
			uint8 pin4 : 1;
			uint8 pin5 : 1;

		}
		DDR;

		/**
		* 
		 * Used to set/get HIGH/LOW voltage levels.
		 * 1 - HIGH level, 0 - LOW level.
		 */
		union
		{
			uint8 pins;

			uint8 pin1 : 1;
			uint8 pin2 : 1;
			uint8 pin3 : 1;
			uint8 pin4 : 1;
			uint8 pin5 : 1;

		}
		PORT;

	private:
		/**
		 * Used to read state of inputs (INPUT mode).
		 * May be changed independently. 
		 * 1 - HIGH level, 0 - LOW level.
		 */
		union
		{
			uint8 pins;

			uint8 pin1 : 1;
			uint8 pin2 : 1;
			uint8 pin3 : 1;
			uint8 pin4 : 1;
			uint8 pin5 : 1;

		}
		PIN;

		static _PORTA_REGISTERS_STRUCT_* Instance;
	
	public:

		/**
		 * Used to get/set alternate functions flag.
		 */
		union  
		{
			uint16 pins[5];

			uint16 pin1 : 16;
			uint16 pin2 : 16;
			uint16 pin3 : 16;
			uint16 pin4 : 16;
			uint16 pin5 : 16;

		}
		AF;


		/**
		 * Returns a mode of a particular pin.
		 * 
		 * \note This internal function does not perform any checks on validness of input data in order to reach maximum performance.
		 * 
		 * \param pinNumber pin to perform actions on.
		 * \return uint8 pin mode (either 0 for LOW or 1 for HIGH).
		 */
		FORCEINLINE uint8 GetPinMode(uint8 pinNumber) const { return (DDR.pins & (0b10000000 >> (pinNumber - 1))) >= 1; }

		/**
		 * Sets a particular pin's mode as INPUT.
		 * 
		 * \note This internal function does not perform any checks on validness of input data in order to reach maximum performance.
		 * 
		 * \param pinNumber pin to perform changes on.
		 */
		FORCEINLINE void SetInputMode(uint8 pinNumber) { DDR.pins &= (0b01111111 >> (pinNumber - 1)); }
		
		/**
		 * Sets a particular pin's mode as OUTPUT.
		 * 
		 * \note This internal function does not perform any checks on validness of input data in order to reach maximum performance.
		 * 
		 * \param pinNumber pin to perform changes on.
		 */
		FORCEINLINE void SetOutputMode(uint8 pinNumber) { DDR.pins |= (0b10000000 >> (pinNumber - 1)); }

		/**
		 * Returns a value of a particular pin.
		 * 
		 * \note This internal function does not perform any checks on validness of input data in order to reach maximum performance.
		 * 
		 * \param pinNumber pin to perform actions on.
		 * \return uint8 pin value (either 0 for LOW or 1 for HIGH).
		 */
		FORCEINLINE uint8 GetPinValue(uint8 pinNumber) const { return (PORT.pins & (0b10000000 >> (pinNumber - 1))) >= 1; }

		/**
		 * Sets a particular pin's value as HIGH.
		 * 
		 * \note This internal function does not perform any checks on validness of input data in order to reach maximum performance.
		 * 
		 * \param pinNumber pin to perform changes on.
		 */
		FORCEINLINE void SetHighPinValue(uint8 pinNumber) { PORT.pins |= (0b10000000 >> (pinNumber - 1)); }

		/**
		 * Sets a particular pin's value as LOW.
		 * 
		 * \note This internal function does not perform any checks on validness of input data in order to reach maximum performance.
		 * 
		 * \param pinNumber pin to perform changes on.
		 */
		FORCEINLINE void SetLowPinValue(uint8 pinNumber) { PORT.pins &= (0b01111111 >> (pinNumber - 1)); }

		/**
		 * Returns a field representing a particular pin's alternate functions.
		 * 
		 * \note This internal function does not perform any checks on validness of input data in order to reach maximum performance.
		 * 
		 * \param  pinNumber pin to perform actions on.
		 * \return uint16    field representing alternate functions.
		 */
		FORCEINLINE uint16 GetAlternateFunctions(uint8 pinNumber) const { return AF.pins[pinNumber - 1]; }
		
		/**
		 * Sets a particular pin's function flag.
		 * 
		 * \note This internal function does not perform any checks on validness of input data in order to reach maximum performance.
		 * 
		 * \param pinNumber pin to perform changes on.
		 * \param function  value that should be set.
		 */
		FORCEINLINE void SetAlternateFunctions(uint8 pinNumber, uint16 function) { AF.pins[pinNumber - 1] |= function; }

		/**
		 * Disables the specified functions.
		 * 
		 * \note This internal function does not perform any checks on validness of input data in order to reach maximum performance.
		 * 
		 * \param pinNumber pin to perform changes on.
		 * \param functions functions that should be disabled.
		 */
		FORCEINLINE void DisableAlternateFunctions(uint8 pinNumber, uint16 functions) { AF.pins[pinNumber - 1] &= !functions; }

		/**
		 * Access physical values of signals on the port pin.
		 * Returns the resulting value.
		 * 
		 * \param pinNumber pin to perform actions on.
		 * \return uint8 input value (0 for LOW and 1 for HIGH).
		 */
		FORCEINLINE uint8 ReadInputValue(uint8 pinNumber) const { return (PIN.pins & (0b10000000 >> (pinNumber - 1))) >= 1; }

		/**
		 * Sets all the fields' bits to zeros.
		 */
		void Clear();
		

		FORCEINLINE void SetDDRScheme(uint8 scheme) { DDR.pins = scheme; }

		FORCEINLINE void SetPORTScheme(uint8 scheme) { PORT.pins = scheme; }

		FORCEINLINE void SetAFScheme(uint16 scheme[5]) { std::memcpy(AF.pins, scheme, sizeof(scheme)); }

		static _PORTA_REGISTERS_STRUCT_* GetInstance();

	};

} }







#endif
