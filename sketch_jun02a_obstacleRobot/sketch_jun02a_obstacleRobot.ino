/*
 *Programmer: Mario Torres
 *Date Started: 06.02.2017
 *Last Updated: 06.02.2017
 *Description:  this is a program to test out the HC-SR04 sensor with an obstacle-avoiding robot
 *              the robot utilises two servo motors and two LEDs
 */
#include <Servo.h>
//constants & objects
const int TRIGGER_PIN = 2;
const int ECHO_PIN = 4;
const int BLU_LED = 7;
const int RED_LED = 8;
const int FWD = 180;  //full speed forward
const int BWD = 0;   //full speed backward
const int STOP = 90;

long sensor;
Servo servoLeft;
Servo servoRight;

void setup() 
{
  Serial.begin (9600);    //begin Serial monitor
  servoLeft.attach(9);    //pin9_leftMotor
  servoRight.attach(10);  //pin10_RightMotor
  pinMode(TRIGGER_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
  /*
   *NOTE: when LEDs were added the sensor seems to pick up low numbers (like 14 - 17)
   *i have added a guard for that in the if statement by adding conditions for the reading to be above this low value
   */
  pinMode(BLU_LED,OUTPUT);  //GO indicator
  pinMode(RED_LED,OUTPUT);  //AVOID indicator
}

void loop()
{ //turn on GO indicator, move straight
  digitalWrite(BLU_LED,HIGH);
  servoLeft.write(FWD);   //goes forward
  servoRight.write(BWD); //flipped, goes forward
  //start trigger (for echo location)
  digitalWrite(TRIGGER_PIN,LOW);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN,LOW);
  //read echo
  sensor = pulseIn(ECHO_PIN,HIGH);
  Serial.println(sensor);
  /*test lines that i used to find the threshold of the low values it would read
  if(sensor < 500){Serial.println(sensor);}   //to monitor sensor values
  */
  //if detects below a certain value (in this case 500), then go a little back and turn
  if( sensor >=20 && sensor <= 750 ) 
  { //turns off GO indicator and turns on AVOID indicator
    digitalWrite(BLU_LED,LOW);
    digitalWrite(RED_LED,HIGH);
    //stops both servos
    servoLeft.write(STOP);
    servoRight.write(STOP);
    delay(150);
    //sets both servos to full speed in opposite direction
    servoLeft.write(BWD);
    servoRight.write(FWD);
    delay(150);
    //sets only the left servo to rotate so the robot turns right
    servoLeft.write(FWD);
    delay(300);
    digitalWrite(RED_LED,LOW);
  }
}
