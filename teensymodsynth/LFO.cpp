#include "Arduino.h"
#include "LFO.h"
#include "Socket.h"

LFO::LFO() {
  float freq = 1;
  _saw.begin(0.5,freq,WAVEFORM_SAWTOOTH);
  _inverseSaw.begin(0.5,freq,WAVEFORM_SAWTOOTH_REVERSE);
  _sine.begin(0.5,freq,WAVEFORM_SINE);
  _triangle.begin(0.5,freq,WAVEFORM_TRIANGLE);
  _square.begin(0.5,freq,WAVEFORM_SQUARE);
  sockets[0].setOutput(_saw, 0);
  sockets[1].setOutput(_inverseSaw, 0);
  sockets[2].setOutput(_sine, 0);
  sockets[3].setOutput(_triangle, 0);
  sockets[4].setOutput(_square, 0);
}
