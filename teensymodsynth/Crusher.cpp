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
  
  float f = controls[4].getSmoothedValue();
  float b = controls[7].getSmoothedValue();
  _bitcrusher.sampleRate(44100*f*f*f);
  _bitcrusher.bits(1+15*b*b);
}
