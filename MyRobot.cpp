#include "WPILib.h"
#include "drive.h"
#include "manipulator.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	Joystick *stick; // only joystick
	Joystick *stick2;
	Compressor *comp;
	DriverStationLCD *ds;
	
	DoubleSolenoid *armlock;
	
	DigitalInput *ball;
	
	drive *d;
	manipulator *arm;
	
	bool wasenabled;
	
public:
	RobotDemo()
	{
		ds = DriverStationLCD::GetInstance();
		stick = new Joystick(1);
		comp = new Compressor(14,4);
		ball = new DigitalInput(1);
	
		wasenabled = false;
		
		//r = new Relay(4);
		d = new drive(stick);
		arm = new manipulator(stick, ds);
		
		armlock = new DoubleSolenoid(5,6);

		
	}
	

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous()
	{
		
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl()
	{
		comp->Start();
		
	
		while(IsOperatorControl())
		{
			
			if(wasenabled == false && IsEnabled() == true)
			{
				
				//arm->initialize();
				
			
				wasenabled = true;
				
				//Wait(.5);
					
			}
			
			wasenabled = IsEnabled();
			d->go();
			
			//r->Set((r->kForward));
			

			
			//ds->PrintfLine(ds->kUser_Line1, "button 11: %d " ,stick->GetRawButton(11));
			//ds->PrintfLine(ds->kUser_Line2, "button 2: %d", stick->GetRawButton(2));
					
			//arm->pickup();
			
			if(stick->GetRawButton(7))
			{
				armlock->Set(armlock->kForward);
			}
			else
			{
				armlock->Set(armlock->kReverse);
			}
			ds->PrintfLine(ds->kUser_Line2, "button 7: %d", stick->GetRawButton(7));
			
			//arm->shoot();
			
		ds->UpdateLCD();
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

