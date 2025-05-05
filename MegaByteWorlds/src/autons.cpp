#include "vex.h"

/**
 * Resets the constants for auton movement.
 * Modify these to change the default behavior of functions like
 * drive_distance(). For explanations of the difference between
 * drive, heading, turning, and swinging, as well as the PID and
 * exit conditions, check the docs.
 */

void default_constants(){
  // Each constant set is in the form of (maxVoltage, kP, kI, kD, startI).
  float drive_max_voltage = 6; // 10
  float drive_kp = 1.5;
  float drive_ki = 0;
  float drive_kd = 10;
  chassis.set_drive_constants(drive_max_voltage, drive_kp, drive_ki, drive_kd, 0);
  float heading_max_voltage = 4; // 6
  chassis.set_heading_constants(heading_max_voltage, .4, 0, 1, 0);
  float turn_max_voltage = 8; // 12
  chassis.set_turn_constants(turn_max_voltage, .4, .03, 3, 15);
  chassis.set_swing_constants(turn_max_voltage, .3, .001, 2, 15);

  // Each exit condition set is in the form of (settle_error, settle_time, timeout).
  chassis.set_drive_exit_conditions(1.5, 300, 5000);
  chassis.set_turn_exit_conditions(1, 300, 3000);
  chassis.set_swing_exit_conditions(1, 300, 3000);
}

/**
 * Sets constants to be more effective for odom movements.
 * For functions like drive_to_point(), it's often better to have
 * a slower max_voltage and greater settle_error than you would otherwise.
 */

void odom_constants(){
  default_constants();
  chassis.heading_max_voltage = 6;//10
  chassis.drive_max_voltage = 4;//8
  chassis.drive_settle_error = 3;
  chassis.boomerang_lead = .5;
  chassis.drive_min_voltage = 0;
}

//////////////////////////
void LiftClamp(){
  Clamp.set(true);
  //Brain.Screen.print("The clamp has been lifted");
}

void DropClamp(){
  Clamp.set(false);
  //Brain.Screen.print("The clamp has been placed down");
}

//////////////////////////////////
// color sorting

bool colorSortingEnabled = false;
bool allianceIsRed = true;

void OpticalObjectDetected() {

  if (colorSortingEnabled == false){
    return;
  }

  // The Brain will print that the Optical Sensor detected
  // an object to the Brain's screen.
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.clearScreen();
  Brain.Screen.print("object detected");
  Brain.Screen.newLine();

  // Set a variable, detectColor, to the color detected by the
  // Optical Sensor.
  //color detectColor = Optical.color();
  double hue = Optical.hue();

  // Print the color detected by the Optical Sensor
  // to the Brain's screen.
  Brain.Screen.print(hue);
  Brain.Screen.newLine();

  //vex::color ringColor = allianceColor.rgb();
  bool tossRing = false;


  if (hue < 30 || hue > 300){
    Brain.Screen.print("The color is red");
    //ringColor = vex::color::red.rgb();
    if (!allianceIsRed) {
      tossRing = true;
    }
  } 
  else if (hue > 170 && hue < 300){
    Brain.Screen.print("The color is blue");
    //ringColor = vex::color::blue.rgb();
    if (allianceIsRed) {
      tossRing = true;
    }
  }
  else {
    Brain.Screen.print("It is not red or blue");
  }

  // toss if we dont want this ring 
  if (tossRing){
    wait(225, msec);
    Intake.stop();

    // restart intake
    wait(500, msec);
    Intake.spin(reverse);
  }
}

void StartColorSorting(){
  colorSortingEnabled = true;
  Brain.Screen.print("starting color sorting");
  Brain.Screen.newLine();
}

void StopColorSorting(){
  //Optical.setLight(ledState::off);
  colorSortingEnabled = false;
  Brain.Screen.print("stopping color sorting");
  Brain.Screen.newLine();

}

//////////////////////////


/**
 * The expected behavior is to return to the start position.
 */

void drive_test(){
  chassis.drive_distance(6);
  chassis.drive_distance(12);
  chassis.drive_distance(18);
  chassis.drive_distance(-36);
}

