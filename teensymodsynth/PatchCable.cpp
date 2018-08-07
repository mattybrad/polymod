#include "Arduino.h"
#include "PatchCable.h"

PatchCable::PatchCable(int highSocketIndex, int lowSocketIndex, Socket& highSocket, Socket& lowSocket) {
  if(highSocket.isOutput()!=lowSocket.isOutput()) {
    _valid = true;
    if(highSocket.isOutput()) {
      _sourceSocket = &highSocket;
      _destSocket = &lowSocket;
    } else {
      _sourceSocket = &lowSocket;
      _destSocket = &highSocket;
    }
  } else {
    _valid = false;
  }
  _highSocket = &highSocket;
  _lowSocket = &lowSocket;
  _highSocketIndex = highSocketIndex;
  _lowSocketIndex = lowSocketIndex;
  if(_valid) {
    _sourceStream = _sourceSocket->getStream();
    _destStream = _destSocket->getStream();
    _audioConnection = new AudioConnection(*_sourceStream, (unsigned char) _sourceSocket->getConnectionIndex(), *_destStream, (unsigned char) _destSocket->getConnectionIndex());
  } else {
    // not valid, no audio connection possible
  }
}

int PatchCable::getHighSocket() {
  return _highSocketIndex;
}

int PatchCable::getLowSocket() {
  return _lowSocketIndex;
}

void PatchCable::disconnect() {
  if(_valid) _audioConnection->disconnect();
}

