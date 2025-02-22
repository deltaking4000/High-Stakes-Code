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
// RightMotorBackBottom motor         16              
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
void LiftClamp(){
  Clamp.set(true);
   Brain.Screen.print("The clamp has been lifted");
}

void DropClamp(){
  Clamp.set(false);
   Brain.Screen.print("The clamp has been placed down");
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
  Intake.setVelocity(100, percent);

  //START

  // 1. Drive towards the alliance wall stake
  Drivetrain.setDriveVelocity(20, percent);
  Drivetrain.setTurnVelocity(100, percent);
  Intake.setVelocity(100, percent);
  Drivetrain.driveFor(reverse, 9, inches);
  
  // 2. Put the preloaded ring on the stake
  Intake.spin(reverse);
  wait(2, seconds);
  Intake.stop();

  // 3. Turn to face ladder
  Drivetrain.driveFor(forward, 6, inches);
  Drivetrain.setTimeout(2, seconds);
  Drivetrain.turnFor(left, 60, degrees);

  // 4. Drive to ladder
  Drivetrain.driveFor(forward, 34, inches);

  //END
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


void buttonR1Pressed() {
    static bool bR1ButtonState = false;
    if( !bR1ButtonState ) {
      bR1ButtonState = true;
      Intake.spin(reverse);
      Brain.Screen.print("intake started...");
      Brain.Screen.newLine();
    }
    else {
      bR1ButtonState = false;
      Intake.stop();
  }
}

void buttonR2Pressed() {
  static bool bR2ButtonState = false;
  if( !bR2ButtonState ) {
    bR2ButtonState = true;
    Intake.spin(forward);
    Brain.Screen.print("intake reversed...");
    Brain.Screen.newLine();
  }
  else {
    bR2ButtonState = false;
    Intake.stop();
  }
}

void buttonL1Pressed() {
  Clamp.set(false);
  Brain.Screen.print("Clamp false...");
  Brain.Screen.newLine();
}

void buttonL2Pressed() {
  Clamp.set(true);
  Brain.Screen.print("Clamp true...");
  Brain.Screen.newLine();
}


void usercontrol(void) {
  // User control code here, inside the loop
      // Sets velocity for the intake and the drivetrain
    Intake.setVelocity(100, percent);
    Drivetrain.setDriveVelocity(100, percent);
}

// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  Controller1.ButtonR1.pressed(buttonR1Pressed);
  Controller1.ButtonR2.pressed(buttonR2Pressed);
  Controller1.ButtonL1.pressed(buttonL1Pressed);
  Controller1.ButtonL2.pressed(buttonL2Pressed);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