/**
 * The expected behavior is to return to the start angle, after making a complete turn.
 */

void turn_test(){
  chassis.turn_to_angle(5);
  chassis.turn_to_angle(30);
  chassis.turn_to_angle(90);
  chassis.turn_to_angle(225);
  chassis.turn_to_angle(0);
}

/**
 * Should swing in a fun S shape.
 */

void swing_test(){
  chassis.left_swing_to_angle(90);
  chassis.right_swing_to_angle(0);
}

/**
 * A little of this, a little of that; it should end roughly where it started.
 */

void full_test(){
  chassis.drive_distance(24);
  chassis.turn_to_angle(-45);
  chassis.drive_distance(-36);
  chassis.right_swing_to_angle(-90);
  chassis.drive_distance(24);
  chassis.turn_to_angle(0);
}

/**
 * Doesn't drive the robot, but just prints coordinates to the Brain screen 
 * so you can check if they are accurate to life. Push the robot around and
 * see if the coordinates increase like you'd expect.
 */

void odom_test(){
  chassis.set_coordinates(0, 0, 0);
  while(1){
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(5,20, "X: %f", chassis.get_X_position());
    Brain.Screen.printAt(5,40, "Y: %f", chassis.get_Y_position());
    Brain.Screen.printAt(5,60, "Heading: %f", chassis.get_absolute_heading());
    Brain.Screen.printAt(5,80, "ForwardTracker: %f", chassis.get_ForwardTracker_position());
    Brain.Screen.printAt(5,100, "SidewaysTracker: %f", chassis.get_SidewaysTracker_position());
    Brain.Screen.printAt(5,120, "RM positions: %f %f %f", 
      RightMotorFront.position(deg), RightMotorBackBottom.position(deg), RightMotorBackTop.position(deg));
    Brain.Screen.printAt(5,140, "RM group positions: %f", chassis.DriveR.position(deg));
    task::sleep(20);
  }
}

/**
 * Should end in the same place it began, but the second movement
 * will be curved while the first is straight.
 */

void tank_odom_test(){
  odom_constants();
  chassis.set_coordinates(0, 0, 0);
  chassis.turn_to_point(24, 24);
  chassis.drive_to_point(24,24);
  chassis.drive_to_point(0,0);
  chassis.turn_to_angle(0);
}

/**
 * Drives in a square while making a full turn in the process. Should
 * end where it started.
 */

void holonomic_odom_test(){
  odom_constants();
  chassis.set_coordinates(0, 0, 0);
  chassis.holonomic_drive_to_pose(0, 18, 90);
  chassis.holonomic_drive_to_pose(18, 0, 180);
  chassis.holonomic_drive_to_pose(0, 18, 270);
  chassis.holonomic_drive_to_pose(0, 0, 0);
}

void autonPreloadOnWallstakeAndTouchLadder(){
  
  // Path
// Path

chassis.drive_distance(39.557);

chassis.drive_distance(22.78);

chassis.drive_distance(-17.311);

chassis.drive_distance(-59.29);

chassis.drive_distance(-32.585);

chassis.drive_distance(17.311);




}

void autonRedLeftAWP() {
  // go to wall-stake
  chassis.drive_distance(18); // fwd
  chassis.turn_to_angle(270); // turnTo
  chassis.drive_distance(-4); 

  // spin preload on wallstake
  Intake.spin(reverse);
  wait(2, seconds);
  Intake.spin(forward);
  wait(250, msec);
  Intake.stop();

  // get mogo
  LiftClamp();
  chassis.turn_to_angle(235); // turnTo
  chassis.drive_distance(16); // fwd
  chassis.turn_to_angle(57); // turnTo
  chassis.drive_distance(-20); // rev
  DropClamp();
  wait(0.5, seconds);

  // pick up red-ring from red/blue stack
  Intake.spin(reverse);
  chassis.turn_to_angle(195); // turnTo
  chassis.drive_distance(21.842); // fwd

  // pick up red-ring from stack on auton-line
  chassis.turn_to_angle(261); // turnTo
  chassis.drive_distance(14.5); // fwd

  // prepare to go to ladder
  chassis.turn_to_angle(270); // turnTo
  chassis.drive_distance(-18); // rev
  chassis.turn_to_angle(340); // turnTo
  chassis.drive_distance(30); // fwd
  Intake.stop();
}

