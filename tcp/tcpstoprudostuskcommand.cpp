#include "tcpstoprudostuskcommand.h"

TCPStopRudostuskCommand::TCPStopRudostuskCommand()
{

}

TCPStopRudostuskCommand::TCPStopRudostuskCommand(ServerConnect *_server, TCPTimeOutWigget *_widget)
    :TCPCommandSet(_server, _widget, {})
{
    addCommand(new TCPCommandStopPitatel(_server, _widget));
    addCommand(new TCPCommand(offiw));
}

void TCPStopRudostuskCommand::go(TCPCommand *_command)
{
    if(!_command){
        commandSet[0]->send(server);
        return;
    } else if(_command->getNum() == 0){
        if(_command->noErrors()){
            commandSet[1]->send(server);
        } else {
            emit commandComplite(_command);
        }
        return;
    } else {
        emit commandComplite(this);
        return;
    }
}
