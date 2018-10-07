#include "Arduino.h"
#include "Noise.h"
#include "Socket.h"

Noise::Noise() {
  _noise.amplitude(1.0);
  _sampleHold.begin(1.0,1.0,WAVEFORM_SAMPLE_HOLD);
  sockets[4].setOutput(_noise, 0);
  sockets[6].setOutput(_sampleHold, 0);
}

void Noise::update() {
  float f = controls[4].getSmoothedValue();
  _sampleHold.frequency(0.01+f*f*200);
}

