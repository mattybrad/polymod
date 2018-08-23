#include "Arduino.h"
#include "Envelope.h"
#include "Socket.h"

Envelope::Envelope() {
  _dc.amplitude(1.0);
  _envelope.attack(2000);
  _envelope.release(3000);
  _patchCable1 = new AudioConnection(_dc,0,_envelope,0);
  _queue.begin();
  sockets[0].setInput(_queue, 0); // gate
  sockets[1].setOutput(_envelope, 0); // audio out
}

void Envelope::update() {
  if(_queue.available() > 0) {
    int16_t test[128];
    memcpy(test, _queue.readBuffer(), 256);
    _queue.freeBuffer();
    int16_t x = test[0];
    boolean newGateReading = (x > 0);
    if(newGateReading != _gateOpen) {
      _gateOpen = newGateReading;
      if(_gateOpen) _envelope.noteOn();
      else _envelope.noteOff();
    }
    _queue.clear();
  }
}
