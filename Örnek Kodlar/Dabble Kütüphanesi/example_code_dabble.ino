//"""REX 8in1 Arm Bot"""
//Check the web site for Robots https://rex-rdt.readthedocs.io/en/latest/

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
#include <Arduino.h>
#include <analogWrite.h>
#include <ESP32Servo.h>

//define pins of motors
#define MotorA1 15
#define MotorA2 23

#define MotorB1 32
#define MotorB2 33

#define MotorC1 17
#define MotorC2 16

#define MotorD1 27
#define MotorD2 14

int position1 = 0 ;
int position2 = 0 ;
int position3 = 0 ;
int position4 = 0 ;

//define buzzer pins which named "horn"
#define horn 2

//define pins of servo motors
Servo Servo1; // Forward-Bakcward
Servo Servo2; // Right-Legt
Servo Servo3; // Up-Down
Servo Servo4; // Open-Close
enum MOTOR_TYPE {
  DC_MOTOR,
  SERVO_MOTOR
};
enum MOTOR_TYPE motorType = DC_MOTOR;

void servo_motor() {

  if (GamePad.isSquarePressed())
  {
    Serial.print("Servo Square");
    if (position4 >0 ) {
      position4 = position4 - 1;
    }
    if (position4 <0){
      position4 = position4 + 1;
    }
    Servo4.write(position4);
  }

  if (GamePad.isCirclePressed())
  {
    Serial.print("Servo Left");
     if (position4 >0) {
      position1 = position1 - 1;
    }
    if (position1 <0){
      position1 = position1 + 1;
    }
    Servo1.write(position1);
  }

  if (GamePad.isCrossPressed())
  {
    if (position2 > 0) {
      position2 = position2 - 1;
    }
    if (position2 < 0){
      position2 = position2 + 1;
    }
    Servo2.write(position2);
  }

  if (GamePad.isTrianglePressed())
  {
    Serial.print("Servo Triangle");
    if (position3 > 0) {
      position3 = position3 - 1;
    }
    if (position3 < 0){
      position3 = position3 + 1;
    }
    Servo3.write(position3);
  }
  stop();
}
void dc_motor() {
  if (GamePad.isUpPressed())
  {
    Serial.print("DC Forward");
    forward();
    delay(300);
    stop();

  }

  if (GamePad.isDownPressed())
  {
    Serial.print("DC Back");
    backward();
    delay(300);
    stop();
  }

  if (GamePad.isLeftPressed())
  {
    Serial.print("DC Left");
    left();
    delay(300);
    stop();
  }

  if (GamePad.isRightPressed())
  {
    Serial.print("DC Right");
    right();
    delay(300);
    stop();
  }
 
}

void forward() {

  digitalWrite(MotorA1, HIGH);
  digitalWrite(MotorA2, LOW);

  digitalWrite(MotorB1, HIGH);
  digitalWrite(MotorB2, LOW);

  digitalWrite(MotorC1, HIGH);
  digitalWrite(MotorC2, LOW);

  digitalWrite(MotorD1, HIGH);
  digitalWrite(MotorD2, LOW);
}

void right() {
  digitalWrite(MotorA1, HIGH);
  digitalWrite(MotorA2, LOW);

  digitalWrite(MotorB1, HIGH);
  digitalWrite(MotorB2, LOW);

  digitalWrite(MotorC1, LOW);
  digitalWrite(MotorC2, HIGH);

  digitalWrite(MotorD1, LOW);
  digitalWrite(MotorD2, HIGH);
}

void left() {

  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, HIGH);

  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, HIGH);

  digitalWrite(MotorC1, HIGH);
  digitalWrite(MotorC2, LOW);

  digitalWrite(MotorD1, HIGH);
  digitalWrite(MotorD2, LOW);

}

void stop() {

  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, LOW);

  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, LOW);

  digitalWrite(MotorC1, LOW);
  digitalWrite(MotorC2, LOW);

  digitalWrite(MotorD1, LOW);
  digitalWrite(MotorD2, LOW);

}

void backward() {

  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, HIGH);

  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, HIGH);

  digitalWrite(MotorC1, LOW);
  digitalWrite(MotorC2, HIGH);

  digitalWrite(MotorD1, LOW);
  digitalWrite(MotorD2, HIGH);
}

void gamepad() {
  digitalWrite(MotorA1, HIGH);
  digitalWrite(MotorA2, LOW);

  digitalWrite(MotorB1, HIGH);
  digitalWrite(MotorB2, LOW);

  digitalWrite(MotorC1, HIGH);
  digitalWrite(MotorC2, LOW);

  digitalWrite(MotorD1, HIGH);
  digitalWrite(MotorD2, LOW);
}

void setup() {
  //first positions of servo motors
  Servo1.write(position1 = 85);
  Servo2.write(position2 = 140);
  Servo3.write(position3 = 22);
  Servo4.write(position4 = 90);

  //active pins which is defined
  pinMode(horn, OUTPUT);

  Servo1.attach(25);
  Servo2.attach(26);
  Servo3.attach(18);
  Servo4.attach(19);

  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);

  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);

  pinMode(MotorC1, OUTPUT);
  pinMode(MotorC2, OUTPUT);

  pinMode(MotorD1, OUTPUT);
  pinMode(MotorD2, OUTPUT);

  Serial.begin(115200);      // make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin("REX_ROBOT"); //set bluetooth name of your device
}

void loop() {
  //Print of servo motor position on Serial Port
  Serial.println(position1);
  Serial.println(position2);
  Serial.println(position3);
  Serial.println(position4);

  //This function is used to keep information coming from the mobile device up to date.
  Dabble.processInput();

  dc_motor();
  servo_motor();
  
  Serial.println('\t');
  int a = GamePad.getAngle();
  Serial.print("Angle: ");
  Serial.print(a);
  Serial.print('\t');

  int b = GamePad.getRadius();
  Serial.print("Radius: ");
  Serial.print(b);
  Serial.print('\t');

  float c = GamePad.getXaxisData();
  Serial.print("x_axis: ");
  Serial.print(c);
  Serial.print('\t');

  float d = GamePad.getYaxisData();
  Serial.print("y_axis: ");
  Serial.println(d);
  Serial.println();

}