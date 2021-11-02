#include "Board.h"




namespace Real
{

	Board* Board::Instance = nullptr;


	Board* Board::GetInstance()
	{
		if (!Instance)
			Instance = new Board();

		return Instance;
	}

	Board::Board()
	{
		Ports[0] = GetPort<PortA>();
	}

	IPort* Board::GetPort(uint8 port_index) const 
	{
		FATAL_ASSERT(port_index >= 0 && port_index < Internal::_PORT_COUNT_, "Board::GetPort(uint8 port_index) - out_of_range");

		return Ports[port_index];
	}

}
