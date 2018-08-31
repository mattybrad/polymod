#include "Arduino.h"
#include "VCF.h"
#include "Socket.h"

VCF::VCF() {
  float freq = 1300;
  float Q = 2;
  _filter.frequency(freq);
  _filter.resonance(Q);
  _filter.octaveControl(2);
  
  sockets[0].setInput(_filter, 0); // input
  sockets[1].setInput(_filter, 1); // freq CV
  sockets[2].setOutput(_filter, 0); // low pass
  sockets[3].setOutput(_filter, 1); // band pass
  sockets[4].setOutput(_filter, 2); // high pass
}

void VCF::update() {
  float freqVal = controls[0].getSmoothedValue();
  _filter.frequency(freqVal*freqVal*4000);
}

