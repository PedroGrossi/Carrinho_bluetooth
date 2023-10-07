#include <SoftwareSerial.h> // Lib to Bluetooth

#define pinRX 2             // Digital pin 2 (RX)
#define pinTX 3             // Digital pin 3 (TX)
#define pinMotor1_1 5       // PWM 1 - motor 1
#define pinMotor1_2 6       // PWM 2 - motor 1
#define pinMotor2_1 10      // PWM 1 - motor 2
#define pinMotor2_2 11      // PWM 2 - motor 2

char bth_data = 0;          // Variable to Bluetooth data
int speed = 255;            // Max speed

// Motor Class
class DCMotor {  
  int spd = 255, pin1, pin2;
  
  public:  
  
    void Pinout(int in1, int in2){ // Method for declaring motor pinout
      pin1 = in1;
      pin2 = in2;
      pinMode(pin1, OUTPUT);
      pinMode(pin2, OUTPUT);
      }       void Speed(int in1){ // Method for saving motor speed
      spd = in1;
      }     
    void Forward(){ // Method to go forward
      analogWrite(pin1, spd);
      digitalWrite(pin2, LOW);
      }   
    void Backward(){ // Method to go backward
      digitalWrite(pin1, LOW);
      analogWrite(pin2, spd);
      }
    void Stop(){ // Method to stop
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      }
   };

DCMotor Motor1, Motor2;                 // Motor objects 
SoftwareSerial bluetooth(pinRX, pinTX); // Pins to emulate serial

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  Motor1.Pinout(pinMotor1_1,pinMotor1_2); // Set motor pins.
  Motor2.Pinout(pinMotor2_1,pinMotor2_2); 
}
void loop() {
  while(bluetooth.available()){
    bth_data = bluetooth.read();
    if (bth_data != 13 && bth_data != 10){
      // Control motors intensity
      motor_control(bth_data, speed, Motor1, Motor2);
    }
    delay(100);
    }
}

void motor_control(char bth_data, int speed, DCMotor Motor1, DCMotor Motor2){
  if (bth_data == 'F'){
    // Only go foward
    Motor1.Speed(speed); // Set motors speed
    Motor2.Speed(speed);
    Motor1.Forward(); // Command to go forward 
    Motor2.Forward();
    delay(10);
  } else if (bth_data == 'B'){
    // Only go backward
    Motor1.Speed(speed); // Set motors speed
    Motor2.Speed(speed);
    Motor1.Backward(); // Command to go backward 
    Motor2.Backward();
    delay(10);
  }else if (bth_data == 'L'){
    // Only go left
    Motor1.Speed(speed*0.2); // Set motors speed
    Motor2.Speed(speed*0.8);
    Motor1.Forward(); // Command to go forward 
    Motor2.Forward();
  }else if (bth_data == 'R'){
    // Only go right
    Motor1.Speed(speed*0.8); // Set motors speed
    Motor2.Speed(speed*0.2);
    Motor1.Forward(); // Command to go forward 
    Motor2.Forward();
  }else if (bth_data == 'G'){
    // Forward Left
    Motor1.Speed(speed*0.6); // Set motors speed
    Motor2.Speed(speed*0.8);
    Motor1.Forward(); // Command to go forward 
    Motor2.Forward();
  }else if (bth_data == 'I'){
    // Forward Right
    Motor1.Speed(speed*0.8); // Set motor speed
    Motor2.Speed(speed*0.6);
    Motor1.Forward(); // Command to go forward 
    Motor2.Forward();
  }else if (bth_data == 'H'){
    // BackLeft
    Motor1.Speed(speed*0.8); // Set motor speed
    Motor2.Speed(speed*0.6);
    Motor1.Backward(); // Command to go backward 
    Motor2.Backward();
  }else if (bth_data == 'J'){
    // BackRight
    Motor1.Speed(speed*0.6); // Set motor speed
    Motor2.Speed(speed*0.8);
    Motor1.Backward(); // Command to go backward 
    Motor2.Backward();
  }else {
    // Stop
    Motor1.Stop(); // Command to stop
    Motor2.Stop();
  }
}
