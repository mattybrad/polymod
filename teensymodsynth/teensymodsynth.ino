#include "Module.h"

#define MODULE_SLOTS 8
#define CLOCK_PIN 4
#define SHIFT_PIN 3
#define DATA_PIN 2

Module *modules[MODULE_SLOTS]; // array of pointers to module instances
byte moduleIdReadings[MODULE_SLOTS];

void setup() {
  // initialise module pointers to null
  for(int i=0;i<MODULE_SLOTS;i++) {
    modules[i] = NULL;
  }

  // initialise shift register pins
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(SHIFT_PIN, OUTPUT);
  pinMode(DATA_PIN, INPUT);

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
      bitWrite(moduleIdReadings[j],i,digitalRead(DATA_PIN));
    }
    digitalWrite(CLOCK_PIN, LOW);
    delay(1);
    digitalWrite(CLOCK_PIN, HIGH);
    delay(1);
  }
  Serial.println(moduleIdReadings[0]);
  delay(50);
}
