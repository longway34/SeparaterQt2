#include "tcpcommandrguupdown2.h"

bool TCPCommandRGUUpDown2::getStarted() const
{
    return started;
}

void TCPCommandRGUUpDown2::setStarted(bool value)
{
    started = value;
}

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
        findCommands(setrgu2).last()->addSendData(&sendData, sizeof(sendData));

        addCommand(new TCPTimeOutCommand(timeoutcommand, 200, 1));
        addCommand(getstate);
        if(up_down){
            addCommand(new TCPTimeOutCommand(timeoutcommand, 1000, 1, getTimeOutWidget(), tr("Движение РГУ"), tr("Поднятие РГУ. Подождите...")));
        } else {
            addCommand(new TCPTimeOutCommand(timeoutcommand, 1000, 1, getTimeOutWidget(), tr("Движение РГУ"), tr("Опускание РГУ. Подождите...")));
        }
        addCommand(getrgu2);

        addCommand(setrgu2);
        findCommands(setrgu2).last()->addSendData(&stop, 1);
        started = false;
    } else {
        if(_command->getCommand() == getrgu2){
            uint8_t res;
            memcpy(&res, &_command->getReplayData().constData()[1], 1);
            if(res == result){
                started = false;
                findCommands(setrgu2).last()->send(server);
                return;
            } else {
                if(started || _command == findCommands(getrgu2).first()){
                    if(_command == findCommands(getrgu2).first()){
                        TCPCommand *set = findCommands(setrgu2).first();
                        if(set){
                            set->send(server);
                            started = true;
                        }
                        return;
                    } else {
                        findCommands(getstate).last()->send(server);
                        return;
                    }
                } else {
                    if(res != result){
                        emit commandNotComplite(this);
                        return;
                    } else {
                        findCommands(setrgu2).last()->send(server);
                        return;
                    }
                }
            }
        }
    }
    TCPCommandSet::go(_command);
}
