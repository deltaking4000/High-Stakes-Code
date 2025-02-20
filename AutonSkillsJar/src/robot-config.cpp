#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen.
brain  Brain;

//The motor constructor takes motors as (port, ratio, reversed), so for example
//motor LeftFront = motor(PORT1, ratio6_1, false);

//Add your devices below, and don't forget to do the same in robot-config.h:
//Left motors
motor LeftMotorFront = motor(PORT1, ratio6_1, true);
motor LeftMotorBackBottom = motor(PORT11, ratio6_1, true);
motor LeftMotorBackTop = motor(PORT13, ratio6_1, false);
//Right Motors
motor RightMotorFront = motor(PORT10, ratio6_1, false);
motor RightMotorBackBottom = motor(PORT16, ratio6_1, false);
motor RightMotorBackTop = motor(PORT18, ratio6_1, true);

//Motor groups for drivetrain
motor_group LeftDriveSmart = motor_group(LeftMotorFront, LeftMotorBackBottom, LeftMotorBackTop);
motor_group RightDriveSmart = motor_group(RightMotorFront, RightMotorBackBottom, RightMotorBackTop);

//Inertial sensor
inertial Inertial = inertial(PORT9);

// Drivetrain https://api.vex.com/v5/home/cpp/Drivetrain.html
double wheelTravel = 260;   // 3.25" wheel is 260mm wheeltravel
double trackWidth = 305;    // left wheel to right wheel
double wheelBase = 228.6;     // backmost wheel to frontmost wheel
double externalGearRatio = 36/24; // output teeth over input teeth
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, Inertial, wheelTravel, trackWidth, wheelBase, mm, externalGearRatio);

// Controller
controller Controller1 = controller(primary);

//Intake programming
motor IntakeMotorA = motor(PORT6, ratio18_1, false);
motor IntakeMotorB = motor(PORT7, ratio18_1, false);
motor_group Intake = motor_group(IntakeMotorA, IntakeMotorB);

// Pneumatics
digital_out Clamp = digital_out(Brain.ThreeWirePort.H);


void vexcodeInit( void ) {
  // nothing to initialize
}