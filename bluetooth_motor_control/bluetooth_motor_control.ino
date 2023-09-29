#include <SoftwareSerial.h> // Lib to Bluetooth
#include <AFMotor.h>        // Lib to motor shield

#define pinRX 2            // Digital pin 2 (RX)
#define pinTX 3            // Digital pin 3 (TX)
#define pinMotor_R 5       // PWM pin to motor right
#define pinMotor_L 6       // PWM pin to motor left

int bth_data = 0;          // Variable to Bluetooth data 
int z_direction = 0;       // direction in axis z
int y_direction = 0;       // direction in axis y
int speed = 0;             // speed control

SoftwareSerial bluetooth(pinRX, pinTX); // Pins to emulate serial

AF_DCMotor motor_R(pinMotor_R);     // Motor R -> right
AF_DCMotor motor_L(pinMotor_L);     // Motor L -> left

void setup(); // Setup for code
void loop(); // Main loop
void direction(int bth_data, int *z_direction, int *y_direction, int *speed); // Verify selected direction 
void motor_control(int z_direction, int y_direction, int speed, AF_DCMotor motor_R, AF_DCMotor motor_L); // Set motor speed based in selected direction

void setup() {
  Serial.begin(9600);      // Init serial communication

  bluetooth.begin(9600);   // Init bluetooth serial

  motor_R.run(RELEASE);    // Stop motor R
  motor_L.run(RELEASE);    // Stop motor L

  delay(100);
}

void loop() {
  // If bluetooth is available
  if(bluetooth.available()){
    bth_data = bluetooth.read(); // Receives bluetooth value

    // Set speed -> in this moment speed will be hardcoded
    speed = 191; // almost 75% of max speed -> will me our max speed for now

    // Verify what to do -> documentation = values from app (Bluetooth RC Controller)
    // Directions
    direction(bth_data, &z_direction, &y_direction, &speed);
  }

  // Control motors intensity
  motor_control(z_direction, y_direction, speed, motor_R, motor_L);
}

void direction(int bth_data, int *z_direction, int *y_direction, int *speed){
  switch (bth_data) {
      case 'F': // Forward
        *z_direction = 1; // Move only axis z -> positive direction
        *y_direction = 0;
        break;
      case 'B': // Backward
        *z_direction = -1; // Move only axis z -> negative direction 
        *y_direction = 0;
        break;
      case 'L': // Left
        *z_direction = 0; // Move only axis y -> positive direction
        *y_direction = 1;
        break;
      case 'R': // Right
        *z_direction = 0; // Move only axis y -> negative direction
        *y_direction = -1;
        break;
      case 'G': // Forward Left
        *z_direction = 1; // Move axis z and y -> positive direction
        *y_direction = 1;
        break;
      case 'I': // Forward Right
        *z_direction = 1; // Move axis z and y -> z = positive | y = negative
        *y_direction = -1;
        break;
      case 'H': // BackLeft
        *z_direction = -1; // Move axis z and y -> z = negative | y = positive
        *y_direction = 1;
        break;
      case 'J': // BackRight
        *z_direction = -1; // Move axis z and y -> negative direction
        *y_direction = -1;
        break;
      default:
        *z_direction = 0; // Stop
        *y_direction = 0;
        *speed = 0;
        break;
  }
}

void motor_control(int z_direction, int y_direction, int speed, AF_DCMotor motor_R, AF_DCMotor motor_L){
  if (z_direction == 1 && y_direction == 0){
    // Only go foward
    motor_R.run(FORWARD);
    motor_L.run(FORWARD);
    motor_R.setSpeed(speed);
    motor_L.setSpeed(speed);
    delay(10);
  } else if (z_direction == -1 && y_direction == 0){
    // Only go backward
    motor_R.run(FORWARD);
    motor_L.run(FORWARD);
    motor_R.setSpeed(speed);
    motor_L.setSpeed(speed);
    delay(10);
  } else if (z_direction == 0 && y_direction == 1){
    // Only go right
    motor_R.run(FORWARD);
    motor_L.run(FORWARD);
    motor_R.setSpeed(speed*0.2); // Set speed to 20%
    motor_L.setSpeed(speed*0.8); // Set speed to 80%
    delay(10);
  } else if (z_direction == 0 && y_direction == -1){
    // Only go left
    motor_R.run(FORWARD);
    motor_L.run(FORWARD);
    motor_R.setSpeed(speed*0.8); // Set speed to 80%
    motor_L.setSpeed(speed*0.2); // Set speed to 20%
    delay(10);
  } else if (z_direction == 1 && y_direction == 1){
    // Go foward and right
    motor_R.run(FORWARD);
    motor_L.run(FORWARD);
    motor_R.setSpeed(speed*0.4); // Set speed to 40%
    motor_L.setSpeed(speed*0.6); // Set speed to 60%
    delay(10);
  } else if (z_direction == 1 && y_direction == -1){
    // Go foward and left
    motor_R.run(FORWARD);
    motor_L.run(FORWARD);
    motor_R.setSpeed(speed*0.6); // Set speed to 60%
    motor_L.setSpeed(speed*0.4); // Set speed to 40%
    delay(10);
  } else if (z_direction == -1 && y_direction == 1){
    // Go backward and right
    motor_R.run(BACKWARD);
    motor_L.run(BACKWARD);
    motor_R.setSpeed(speed*0.4); // Set speed to 40%
    motor_L.setSpeed(speed*0.6); // Set speed to 60%
    delay(10);
  } else if (z_direction == -1 && y_direction == -1){
    // Go backward and left
    motor_R.run(BACKWARD);
    motor_L.run(BACKWARD);
    motor_R.setSpeed(speed*0.6); // Set speed to 60%
    motor_L.setSpeed(speed*0.4); // Set speed to 40%
    delay(10);
  } else {
    // stop
    motor_R.run(RELEASE);
    motor_L.run(RELEASE);
    delay(10);
  }
}
