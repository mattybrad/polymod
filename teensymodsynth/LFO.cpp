#include "Arduino.h"
#include "LFO.h"
#include "Socket.h"

LFO::LFO() {
  float freq = 0.3;
  _patchCable1 = new AudioConnection(_freqMod, 0, _saw, 0);
  _patchCable2 = new AudioConnection(_freqMod, 0, _square, 0);
  _patchCable3 = new AudioConnection(_freqMod, 0, _triangle, 0);
  _patchCable4 = new AudioConnection(_freqMod, 0, _sine, 0);
  _patchCable5 = new AudioConnection(_freqMod, 0, _inverseSaw, 0);
  _saw.begin(1.0,freq,WAVEFORM_SAWTOOTH);
  _inverseSaw.begin(1.0,freq,WAVEFORM_SAMPLE_HOLD);
  _sine.begin(1.0,freq,WAVEFORM_SINE);
  _triangle.begin(1.0,freq,WAVEFORM_TRIANGLE);
  _square.begin(1.0,freq,WAVEFORM_SQUARE);
  sockets[2].setOutput(_saw, 0);
  sockets[6].setOutput(_inverseSaw, 0);
  sockets[1].setOutput(_sine, 0);
  sockets[7].setOutput(_triangle, 0);
  sockets[0].setOutput(_square, 0);
  sockets[4].setInput(_freqMod, 0);
}

void LFO::update() {
  float cv = controls[6].getSmoothedValue();
  float freq = 0.1 + 200 * controls[4].getSmoothedValue();
  _saw.frequency(freq);
  _inverseSaw.frequency(freq);
  _square.frequency(freq);
  _triangle.frequency(freq);
  _sine.frequency(freq);
  _freqMod.gain(cv);
}