void autonRedLeft3RingsAndLadder() {

    allianceIsRed = true;
  StartColorSorting();
  default_constants();

  chassis.set_coordinates(0, 0, 0);

  // Path.tomio
// 1. Preload
LiftClamp();
chassis.drive_distance(11); // rev
Ladybrown.spinToPosition(500, degrees);
wait(0.5, seconds);


// 2. Mogo and 1st ring
//chassis.turn_to_angle(348.3); // turnTo
chassis.drive_distance(-39.5); // fwd
DropClamp();  
Intake.spin(reverse);
chassis.turn_to_angle(112); // turnTo
chassis.drive_distance(23); // rev
wait(0.5, seconds);
chassis.drive_distance(-3);

// 3. 2nd, 3rd ring and ladder
chassis.turn_to_angle(190); // turnTo
chassis.drive_distance(11.5); // fwd
chassis.drive_distance(-5);
chassis.turn_to_angle(144); // turnTo
chassis.drive_distance(-40); // rev
Ladybrown.spinTo(800, degrees);
chassis.turn_to_angle(193); // turnTo
Intake.stop();
chassis.drive_distance(4);



/*
  // 1. Pick up mobile goal
  LiftClamp();
  chassis.drive_distance(-29.5); // rev
  DropClamp();
  wait(0.25, seconds);
  
  // 2. Put preload
  Intake.spin(reverse);
  wait(1, seconds); 

  // 3. Pick up red ring from bottom of stack
  chassis.turn_to_angle(55); // turnTo
  chassis.drive_distance(16); // fwd
  wait(0.25, seconds);

  // 4. Pick up red ring from auton line
  chassis.turn_to_angle(138.5); // turnTo
  chassis.drive_distance(13); // fwd
  wait(0.25, seconds);

  // 5. Go to ladder
  chassis.turn_to_angle(154); // turnTo
  chassis.drive_distance(-20); // rev
  chassis.turn_to_angle(222); // turnTo
  Intake.stop();
  chassis.drive_distance(35.5); // fwd*/
}

void autonBlueRight3RingsAndLadder() {
  allianceIsRed = false;
  StartColorSorting();
  default_constants();

  chassis.set_coordinates(0, 0, 0);

  // Path.tomio
// 1. Preload
LiftClamp();
chassis.drive_distance(11); // rev
Ladybrown.spinToPosition(500, degrees);
wait(0.5, seconds);


// 2. Mogo and 1st ring
//chassis.turn_to_angle(348.3); // turnTo
chassis.drive_distance(-39.5); // fwd
DropClamp();  
Intake.spin(reverse);
chassis.turn_to_angle(234.5); // turnTo
chassis.drive_distance(23); // rev
wait(0.5, seconds);
chassis.drive_distance(-3);

// 3. 2nd, 3rd ring and ladder
chassis.turn_to_angle(325-180); // turnTo
chassis.drive_distance(11.5); // fwd
chassis.drive_distance(-5);
chassis.turn_to_angle(30-180); // turnTo
chassis.drive_distance(-45); // rev
Ladybrown.spinTo(800, degrees);
chassis.turn_to_angle(315-180); // turnTo
Intake.stop();
chassis.drive_distance(4);


/*
  // 1. Pick up mobile goal
  LiftClamp();
  chassis.drive_distance(-29.5); // rev
  DropClamp();
  wait(0.25, seconds);
  
  // 2. Put preload
  Intake.spin(reverse);
  wait(1, seconds);

  // 3. Pick up red ring from bottom of stack
  chassis.turn_to_angle(305); // turnTo
  chassis.drive_distance(15.975); // fwd
  wait(0.25, seconds);

  // 4. Pick up red ring from auton line
  chassis.turn_to_angle(221.5); // turnTo
  chassis.drive_distance(13); // fwd
  wait(0.25, seconds);

  // 5. Go to ladder
  chassis.turn_to_angle(26); // turnTo
  chassis.drive_distance(-20); // rev
  chassis.turn_to_angle(138); // turnTo
  chassis.drive_distance(27.5); // fwd
  Intake.stop();
*/


}

