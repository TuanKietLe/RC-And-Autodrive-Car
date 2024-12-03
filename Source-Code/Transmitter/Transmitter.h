///////////////////////////////////////////////////////////////////////////////////////
// OVERVIEW
////////////////////////////////////////////////////////////////////////////////////////
/*
DESCRIPTION: Header file for Transmitter.ino. This file holds
             all the includes, defines, macros, and user - defined
             functions.

AUTHOR: Tuan Kiet Le & Brendan Thomas.

COMMENTS:

*/
///////////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <esp_now.h>
#include <WiFi.h>
///////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
//STRUCTURE

//Define the message structure
typedef struct sensor_message{
  int middleSensor;
  int rightSensor;
  int leftSensor;
}sensor_message;
struct sensor_message receivedData;
typedef struct struct_message
{
  int motorStatus;
  int motorSpeed;
  int motorMode;
}struct_message;
struct_message sentData;

//Define the joystick structure
typedef struct Joystick
{
  int xPin;//Hold the pin that reads X axis
  int yPin;//Hold the pin that reads Y axis
  int xVal;//Hold X value
  int yVal;//Hold Y value
}Joystick;

struct Joystick SpeedController;
struct Joystick DirController;
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
//DEFINES, CONSTANT AND MACROS

const int inputPin = 25;
const int AUTO_DRIVE = 1;
const int MANUAL_DRIVE =0;

//Recevier MAC address
uint8_t RxMACaddress[] = {0x88,0x13,0xbf,0x01,0xec,0x6c};
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
//FUNCTIONS

//This function reads the xVal of the directional joystick
int ReadXVal(int pinNumber){
    return analogRead(pinNumber);
}
//This function reads the yVal of the directional joystick
int ReadYVal(int pinNumber){
    return analogRead(pinNumber);
}
//This function will be called whenever the data is received 
void OnDataRecv(const esp_now_recv_info_t  *mac_addr, const uint8_t *incomingData, int len) { 
  memcpy(&receivedData, incomingData, sizeof(receivedData));//Copy the incomingData to the receivedData
}
//Callback function that will be executed when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

//Get motor status based on the xVal and yVal
//xVal ranges from 0 to 4095, where 0 is fully reverse and 4095 is fully forward
//yVal ranges from 0 to 4095, where 0 is fully left and 4095 is fully right
int getMotorStatus(int xVal, int yVal)// MODE 1: FORWARD, MODE 2: REVERSE, MODE 3: LEFT, MODE 4: RIGHT
{
  if (yVal > 3500){
    if (xVal > 3500) {
      return 1;
    } else if (xVal < 1000) {
      return 2;
    } else {
      return 4;
    }
  } else if (yVal < 1000) {
    if (xVal > 3500) {
      return 1;
    } else if (xVal < 1000) {
      return 2;
    } else {
      return 3;
    }
  } else {
    if (xVal > 3500) {
      return 1;
    } else if (xVal < 1000) {
      return 2;
    } else {
      return 0;
    }
  }
}
//This function will get the status of the speed based on the xVal of the speed joystick
int getMotorSpeed(int xValSpeed)// MODE 1: FULL SPEED, MODE 2: HALF-SPEED, MODE 3: QUARTER-SPEED
{
//If the joystick is pushed forward, increase speed to max speed
  if (xValSpeed > 3500) {
    return 1;
//If the joystick is pulled backward, decrease speed to quarter speed
  } else if (xValSpeed < 1000) {
    return 3;
//If the joystick is idle or not pushed/pulled to previous values, speed remains at half speed
  } else {
    return 2;
  }
}
//This function will read the mode of the RC car
int getMotorMode(int inputPin){
  return digitalRead(inputPin);
}
//This function is used for testing and debugging onlyy
void printStatus(struct Joystick DirController, struct Joystick SpeedController, int inputPin){
  Serial.print("X: ");
  Serial.print(DirController.xVal);
  Serial.print(" | Y: ");
  Serial.print(DirController.yVal);
  Serial.print(" | Speed: ");
  Serial.print(SpeedController.xVal);
  Serial.print(" | Status: ");
  Serial.print(getMotorStatus(DirController.xVal, DirController.yVal));
  Serial.print(" | Mode: ");
  Serial.println(getMotorMode(inputPin));
  Serial.print("Middle: ");
  Serial.print(receivedData.middleSensor);
  Serial.print("Right: ");
  Serial.print(receivedData.rightSensor);
  Serial.print("Left: ");
  Serial.print(receivedData.leftSensor);
  
}
