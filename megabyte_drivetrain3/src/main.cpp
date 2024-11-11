/*----------------------------------------------------------------------------*/
/* Team Members: Anderson, Anish, Cici, Daniel, Dylan, and Sehaj               */
/* Module: main.cpp                                                             */
/* Author: 938M Megabyte Coders                                                  */
/* Created: 9/7/2024, 3:05:00 PM                                                  */
/*This is the code for the driving                                                 */
/* Description: V5 project                                                        */
/* Coders: Sehaj, Daniel, and Anderson                                           */
/* Current Code Owners: Sehaj and Daniel                                        */
/* This is the third version of this code.                                     */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include "vex_brain.h"
using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen

// define your global instances of motors and other devices here

#include "vex_controller.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
controller Controller;
//Note: We noticed some ports were not working on our brain, so some of these are out of order.
//Left motors
motor leftMotorBack(PORT1, ratio6_1, false);
motor leftMotorFront(PORT9, ratio6_1, false);
//Right Motors
motor rightMotorBack(PORT3, ratio6_1, true);
motor rightMotorfront(PORT6, ratio6_1, true);

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
//Right Motors
rightMotorBack.spin(forward, forwardPos - turnPos, percent);
rightMotorfront.spin(forward, forwardPos - turnPos, percent);
//Code for Arcade Drive
// Allow other tasks to run

}
}



