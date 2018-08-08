#include "Arduino.h"
#include "Envelope.h"
#include "Socket.h"

Envelope::Envelope() {
  _envelope.attack(500);
  _envelope.release(3000);
  sockets[0].setInput(_envelope, 0);
  sockets[1].setOutput(_envelope, 0);
}
