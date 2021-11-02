#ifndef __REAL_BOARD__
#define __REAL_BOARD__

#include "PortA.h"
#include <array>



namespace Real {


	/**
	 * \brief Platform board.
	 * 
	 * Structure that contains basic platform data.
	 * Allows to access all the ports and operate them utilizing IPort interface.
	 */
	class Board
	{
	public:

		Board();

		static Board* GetInstance();

		/// Shows if this board has analog input ports
		static FORCEINLINE bool HasAnalogInputs() { return true; }

		/// Shows if this board has ports that support PWM
		static FORCEINLINE bool SupportPWM()  { return true; }

		/// Shows if this board has ports that support UART
		static FORCEINLINE bool SupportUART() { return true; }

		/// Shows if this board has ports that support I2C
		static FORCEINLINE bool SupportI2C()  { return true; }

		/// Shows if this board has ports that support SPI
		static FORCEINLINE bool SupportSPI()  { return true; }

		/// Shows if this board has ports that support Serial interface
		static FORCEINLINE bool SupportSerial() { return true; }

		IPort* GetPort(uint8 port_index) const;

		template<typename PortT> static PortT* GetPort() { return PortT::GetInstance(); }


	private:

		/// Board object
		static Board* Instance;

		/**
		 * Array that contains all the ports pointers.
		 * Used to set state for all the ports simultaneously.
		 */
		std::array<IPort*, Internal::_PORT_COUNT_> Ports;

	};

	

}


#endif

