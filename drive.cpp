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
	if(prime && joy->GetTrigger())
	{
		starsol->Set(shifted);
		portsol->Set(!shifted);
		
		shifted = !shifted;
	}
		
	if(!joy->GetTrigger())
		prime = true;
	else
		prime = false;
	
}

void drive::go()
{
	if(fabs(joy->GetY()) > 0.15 || fabs(joy->GetZ()) > 0.15)
	{
		starboardtalon->Set(ldexp(joy->GetY(),5/3) - 0.7 * ldexp(joy->GetZ(),5/3));
		porttalon->Set(-(ldexp(joy->GetY(),5/3) + 0.7 * ldexp(joy->GetZ(),5/3)));
	}
	else
	{
		starboardtalon->Set(0);
		porttalon->Set(0);
	}
	drive::shift();
}


