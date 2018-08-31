#include "Arduino.h"
#include "Master.h"

Master::Master() {
  note = 20;
  gate = false;
  sockets[4].setInput(_main, 0);
  sockets[1].setOutput(_keyboardCV, 0);
  sockets[2].setOutput(_keyboardGate, 0);
}

AudioAmplifier& Master::getMainOutput() {
  return _main;
}

void Master::update() {
  _keyboardGate.amplitude(gate?1:0);
  _keyboardCV.amplitude(((float) (note-20)) * 1/8/12);
  float vol = controls[0].getSmoothedValue();
  _main.gain(vol*vol);
}

