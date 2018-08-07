#include "Arduino.h"
#include "VCO.h"
#include "Socket.h"

VCO::VCO() {
  _saw.begin(0.5,220,WAVEFORM_SAWTOOTH);
  _square.begin(0.5,270,WAVEFORM_SQUARE);
  _triangle.begin(0.5,330,WAVEFORM_TRIANGLE);
  _sine.begin(0.5,440,WAVEFORM_SINE);
  sockets[0].setOutput(_saw, 0);
  sockets[1].setOutput(_square, 0);
  sockets[2].setOutput(_triangle, 0);
  sockets[3].setOutput(_sine, 0);
}
