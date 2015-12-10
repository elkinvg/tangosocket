#include "tangosocket.h"

TangoSocket::TangoSocket(std::string socket)
{
    socketProxy = new Tango::DeviceProxy(socket);

    try {

        socketProxy->command_inout("Init");
        checkSocketState();

    } catch (Tango::DevFailed &e) {
//        DEBUG_STREAM << "Can't connect to socket " << socket << endl;
        Tango::Except::print_exception(e);

//        set_state(Tango::FAULT);
//        set_status("Can't connect to socket " + socket);
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
//            DEBUG_STREAM << " Socket " << socket << " is ON" << endl;
            isSocketOn = true;
//            set_state(Tango::ON);
//            set_status("Device is ON");
        }
        else if (stateSocket == Tango::OFF || stateSocket == Tango::FAULT)
        {
//            DEBUG_STREAM << " Socket " << socket << " is OFF" << endl;
            isSocketOn = false;
//            set_state(Tango::OFF);
//            set_status("Device is OFF or Socket is FAULT");
        }
        return isSocketOn;
    } catch (Tango::DevFailed &e) {
        Tango::Except::print_exception(e);
//        set_state(Tango::FAULT);
//        set_status("Can't connect to socket " + socket);
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
