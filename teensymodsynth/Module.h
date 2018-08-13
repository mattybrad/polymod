#ifndef Module_h
#define Module_h
#include "Arduino.h"
#include "Socket.h"
#include "Control.h"
#include <Audio.h>

class Module {
  public:
    Module();
    Socket sockets[8];
    Control controls[8];
    int getID();
    Socket& getSocket(int moduleSocketNumber);
    void removePatchCable(int moduleSocketNumber);
    virtual void update() {};
    virtual AudioAmplifier& getMainOutput() {};
  private:
    int _id;
    
};

#endif
