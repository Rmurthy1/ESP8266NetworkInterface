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
SoftwareSerial mySerial (rxPin, txPin);

const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;
void recvwithStartEndMarkers();
void showNewData();

// Function to receive data from the serial port and store it in a buffer. the data must be formated as "<n1;n2>"
void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (mySerial.available() > 0 && newData == false) {
        rc = mySerial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}


void showNewData() {
    if (newData == true) {
        network.writeDataToThingSpeak(String(receivedChars));
        newData = false;
        //network.writeDataToThingSpeak("5;6;7;8;9;10");
    }
}

void setup() {
  Serial.begin(9600);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);
  network.setup(10000);
}

void loop() {
  //network.writeDataToThingSpeak("5;6;7;8;9;10");
  recvWithStartEndMarkers();
  showNewData();
}
