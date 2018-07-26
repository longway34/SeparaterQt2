#include "tcpcommandrguupdown2.h"

TCPCommandRGUUpDown2::TCPCommandRGUUpDown2(ServerConnect *_server, TCPTimeOutWigget *_towidget, bool _up_down)
    :TCPCommandSet(_server, _towidget, {}), up_down(_up_down)
{
    command = setRGUUpDown;
    if(up_down){
        result = 1;
        sendData = 1;
    } else {
        result = 2;
        sendData = 2;
    }
}


void TCPCommandRGUUpDown2::go(TCPCommand *_command)
{
    if(!_command){
        clear();
        addCommand(getstate);
        addCommand(getrgu2);

        char stop = '\0';
        addCommand(setrgu2);
        findCommands(setrgu2).last()->setSendData(&sendData, sizeof(sendData));
        if(up_down){
            addCommand(new TCPTimeOutCommand(timeoutcommand, 1000, 1, getTimeOutWidget(), tr("Движение РГУ"), tr("Поднятие РГУ. Подождите...")));
        } else {
            addCommand(new TCPTimeOutCommand(timeoutcommand, 1000, 1, getTimeOutWidget(), tr("Движение РГУ"), tr("Поднятие РГУ. Подождите...")));
        }
        addCommand(getrgu2);

        addCommand(setrgu2);
        findCommands(setrgu2).last()->setSendData(&stop, 1);
    } else {
        if(_command->getCommand() == getrgu2){
            uint8_t res;
            memcpy(&res, _command->getReplayData().constData(), 1);
            if(res == result){
                findCommands(setrgu2).last()->send(server);
                return;
            } else {
                if(_command == findCommands(getrgu2).first()){
                    findCommands(setrgu2).first(),send(server);
                    return;
                } else {
                    findCommands(timeoutcommand).first()->send(server);
                    return;
                }
            }
        }
    }
    TCPCommandSet::go(_command);
}
