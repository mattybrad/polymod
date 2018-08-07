#include "Arduino.h"
#include "VCO.h"
#include "Socket.h"

VCO::VCO() {
  _wave.begin(0.5,440,WAVEFORM_TRIANGLE);
  sockets[1].setOutput(_wave, 0);
  sockets[0].setInput(_output, 0);
  //_tempConnection = new AudioConnection(_wave,0,_output,0);
}
