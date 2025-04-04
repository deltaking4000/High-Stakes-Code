#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors

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

// Drivetrain https://api.vex.com/v5/home/cpp/Drivetrain.html
double wheelTravel = 260;   // 3.25" wheel is 260mm wheeltravel
double trackWidth = 305;    // left wheel to right wheel
double wheelBase = 228.6;     // backmost wheel to frontmost wheel
double externalGearRatio = 60/48; // output teeth over input teeth
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, wheelTravel, trackWidth, wheelBase, mm, externalGearRatio);

// Controller
controller Controller1 = controller(primary);

//Intake programming
motor IntakeMotorA = motor(PORT6, ratio18_1, false);
motor IntakeMotorB = motor(PORT7, ratio6_1, false);
motor_group Intake = motor_group(IntakeMotorA, IntakeMotorB);

// Ladybrown
motor Ladybrown = motor(PORT2, ratio18_1, false);

// PNEUMATICS //

// Clamp
digital_out Clamp = digital_out(Brain.ThreeWirePort.H);

// Doinker
digital_out Doinker = digital_out(Brain.ThreeWirePort.A);

// Optical Sensor
optical Optical = optical(PORT19);


// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3 + Axis1
      // right = Axis3 - Axis1
      int drivetrainLeftSideSpeed = Controller1.Axis3.position() + Controller1.Axis1.position();
      int drivetrainRightSideSpeed = Controller1.Axis3.position() - Controller1.Axis1.position();
      
      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller1) {
          // stop the left drive motor
          LeftDriveSmart.stop();
          // tell the code that the left motor has been stopped
          DrivetrainLNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5) {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller1) {
          // stop the right drive motor
          RightDriveSmart.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller1) {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller1) {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);
}