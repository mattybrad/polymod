#include "Arduino.h"
#include "KeyboardHandler.h"

KeyboardHandler::KeyboardHandler() {
  
}

void KeyboardHandler::setPolyphony(int polyphony) {
  _polyphony = polyphony;
}

void KeyboardHandler::setKey(int keyNum, boolean keyDown) {
  _keyStatus[keyNum] = keyDown;
}

void KeyboardHandler::update() {
  for(int i=0;i<32;i++) {
    if(_prevKeyStatus[i] != _keyStatus[i]) {
      // key has changed

      if(_keyStatus[i]) {
        // note pressed
        
        int bestChannel = -1;
        int backupChannel = -1;
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
          } else {
            // channel not free but check in case it is best backup channel

            if(backupChannel >= 0) {
              if(_channels[j].note < _channels[backupChannel].note) {
                backupChannel = j;
              }
            } else {
              backupChannel = j;
            }
          }
        }
        if(bestChannel >= 0) {
          // free channel found
          _channels[bestChannel].note = i;
          _channels[bestChannel].noteOn = true;
        } else if(backupChannel >= 0) {
          _channels[backupChannel].note = i;
          _channels[backupChannel].noteOn = true;
        } else {
          // can't find channel
        }
        
      } else {
        // note released
        
        for(int j=0;j<_polyphony;j++) {
          if(_channels[j].note == i) {
            _channels[j].noteOn = false;
            _channels[j].lastNoteOff = millis();
          }
        }
      }
        
    }
    _prevKeyStatus[i] = _keyStatus[i];
  }
}

int KeyboardHandler::getNote(int channelIndex) {
  return _channels[channelIndex].note + 24;
}

boolean KeyboardHandler::getGate(int channelIndex) {
  return _channels[channelIndex].noteOn;
}

KeyboardChannel::KeyboardChannel() {
  
}

