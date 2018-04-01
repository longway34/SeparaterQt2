#include "tcpcommandrguupdown.h"

void TCPCommandRGUUpDown::setFinish(bool value)
{
    finish = value;
}

bool TCPCommandRGUUpDown::getFinish() const
{
    return finish;
}

TCPCommandRGUUpDown::TCPCommandRGUUpDown()
{

}
TCPCommandRGUUpDown::TCPCommandRGUUpDown(ServerConnect *_server, TCPTimeOutWigget *_widget, bool _upDown):
    TCPCommandSet(_server, _widget, {}), upDown(_upDown)
{
    QByteArray rguUpDown;
    if(upDown){
        rguUpDown.append(QByteArray::fromRawData("\x01", 1));
        result = 1;
    } else {
        rguUpDown.append(QByteArray::fromRawData("\x02", 1));
        result = 2;
    }
//    addCommand(new TCPCommand(getrgu2));
    TCPCommand *crgu = new TCPCommand(setrgu2); crgu->setSendData(rguUpDown);
    addCommand(crgu); // 0
    if(upDown){
        addCommand(new TCPTimeOutCommand(timeoutcommand, 1000, 5, widget,
                                      tr("Поднятие РГУ."), tr("Поднятие РГУ. \nПодождите..."))); // 1
    } else {
        addCommand(new TCPTimeOutCommand(timeoutcommand, 1000, 5, widget,
                                      tr("Опускание РГУ."), tr("Опускание РГУ. \nПодождите..."))); // 1
    }
    addCommand(new TCPCommand(getstate)); // 2
    addCommand(new TCPCommand(getrgu2)); // 3
    QByteArray rgu0 = QByteArray::fromRawData("\x00", 1);
    TCPCommand *command0 = new TCPCommand(setrgu2); command0->setSendData(rgu0);
    addCommand(command0); // 4

}

void TCPCommandRGUUpDown::go(TCPCommand *_command)
{
    if(_command){
          if(getFinish()){
              setFinish(false);
              return;
          }
          int num = _command->getNum();

          if(num == 3){ // getrgu
             QByteArray res = commandSet[3]->getResult();
             if(res.size() > 0){
                 if(res[0] == result){ // complite command
                    emit commandComplite(this);
                     return;
                 } else {
                    commandSet[1]->send(server); // wait more...
                    return;
                 }
             } else {
                 emit commandComplite(this);
                 return;
             }
         } else if(num >= commandSet.size()-1){ // last command
             emit commandComplite(this);
              return;
         } else {
             commandSet[num +1]->send(server);
             return;
         }
    } else { // begin command need
        setFinish(false);
        commandSet[0]->send(server);
        return;
    }

}
