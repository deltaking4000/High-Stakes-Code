/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LeftMotorFront       motor         1               
// LeftMotorBackBottom  motor         11              
// LeftMotorBackTop     motor         13              
// RightMotorFront      motor         10              
// RightMotorBackBottom motor         20              
// RightMotorBackTop    motor         18              
// Drivetrain           drivetrain    2, 3, 4, 5      
// Controller1          controller                    
// Intake               motor_group   6, 7            
// Clamp                digital_out   H
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/


 void autonomous(void) {
    // Drives to the ladder
  Drivetrain.setDriveVelocity(100, percent);
  Drivetrain.driveFor(46, inches);
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void buttonUpPressed() {
    Intake.spin(forward);
    Brain.Screen.print("intake started...");
    Brain.Screen.newLine();
}

void buttonDownPressed() {
    Intake.stop();
    Brain.Screen.print("intake stopped...");
    Brain.Screen.newLine();
}

void buttonL1Pressed() {
  Clamp.set(false);
}

void buttonL2Pressed() {
  Clamp.set(true);
}


void usercontrol(void) {
  // User control code here, inside the loop
      // Sets velocity for the intake and the drivetrain
    Intake.setVelocity(100, percent);
    Drivetrain.setDriveVelocity(100, percent);
    Controller1.ButtonUp.pressed(buttonUpPressed);
    Controller1.ButtonDown.pressed(buttonDownPressed);
    Controller1.ButtonL1.pressed(buttonL1Pressed);
    Controller1.ButtonL2.pressed(buttonL2Pressed);
}

// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
