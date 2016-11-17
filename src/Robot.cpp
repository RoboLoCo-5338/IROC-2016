#include "WPILib.h"
#include "CANTalon.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	CanTalonSRX *DriveL1 = new CanTalonSRX(1);
	CanTalonSRX *DriveL2 = new CanTalonSRX(2);
	CanTalonSRX *DriveR1 = new CanTalonSRX(3);
	CanTalonSRX *DriveR2 = new CanTalonSRX(4);

	CanTalonSRX *ShootL = new CanTalonSRX(5);
	CanTalonSRX *ShootR = new CanTalonSRX(6);
	CanTalonSRX *Intake = new CanTalonSRX(7);



	Joystick *xbox = new Joystick(0);

	double speed;
	double shooter;
	bool button;
	double shootspeed;
	bool reverse;
	double autoCounter;


	void RobotInit()
	{
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);
	}


	/**
	 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
	 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
	 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
	 * below the Gyro
	 *
	 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
	 * If using the SendableChooser make sure to add them to the chooser code above as well.
	 */
	void AutonomousInit()
	{
		autoSelected = *((std::string*)chooser->GetSelected());
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
		autoCounter = 0;
		reverse = false;
		speed = 1;
	}

	void AutonomousPeriodic()
	{
		if(autoSelected == autoNameCustom){
			autoCounter++;
			if (autoCounter<25)
				Drive(autoCounter/-33,autoCounter/-33);
			else if (autoCounter<100)
				Drive(-.75,-.75);
			else
				Drive(0,0);
		} else {
			//Default Auto goes here
		}
	}

	void TeleopInit()
	{
		button = false;
		reverse = false;
		shootspeed = 0.5;
		speed = 0.5;
		shooter = 0;
	}

	void TeleopPeriodic()
	{


		//LB - full speed
		//RB - half speed

		if (xbox->GetRawButton(5)) speed = 1;
		if (xbox->GetRawButton(6)) speed = 0.5;
		Drive(xbox->GetRawAxis(1),xbox->GetRawAxis(5));

		//BACK - reverse
		//START - forwards

		if (xbox->GetRawButton(7)) reverse = true;
		if (xbox->GetRawButton(7) == true) {
			xbox->SetRumble(xbox->kLeftRumble,1);
		}
		else {
			xbox->SetRumble(xbox->kLeftRumble,0);

		}
		if (xbox->GetRawButton(8)) reverse = false;

		//LT - intake
		//RT - reverse
		//LJ - 30% speed

		if (xbox->GetRawButton(9))
		{Intake->Set(0.3);} else
		{Intake->Set(0.6*(xbox->GetRawAxis(2)-xbox->GetRawAxis(3)));}

		//A - start shooter
		//Y - stop shooter
		//X - reduce speed by 5%
		//B - increase speed by 5%

		if (xbox->GetRawButton(1)) shooter = shootspeed;
		if (xbox->GetRawButton(4)) shooter = 0;
		ShootL->Set(shooter);
		ShootR->Set(shooter);
		if (button){
			button = xbox->GetRawButton(2)||xbox->GetRawButton(3);
		} else {
			if (xbox->GetRawButton(2)) {
				shootspeed+=0.05;
				button = true;
			}
			if (xbox->GetRawButton(3)) {
				shootspeed-=0.05;
				button = true;
			}
		}


	}

	void TestPeriodic()
	{
		lw->Run();
	}

	void Drive(double driveL, double driveR)
	{
		if (reverse)
		{
			DriveL1->Set(driveR*speed*-1);
			DriveL2->Set(driveR*speed*-1);
			DriveR1->Set(driveL*speed);
			DriveR2->Set(driveL*speed);
		} else {
			DriveL1->Set(driveL*speed*-1);
			DriveL2->Set(driveL*speed*-1);
			DriveR1->Set(driveR*speed);
			DriveR2->Set(driveR*speed);
		}

	}
};

START_ROBOT_CLASS(Robot)
