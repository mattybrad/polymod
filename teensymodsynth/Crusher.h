#ifndef Crusher_h
#define Crusher_h
#include "Arduino.h"
#include "Module.h"
#include <Audio.h>

class Crusher : public Module {
  public:
    Crusher();
    virtual void update();
  private:
    AudioEffectBitcrusher _bitcrusher;
    AudioEffectWaveshaper _waveshaper;
    AudioConnection* _patchCable1;
    AudioRecordQueue _rateQueue;
    AudioRecordQueue _bitsQueue;
};

#endif
