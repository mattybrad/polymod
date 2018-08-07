#include "Arduino.h"
#include "Output.h"
#include "Socket.h"

Output::Output() {
  sockets[0].setInput(_output, 0);
}
