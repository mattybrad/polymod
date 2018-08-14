#include "Arduino.h"
#include "VCF.h"
#include "Socket.h"

VCF::VCF() {
  float freq = 800;
  float Q = 0.9;
  _filter.frequency(freq);
  _filter.resonance(Q);
  sockets[0].setInput(_filter, 0); // input
  sockets[1].setInput(_filter, 1); // freq CV
  sockets[2].setOutput(_filter, 0); // low pass
  sockets[3].setOutput(_filter, 1); // band pass
  sockets[4].setOutput(_filter, 2); // high pass
}
