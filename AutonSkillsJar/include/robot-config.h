using namespace vex;

extern brain Brain;

//Add your devices below, and don't forget to do the same in robot-config.cpp:
extern motor LeftMotorFront;
extern motor LeftMotorBackBottom;
extern motor LeftMotorBackTop;
extern motor RightMotorFront;
extern motor RightMotorBackBottom;
extern motor RightMotorBackTop;
extern motor_group LeftDriveSmart; 
extern motor_group RightDriveSmart;
extern smartdrive Drivetrain;
extern controller Controller1;
extern motor_group Intake;
extern digital_out Clamp;
extern inertial Inertial;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );