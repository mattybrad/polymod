#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "Module.h"
#include "Output.h"
#include "VCO.h"
#include "Mixer.h"
#include "LFO.h"
#include "PatchCable.h"

#define EMPTY_MODULE 0
#define OUTPUT_MODULE 1
#define VCO_MODULE 2
#define VCF_MODULE 3
#define VCA_MODULE 4
#define MIXER_MODULE 5
#define LFO_MODULE 6

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

  // initialise mux/demux pins
  for(int i=0;i<3;i++) {
    pinMode(SOCKET_SEND_ROOT_SELECT_PINS[i], OUTPUT);
    pinMode(SOCKET_SEND_MODULE_SELECT_PINS[i], OUTPUT);
    pinMode(SOCKET_RECEIVE_ROOT_SELECT_PINS[i], OUTPUT);
    pinMode(SOCKET_RECEIVE_MODULE_SELECT_PINS[i], OUTPUT);
  }
  pinMode(SOCKET_RECEIVE_DATA_PIN, INPUT);
  pinMode(MODULE_ID_PIN, INPUT);
  
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

    // test code, overrides actual readings
    if(true) {
      moduleIdReadings[a] = EMPTY_MODULE;
      moduleIdReadings[0] = OUTPUT_MODULE;
      moduleIdReadings[1] = VCO_MODULE;
      moduleIdReadings[2] = MIXER_MODULE;
      moduleIdReadings[3] = LFO_MODULE;
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

      case MIXER_MODULE:
      modules[a] = new Mixer();
      break;

      case LFO_MODULE:
      modules[a] = new LFO();
      break;

      default:
      modules[a] = NULL;
    }
  }
}

void loop() {

  boolean newConnection;
  for(int a=0;a<8;a++) {
    // switch the root socket send channel
    digitalWrite(SOCKET_SEND_ROOT_SELECT_PINS[0],bitRead(a,0));
    digitalWrite(SOCKET_SEND_ROOT_SELECT_PINS[1],bitRead(a,1));
    digitalWrite(SOCKET_SEND_ROOT_SELECT_PINS[2],bitRead(a,2));
    
    for(int b=0;b<8;b++) {
      // switch the module socket send channel
      digitalWrite(SOCKET_SEND_MODULE_SELECT_PINS[0],bitRead(b,0));
      digitalWrite(SOCKET_SEND_MODULE_SELECT_PINS[1],bitRead(b,1));
      digitalWrite(SOCKET_SEND_MODULE_SELECT_PINS[2],bitRead(b,2));

      for(int c=0;c<8;c++) {
        // switch the root socket receive channel
        digitalWrite(SOCKET_RECEIVE_ROOT_SELECT_PINS[0],bitRead(c,0));
        digitalWrite(SOCKET_RECEIVE_ROOT_SELECT_PINS[1],bitRead(c,1));
        digitalWrite(SOCKET_RECEIVE_ROOT_SELECT_PINS[2],bitRead(c,2));

        for(int d=0;d<8;d++) {
          // switch the module socket receive channel
          digitalWrite(SOCKET_RECEIVE_MODULE_SELECT_PINS[0],bitRead(d,0));
          digitalWrite(SOCKET_RECEIVE_MODULE_SELECT_PINS[1],bitRead(d,1));
          digitalWrite(SOCKET_RECEIVE_MODULE_SELECT_PINS[2],bitRead(d,2));

          int socket1 = a*8+b;
          int socket2 = c*8+d;

          if(socket1 > socket2) {
            newConnection = false;
            if(digitalRead(SOCKET_RECEIVE_DATA_PIN)) {
              newConnection = true;
            }

            // testing code, overrides any actual connections
            if(true) {
              if(a==2 && b==0 && c==1 && d==0) newConnection = true;
              if(a==2 && b==1 && c==1 && d==1) newConnection = true;
              if(a==2 && b==2 && c==1 && d==2) newConnection = true;
              if(a==2 && b==3 && c==1 && d==3) newConnection = true;
              if(a==3 && b==4 && c==1 && d==4) newConnection = true;
              if(a==2 && b==4 && c==0 && d==0) newConnection = true;
            }
      
            if(newConnection != patchCableConnections[socket1][socket2]) {
              if(newConnection) {
                // make connection
                addPatchCable(socket1,socket2);
              } else {
                // break connection
                removePatchCable(socket1,socket2);
              }
            }
            patchCableConnections[socket1][socket2] = newConnection;
          }
        }
      }
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

