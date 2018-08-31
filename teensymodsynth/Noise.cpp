#include "Arduino.h"
#include "Noise.h"
#include "Socket.h"

Noise::Noise() {
  _noise.amplitude(0.4);
  sockets[4].setOutput(_noise, 0);
}

void Noise::update() {
  float vol = controls[4].getSmoothedValue();
  _noise.amplitude(vol * vol);
}

