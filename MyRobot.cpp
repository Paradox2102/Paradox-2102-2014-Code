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
	Talon *porttalon, *starboardtalon;
	Joystick *stick; // only joystick
	Compressor *comp;
	DriverStationLCD *ds;
	
	drive *d;
	manipulator *arm;
	
public:
	RobotDemo()
	{
		ds = DriverStationLCD::GetInstance();
		porttalon = new Talon(1);
		starboardtalon = new Talon(2);
		stick = new Joystick(1);
		comp = new Compressor(1,8);
		
		d = new drive(stick, porttalon, starboardtalon);
		arm = new manipulator(stick);
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
			
			d->go();
			
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

