#ifndef Output_h
#define Output_h
#include "Arduino.h"
#include "Module.h"
#include <Audio.h>

class Output : public Module {
  public:
    Output(AudioOutputI2S* out);
  private:
    AudioAmplifier _outputSplitter;
    AudioConnection* _patchCable1;
    AudioConnection* _patchCable2;
};

#endif
