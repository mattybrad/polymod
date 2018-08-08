#include "Arduino.h"
#include "VCA.h"
#include "Socket.h"

VCA::VCA() {
  sockets[0].setInput(_vca, 0);
  sockets[1].setInput(_vca, 1);
  sockets[2].setOutput(_vca, 0);
}
