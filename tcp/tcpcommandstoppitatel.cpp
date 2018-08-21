#include "tcpcommandstoppitatel.h"

TCPCommandStopPitatel::TCPCommandStopPitatel()
{

}

TCPCommandStopPitatel::TCPCommandStopPitatel(ServerConnect *_server, TCPTimeOutWigget *_widget):
    TCPCommandSet(_server, _widget, {})
{
    command = setStopPitatel;
//    addCommand(new TCPCommand(stoppuw));
}

void TCPCommandStopPitatel::go(TCPCommand *_command)
{
    if(!_command){
        clear();
        addCommand(stoppuw);
        commandSet[0]->send(server);
        addCommand(new TCPTimeOutCommand(timeoutcommand, 4000, 100, getTimeOutWidget(),
                        MSG_TIME_OUT_OFF_PITATEL, MSG_TIME_OUT_OFF_PITAPEL_MSG( 4 )));
    }
    TCPCommandSet::go(_command);
}
