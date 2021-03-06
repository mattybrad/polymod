#include "Arduino.h"
#include "VCO.h"
#include "Socket.h"

VCO::VCO() {
  float freq = 87.31;
  _patchCable1 = new AudioConnection(_freqMod, 0, _saw, 0);
  _patchCable2 = new AudioConnection(_freqMod, 0, _square, 0);
  _patchCable3 = new AudioConnection(_freqMod, 0, _triangle, 0);
  _patchCable4 = new AudioConnection(_freqMod, 0, _sine, 0);
  _saw.begin(0.5,freq,WAVEFORM_SAWTOOTH);
  _square.begin(0.5,freq,WAVEFORM_SQUARE);
  _triangle.begin(0.5,freq,WAVEFORM_TRIANGLE);
  _sine.begin(0.5,freq,WAVEFORM_SINE);
  sockets[4].setInput(_freqMod, 0); // main CV for note input
  sockets[2].setInput(_freqMod, 1); // attenuated CV for LFOs etc
  sockets[7].setOutput(_saw, 0);
  sockets[6].setOutput(_square, 0);
  sockets[0].setOutput(_triangle, 0);
  sockets[1].setOutput(_sine, 0);
  _freqMod.gain(0, 1.0);
}

void VCO::update() {
  int rangeCoeff;
  bitWrite(rangeCoeff, map(controls[4].rawValue, 0, 1023, 0, 8), 1);
  float freqCoeff = controls[6].getSmoothedValue();
  float cv = controls[7].getSmoothedValue();
  float finalFreq = 16 * rangeCoeff * (0.5 + 1.5 * freqCoeff);
  _saw.frequency(finalFreq);
  _square.frequency(finalFreq);
  _triangle.frequency(finalFreq);
  _sine.frequency(finalFreq);
  _freqMod.gain(1, cv);
}
