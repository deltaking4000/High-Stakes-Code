#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen.
brain  Brain;

//Add your devices below, and don't forget to do the same in robot-config.h:
//Left motors
motor LeftMotorFront = motor(PORT1, ratio6_1, true);
motor LeftMotorBackBottom = motor(PORT11, ratio6_1, true);
motor LeftMotorBackTop = motor(PORT13, ratio6_1, false);
//Right Motors
motor RightMotorFront = motor(PORT10, ratio6_1, false);
motor RightMotorBackBottom = motor(PORT16, ratio6_1, false);
motor RightMotorBackTop = motor(PORT18, ratio6_1, true);

// Controller
controller Controller1 = controller(primary);

//Intake programming
motor IntakeMotorA = motor(PORT6, ratio18_1, false);
motor IntakeMotorB = motor(PORT7, ratio18_1, false);
motor_group Intake = motor_group(IntakeMotorA, IntakeMotorB);

// Pneumatics
digital_out Clamp = digital_out(Brain.ThreeWirePort.H);

// Optical Sensor
optical Optical = optical(PORT3);

void vexcodeInit( void ) {
  // nothing to initialize
}