#ifndef VCF_h
#define VCF_h
#include "Arduino.h"
#include "Module.h"
#include <Audio.h>

class VCF : public Module {
  public:
    VCF();
  private:
    AudioFilterStateVariable _filter;
};

#endif
