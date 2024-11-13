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

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// IntakeMotorGroup     motor_group   5, 11           
// Controller1          controller                    
// Drivetrain           drivetrain    9, 1, 6, 3      
// ---- END VEXCODE CONFIGURED DEVICES ----
#include "vex_brain.h"
using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen

// define your global  motors and other devices here

#include "vex_controller.h"

using namespace vex;


// define your global instances of motors and other devices here;


int main() {
    // Initializing Robot Configuration. DO NOT REMOVE!
    // SEMICOLONS!!!!!
  vexcodeInit();
  IntakeMotorGroup.setVelocity(100, percent);

}
