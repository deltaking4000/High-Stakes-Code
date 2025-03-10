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
  Brain.Screen.print("running preauton");
  Brain.Screen.newLine();
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  // Start calibration with the calibration time set to 3 seconds.
  Inertial.calibrate(3);
}
void LiftClamp(){
  Clamp.set(true);
   Brain.Screen.print("The clamp has been lifted");
}

void DropClamp(){
  Clamp.set(false);
   Brain.Screen.print("The clamp has been placed down");
}

void TurntoHeadingCorrection(double angle){
  Drivetrain.turnToHeading(angle, degrees);
  wait(0.5, seconds);
  Drivetrain.turnToHeading(angle, degrees);

}
//////////////////////////

void drivePID(double distanceInTurns, double velocity){
  double kP = 0.8;

  // Resetting positions for the motors
  LeftDriveSmart.resetPosition();
  RightDriveSmart.resetPosition();
  //double originalHeading = Inertial.heading();
  Inertial.resetRotation();
  
  while(RightDriveSmart.position(turns) < distanceInTurns ) {
  
    double headingError = kP * Inertial.rotation();

    LeftDriveSmart.setVelocity(velocity, percent);
    RightDriveSmart.setVelocity(velocity + headingError, percent);

    LeftDriveSmart.spin(forward);
    RightDriveSmart.spin(forward);
    Brain.Screen.clearScreen();
    Brain.Screen.print(headingError);

    //Brain.Screen.newLine(); 
    wait(20, msec);    

  }
  LeftDriveSmart.stop();
  RightDriveSmart.stop();

}

/////////////////////////
 void auton17PTS(void) {

  //START

  // 1. Drive towards the alliance wall stake
  Drivetrain.setDriveVelocity(10, percent);
  Drivetrain.setTurnVelocity(10, percent);
  Intake.setVelocity(100, percent);
 // Drivetrain.driveFor(reverse, 9, inches);
  
  // 2. Put the preloaded ring on the stake
  Intake.spin(reverse);
  wait(2, seconds);
  Intake.spin(forward);
  wait(1, seconds);
  Intake.stop();

  // 3. Turn to face mobile goal
  LiftClamp();
  Drivetrain.driveFor(forward, 15, inches);
  //Drivetrain.setTimeout(2, seconds);
  TurntoHeadingCorrection(270);

  // 4. Drive into mobile goal
  Drivetrain.driveFor(reverse, 19, inches);
  wait(0.5,seconds);
  DropClamp();
  wait(0.5, seconds);

  // 5. Turn to pick up rings
  TurntoHeadingCorrection(90);
  Intake.spin(reverse);
  Drivetrain.setTimeout(10, seconds);
  Drivetrain.driveFor(forward, 60, inches);
  wait(2, seconds);

  // 6. Turn to go into the positive corner
  Brain.Screen.print("The robot is turning to go into the positive corner");
  Drivetrain.turnToHeading(325, degrees);
  Drivetrain.setTimeout(5, seconds);
  Drivetrain.setDriveVelocity(100, percent);
  Drivetrain.driveFor(reverse, 50, inches);
  Intake.stop();
  LiftClamp();
  Drivetrain.driveFor(forward, 3, inches);

  // 7. Go to wall as reference point
  Drivetrain.setDriveVelocity(80, percent);
  Drivetrain.turnToHeading(315, degrees);
  Drivetrain.driveFor(forward, 40, inches);
  Drivetrain.turnToHeading(0, degrees);
  Drivetrain.driveFor(reverse, 35, inches);
  Drivetrain.setDriveVelocity(10, percent);
  Drivetrain.driveFor(reverse, 15, inches);

  // 8. Go to corner 2 stake
  Drivetrain.setTimeout(10, seconds);
  Drivetrain.driveFor(forward, 18, inches);
  TurntoHeadingCorrection(90);
  Drivetrain.driveFor(reverse, 30, inches);
  TurntoHeadingCorrection(90);
  Drivetrain.driveFor(reverse, 33, inches);
  wait(0.5, seconds);
  DropClamp();
  wait(0.5, seconds);

  // 9. Put mobile goal in corner
  Drivetrain.turnToHeading(55, degrees);
  Drivetrain.setDriveVelocity(100, percent);
  Drivetrain.setTimeout(2, seconds);
  Drivetrain.driveFor(reverse, 100, inches);
  LiftClamp();
  Drivetrain.driveFor(forward, 3, inches);

  //END
}

