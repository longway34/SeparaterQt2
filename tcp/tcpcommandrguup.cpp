#include "tcpcommandrguup.h"

TCPCommandRGUUp::TCPCommandRGUUp()
{

}

TCPCommandRGUUp::TCPCommandRGUUp(ServerConnect *_server, TCPTimeOutWigget *_widget):
    TCPCommandSet(_server, _widget, {})
{
    QByteArray rguUp = QByteArray::fromRawData("\x01", 1);
//    addCommand(new TCPCommand(getrgu2));
    TCPCommand *crgu = new TCPCommand(setrgu2); crgu->setSendData(rguUp);
    addCommand(crgu);
    addCommand(new TCPTimeOutCommand(timeoutcommand, 1000, 5, widget,
                                      tr("Поднятие РГУ."), tr("Поднятие РГУ. \nПодождите...")));
    addCommand(new TCPCommand(getrgu2));
}

void TCPCommandRGUUp::go(TCPCommand *_command)
{
    if(_command){
          int num = _command->getNum();
          if(num < commandSet.size() - 1){
            commandSet[num+1]->send(server);
            return;
           } else {
            QByteArray res = commandSet[commandSet.size()-1]->getResult();
            if(res.size()>0){
                if(res[0] != 1){
                    commandSet[1]->send(server);
                } else {
                    emit commandComplite(this);
                }
            } else {
                emit commandComplite(this);
            }
        }
    } else {
        if(commandSet.size() > 0){
            commandSet[0]->send(server);
        } else {
            emit commandComplite(this);
        }
    }

}
