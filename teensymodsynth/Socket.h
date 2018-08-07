#ifndef Socket_h
#define Socket_h
#include "Arduino.h"
#include <AudioStream.h>

class Socket {
  public:
    Socket();
    void setOutput(AudioStream& stream, int connectionIndex);
    void setInput(AudioStream& stream, int connectionIndex);
    boolean isOutput();
    AudioStream *getStream();
    int getConnectionIndex();
    
  private:
    AudioStream* _stream;
    int _connectionIndex;
    int _socketType;
};

#endif
