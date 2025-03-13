using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor LeftMotorFront;
extern motor LeftMotorBackBottom;
extern motor LeftMotorBackTop;
extern motor RightMotorFront;
extern motor RightMotorBackBottom;
extern motor RightMotorBackTop;
extern drivetrain Drivetrain;
extern controller Controller1;
extern motor IntakeMotorA; 
extern motor IntakeMotorB; 
extern motor_group Intake;
extern digital_out Clamp;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );