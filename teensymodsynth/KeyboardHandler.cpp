#include "Arduino.h"
#include "KeyboardHandler.h"

const int KEYBOARD_PINS[] = {6,7,10,12,14}; // temporary pins for reading notes
const int NUM_KEYS = 5;

KeyboardHandler::KeyboardHandler(int polyphony) {
  _polyphony = polyphony;
}

void KeyboardHandler::update() {
  for(int i=0;i<NUM_KEYS;i++) {
    boolean keyDown = !digitalRead(KEYBOARD_PINS[i]);
    if(_keyStatus[i] != keyDown) {
      // key has changed

      if(keyDown) {
        // note pressed
        
        int bestChannel = -1;
        for(int j=0;j<_polyphony;j++) {
          if(!_channels[j].noteOn) {
            // channel is free
            
            if(bestChannel >= 0) {
              if(_channels[j].lastNoteOff < _channels[bestChannel].lastNoteOff) {
                bestChannel = j;
              }
            } else {
              bestChannel = j;
            }
          }
        }
        if(bestChannel >= 0) {
          // free channel found
          _keyStatus[i] = true; // only set this to true here to allow note to trigger in future when channels become available, or something
          _channels[bestChannel].note = i;
          _channels[bestChannel].noteOn = true;
        }
        
      } else {
        // note released
        _keyStatus[i] = false;
        
        for(int j=0;j<_polyphony;j++) {
          if(_channels[j].note == i) {
            _channels[j].noteOn = false;
            _channels[j].lastNoteOff = millis();
          }
        }
      }
        
    }
  }
}

int KeyboardHandler::getNote(int channelIndex) {
  return _channels[channelIndex].note * 2 + 24;
}

boolean KeyboardHandler::getGate(int channelIndex) {
  return _channels[channelIndex].noteOn;
}

KeyboardChannel::KeyboardChannel() {
  
}

