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
  chassis.heading_max_voltage = 10 ;//10
  chassis.drive_max_voltage = 8;//8
  chassis.turn_max_voltage = 12; // 12
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

    wait(250, msec);
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
  wait(1, seconds);
  //Intake.spin(forward);
  //wait(1, seconds);
  Intake.stop();

  // 2. Drive into mobile goal
  LiftClamp();
  chassis.drive_distance(14);
  chassis.turn_to_angle(90);
  chassis.drive_distance(-18.5);
  wait(0.5, seconds);
  DropClamp();
  wait(0.5, seconds);

  // 3. Pick up rings in left corner
  chassis.turn_to_angle(270);
  Intake.spin(reverse); 
  chassis.drive_timeout = 3000;
  chassis.drive_distance(32);
  wait(1, seconds);
  chassis.drive_distance(11);
  chassis.turn_to_angle(225, 12);
  chassis.drive_distance(-14);
  chassis.turn_to_angle(180, 12);
  chassis.drive_timeout = 2000;
  chassis.drive_distance(19.5);

  // 4. Put mobile goal in left corner
  chassis.turn_timeout = 1000;
  chassis.turn_to_angle(55, 12);
  chassis.drive_timeout = 1000;  
  chassis.drive_distance(-11);
  LiftClamp();
  wait(0.5, seconds);
  chassis.drive_distance(21);
  Intake.stop();/*

  // 5. Pick up second mogo
  odom_constants();
  chassis.turn_to_angle(270);
  Brain.Screen.print( "Heading: %f", chassis.get_absolute_heading());
  Brain.Screen.newLine();
  chassis.drive_distance(-62);
  Brain.Screen.print( "Heading: %f", chassis.get_absolute_heading());
  Brain.Screen.newLine();
  DropClamp();
  wait(250, msec);
  chassis.turn_to_angle(90);
  Intake.spin(reverse);

  // 6. Pick up rings in right corner
  chassis.drive_timeout = 3000;
  chassis.drive_distance(38);
  chassis.turn_to_angle(135);
  chassis.drive_distance(-14);
  chassis.turn_to_angle(180);
  chassis.drive_timeout = 2000;  
  chassis.drive_distance(19.5);

  // 7. Put mobile goal in right corner
  chassis.turn_timeout = 1000;
  chassis.turn_to_angle(305);
  chassis.drive_timeout = 1000;  
  chassis.drive_distance(-12);
  LiftClamp();
  wait(250, msec);
  chassis.drive_distance(10); 
  Intake.stop();
  odom_constants();

  // 8. pick up first ring partially
  chassis.turn_to_angle(0-1);
  Intake.spin(reverse);
  chassis.drive_distance(39);
  // partial intake
  Intake.stop();

  // 9. pick up second ring partially by only using flywheel
  IntakeMotorA.spin(reverse);
  chassis.turn_to_angle(0);
  chassis.drive_distance(41);

  // 9. Pick up mobile goal
  chassis.turn_to_angle(105);
  chassis.drive_distance(-43.5);
  chassis.turn_to_angle(144.5);
  chassis.drive_distance(-15);
  DropClamp();
  wait(250, msec);

  // 10. Spin rings onto mobile goal
  Intake.spin(reverse);
  chassis.turn_to_angle(78);

  // 11. Throw third mobile goal to the side
  chassis.drive_distance(-19);
  wait(1, seconds); // look at removing dis if the ring lands before the robot stops
  Intake.stop();
  LiftClamp();
  wait(150, msec);

  // 12. Put corner 3 stake in the positive
  chassis.drive_max_voltage = 10;
  chassis.turn_max_voltage = 10;
  chassis.turn_to_angle(240);
  chassis.drive_distance(-35);
  chassis.turn_to_angle(262);
  chassis.drive_timeout = 3000;
  chassis.drive_distance(-48);

  // 13. Put corner 4 stake in the positive
  chassis.turn_to_angle(235);
  chassis.drive_distance(27);
  chassis.turn_to_angle(105);
  chassis.drive_timeout = 5000;
  chassis.drive_distance(-200);
  Brain.Screen.print("Sigma or Skibidi?");

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

// Define the detected function with a void return type,
// showing it doesn't return a value.


void auton_debug(){
// Path

// 0. Set velocities
odom_constants();
// Drivetrain.setHeading(145, degrees);
// Drivetrain.setDriveVelocity(10, percent);
// Drivetrain.setTurnVelocity(10, percent);
Intake.setVelocity(100, percent);
chassis.set_coordinates(-59, 0, 90);

// Path


// Path
Intake.spin(reverse);
chassis.drive_to_pose(-46.761, 9.569, 179.31);
chassis.drive_to_pose(-46.928, 23.381, 178.34);
chassis.drive_to_pose(-23.963, 24.046, 46.313);
chassis.drive_to_pose(-0.166, 58.826, 45);
chassis.drive_to_pose(23.464, 47.094, 145.222);
Intake.stop();

}
