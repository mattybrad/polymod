#ifndef PatchCable_h
#define PatchCable_h
#include "Arduino.h"
#include "Socket.h"
#include <Audio.h>
#include <AudioStream.h>

class PatchCable {
  public:
    PatchCable(int highSocketIndex, int lowSocketIndex, Socket& highSocket, Socket& lowSocket);
    void disconnect();
    boolean isValid();
    int getHighSocket();
    int getLowSocket();
  private:
    boolean _valid;
    int _highSocketIndex;
    int _lowSocketIndex;
    Socket* _highSocket;
    Socket* _lowSocket;
    Socket* _sourceSocket;
    Socket* _destSocket;
    AudioStream* _sourceStream;
    AudioStream* _destStream;
    AudioConnection* _audioConnection;
};

#endif
