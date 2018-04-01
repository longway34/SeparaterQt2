#include "tcpcommandstoppitatel.h"

TCPCommandStopPitatel::TCPCommandStopPitatel()
{

}

TCPCommandStopPitatel::TCPCommandStopPitatel(ServerConnect *_server, TCPTimeOutWigget *_widget):
    TCPCommandSet(_server, _widget, {})
{
    addCommand(new TCPCommand(stoppuw));
}

void TCPCommandStopPitatel::go(TCPCommand *_command)
{
    if(!_command){
        commandSet[0]->send(server);
        return;
    } else {
        if(_command->noErrors()){
            server->clearState(spr_state_pitatel_on);
        }
        emit commandComplite(this);
        return;
    }
    return;
}
