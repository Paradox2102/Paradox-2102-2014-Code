#include "WPILib.h"


class drive
{
public:

	drive(Joystick *j, Talon *p, Talon *s);
	
	void go();
	void shift();
	void forward();
	bool prime, shifted;
	
protected:
	Talon *starboardtalon, *porttalon;
	Joystick *joy;
	Solenoid *starsol, *portsol;
	Encoder *portencoder, *starboardencoder;
};
