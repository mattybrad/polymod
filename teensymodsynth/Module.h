#ifndef Module_h
#define Module_h
#include "Arduino.h"
#include "Socket.h"
#include "Control.h"

class Module {
  public:
    Module(int moduleType);
    Socket sockets[8];
    Control controls[8];
  private:
    
};

#endif
