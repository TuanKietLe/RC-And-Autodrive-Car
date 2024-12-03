///////////////////////////////////////////////////////////////////////////////////////
// OVERVIEW
////////////////////////////////////////////////////////////////////////////////////////
/*
DESCRIPTION: Receiver setup and loop

AUTHOR: Tuan Kiet Le & Brendan Thomas.

COMMENTS:

*/

#include "Receiver.h"

void setup() {
  //These pins control left wheel
  leftMotor.input1Pin = 23;
  leftMotor.input2Pin = 21;
  leftMotor.enablePin = 18;
  
  //These pins control right wheel
  rightMotor.input1Pin = 33;
  rightMotor.input2Pin = 26;
  rightMotor.enablePin = 27;

  //Initialize Serial monitor
  Serial.begin(9600);
  //Set mode for the ESP32
  WiFi.mode(WIFI_STA);
  //Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //Once ESPNow is successfully Init, we will register for recv CB to
  //get recv packer info and register for send CB to send the packer info
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);
  //Initialize peerInfo
  esp_now_peer_info_t peerInfo;
  //Copy the receiver MAC address to the peer_addr
  memcpy(peerInfo.peer_addr, RxMACaddress, 6);
  //Both ESP32 must be on the same channel
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  //Using esp_now_add_peer to make the connection the second ESP32
  if(esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
  // Pins for left wheel  
  pinMode(leftMotor.input1Pin, OUTPUT);
  pinMode(leftMotor.input2Pin, OUTPUT);
  pinMode(leftMotor.enablePin, OUTPUT);
  // Pins for right wheel
  pinMode(rightMotor.input1Pin, OUTPUT);
  pinMode(rightMotor.input2Pin, OUTPUT);
  pinMode(rightMotor.enablePin, OUTPUT);

  //Pins for right sensor
  pinMode(rightSensor_TRIG, OUTPUT);
  pinMode(rightSensor_ECHO, INPUT);

  //Pins for left sensor
  pinMode(leftSensor_TRIG, OUTPUT);
  pinMode(leftSensor_ECHO, INPUT);

  //Pins for middle sensor
  pinMode(middleSensor_TRIG, OUTPUT);
  pinMode(middleSensor_ECHO, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Status:\t");
  Serial.println(receivedData.motorStatus);
  Serial.print("Speed:\t");
  Serial.println(receivedData.motorSpeed);
  Serial.print("Mode:\t");
  Serial.println(receivedData.motorMode);

  //using NewPing
//  sentData.rightSensor = rightSonar.ping_cm();
//  sentData.leftSensor = leftSonar.ping_cm();
//  sentData.middleSensor = middleSonar.ping_cm();
  //using pulse in 
  sentData.rightSensor = getDistance(rightSensor_TRIG, rightSensor_ECHO);
  sentData.leftSensor = getDistance(leftSensor_TRIG, leftSensor_ECHO);
  sentData.middleSensor = getDistance(middleSensor_TRIG, middleSensor_ECHO);

  
  esp_err_t result = esp_now_send(RxMACaddress, (uint8_t *) &sentData, sizeof(sentData));
  if (result == ESP_OK) Serial.println("Sent with success");
  else Serial.println("Error sending the data");

  
  if (receivedData.motorMode == 0) {
    //Get the speed from the received data
    Speed = motorSpeed(receivedData.motorSpeed);
    //Speed = 255;
    if(receivedData.motorStatus == 1){
      //Go Forward Right Wheel
    GoForward(rightMotor, leftMotor, Speed);
    }else if(receivedData.motorStatus == 2){
      GoBackward(rightMotor, leftMotor, Speed);
    }else if(receivedData.motorStatus == 3){
      TurnLeft(rightMotor, leftMotor, Speed);
    }else if(receivedData.motorStatus == 4){
      TurnRight(rightMotor, leftMotor, Speed);
    }else{
      Station(rightMotor, leftMotor);
    }

    /////// AUTODRIVE ////////
  } else if (receivedData.motorMode == 1) {
//    rightSensor_DIS = rightSonar.ping_cm();
//    //Get the distance of leftSensor
//    leftSensor_DIS = leftSonar.ping_cm();
//    //Get the distance of middleSensor
//    middleSensor_DIS = middleSonar.ping_cm();
    
    rightSensor_DIS = getDistance(rightSensor_TRIG, rightSensor_ECHO);
    leftSensor_DIS = getDistance(leftSensor_TRIG, leftSensor_ECHO);
    middleSensor_DIS = getDistance(middleSensor_TRIG, middleSensor_ECHO);
    if(leftSensor_DIS == 0){
      lefSensor_DIS = 21;
    }
    if(rightSensor_DIS == 0){
      rightSensor_DIS = 21;
    }
    if(middleSensor_DIS == 0{
      middleSensor_DIS = 21;
    }
    AutoDrive(middleSensor_DIS, rightSensor_DIS, leftSensor_DIS);
  }
  
}
