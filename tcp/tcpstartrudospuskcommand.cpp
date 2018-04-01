#include "tcpstartrudospuskcommand.h"

TCPStartRudospuskCommand::TCPStartRudospuskCommand()
{

}

TCPStartRudospuskCommand::TCPStartRudospuskCommand(ServerConnect *_server, TCPTimeOutWigget *_widget)
    :TCPCommandSet(_server, _widget, {})
{
    addCommand(new TCPCommand(oniw));
    addCommand(new TCPCommandStartPitatel(_server, _widget));
}

void TCPStartRudospuskCommand::go(TCPCommand *_command)
{
    if(!_command){
        commandSet[0]->send(server);
    } else {
        int num = _command->getNum();
        if(num == 0){
            if(_command->noErrors()){
               commandSet[1]->send(server);
               return;
            } else {
               emit commandComplite(_command);
               return;
            }
        } else {
            emit commandComplite(this);
            return;
        }
    }
}
