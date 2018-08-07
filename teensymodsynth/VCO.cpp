#include "Arduino.h"
#include "VCO.h"
#include "Socket.h"

VCO::VCO() {
  _wave.begin(0.5,440,WAVEFORM_TRIANGLE);
  sockets[0].setOutput(_wave, 0);
}