void auton_nointake(){

  /* INSTRUCTIONS: This code is for when the judge
  forces our intake to be ziptied. The placement 
  is different, as the robot must be at a 45 degree 
  angle with the clamp parallel to the side of the
  left side stake. */

  //START

  // 0. Set velocities
  Brain.Screen.print("auton no intake running");
  Brain.Screen.newLine();
  Drivetrain.setHeading(145, degrees);
  Drivetrain.setDriveVelocity(10, percent);
  Drivetrain.setTurnVelocity(10, percent);
  Intake.setVelocity(100, percent);

  // 1. Drive into mobile goal
  LiftClamp();
  wait(0.5, seconds);
  Drivetrain.driveFor(reverse, 3, inches);
  DropClamp();
  wait(0.5, seconds);

  // 2. Put the preloaded ring on the stake
  Intake.spin(reverse);
  wait(2, seconds);
  Intake.stop();
  
  // 3. Put mobile goal in corner
  Drivetrain.turnToHeading(55, degrees);
  Drivetrain.setDriveVelocity(100, percent);
  Drivetrain.setTimeout(2, seconds);
  Drivetrain.driveFor(reverse, 100, inches);
  LiftClamp();
  Drivetrain.driveFor(forward, 3, inches);

  // 4. Go to wall to reset heading
  Drivetrain.turnToHeading(45, degrees);
  Drivetrain.setDriveVelocity(20, percent);
  Drivetrain.driveFor(forward, 24, inches);
  Drivetrain.turnToHeading(0, degrees);
  Drivetrain.setTimeout(3, seconds);
  Drivetrain.driveFor(reverse, 40, inches);
  wait(0.2, seconds);
  Inertial.setHeading(0, degrees);
  Drivetrain.setHeading(0, degrees);

  // 5. Go to corner 1 stake
  Drivetrain.setDriveVelocity(10, percent);
  Drivetrain.driveFor(forward, 21.5, inches);
  TurntoHeadingCorrection(90);
  drivePID(5.5, 20);
  TurntoHeadingCorrection(270);
  Drivetrain.setDriveVelocity(15, percent);
  Drivetrain.driveFor(reverse, 14, inches);
  wait(0.5, seconds);
  DropClamp();

  // 6. Put mobile goal in corner
  Drivetrain.turnToHeading(305, degrees);
  Drivetrain.setDriveVelocity(100, percent);
  Drivetrain.setTimeout(2, seconds);
  Drivetrain.driveFor(reverse, 100, inches);
  LiftClamp();
  Drivetrain.driveFor(forward, 3, inches);

  // 7. Go to wall as reference point
  Drivetrain.driveFor(forward, 16, inches);
  TurntoHeadingCorrection(0);
  Drivetrain.setDriveVelocity(10, percent);
  Drivetrain.setTimeout(4, seconds);
  Drivetrain.driveFor(reverse, 14, inches);
  wait(0.5, seconds);

  // 8. Go to the other side and slam the corner 3 stake in
  drivePID(10, 30);
  Drivetrain.turnToHeading(315, degrees);
  drivePID(5.5, 30);
  TurntoHeadingCorrection(250);
  Drivetrain.setTimeout(3, seconds);
  Drivetrain.driveFor(reverse, 100, inches);
  wait(0.5, seconds);
  Drivetrain.driveFor(forward, 4, inches);
  Drivetrain.turnToHeading(225, degrees);

  // 9. Get corner 4 stake
  Brain.Screen.print("SIGMA SIGMA SIGMA SIGMA ALERT");
  drivePID(2.2, 20);
  TurntoHeadingCorrection(275);
  drivePID(13, 40);
  Drivetrain.driveFor(reverse, 5, inches);
  // sigmaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaarizzlerrrrrrrrrrrrrrrrrrrrr

  // 23 points getting all four corners and 3 pts for the preload on one of the stakes

  //END

}

