///////////////////////////////////////////////////////////////////////////////////////
// OVERVIEW
////////////////////////////////////////////////////////////////////////////////////////
/*
DESCRIPTION: Receiver setup and loop

AUTHOR: Tuan Kiet Le & Brendan Thomas.

COMMENTS:

*/

///////////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <ESP32_NOW.h>
#include <WiFi.h>
#include "NewPing.h"
///////////////////////////////////////////////////////////////////////////////////////

//DEFINES, CONSTANT AND MACROS
//Address of the controller
uint8_t RxMACaddress[] = {0xec,0x64,0xc9,0x86,0x32,0x68};
//Define the message structure
typedef struct sensor_message{
  int middleSensor;
  int rightSensor;
  int leftSensor;
}sensor_message;
struct sensor_message sentData;

typedef struct struct_message
{
  int motorStatus;
  int motorSpeed;
  int motorMode;
}struct_message;
struct_message receivedData;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
//Define motor structure
typedef struct motor{
    int enablePin;
    int input1Pin;
    int input2Pin;
}motor;
//Initialize left and right motor
motor rightMotor;
motor leftMotor;
//This variable hold the Speed of the RC car
int Speed;
//Define constant delay time
#define STATION_DELAY 1000
#define TURN_DELAY 200

//Define constant and pin out for three sensor
#define MAX_DISTANCE 400

#define rightSensor_TRIG  13
#define rightSensor_ECHO  12

#define leftSensor_TRIG  15
#define leftSensor_ECHO  4

#define middleSensor_TRIG  14
#define middleSensor_ECHO  36
//Initialize distance of each sensor
float rightSensor_DIS;
float leftSensor_DIS;
float middleSensor_DIS;
//Minimum Distance Before Action
const int autoSpeed = 255;
const int autoSpeedTurn = 191;
const int MIN_DISTANCE = 20;
const int AUTO_DRIVE = 1;
const int MANUAL_DRIVE = 0;

int hadBackward = 0;
//Initialize module NewPing for each sensor
//NewPing rightSonar(rightSensor_TRIG, rightSensor_ECHO, MAX_DISTANCE);
//NewPing leftSonar(leftSensor_TRIG, leftSensor_ECHO, MAX_DISTANCE);
//NewPing middleSonar(middleSensor_TRIG, middleSensor_ECHO, MAX_DISTANCE);

//////////////////////////////////////////////////////////////////////////////////////
//FUNCTIONS
int motorSpeed(int motorStatus) {
  if (motorStatus == 1) {
    return 255;
  } else if (motorStatus == 2) {
    return 191;
  } else if (motorStatus == 3) {
    return 137;
  }
}
//Go Forward Function
void GoForward(struct motor rightMotor, struct motor leftMotor, int speed) {
  //Go Forward Right Wheel
  digitalWrite(rightMotor.input1Pin, HIGH);
  digitalWrite(rightMotor.input2Pin, LOW);
  analogWrite(rightMotor.enablePin, speed);
  // Go Forward Left Wheel
  digitalWrite(leftMotor.input1Pin, HIGH);
  digitalWrite(leftMotor.input2Pin, LOW);
  analogWrite(leftMotor.enablePin, speed);
}
void Station(struct motor rightMotor, struct motor leftMotor){
  digitalWrite(rightMotor.input1Pin, LOW);
  digitalWrite(rightMotor.input2Pin, LOW);
  digitalWrite(rightMotor.enablePin, LOW);
  // Go Forward Left Wheel
  digitalWrite(leftMotor.input1Pin, LOW);
  digitalWrite(leftMotor.input2Pin, LOW);
  digitalWrite(leftMotor.enablePin, LOW);
}
//Reverse Funtion
void GoBackward(struct motor rightMotor, struct motor leftMotor, int speed) {
  //Reverse Right wheel
  digitalWrite(rightMotor.input1Pin, LOW);
  digitalWrite(rightMotor.input2Pin, HIGH);
  analogWrite(rightMotor.enablePin, speed);
  //Reverse Left Wheel
  digitalWrite(leftMotor.input1Pin, LOW);
  digitalWrite(leftMotor.input2Pin, HIGH);
  analogWrite(leftMotor.enablePin, speed);
}

