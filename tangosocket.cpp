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

std::string TangoSocket::toSocketWriteAndReadBinary(std::string command)
{
    Tango::DeviceData input, output;
    Tango::DevVarCharArray* in;
    const Tango::DevVarCharArray* out;
    std::string reply;
    int tmpSz = command.size();

    try {
        in = new Tango::DevVarCharArray();
        in->length(tmpSz);

        for (int i = 0; i < tmpSz; i++)
        {
            (*in)[i] = command[i];
        }

        input<< in;
        output = socketProxy->command_inout("WriteAndReadBinary",input);

        output >> out;
        tmpSz = out->length();

        for (int i = 0; i < tmpSz; i++)
        {
             reply.push_back((*out)[i]);
        }
    } catch (Tango::DevFailed &e) {
        Tango::Except::print_exception(e);
        reply = "?";
    }
    return reply;
}

void TangoSocket::toSocketWrite(std::string command)
{
    Tango::DeviceData input;

    try {
        input << command;

        socketProxy->command_inout("Write",input);

    } catch (Tango::DevFailed &e) {
        Tango::Except::print_exception(e);
    }
}

std::string TangoSocket::fromSockedReadBinary()
{
    Tango::DeviceData outNum;
    Tango::DeviceData output;
    const Tango::DevVarCharArray* out;
    Tango::DevLong outSize;
    std::string reply = "";
    try {
        outNum = socketProxy->command_inout("AvalaibleBytes");
        outNum >> outSize;

        if (outSize > 0)
        {
            output = socketProxy->command_inout("ReadBinary");
            outNum >> out;
            int size = out->length();

            for (int i = 0; i < size; i++)
            {
                 reply.push_back((*out)[i]);
            }
        }
    }   catch (Tango::DevFailed &e) {
        Tango::Except::print_exception(e);
        reply = "?";
    }
    return reply;
}
