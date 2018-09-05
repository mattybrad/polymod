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
    KeyboardHandler(int polyphony);
    void update();
    void setKey(int keyNum, boolean keyDown);
    int getNote(int channelIndex);
    boolean getGate(int channelIndex);
  private:
    boolean _keyStatus[32];
    boolean _prevKeyStatus[32];
    KeyboardChannel _channels[10]; // 10 channels just to be safe, unlikely to ever get that much polyphony
    int _polyphony;
};

#endif
