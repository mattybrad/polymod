#include "Arduino.h"
#include "Socket.h"

#define OUTPUT_SOCKET 0
#define INPUT_SOCKET 1

Socket::Socket() {
  _stream = NULL;
}

void Socket::setOutput(AudioStream &stream, int connectionIndex) {
  _socketType = OUTPUT_SOCKET;
  _stream = &stream;
  _connectionIndex = connectionIndex;
}

void Socket::setInput(AudioStream& stream, int connectionIndex) {
  _socketType = INPUT_SOCKET;
  _stream = &stream;
  _connectionIndex = connectionIndex;
}

boolean Socket::isOutput() {
  return _socketType == OUTPUT_SOCKET;
}

AudioStream* Socket::getStream() {
  return _stream;
}

int Socket::getConnectionIndex() {
  return _connectionIndex;
}

