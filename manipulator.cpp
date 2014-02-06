#include "manipulator.h"

#define ARM_LOW 1
#define ARM_SHOOT 3
#define ARM_HIGH 5

manipulator::manipulator(Joystick *j)
{
	pickupstart = false;
	isshooting = false;
	j =joy;
	pat = new AnalogChannel(1);
	backstop = new DigitalInput(2);
	winderswitch = new DigitalInput(3);
	unwinderswitch = new DigitalInput(4);
	winch = new Relay(2);
	intakeone = new Relay(3);
	intaketwo = new Relay(4);
	armmotor = new Victor(3);
	armset = new PIDController(.5,.05,.1,pat,armmotor);
	triggerone = new Solenoid(3);
	triggertwo = new Solenoid(4);
}

void manipulator::pickup()
{
	if(joy->GetRawButton(7))
		pickupstart = true;
	else if(joy->GetRawButton(8))
		pickupstart =false;
	
	if(pickupstart)
	{
		armset->SetSetpoint(ARM_LOW);
		intakeone->Set(Relay::kForward);
		intaketwo->Set(Relay::kForward);
	}
	else if(!pickupstart)
	{
		armset->SetSetpoint(ARM_HIGH);
		intakeone->Set(Relay::kOff);
		intaketwo->Set(Relay::kOff);
	}
	
}

void manipulator::shoot()
{
	if(joy->GetRawButton(4))
		isshooting = true;
	
	
	if(isshooting)
	{
		manipulator::manualwind();
	
		armset->SetSetpoint(ARM_SHOOT);
	
		
		if(joy->GetRawButton(5)) 
	
	
	
		if(joy->GetTrigger() && winderswitch->Get())
		{
			triggerone->Set(true);
			triggertwo->Set(false);
			winch->Set(Relay::kReverse);
			isshooting = false;
		}
	}
	
	if(unwinderswitch->Get())
	{
		winch->Set(Relay::kOff);
		triggerone->Set(false);
		triggertwo->Set(true);
	}
}

void manipulator::manualwind()
{
	if(backstop->Get() && !winderswitch->Get())
	{
		winch->Set(Relay::kForward);
	}
	else
	{
		winch->Set(Relay::kOff);
	}
}
