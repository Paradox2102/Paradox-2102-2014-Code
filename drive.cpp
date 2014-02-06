#include "drive.h"
#include "math.h"


drive::drive(Joystick *j, Talon *p, Talon *s)
{
	joy = j;
	starboardtalon = s;
	porttalon = p;
	
	starsol = new Solenoid(1);
	portsol = new Solenoid(2);
	
	starboardencoder = new Encoder(1,2,true);	
	portencoder = new Encoder(3,4,true);
	
	prime = false;
	shifted = true;
}

void drive::shift()
{
	if(prime && joy->GetRawButton(2))
	{
		starsol->Set(shifted);
		portsol->Set(!shifted);
		
		shifted = !shifted;
	}
		
	if(!joy->GetRawButton(2))
		prime = true;
	else
		prime = false;
}

void drive::go()
{
	if(fabs(joy->GetY()) > 0.15 || fabs(joy->GetZ()) > 0.15)
	{
		starboardtalon->Set(joy->GetY() - 0.7 * joy->GetX());
		porttalon->Set(-(joy->GetY() + 0.7 * joy->GetX()));
	}
	else
	{
		starboardtalon->Set(0);
		porttalon->Set(0);
	}
	drive::shift();
}


