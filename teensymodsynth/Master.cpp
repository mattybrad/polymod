#include "Arduino.h"
#include "Master.h"

Master::Master() {
  sockets[0].setInput(_main, 0);
}

AudioAmplifier& Master::getMainOutput() {
  return _main;
}

