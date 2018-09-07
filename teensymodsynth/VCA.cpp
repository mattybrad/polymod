#include "Arduino.h"
#include "VCA.h"
#include "Socket.h"

VCA::VCA() {
  sockets[4].setInput(_cvMixer, 0);
  sockets[2].setInput(_cvMixer, 1);
  sockets[6].setInput(_inputMixer, 0);
  sockets[1].setInput(_inputMixer, 1);
  sockets[7].setOutput(_multiplier, 0);
  sockets[0].setOutput(_multiplier, 0);
  _patchCable1 = new AudioConnection(_inputMixer,0,_multiplier,0);
  _patchCable2 = new AudioConnection(_cvMixer,0,_multiplier,1);
}

void VCA::update() {
  _cvMixer.gain(1,controls[4].getSmoothedValue());
  float crossfade = controls[6].getSmoothedValue();
  _inputMixer.gain(0,constrain(2-2*crossfade,0,1));
  _inputMixer.gain(1,constrain(2*crossfade,0,1));
}

