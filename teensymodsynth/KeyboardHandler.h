#ifndef KeyboardHandler_h
#define KeyboardHandler_h
#include "Arduino.h"

class KeyboardChannel {
  public:
    KeyboardChannel();
    int note = -12;
    bool noteOn = false;
    int lastNoteOn = 0;
    int lastNoteOff = 0;
  private:
};

class KeyboardHandler {
  public:
    KeyboardHandler();
    void update();
    int getNote(int channelIndex);
    boolean getGate(int channelIndex);
  private:
    boolean _keyStatus[40];
    KeyboardChannel _channels[4];
};

#endif
