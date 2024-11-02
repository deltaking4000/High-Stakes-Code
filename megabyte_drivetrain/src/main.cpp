/*----------------------------------------------------------------------------*/
/* */
/* Module: main.cpp */
/* Author: 938M Megabyte */
/* Created: 7/17/2024, 1:40:00 PM */
/* Description: V5 project */
/* */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include "vex_brain.h"
using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen


// define your global instances of motors and other devices here


#include "vex_controller.h"
//#include "vex_signaltower.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
//vex::brain Brain;
controller Controller;
//Back motors
motor leftMotorBack(PORT1, ratio6_1, false);
motor rightMotorBack(PORT3, ratio6_1, true);
//Front Motors
motor leftMotorFront(PORT9, ratio6_1, false);
motor rightMotorFront(PORT6, ratio6_1, true);
//Middle motors
motor leftMotorMiddle(PORT7, ratio6_1, false);
motor rightMotorMiddle(PORT10, ratio6_1, true);
// define your global instances of motors and other devices here;


int main() {
//Controller.Axis1.position();
//Controller.ButtonA.pressing();
//Brain.Screen.printAt( 10, 50, "Hello V5" );
while(1) {
/*Tank Drive 
int leftPos = Controller.Axis3.position();
int rightPos = Controller.Axis2.position();
leftMotorBack.spin(forward, leftPos, percent);
leftMotorFront.spin(forward, leftPos, percent);
rightMotorBack.spin(forward, rightPos, percent);
rightMotorFront.spin(forward,rightPos,percent)
*/
//Arcade Drive (Split)
int forwardPos = Controller.Axis3.position();
int turnPos = Controller.Axis1.position();
int deadzone = 5;
if (abs(forwardPos) < deadzone) {
forwardPos = 0;
}
if (abs(turnPos)< deadzone)
//Assuming only using axis 3 -> left motor a3, right motor a3
//Assuming only using axis 1 -> left motor : a1; right motor -a1
// Combine axis 3 and 1 -> motor : a3+a1; right motor : a3 - a1
//Arcade
//Left Motors
leftMotorFront.spin(forward, forwardPos + turnPos, percent);
leftMotorBack.spin(forward, forwardPos + turnPos, percent);
leftMotorMiddle.spin(forward, forwardPos + turnPos, percent);
//Right Motors
rightMotorBack.spin(forward, forwardPos - turnPos, percent);
rightMotorFront.spin(forward, forwardPos - turnPos, percent);
rightMotorMiddle.spin(forward, forwardPos - turnPos, percent);
//Code for Arcade Drive
// Allow other tasks to run

}
}


