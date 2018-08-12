#include "Arduino.h"
#include "Output.h"
#include "Socket.h"

Output::Output(AudioOutputI2S* out) {
  _patchCable1 = new AudioConnection(_outputSplitter,0,*out,0);
  _patchCable1 = new AudioConnection(_outputSplitter,0,*out,1);
  sockets[0].setInput(_outputSplitter, 0);
}
