#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "Module.h"
#include "VCO.h"
#include "PatchCable.h"

#define MODULE_SLOTS 8
#define CLOCK_PIN 4
#define SHIFT_PIN 3
#define SOCKET_RECEIVE_DATA_PIN 14
#define MAX_CABLES 100
const int NUM_SOCKETS = MODULE_SLOTS * 8;
const int MODULE_ID_PINS[MODULE_SLOTS] = {2,2,2,2,2,2,2,2};
const int SOCKET_SEND_SELECT_PINS[] = {5,6,7};
const int SOCKET_RECEIVE_SELECT_PINS[] = {8,10,12};

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
  
  // initialise module pointers to null
  for(int i=0;i<MODULE_SLOTS;i++) {
    modules[i] = NULL;
  }

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

  delay(2000);
}

void loop() {
  // read module slots
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(SHIFT_PIN, LOW);
  delay(5);
  digitalWrite(SHIFT_PIN, HIGH);
  for(int i=0;i<8;i++) {
    for(int j=0;j<MODULE_SLOTS;j++) {
      bitWrite(moduleIdReadings[j],i,digitalRead(MODULE_ID_PINS[j]));
    }
    digitalWrite(CLOCK_PIN, LOW);
    delay(1);
    digitalWrite(CLOCK_PIN, HIGH);
    delay(1);
  }

  // for each module slot, check if module has been added, changed (very unlikely in one loop cycle!), or removed
  // for now, only checking one module slot for testing
  for(int i=0;i<1;i++) {
    int currentModuleID = 0; // default to ID=0, i.e. module slot is empty
    if(modules[i]) currentModuleID = modules[i]->getID(); // get ID of current module if slot not empty
    if(moduleIdReadings[i] != currentModuleID) {
      // something has changed

      if(moduleIdReadings[i] == 0) {
        // module has been removed
        delete modules[i];
        modules[i] = NULL;
        Serial.println("REMOVED MODULE");
      } else if(currentModuleID == 0) {
        // module has been added
        modules[i] = new VCO();
        Serial.println("ADDED MODULE");
      } else {
        // module has been changed (removed then added very quickly!)
        delete modules[i];
        modules[i] = new VCO();
        Serial.println("CHANGED MODULE");
      }
    }
  }

  // check for patch cable connections
  // for now, just add them manually
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

