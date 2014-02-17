#include "drive.h"
#include "math.h"


drive::drive(Joystick *j)
{
	joy = j;
	starboardtalon = new Talon(9);
	porttalon = new Talon(10);
	
	starsol = new Solenoid(1);
	portsol = new Solenoid(2);
	
	starboardencoder = new Encoder(5,6,true);	
	portencoder = new Encoder(7,8,true);
	
	prime = true;
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
	//!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!
	if(fabs(joy->GetY()) > 0.15 || fabs(joy->GetX()) > 0.15)//DO NOT CHANGE TURN AXIS. Z AXIS IS INACCURATE!!!!!!!!!!!!
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


