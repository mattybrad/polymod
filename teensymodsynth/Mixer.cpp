#include "Arduino.h"
#include "Mixer.h"
#include "Socket.h"

Mixer::Mixer() {
  for(int i=0;i<4;i++) {
    _mixer.gain(i,0.1);
  }
  sockets[0].setInput(_mixer, 0);
  sockets[1].setInput(_mixer, 1);
  sockets[2].setInput(_mixer, 2);
  sockets[3].setInput(_mixer, 3);
  sockets[4].setOutput(_mixer, 0);
}
