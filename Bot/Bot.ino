// thatonebyte
// Mechanum Wheel Control for Tetrix
// March 19, 2024

 // TODO
 // Implment control scheme flip for when bot is backwards

 // Heres where you want to connect motors:

 // left front motor connected to motor 1
 // right front motor connected to motor 2
 // left back motor connected to exc motor 1
 // right back motor connected to exc motor 2
 
#include <PRIZM.h>      // Required Prizm Libary
#include <TELEOP.h>     // TeleOP Library

PRIZM prizm;            // Instantiate an object in the PRIZM class named "prizm"
EXPANSION exc;          // Instantiate an object in the EXPANSION class named "exc"
PS4 ps4;                // Instantiate an object in the PS4 class named "ps4"

float highSpeed = 1;                  // variable to set the motor percentage (100%) for turbo mode
float lowSpeed = highSpeed*.25;       // variable to set the motor percentage (25%) for crawl mode
float medSpeed = highSpeed*.40;       // variable to set the motor percentage (40%) for normal mode
float powerMultiplier = medSpeed;     // variable to set the motor power percentage (currently at medPower or 40%)
int mPowers[] = {0,0,                 // array to track motor powers for the 4 different motors (front left, front right...
                 0,0};                //                                                         back left,  back right)

void setup() {
  
  
  prizm.PrizmBegin();  
  prizm.setServoSpeed(1, 100); // set servo speeds and position
  prizm.setServoPosition(1, 0);        
  prizm.setServoSpeed(2, 25); 
  prizm.setServoPosition(1, 90); 
  ps4.setDeadZone(LEFT,10);   // Deadzones, adjust this if controller is drifting   
  ps4.setDeadZone(RIGHT,10);          
  prizm.setMotorInvert(1,1); 
  exc.setMotorInvert(1,1,1); // Invert all motors
  
}

void loop() {
  ps4.getPS4(); // get the status of the PS4 button states

  adjustMotorPowers(); // Function to adjust the motor powers (L1 = low, R1 = high, Neutral = Med)

 // These next two lines of code set the mPowers array to the motor power values needed for the four different motors. 
 //                                               
 // Equations are used to determine the power level of each motor based on PS4 stick positions. Here are the equations for each motor:
 // Front Left Motor:  (LY + LX - RX) * powerMultiplier         (Left Stick Y Position + Left Stick X Position - Right Stick X Position)
 // Front Right Motor: (LY - LX + RX) * powerMultiplier         (Left Stick Y Position - Left Stick X Position + Right Stick X Position)
 // Back Left Motor:   (LY - LX - RX) * powerMultiplier         (Left Stick Y Position - Left Stick X Position - Right Stick X Position)
 // Back Right Motor:  (LY + LX + RX) * powerMultiplier         (Left Stick Y Position + Left Stick X Position + Right Stick X Position)
 //  Each motor power is then constrained between -100 and 100 so that when both sticks are used at the same time the power never exceeds 100
 //
  
  int mPowers[] = {constrain((ps4.Motor(LY)+ps4.Motor(LX)-ps4.Motor(RX))*powerMultiplier,-100,100), constrain((ps4.Motor(LY)-ps4.Motor(LX)+ps4.Motor(RX))*powerMultiplier,-100,100),
                   constrain((ps4.Motor(LY)-ps4.Motor(LX)-ps4.Motor(RX))*powerMultiplier,-100,100), constrain((ps4.Motor(LY)+ps4.Motor(LX)+ps4.Motor(RX))*powerMultiplier,-100,100)};
  
  prizm.setMotorPowers(mPowers[0], mPowers[1]); // set PRIZM motor powers based on the values from the mPowers array
  exc.setMotorPowers(1,mPowers[2], mPowers[3]);  

  if(ps4.Button(DOWN) == 1) { // Servo control 1
    prizm.setServoPosition(1, 180);
  }
  else if(ps4.Button(UP) == 1) {
    prizm.setServoPosition(1, -180);
  }
  else {
    prizm.setServoPosition(1, 90);
  }

  if(ps4.Button(LEFT) == 1) { // Servo control 1
    prizm.setServoPosition(2, 180);
  }
  else if(ps4.Button(RIGHT) == 1) {
    prizm.setServoPosition(2, -180);
  }
  else {
    prizm.setServoPosition(2, 90);
  }
}

void adjustMotorPowers(){                                          
  if(ps4.Button(L1)==1){                                     
    powerMultiplier = lowSpeed;                                        
    ps4.setRumble(SLOW);                                               
    ps4.setLED(RED);}                                                   
  else if(ps4.Button(R1)==1){                                       
    powerMultiplier = highSpeed;                                       
    ps4.setRumble(FAST);                                                
    ps4.setLED(GREEN);}                                                 
  else {                                                             
    powerMultiplier = medSpeed;                                        
    ps4.setRumble(STOP);                                                
    ps4.setLED(YELLOW);}                                             
}                                                                  