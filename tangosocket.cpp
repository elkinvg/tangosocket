#include "tangosocket.h"

TangoSocket::TangoSocket(std::string socket)
{
    socketProxy = new Tango::DeviceProxy(socket);

    try {

        socketProxy->command_inout("Init");
        checkSocketState();

    } catch (Tango::DevFailed &e) {
        Tango::Except::print_exception(e);
    }
}

TangoSocket::~TangoSocket()
{
    delete socketProxy;
}

bool TangoSocket::checkSocketState()
{
    Tango::DevState stateSocket;
    Tango::DeviceData outputCom;

    try {
        // ??? command_inout("State") or other command for exception
        outputCom = socketProxy->command_inout("State");
        outputCom >> stateSocket;
        if (stateSocket == Tango::ON) {
            isSocketOn = true;
        }
        else if (stateSocket == Tango::OFF || stateSocket == Tango::FAULT)
        {
            isSocketOn = false;
        }
        return isSocketOn;
    } catch (Tango::DevFailed &e) {
        Tango::Except::print_exception(e);
        isSocketOn = false;
        return isSocketOn;
    }
}

std::string TangoSocket::toSocketWriteAndRead(std::string command)
{
    Tango::DeviceData input, output;
    std::string reply;

    try {
        input << command;
        output = socketProxy->command_inout("WriteAndRead",input);

        output >> reply;
    } catch (Tango::DevFailed &e) {
        Tango::Except::print_exception(e);
        reply = "?";
    }
    return reply;
}
