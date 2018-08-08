#include "Arduino.h"
#include "Output.h"
#include "Socket.h"

Output::Output() {
  _patchCable1 = new AudioConnection(_outputSplitter,0,_output,0);
  _patchCable1 = new AudioConnection(_outputSplitter,0,_output,1);
  sockets[0].setInput(_outputSplitter, 0);
}
