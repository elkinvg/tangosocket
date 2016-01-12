#ifndef TANGOSOCKET_H
#define TANGOSOCKET_H

#include <tango.h>

class TangoSocket
{
public:
    TangoSocket(std::string socket);
    ~TangoSocket();
    bool checkSocketState();
    std::string toSocketWriteAndRead(std::string command, bool isBinary = true); // send command to socket

private:
    Tango::DeviceProxy *socketProxy;
    bool isSocketOn;
};

#endif // TANGOSOCKET_H
