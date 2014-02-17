#include "WPILib.h"


class drive
{
public:

	drive(Joystick *j);
	
	void go();
	void shift();
	void forward();
	bool prime, shifted;
	
protected:
	Talon *starboardtalon, *porttalon;
	Joystick *joy;
	Solenoid *starsol, *portsol;
	Encoder *portencoder, *starboardencoder;
	DigitalInput *di;
};
