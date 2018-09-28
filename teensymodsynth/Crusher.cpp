#include "Arduino.h"
#include "Crusher.h"
#include "Socket.h"

Crusher::Crusher() {
  //_patchCable1 = new AudioConnection(_waveshaper, 0, _bitcrusher, 0); // shaper first, crusher second for now
  
  sockets[4].setInput(_rateQueue, 0); // rate cv
  sockets[2].setInput(_bitsQueue, 0); // bits cv
  sockets[6].setInput(_bitcrusher, 0); // in
  sockets[1].setOutput(_bitcrusher, 0); // out
}

void Crusher::update() {
  // to do: use queues to add cv control
  // to do: make params non linear so the interesting FX fade in apparently linearly
  _bitcrusher.sampleRate(44100*controls[4].getSmoothedValue());
  _bitcrusher.bits(1+15*controls[7].getSmoothedValue());
}