//Turn Left Function
void TurnLeft(struct motor rightMotor, struct motor leftMotor, int speed) {
  // Go Forward Right Wheel
  digitalWrite(rightMotor.input1Pin, HIGH);
  digitalWrite(rightMotor.input2Pin, LOW);
  analogWrite(rightMotor.enablePin, speed);
  //Rev left wheel
  digitalWrite(leftMotor.input1Pin, LOW);
  digitalWrite(leftMotor.input2Pin, HIGH);
  analogWrite(leftMotor.enablePin, speed);
  delay(500);
  Station(rightMotor, leftMotor);
  delay(500);
}

//Turn Right Function
void TurnRight(struct motor rightMotor, struct motor leftMotor, int speed) {
  //Go Forward left wheel
  digitalWrite(rightMotor.input1Pin, LOW);
  digitalWrite(rightMotor.input2Pin, HIGH);
  analogWrite(rightMotor.enablePin, speed );
  //Left wheel go forward
  digitalWrite(leftMotor.input1Pin, HIGH);
  digitalWrite(leftMotor.input2Pin, LOW);
  analogWrite(leftMotor.enablePin, speed);
  delay(500);
  Station(rightMotor, leftMotor);
  delay(500);
}



//Callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info_t  *mac_addr, const uint8_t *incomingData, int len) { 
  memcpy(&receivedData, incomingData, sizeof(receivedData));//Copy the incomingData to the receivedData
  Serial.print("\r\nMotor status:\t ");
  Serial.println(receivedData.motorStatus);
  //Speed
  Serial.print("\r\nMotor Speed:\t ");
  Serial.println(receivedData.motorSpeed);
}
//Based on the motor print the output of the motor's action
void controlMotor(int motorStatus, int motorSpeed){
  switch(motorStatus){
    case 1: Serial.print("Forward");
             Serial.println();
             break;
    case 2: Serial.print("REv");
             Serial.println();
             break;        
    case 3: Serial.print("left");
             Serial.println();
             break;
    case 4: Serial.print("right");
             Serial.println();
             break;
    case 0: Serial.print("do nothing");
             Serial.println();
             break;
  }
  switch(motorSpeed){
    case 1: Serial.print("Full Speed");
              Serial.println();
              break;
    case 2: Serial.print("Half Speed");
              Serial.println();
              break;
    case 3: Serial.print("Quarter Speed");
              Serial.println();
              break;
  }
}
//////////////////////////////////
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}
//This function control the autodrive algorithm

void AutoDrive(float middleSensor_DIS, float rightSensor_DIS, float leftSensor_DIS){
  if(middleSensor_DIS < MIN_DISTANCE && rightSensor_DIS < MIN_DISTANCE && leftSensor_DIS < MIN_DISTANCE){//If three way are blocked, go backward
    Station(rightMotor, leftMotor);
    delay(STATION_DELAY / 2);
    GoBackward(rightMotor, leftMotor, autoSpeed);
    hadBackward = 1;
    delay(STATION_DELAY);//move backward for 1 second
  }else if(middleSensor_DIS < MIN_DISTANCE){//If the front is blocked
    Station(rightMotor, leftMotor);//Stop the car
    delay(STATION_DELAY / 2);//Delay to read in data
    if(rightSensor_DIS > leftSensor_DIS){//Compare between right and left sensor
      TurnRight(rightMotor, leftMotor, autoSpeedTurn);
      delay(500);
    }
    else{
      TurnLeft(rightMotor, leftMotor, autoSpeedTurn);
      delay(500);
    }
  }else if(rightSensor_DIS < MIN_DISTANCE){
    TurnLeft(rightMotor, leftMotor, autoSpeedTurn);
  }else if(leftSensor_DIS < MIN_DISTANCE){
    TurnRight(rightMotor, leftMotor, autoSpeedTurn);
  }
  else{
    if(hadBackward){//if the car just went backward, turn left or right only
      if(rightSensor_DIS > leftSensor_DIS){//Compare between right and left sensor
        TurnRight(rightMotor, leftMotor, autoSpeedTurn);
        delay(500);
      }
      else{
        TurnLeft(rightMotor, leftMotor, autoSpeedTurn);
        delay(500);
      }
    }
    else{//else go forward and reset hadBackward back to 0
      GoForward(rightMotor, leftMotor, autoSpeed);
      
    }
    hadBackward = 0;
  }
}  
