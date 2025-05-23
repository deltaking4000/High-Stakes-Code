#pragma once
#include "JAR-Template/drive.h"

class Drive;

extern Drive chassis;

void default_constants();

void autonPreloadOnWallstakeAndTouchLadder();
void autonRedLeftAWP();
void autonRedLeft3RingsAndLadder();
void autonBlueRight3RingsAndLadder();
void autonLeft2RingsAndLadder();
void autonRight2RingsAndLadder();
void autonStates();

// JAR test autons
void drive_test();
void turn_test();
void swing_test();
void full_test();
void odom_test();
void tank_odom_test();
void holonomic_odom_test();
void auton_debug();

// callback for optical
void OpticalObjectDetected();

// Color sorting start/stop
void StartColorSorting();
void StopColorSorting();
extern bool allianceIsRed;

