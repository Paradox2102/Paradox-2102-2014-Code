#include "WPILib.h"

class manipulator
{
public:
	manipulator(Joystick *j);
	
	void manualwind();
	
	void shoot();
	
	void pickup();
	
private:
	AnalogChannel *pat;
	DigitalInput *backstop, *winderswitch, *unwinderswitch;
	Relay *winch, *intakeone, *intaketwo;
	Solenoid *triggerone, *triggertwo;
	Victor *armmotor;
	PIDController *armset;
	Joystick *joy;
	
	bool pickupstart,isshooting;
	
};
