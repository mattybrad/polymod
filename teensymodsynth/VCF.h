#ifndef VCF_h
#define VCF_h
#include "Arduino.h"
#include "Module.h"
#include <Audio.h>

class VCF : public Module {
  public:
    VCF();
    virtual void update();
  private:
    AudioFilterStateVariable _filter;
    AudioMixer4 _cvMixer;
    AudioConnection* _patchCable1;
};

#endif
