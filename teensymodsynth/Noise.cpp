#include "Arduino.h"
#include "Noise.h"
#include "Socket.h"

Noise::Noise() {
  _noise.amplitude(0.4);
  sockets[4].setOutput(_noise, 0);
}

void Noise::update() {
  float vol = ((float) controls[4].value)/1023.0;
  _noise.amplitude(vol * vol);
}

