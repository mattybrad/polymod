#ifndef Envelope_h
#define Envelope_h
#include "Arduino.h"
#include "Module.h"
#include <Audio.h>

class Envelope : public Module {
  public:
    Envelope();
    virtual void update();
  private:
    boolean _gateOpen = false;
    AudioEffectEnvelope _envelope;
    AudioSynthWaveformDc _dc;
    AudioConnection* _patchCable1;
    AudioRecordQueue _queue;
};

#endif
