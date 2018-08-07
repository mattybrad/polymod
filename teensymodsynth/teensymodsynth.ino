#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "Module.h"
#include "VCO.h"
#include "PatchCable.h"

#define EMPTY_MODULE 0
#define OUTPUT_MODULE 1
#define VCO_MODULE 2
#define VCF_MODULE 3
#define VCA_MODULE 4

#define MODULE_SLOTS 8
#define SOCKET_RECEIVE_DATA_PIN 27
#define MODULE_ID_PIN 28
#define ANALOG_DATA_PIN 33
#define MAX_CABLES 100
const int NUM_SOCKETS = MODULE_SLOTS * 8;
const int SOCKET_SEND_ROOT_SELECT_PINS[] = {2,3,4};
const int SOCKET_RECEIVE_ROOT_SELECT_PINS[] = {17,20,21};
const int SOCKET_SEND_MODULE_SELECT_PINS[] = {5,8,16};
const int SOCKET_RECEIVE_MODULE_SELECT_PINS[] = {24,25,26};

Module *modules[MODULE_SLOTS]; // array of pointers to module instances
PatchCable *patchCables[MAX_CABLES];
byte moduleIdReadings[MODULE_SLOTS]; // readings of module IDs - changes will causes program to update module listing
boolean patchCableConnections[NUM_SOCKETS][NUM_SOCKETS];

AudioControlSGTL5000 sgtl;

void setup() {
  
  // initialise audio board
  AudioMemory(50);
  sgtl.enable();
  sgtl.volume(0.5);

  // initialise shift register pins
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(SHIFT_PIN, OUTPUT);
  for(int i=0;i<MODULE_SLOTS;i++) {
    pinMode(MODULE_ID_PINS[i],INPUT); 
  }

  // initialise mux/demux pins
  for(int i=0;i<3;i++) {
    pinMode(SOCKET_SEND_SELECT_PINS[i], OUTPUT);
    pinMode(SOCKET_RECEIVE_SELECT_PINS[i], OUTPUT);
  }
  pinMode(SOCKET_RECEIVE_DATA_PIN, INPUT);
  
  Serial.begin(9600);

  // read module slots
  for(int a=0;a<MODULE_SLOTS;a++) {
    digitalWrite(SOCKET_SEND_ROOT_SELECT_PINS[0],bitRead(a,0));
    digitalWrite(SOCKET_SEND_ROOT_SELECT_PINS[1],bitRead(a,1));
    digitalWrite(SOCKET_SEND_ROOT_SELECT_PINS[2],bitRead(a,2));
    
    for(int b=0;b<8;b++) {
      digitalWrite(SOCKET_SEND_MODULE_SELECT_PINS[0],bitRead(b,0));
      digitalWrite(SOCKET_SEND_MODULE_SELECT_PINS[1],bitRead(b,1));
      digitalWrite(SOCKET_SEND_MODULE_SELECT_PINS[2],bitRead(b,2));

      bitWrite(moduleIdReadings[a],b,digitalRead(MODULE_ID_PIN));
    }

    switch(moduleIdReadings[a]) {
      case EMPTY_MODULE:
      modules[a] = NULL;
      break;

      case OUTPUT_MODULE:
      modules[a] = new Output();
      break;

      case VCO_MODULE:
      modules[a] = new VCO();
      break;

      default:
      modules[a] = NULL;
    }
  }
}

void loop() {

  boolean newConnection;
  for(int i=0;i<8;i++) {
    // switch the output channel
    digitalWrite(SOCKET_SEND_SELECT_PINS[0],bitRead(i,0));
    digitalWrite(SOCKET_SEND_SELECT_PINS[1],bitRead(i,1));
    digitalWrite(SOCKET_SEND_SELECT_PINS[2],bitRead(i,2));
    
    for(int j=0;j<8;j++) {
      // switch the input channel
      digitalWrite(SOCKET_RECEIVE_SELECT_PINS[0],bitRead(j,0));
      digitalWrite(SOCKET_RECEIVE_SELECT_PINS[1],bitRead(j,1));
      digitalWrite(SOCKET_RECEIVE_SELECT_PINS[2],bitRead(j,2));

      delayMicroseconds(10);
      
      newConnection = false;
      if(i>j && digitalRead(SOCKET_RECEIVE_DATA_PIN)) {
        newConnection = true;
      }

      if(newConnection != patchCableConnections[i][j]) {
        if(newConnection) {
          // make connection
          addPatchCable(i,j);
        } else {
          // break connection
          removePatchCable(i,j);
        }
      }
      patchCableConnections[i][j] = newConnection;
    }
  }
}

void addPatchCable(int highSocket, int lowSocket) {
  int foundIndex = -1;
  for(int i=0;i<MAX_CABLES&&foundIndex==-1;i++) {
    if(patchCables[i]==NULL) foundIndex = i;
  }
  if(foundIndex >= 0) {
    patchCables[foundIndex] = new PatchCable(highSocket, lowSocket, getSocket(highSocket), getSocket(lowSocket));
    Serial.println("ADDED CABLE");
  } else {
    Serial.println("CAN'T ADD NEW CABLE");
  }
}

void removePatchCable(int highSocket, int lowSocket) {
  for(int i=0;i<MAX_CABLES;i++) {
    if(patchCables[i]!=NULL) {
      // a cable exists in this slot
      if(highSocket==patchCables[i]->getHighSocket() && lowSocket==patchCables[i]->getLowSocket()) {
        // remove cable
        patchCables[i]->disconnect();
        delete patchCables[i];
        patchCables[i] = NULL;
        Serial.println("REMOVED CABLE");
      }
    }
  }
}

Socket &getSocket(int socketNumber) {
  int moduleNumber = socketNumber / 8;
  int moduleSocketNumber = socketNumber % 8;
  Serial.println(moduleNumber);
  Serial.println(moduleSocketNumber);
  return modules[moduleNumber]->getSocket(moduleSocketNumber);
}

