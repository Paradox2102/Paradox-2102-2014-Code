#include "manipulator.h"

manipulator::manipulator()
{
	pat = new AnalogChannel(1);
}

float manipulator::getarmangle()
{
	return pat->GetVoltage();
}
