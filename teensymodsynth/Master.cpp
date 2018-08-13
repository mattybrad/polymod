#include "Arduino.h"
#include "Master.h"

Master::Master() {
  note = 20;
  sockets[0].setInput(_main, 0);
  sockets[1].setOutput(_keyboardCV, 0);
  sockets[2].setOutput(_keyboardGate, 0);
}

AudioAmplifier& Master::getMainOutput() {
  return _main;
}

void Master::update() {
  _keyboardCV.amplitude(((float) (note-20)) * 1/8/12);
}

