///////////////////////////////////////////////////////////////////////////////////////
// OVERVIEW
////////////////////////////////////////////////////////////////////////////////////////
/*
DESCRIPTION: Transmitter setup and loop 

AUTHOR: Tuan Kiet Le & Brendan Thomas.

COMMENTS:

*/
//////////////////////////////////////////////////////////////////////////////////////////
#include "Transmitter.h"

void setup()
{
  //Define pin for the directional controller
  DirController.xPin = 34;
  DirController.yPin = 35;

  //Define pin for the speed controller
  SpeedController.xPin = 36;
  //Initialize Serial monitor
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  //Initialize ESP-NOW
  if(esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //Once ESP-NOW is successfully Init, we will register for senb CB to
  //send the data
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  //Initialize peerInfo
  esp_now_peer_info_t peerInfo;
  //Copy the receiver MAC address to the peer_addr
  memcpy(peerInfo.peer_addr, RxMACaddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  //Add peer and send data
  if(esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }

  //Set up GPIO
  pinMode(DirController.xPin, INPUT);
  pinMode(DirController.yPin, INPUT);
  pinMode(SpeedController.xPin, INPUT);
  pinMode(inputPin, INPUT);
}

void loop()
{
  //Get input from the controller
  DirController.xVal   = ReadXVal(DirController.xPin);
  DirController.yVal   = ReadYVal(DirController.yPin);
  SpeedController.xVal = ReadXVal(SpeedController.xPin);
  //Get status from read value
  sentData.motorMode   = getMotorMode(inputPin); 
  sentData.motorStatus = getMotorStatus(DirController.xVal, DirController.yVal);
  sentData.motorSpeed  = getMotorSpeed(SpeedController.xVal);
  //Send data to the receiver
  esp_err_t result = esp_now_send(RxMACaddress, (uint8_t *) &sentData, sizeof(sentData));
  if (result == ESP_OK) Serial.println("Sent with success");
  else Serial.println("Error sending the data");
  
  //For testing purposes
  printStatus(DirController, SpeedController, inputPin);

  delay(500);
    
}
