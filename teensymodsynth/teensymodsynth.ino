#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "Module.h"
#include "PatchCable.h"
#include "KeyboardHandler.h"
#include "Master.h"
#include "VCO.h"
#include "Mixer.h"
#include "LFO.h"
#include "VCF.h"
#include "Noise.h"
#include "Envelope.h"
#include "VCA.h"

#define EMPTY_MODULE 0
#define MASTER_MODULE 255
#define VCO_MODULE 1
#define VCA_MODULE 2
#define LFO_MODULE 3
#define ENVELOPE_MODULE 4
#define NOISE_MODULE 5
#define VCF_MODULE 6
#define MIXER_MODULE 7

#define MULTIPLEXER_DELAY 5 // can probably go as low as 3 but best to be safe, latency not affected too much
#define MAX_POLYPHONY 1
#define MODULE_SLOTS 8
#define SOCKET_RECEIVE_DATA_PIN 25
#define MODULE_ID_PIN 26
#define ANALOG_DATA_PIN 21
#define MAX_CABLES 100
const int NUM_SOCKETS = MODULE_SLOTS * 8;
const int SOCKET_SEND_ROOT_SELECT_PINS[] = {30,31,32}; // A
const int SOCKET_RECEIVE_ROOT_SELECT_PINS[] = {2,3,4}; // C
const int SOCKET_SEND_MODULE_SELECT_PINS[] = {33,34,35}; // B
const int SOCKET_RECEIVE_MODULE_SELECT_PINS[] = {27,29,28}; // D

//const int KEYBOARD_PINS[] = {6,7,10,12,14}; // temporary pins for reading notes

Module *modules[MODULE_SLOTS][MAX_POLYPHONY]; // array of pointers to module instances
PatchCable *patchCables[MAX_CABLES];
KeyboardHandler keyboardHandler(MAX_POLYPHONY);
byte moduleIdReadings[MODULE_SLOTS]; // readings of module IDs - changes will causes program to update module listing
boolean patchCableConnections[NUM_SOCKETS][NUM_SOCKETS];

AudioControlSGTL5000 sgtl;
AudioOutputI2S mainOutput;
AudioMixer4 mainMixer;
AudioConnection con1(mainMixer,0,mainOutput,0);
AudioConnection con2(mainMixer,0,mainOutput,1);
AudioConnection* masterConnections[MAX_POLYPHONY];
Master masterModules[MAX_POLYPHONY];

void setup() {

  Serial.begin(9600);

  delay(3000);

  // initialise audio board
  AudioMemory(300);
  sgtl.enable();
  sgtl.volume(0.2);

  // initialise mux/demux pins
  for(int i=0;i<3;i++) {
    pinMode(SOCKET_SEND_ROOT_SELECT_PINS[i], OUTPUT);
    pinMode(SOCKET_SEND_MODULE_SELECT_PINS[i], OUTPUT);
    pinMode(SOCKET_RECEIVE_ROOT_SELECT_PINS[i], OUTPUT);
    pinMode(SOCKET_RECEIVE_MODULE_SELECT_PINS[i], OUTPUT);
  }
  pinMode(SOCKET_RECEIVE_DATA_PIN, INPUT);
  pinMode(MODULE_ID_PIN, INPUT_PULLUP);

  // init temp keyboard pins
  for(int i=0;i<5;i++) {
    //pinMode(KEYBOARD_PINS[i], INPUT_PULLUP);
  }

  // read module slots
  for(int a=0;a<MODULE_SLOTS;a++) {
    digitalWrite(SOCKET_SEND_ROOT_SELECT_PINS[0],bitRead(a,0));
    digitalWrite(SOCKET_SEND_ROOT_SELECT_PINS[1],bitRead(a,1));
    digitalWrite(SOCKET_SEND_ROOT_SELECT_PINS[2],bitRead(a,2));
    
    for(int b=0;b<8;b++) {
      digitalWrite(SOCKET_SEND_MODULE_SELECT_PINS[0],bitRead(b,0));
      digitalWrite(SOCKET_SEND_MODULE_SELECT_PINS[1],bitRead(b,1));
      digitalWrite(SOCKET_SEND_MODULE_SELECT_PINS[2],bitRead(b,2));
      delayMicroseconds(MULTIPLEXER_DELAY);
      bitWrite(moduleIdReadings[a],b,!digitalRead(MODULE_ID_PIN));
    }

    moduleIdReadings[7] = MASTER_MODULE;

    // test code, overrides actual readings
    if(false) {
      moduleIdReadings[a] = EMPTY_MODULE;
      moduleIdReadings[0] = MASTER_MODULE;
      moduleIdReadings[1] = VCO_MODULE;
      moduleIdReadings[2] = MIXER_MODULE;
      moduleIdReadings[3] = VCA_MODULE;
      moduleIdReadings[4] = LFO_MODULE;
      moduleIdReadings[5] = VCF_MODULE;
      moduleIdReadings[6] = ENVELOPE_MODULE;
      moduleIdReadings[7] = ENVELOPE_MODULE;
    }

    Serial.println(moduleIdReadings[a]);

    for(int p=0;p<MAX_POLYPHONY;p++) {
      switch(moduleIdReadings[a]) {
        case EMPTY_MODULE:
        modules[a][p] = NULL;
        break;
  
        case MASTER_MODULE:
        modules[a][p] = &masterModules[p];
        masterConnections[p] = new AudioConnection(modules[a][p]->getMainOutput(),0,mainMixer,p);
        break;
  
        case VCO_MODULE:
        modules[a][p] = new VCO();
        break;
  
        case MIXER_MODULE:
        modules[a][p] = new Mixer();
        break;
  
        case LFO_MODULE:
        modules[a][p] = new LFO();
        break;
  
        case VCF_MODULE:
        modules[a][p] = new VCF();
        break;
  
        case VCA_MODULE:
        modules[a][p] = new VCA();
        break;
  
        case NOISE_MODULE:
        modules[a][p] = new Noise();
        break;
  
        case ENVELOPE_MODULE:
        modules[a][p] = new Envelope();
        break;
  
        default:
        modules[a][p] = NULL;
      }
    }
  }

  AudioProcessorUsageMaxReset();
  AudioMemoryUsageMaxReset();
}