void autonLeft2RingsAndLadder() {
  //chassis.set_coordinates(0, 0, 0);

  // 1. Pick up mobile goal
  LiftClamp();
  chassis.drive_distance(-29.5);
  DropClamp();
  wait(0.25, seconds);
  
  // 2. Put preload
  Intake.spin(reverse);
  wait(1, seconds);

  // 3. Pick up red ring from bottom of stack
  chassis.turn_to_angle(55);
  chassis.drive_distance(15.975);
  wait(0.25, seconds);

  // 4. Go to ladder
  chassis.turn_to_angle(240);
  chassis.drive_timeout = 3000;
  chassis.drive_distance(30);
  Intake.stop();
}

void autonRight2RingsAndLadder() {
  //chassis.set_coordinates(0, 0, 0);

  // 1. Pick up mobile goal
  LiftClamp();
  chassis.drive_distance(-29.5); // rev
  DropClamp();
  wait(0.25, seconds);
  
  // 2. Put preload
  Intake.spin(reverse);
  wait(1, seconds);

  // 3. Pick up red ring from bottom of stack
  chassis.turn_to_angle(305); // turnTo
  chassis.drive_distance(15.975); // fwd
  wait(0.25, seconds);

  // 4. Go to ladder
  chassis.turn_to_angle(120);
  chassis.drive_timeout = 3000;
  chassis.drive_distance(30);
  Intake.stop();}