void autonStates(){ // NOT COMPLETE

  /* INSTRUCTIONS: This code is for when the judge
  forces our intake to be ziptied. The placement 
  is different, as the robot must be at a 45 degree 
  angle with the clamp parallel to the side of the
  left side stake. */

  //START

  // 0. Set velocities
  Brain.Screen.print("auton no intake running");
  Brain.Screen.newLine();
  Drivetrain.setHeading(145, degrees);
  Drivetrain.setDriveVelocity(10, percent);
  Drivetrain.setTurnVelocity(10, percent);
  Intake.setVelocity(100, percent);

  // 1. Drive into mobile goal
  LiftClamp();
  wait(0.5, seconds);
  Drivetrain.driveFor(reverse, 3, inches);
  DropClamp();
  wait(0.5, seconds);

  // 2. Put the preloaded ring on the stake
  Intake.spin(reverse);
  wait(2, seconds);
  //Intake.stop();
  
  // 3. Pick up first five rings and max out the stake
  Drivetrain.turnToHeading(352, degrees);
  //Intake.spin(reverse);
  // first ring
  drivePID(3.5, 35);
  TurntoHeadingCorrection(270);
  // second ring
  drivePID(3, 35);
  TurntoHeadingCorrection(180);
  // third rings
  drivePID(4.25, 20);// pick the ring not on the line as bonus after code finished
  Drivetrain.setDriveVelocity(15, percent);
  Drivetrain.setTimeout(1, seconds);
  Drivetrain.drive(forward);
  Drivetrain.driveFor(reverse, 22, inches);
  TurntoHeadingCorrection(270);
  drivePID(1.5, 20);
  Drivetrain.driveFor(reverse, 10, inches);
  
/*
  // 4. Use wall as reference point
  Drivetrain.setTimeout(1500, msec);
  Drivetrain.setDriveVelocity(20, percent);
  Drivetrain.driveFor(forward, 10, inches);
  wait(0.2, seconds);
  Drivetrain.setHeading(180, degrees);
  Drivetrain.driveFor(reverse, 5, inches);
  */

  // 3. Put mobile goal in corner
  Drivetrain.turnToHeading(55, degrees);
  Drivetrain.setDriveVelocity(100, percent);
  Drivetrain.setTimeout(1, seconds);
  Drivetrain.driveFor(reverse, 50, inches);
  LiftClamp();
  Drivetrain.driveFor(forward, 3, inches);
  Intake.stop();

  // 4. Go to wall to reset heading
  Drivetrain.turnToHeading(45, degrees);
  Drivetrain.setDriveVelocity(20, percent);
  Drivetrain.driveFor(forward, 24, inches);
  Drivetrain.turnToHeading(0, degrees);
  Drivetrain.setTimeout(3, seconds);
  Drivetrain.driveFor(reverse, 40, inches);
  wait(0.2, seconds);
  Inertial.setHeading(0, degrees);
  Drivetrain.setHeading(0, degrees);

  // 5. Go to corner 1 stake
  Drivetrain.setDriveVelocity(10, percent);
  Drivetrain.driveFor(forward, 21.5, inches);
  TurntoHeadingCorrection(90);
  drivePID(5.5, 20);
  TurntoHeadingCorrection(270);
  Drivetrain.setDriveVelocity(15, percent);
  Drivetrain.driveFor(reverse, 14, inches);
  wait(0.5, seconds);
  DropClamp();
/*
  // 5. Turn to pick up rings
  TurntoHeadingCorrection(90);
  Intake.spin(reverse);
  Drivetrain.setTimeout(10, seconds);
  Drivetrain.driveFor(forward, 60, inches);
  wait(2, seconds);

  // 6. Turn to go into the positive corner
  Brain.Screen.print("The robot is turning to go into the positive corner");
  Drivetrain.turnToHeading(325, degrees);
  Drivetrain.setTimeout(5, seconds);
  Drivetrain.setDriveVelocity(100, percent);
  Drivetrain.driveFor(reverse, 50, inches);
  Intake.stop();
  LiftClamp();
  Drivetrain.driveFor(forward, 3, inches);

  // 7. Go to wall as reference point
  Drivetrain.driveFor(forward, 16, inches);
  TurntoHeadingCorrection(0);
  Drivetrain.setDriveVelocity(10, percent);
  Drivetrain.setTimeout(4, seconds);
  Drivetrain.driveFor(reverse, 14, inches);
  wait(0.5, seconds);

  // 8. Go to the other side and slam the corner 3 stake in
  drivePID(10, 30);
  Drivetrain.turnToHeading(315, degrees);
  drivePID(5.5, 30);
  TurntoHeadingCorrection(250);
  Drivetrain.setTimeout(3, seconds);
  Drivetrain.driveFor(reverse, 100, inches);
  wait(0.5, seconds);
  Drivetrain.driveFor(forward, 4, inches);
  Drivetrain.turnToHeading(225, degrees);

  // 9. Get corner 4 stake
  Brain.Screen.print("SIGMA SIGMA SIGMA SIGMA ALERT");
  drivePID(2.2, 20);
  TurntoHeadingCorrection(275);
  drivePID(15, 40); // 13 inches originally
  Drivetrain.driveFor(reverse, 5, inches);
  // sigmaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaarizzlerrrrrrrrrrrrrrrrrrrrr

  // 23 points getting all four corners and 3 pts for the preload on one of the stakes

  //END*/

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
  //auton17PTS();
  auton_nointake();
  //autonStates();
  //drivePID(15, 30); // 10 turns = 88 inches, 7 is 60 inches
  //Drivetrain.setDriveVelocity(20, percent);
  //Drivetrain.driveFor(forward, 10, inches);
}
//

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
