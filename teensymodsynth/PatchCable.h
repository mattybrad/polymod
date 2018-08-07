#ifndef PatchCable_h
#define PatchCable_h
#include "Arduino.h"
#include "Socket.h"
#include <Audio.h>
#include <AudioStream.h>

class PatchCable {
  public:
    PatchCable(Socket& highSocket, Socket& lowSocket);
    void disconnect();
    boolean isValid();
    int getHighSocket();
    int getLowSocket();
  private:
    boolean _valid;
    Socket* _highSocket;
    Socket* _lowSocket;
    Socket* _sourceSocket;
    Socket* _destSocket;
    AudioStream* _sourceStream;
    AudioStream* _destStream;
    AudioConnection* _audioConnection;
};

#endif
