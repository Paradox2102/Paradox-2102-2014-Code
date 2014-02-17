#include "WPILib.h"
#include "math.h"

class digitalaverage
{
public:
	digitalaverage()
	{
		counter = 0;
	}
	
	bool getaverage(DigitalInput *dig)
	{
		if(dig->Get())
		{
			counter++;
		}
		else
		{
			counter = 0;
		}
		
		if(counter > 5)
		{
			counter = 6;
			
			return true;
		}
		else
		{
			
			return false;
		}
		
	}
protected:
	int counter;
};




class manipulator
{
public:
	manipulator(Joystick *j, DriverStationLCD *l);
	
	void initialize();
	
	void shoot();
	
	void pickup();
	
	digitalaverage average, averagetwo, averagethree;
	
private:
	
	enum winchstate
	{
		winding,
		unwinding,
		ready,
		idle,
		unratchet
		
	};
	enum armstate
	{
		shootlocked,
		dropped,
		rising
	};
	
	
	winchstate winchstate;
	armstate armstate;
	
	AnalogChannel *pat, *winchpot;
	DigitalInput *shooterstop,*ballstop, *winderswitch, *unwinderswitch, *armstop;
	Relay  *intakeone, *intaketwo;
	DoubleSolenoid *trigger, *ratchet, *armlock;
	Talon *armmotor, *winch;
	PIDController *armset, *winchset;
	Joystick *joy;
	Timer *timer;
	DriverStationLCD *ds;

	float maxwinch;
	
	float potdrift;
	
	bool intakestate;
	
	bool pickupstart,isshooting;
	

	
};
