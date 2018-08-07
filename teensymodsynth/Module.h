#ifndef Module_h
#define Module_h
#include "Arduino.h"
#include "Socket.h"
#include "Control.h"

class Module {
  public:
    Module();
    Socket sockets[8];
    Control controls[8];
    int getID();
    Socket& getSocket(int moduleSocketNumber);
  private:
    int _id;
    
};

#endif
