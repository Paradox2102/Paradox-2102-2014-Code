#include "manipulator.h"

#define ARM_LOW 696
#define ARM_SHOOT 667
#define ARM_HIGH 494

#define MAX_WINCH 2.48
#define MIN_WINCH .53
#define WINCH_TOLERANCE .05

manipulator::manipulator(Joystick *j, DriverStationLCD *l)
{
	maxwinch = MAX_WINCH;
	
	ds = l;
	
	winchstate = ready;
	
	intakestate = false;
	isshooting = false;
	joy = j;

	timer = new Timer();
	
	winch = new Talon(8);
	winchpot = new AnalogChannel(2);
	//winchset = new PIDController(.5,.05,.1,winchpot,winch);
	winchset = new PIDController(.5,.001,.01,winchpot,winch);
	
	ballstop = new DigitalInput(1);
	shooterstop = new DigitalInput(8);
	winderswitch = new DigitalInput(3);
	intakeone = new Relay(1);
	intaketwo = new Relay(2);
	
	armstop = new DigitalInput(7);
	
	pat = new AnalogChannel(3);
	armmotor = new Talon(7);
	//armset = new PIDController(.5,.05,.1,pat,armmotor);
	armset = new PIDController(.01,.001,.1,pat,armmotor);
	
	trigger = new DoubleSolenoid(3,4);
	ratchet = new DoubleSolenoid(7,8);
	//armlock = new DoubleSolenoid(5,6);

}

void manipulator::initialize()
{
	//timer->Start();
	winchstate = idle;
	//armset->Enable();
	armset->SetInputRange(5,0);
	armstate = shootlocked;
	//armset->SetOutputRange(1,-1);
	//winchset->SetSetpoint(MAX_WINCH);
	//winch->Set(1);
}

void manipulator::pickup()
{	
	
	if(joy->GetRawButton(9))
	{
		intakeone->Set(intakeone->kForward);
		intaketwo->Set(intakeone->kReverse);
	}
	else
	{
		intakeone->Set(intakeone->kOff);
		intaketwo->Set(intakeone->kOff);
	}
	ds->PrintfLine(ds->kUser_Line3, "pid value: %lf", pat->PIDGet());
	ds->PrintfLine(ds->kUser_Line4, "ballstop: %d", ballstop->Get());
	ds->PrintfLine(ds->kUser_Line5,"talon power", armmotor->Get());
	
	switch(armstate)
	{
	case shootlocked:
		if(joy->GetRawButton(7))
		{
			armlock->Set(armlock->kReverse);
			intakeone->Set(intakeone->kForward);
			intaketwo->Set(intaketwo->kReverse);
			armstate = dropped;
		}
		break;
	case dropped:
		if(!joy->GetRawButton(7) || !ballstop->Get())
		{
			armset->SetPID(1.5/300,0,0);
			armset->SetSetpoint(300);
			armset->Enable();
			
			armstate = rising;
		}
		break;
	case rising:
		if(fabs(armset->GetError()) < 5 )
		{
			armset->Reset();
			
			armlock->Set(armlock->kForward);
			
			armstate = shootlocked;
			
		}
		break;
	}
	
#ifdef notdef
	if(!armstate && timer->Get() > .1)
	{
		armmotor->Set(0);
		armstate = true;
	}
	
	ds->PrintfLine(ds->kUser_Line1,"arm angle: %lf", armset->GetError());
	ds->PrintfLine(ds->kUser_Line2,"arm voltage: %lf", pat->GetVoltage());
	if(joy->GetRawButton(11) && ballstop->Get() )
	{
		
		armset->Reset();
		armlock->Set(armlock->kReverse);
		
		if(armstate)
		{	
			armmotor->Set(-.3);
			timer->Reset();
			timer->Start();
			armstate = false;
		}
		intakeone->Set(Relay::kForward);
		intaketwo->Set(Relay::kReverse);
			
	}
	else if(joy->GetRawButton(7))
	{
		//armset->SetPID(.5/500,.0001,1/500, .2/(potdrift-ARM_SHOOT));
		
		armset->SetPID(.5/500,.0001,1/500, .2/500);
		armset->SetSetpoint(300);
		armset->Enable();
		if(fabs(armset->GetError()) < 5)
		{
			armlock->Set(armlock->kForward);
			armset->Reset();
		}
	}
	else
	{
		/*if(averagethree.getaverage(armstop))
		{
			potdrift = armset->GetError();
		}*/
		
		if(uparmstate)
		{
			armlock->Set(armlock->kReverse);
			armset->SetPID(1.5/500,0,0);
			//armset->SetSetpoint(500 + potdrift);
			armset->SetSetpoint(486);
			armset->Enable();
		}
		if(fabs(armset->GetError() < 5 )
		{
			
		}
		
		intakeone->Set(Relay::kOff);
		intaketwo->Set(Relay::kOff);
	}
#endif
}

void manipulator::shoot()
{
	//ds->PrintfLine(ds->kUser_Line1,"winderswitch: %d", windervalue);
	ds->PrintfLine(ds->kUser_Line1,"shooterstop: %d", shooterstop->Get());
	ds->PrintfLine(ds->kUser_Line2,"winderswitch: %d", winderswitch->Get());
	switch (winchstate)
	{
		case idle:
			//if(!average.getaverage(winderswitch))
			//{
				ratchet->Set(ratchet->kReverse);
				winch->Set(1);
				winchstate = winding;
			//}
		
		case winding:
			if(average.getaverage(winderswitch))
			{
				//ds->PrintfLine(ds->kUser_Line1,"winderswitch: %d", windervalue);
				
				ratchet->Set(DoubleSolenoid::kForward);
				winch->Set(0);
				//winchset->Disable();
				winchstate = ready;
			}
			break;
		case unwinding:
			if(averagetwo.getaverage(shooterstop))
			{
				trigger->Set(DoubleSolenoid::kForward);
				
				winch->Set(1);
				winchstate = winding;
			}
			break;
		case ready:
			if(joy->GetTrigger())
			{
				trigger->Set(DoubleSolenoid::kReverse);
				ratchet->Set(DoubleSolenoid::kReverse);
				winch->Set(1);
				timer->Reset();
				timer->Start();
				//winchset->SetSetpoint(MIN_WINCH);
				winchstate = unratchet;
			}
			break;
		case unratchet:
			if(timer->Get()> .1)
			{
				winch->Set(-1);
				winchstate = unwinding;
			}
			break;
		
	}
#ifdef notdef
	if(joy->GetRawButton(4))
		isshooting = true;
	
	
	if(isshooting)
	{	
		armset->SetSetpoint(ARM_SHOOT);
	
		
		if(joy->GetRawButton(5)) 
	
	
	
		if(joy->GetTrigger() && winderswitch->Get())
		{
			trigger->Set(DoubleSolenoid::kReverse);
			ratchet->Set(DoubleSolenoid::kReverse);
			winch->Set(Relay::kReverse);
			isshooting = false;
		}
	}
#endif
	
}

