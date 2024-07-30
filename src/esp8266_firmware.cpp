#include <SoftwareSerial.h>
#include <Arduino.h>
#include "networking.h"

// networking class
Networking network;

String message = "";
bool messageReady = false;

const byte rxPin = 14; // d5
const byte txPin = 12; // d6

// Set up a new SoftwareSerial object
// probably going to switch to the hardware serial once everything is programmed
SoftwareSerial mySerial (rxPin, txPin);

void serialFlush();

void setup() {
  Serial.begin(9600);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // Set the baud rate for the SoftwareSerial object
  mySerial.begin(9600);
  network.setup();
}

// flush the serial buffer
void serialFlush(){
  while(mySerial.available() > 0) {
    char t = mySerial.read();
    Serial.println(t);
  }
}

void loop() {

  // put your main code here, to run repeatedly:
  while (mySerial.available()) { 
    message = mySerial.readString();
    serialFlush();
    messageReady = true;
  }
  if (messageReady == true) {
    messageReady = false;
    // this is where the data should be collected from serial and stored for upload every two minutes.
     network.writeDataToThingSpeak(message);
  }
  //network.writeDataToThingSpeak("5;6;7;8;9;10");
}
