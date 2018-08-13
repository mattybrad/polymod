#ifndef Master_h
#define Master_h
#include "Arduino.h"
#include "Module.h"
#include <Audio.h>

class Master : public Module {
  public:
    Master();
    AudioAmplifier& getMainOutput();
  private:
    AudioAmplifier _main;
};

#endif