void autonStates(){

  //START

  Brain.Screen.print("autonStates running");
  Brain.Screen.newLine();

  // 0. Set velocities
  odom_constants();
  // Drivetrain.setHeading(145, degrees);
  // Drivetrain.setDriveVelocity(10, percent);
  // Drivetrain.setTurnVelocity(10, percent);
  Intake.setVelocity(100, percent);
  chassis.set_coordinates(0, 0, 0);


  // 1. Put the preloaded ring on the stake
  Intake.spin(reverse);
  wait(2, seconds);
  Intake.spin(forward);
  wait(1, seconds);
  Intake.stop();

  // 2. Drive into mobile goal
  LiftClamp();
  chassis.drive_distance(12.5);
  chassis.turn_to_angle(90);
  chassis.drive_distance(-18.5);
  wait(0.5, seconds);
  DropClamp();
  wait(0.5, seconds);

  // 3. Pick up rings in left corner
  chassis.turn_to_angle(270);
  Intake.spin(reverse);  
  chassis.drive_distance(43);
  chassis.turn_to_angle(225);
  chassis.drive_distance(-14);
  chassis.turn_to_angle(180);
  chassis.drive_distance(19.5);
  /*// rings 3 and 4
  chassis.drive_to_pose(-48, 26.4, 180);
  chassis.drive_distance(24);*/

  // 4. Put mobile goal in left corner
  chassis.turn_to_angle(55);
  chassis.drive_distance(-11);
  LiftClamp();
  wait(0.5, seconds);
  chassis.drive_distance(19);
  Intake.stop();

  // 5. Pick up second mogo
  chassis.turn_to_angle(270);
  Brain.Screen.print( "Heading: %f", chassis.get_absolute_heading());
  Brain.Screen.newLine();
  chassis.drive_distance(-31);
  chassis.turn_to_angle(275);
  chassis.drive_distance(-31);
  Brain.Screen.print( "Heading: %f", chassis.get_absolute_heading());
  Brain.Screen.newLine();
  wait(0.5, seconds);
  DropClamp();
  wait(0.5, seconds);
  chassis.turn_to_angle(90);
  Intake.spin(reverse);

  // 6. Pick up rings in right corner
  chassis.drive_distance(40);
  chassis.turn_to_angle(135);
  chassis.drive_distance(-14);
  chassis.turn_to_angle(180);
  chassis.drive_distance(19.5);

  // 7. Put mobile goal in right corner
  chassis.turn_to_angle(315);
  chassis.drive_distance(-12);
  LiftClamp();
  wait(0.5, seconds);
  chassis.drive_distance(20);
  Intake.stop();
  



  // 2. Put the preloaded ring on the stake
  // Intake.spin(reverse);
  // wait(2, seconds);
  
  // 3. Pick up first five rings and max out the stake
  // Drivetrain.turnToHeading(352, degrees);
  // a. first ring
  // drivePID(3.5, 35);
  // TurntoHeadingCorrection(270);
  // b. second ring
  // drivePID(3, 35);
  // TurntoHeadingCorrection(180);
  // c. third rings
  // drivePID(4.25, 20);// pick the ring not on the line as bonus after code finished
  // Drivetrain.setDriveVelocity(15, percent);
  // Drivetrain.setTimeout(1, seconds);
  // Drivetrain.drive(forward);
  // Drivetrain.driveFor(reverse, 22, inches);
  // TurntoHeadingCorrection(270);
  // drivePID(1.5, 20);
  // Drivetrain.driveFor(reverse, 10, inches);
  
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
  // Drivetrain.turnToHeading(55, degrees);
  // Drivetrain.setDriveVelocity(100, percent);
  // Drivetrain.setTimeout(1, seconds);
  // Drivetrain.driveFor(reverse, 50, inches);
  // LiftClamp();
  // Drivetrain.driveFor(forward, 3, inches);
  // Intake.stop();

  // 4. Go to wall to reset heading
  // Drivetrain.turnToHeading(45, degrees);
  // Drivetrain.setDriveVelocity(20, percent);
  // Drivetrain.driveFor(forward, 24, inches);
  // Drivetrain.turnToHeading(0, degrees);
  // Drivetrain.setTimeout(3, seconds);
  // Drivetrain.driveFor(reverse, 40, inches);
  // wait(0.2, seconds);
  // Inertial.setHeading(0, degrees);
  // Drivetrain.setHeading(0, degrees);

  // 5. Go to corner 1 stake
  // Drivetrain.setDriveVelocity(10, percent);
  // Drivetrain.driveFor(forward, 21.5, inches);
  // TurntoHeadingCorrection(90);
  // drivePID(5.5, 20);
  // TurntoHeadingCorrection(270);
  // Drivetrain.setDriveVelocity(15, percent);
  // Drivetrain.driveFor(reverse, 14, inches);
  // wait(0.5, seconds);
  // DropClamp();
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

  Brain.Screen.print("drive_stop");
  Brain.Screen.newLine();
  chassis.drive_stop(brakeType::hold);

}

void TakeTopRingInStack() {
  IntakeMotorA.spin(reverse);
  //chassis.drive_timeout = 1000;
  // chassis.turn_timeout = 1000;
  chassis.drive_distance(3);
  // Take bottom ring
  wait(0.5, seconds);
  Intake.stop();
  // Turn to side and spin out bottom ring
  chassis.drive_distance(-3);
  chassis.turn_to_angle(60);
  IntakeMotorA.spin(forward);
  wait(2, seconds);
  // Turn back to top ring now on floor and intake
  chassis.turn_to_angle(0);
  Intake.spin(reverse);
  chassis.drive_distance(6);
  wait(1, seconds);
  Intake.stop(); 
}

void auton_debug(){
  odom_constants();
  chassis.set_coordinates(0, 0, 0);
  chassis.set_drive_exit_conditions(1.5, 300, 5000);

  chassis.drive_distance(10);
  Brain.Screen.printAt(5, 160, "L: %f R: %f", chassis.get_left_position_in(), chassis.get_right_position_in());
  wait(2, seconds);

  chassis.drive_distance(20);
  Brain.Screen.printAt(5, 180, "L: %f R: %f", chassis.get_left_position_in(), chassis.get_right_position_in());

  wait(2, seconds);
  chassis.drive_distance(-10);
  Brain.Screen.printAt(5, 200, "L: %f R: %f", chassis.get_left_position_in(), chassis.get_right_position_in());

  //chassis.drive_stop(brakeType::hold);

  //LiftClamp();
  //DropClamp();
}