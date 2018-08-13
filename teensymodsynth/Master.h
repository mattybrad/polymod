#ifndef Master_h
#define Master_h
#include "Arduino.h"
#include "Module.h"
#include <Audio.h>

class Master : public Module {
  public:
    Master();
    AudioAmplifier& getMainOutput();
    int note;
    virtual void update();
  private:
    AudioAmplifier _main;
    AudioSynthWaveformDc _keyboardCV;
    AudioSynthWaveformDc _keyboardGate;
};

#endif
