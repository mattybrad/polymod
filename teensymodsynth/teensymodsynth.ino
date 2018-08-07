#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "Module.h"

#define MODULE_SLOTS 8
#define CLOCK_PIN 4
#define SHIFT_PIN 3
const int MODULE_ID_PINS[MODULE_SLOTS] = {2,2,2,2,2,2,2,2};

Module *modules[MODULE_SLOTS]; // array of pointers to module instances
byte moduleIdReadings[MODULE_SLOTS];

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
  Serial.begin(9600);
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
        Serial.println("REMOVED");
      } else if(currentModuleID == 0) {
        // module has been added
        modules[i] = new Module(moduleIdReadings[i]);
        Serial.println("ADDED");
      } else {
        // module has been changed (removed then added very quickly!)
        delete modules[i];
        modules[i] = new Module(moduleIdReadings[i]);
        Serial.println("CHANGED");
      }
    }
  }
  
  delay(500);
}
