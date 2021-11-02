#include <iostream>
#include "Board.h"
using namespace Real;


Board* GBoard = Board::GetInstance();

/**
 * Function that set the initial state of the board.
 */
void Init()
{
	PortA::RegistersT& regs = *PortA::RegistersT::GetInstance();

	regs.SetDDRScheme(0b01011000u); // 2, 4, 5 pins are OUTPUT, others are INPUT
	regs.SetPORTScheme(0b00010000); // 4 pin is HIGH, others are LOW

	regs.SetAlternateFunctions(1, static_cast<uint16>(Internal::EAlternateFunction::PWM));
	regs.SetAlternateFunctions(4, static_cast<uint16>(Internal::EAlternateFunction::PWM));
	regs.SetAlternateFunctions(5, static_cast<uint16>(Internal::EAlternateFunction::PWM));
	regs.SetAlternateFunctions(1, static_cast<uint16>(Internal::EAlternateFunction::MISO));

}


int main(int argc, char* argv[])
{
	Init();

	/// PortA has index 0 in the array.
	/// Alternatively, we could use GBoard->GetPort<PortA>().
	/// In this case, we just know that PortA instance is located at 0's position in the board array.
	IPort* portA = GBoard->GetPort(0); 
	
	// Can set any functions manually.
	//portA->SetAlternateFunctions(1, Internal::EAlternateFunction::MISO | Internal::EAlternateFunction::PWM);

	/// check if pin # 1 supports PWM and MISO (SPI) in port A
	bool supported = portA->SupportFunctions(1, EAlternateFunction::PWM | EAlternateFunction::MISO);

	if (supported) LOG("PWM and MISO are supported on pin 1 Port A");
	else LOG("PWM and MISO are NOT supported on pin 1 Port A");


	return 0;
}
