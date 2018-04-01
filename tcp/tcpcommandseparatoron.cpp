#include "tcpcommandseparatoron.h"

void TCPCommandSeparatorOn::setModel(SPRSettingsRentgenModel *value)
{
    model = value;
}

TCPCommandSeparatorOn::TCPCommandSeparatorOn()
{

}

TCPCommandSeparatorOn::TCPCommandSeparatorOn(ServerConnect *_server, TCPTimeOutWigget *_widget, SPRSettingsRentgenModel *_model)
    :TCPCommandSet(_server, _widget, {}), model(_model)
{
    addCommand(new TCPCommand(initada))->
    addCommand(new TCPCommand(onsep))->
    addCommand(new TCPCommand(onosw));
}

void TCPCommandSeparatorOn::go(TCPCommand *_command)
{
    if(!_command){
        commandSet[0]->send(server);
        return;
    } else {
        if(_command->getNum() >= commandSet.size() - 1){
            emit commandComplite(this);
            return;
        }
        commandSet[_command->getNum() + 1]->send(server);
        return;
    }
}