void loop() {

  int loopStart = millis();

  boolean newConnectionReading;
  for(int a=0;a<8;a++) {
    // switch the root socket send channel (set which module to send a 3.3V signal to)
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

          delayMicroseconds(MULTIPLEXER_DELAY);

          if(socket1 < socket2) {
            newConnectionReading = false;
            if(digitalRead(SOCKET_RECEIVE_DATA_PIN)) {
              newConnectionReading = true;
            }

            // testing code, overrides any actual connections
            if(false) {
              newConnectionReading = false;
              if(fakeConnection(socket1,socket2,1,3,2,0)) newConnectionReading = true;
              if(fakeConnection(socket1,socket2,1,2,2,1)) newConnectionReading = true;
              if(fakeConnection(socket1,socket2,0,1,1,0)) newConnectionReading = true;
              if(fakeConnection(socket1,socket2,2,4,3,0)) newConnectionReading = true;
              if(fakeConnection(socket1,socket2,0,2,6,0)) newConnectionReading = true;
              //if(fakeConnection(socket1,socket2,0,2,7,0)) newConnectionReading = true;
              if(fakeConnection(socket1,socket2,6,1,3,1)) newConnectionReading = true;
              if(fakeConnection(socket1,socket2,3,2,5,0)) newConnectionReading = true;
              //if(fakeConnection(socket1,socket2,4,2,5,1)) newConnectionReading = true;
              //if(fakeConnection(socket1,socket2,7,1,5,1)) newConnectionReading = true;
              if(fakeConnection(socket1,socket2,5,2,0,0)) newConnectionReading = true;
            }
      
            if(newConnectionReading != patchCableConnections[socket1][socket2]) {
              if(newConnectionReading) {
                // make connection
                addPatchCable(socket1,socket2);
              } else {
                // break connection
                removePatchCable(socket1,socket2);
              }
            }
            patchCableConnections[socket1][socket2] = newConnectionReading;
          }

          int controlReading = analogRead(ANALOG_DATA_PIN); // could reduce latency by only doing a control reading when a module is using that channel
          keyboardHandler.update();
          for(int p=0;p<MAX_POLYPHONY;p++) {
            if(modules[c][p]) {
              modules[c][p]->updateControlValue(d,controlReading);
            }
            for(int m=0;m<8;m++) {
              if(modules[m][p]) {
                modules[m][p]->update();
              }
            }
            if(a==0) {
              masterModules[p].note = keyboardHandler.getNote(p);
              masterModules[p].gate = keyboardHandler.getGate(p);
            }
          }
        }
      }
    }
  }
  //Serial.print("MEM: ");
  //Serial.println(AudioMemoryUsageMax());
  //Serial.print("CPU: ");
  //Serial.println(AudioProcessorUsageMax());

  Serial.println(millis() - loopStart);
}

void addPatchCable(int highSocket, int lowSocket) {
  for(int p=0;p<MAX_POLYPHONY;p++) {
    int foundIndex = -1;
    for(int i=0;i<MAX_CABLES&&foundIndex==-1;i++) {
      if(patchCables[i]==NULL) foundIndex = i;
    }
    if(foundIndex >= 0) {
      patchCables[foundIndex] = new PatchCable(highSocket, lowSocket, getSocket(highSocket, p), getSocket(lowSocket, p));
      Serial.println("ADDED CABLE");
    } else {
      Serial.println("CAN'T ADD NEW CABLE");
    }
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

Socket &getSocket(int socketNumber, int polyIndex) {
  int moduleNumber = socketNumber / 8;
  int moduleSocketNumber = socketNumber % 8;
  Serial.println(moduleNumber);
  Serial.println(moduleSocketNumber);
  return modules[moduleNumber][polyIndex]->getSocket(moduleSocketNumber);
}

boolean fakeConnection(int socket1, int socket2, int module1, int moduleSocket1, int module2, int moduleSocket2) {
  int testSocket1 = 8*module1 + moduleSocket1;
  int testSocket2 = 8*module2 + moduleSocket2;
  if(socket1==testSocket1 && socket2==testSocket2) return true;
  if(socket1==testSocket2 && socket2==testSocket1) return true;
  return false;
}

