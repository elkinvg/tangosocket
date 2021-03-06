#ifndef TANGOSOCKET_H
#define TANGOSOCKET_H

#include <tango.h>

class TangoSocket
{
public:
    TangoSocket(std::string socket);
    ~TangoSocket();
    bool checkSocketState();
    std::string toSocketWriteAndRead(std::string command); // send command to socket
    std::string toSocketWriteAndReadBinary(std::string command);
    void toSocketWrite(std::string command);
    std::string fromSockedReadBinary();
	void reconnect();

private:
    Tango::DeviceProxy *socketProxy;
    bool isSocketOn;
};

#endif // TANGOSOCKET_H
