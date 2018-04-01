#include "tcpcommandstartpitatel.h"
#include <QMessageBox>


TCPCommandStartPitatel::TCPCommandStartPitatel()
{
}

TCPCommandStartPitatel::TCPCommandStartPitatel(ServerConnect *_server, TCPTimeOutWigget *_widget):
    TCPCommandSet(_server, _widget, {})
{
    addCommand(new TCPCommand(getrgu2))->addCommand(new TCPCommand(setpuw))->addCommand(new TCPCommand(startpuw));
    percents = 44;
}

void TCPCommandStartPitatel::setPercents(uint _persents){
    percents = _persents;
}

void TCPCommandStartPitatel::go(TCPCommand *_command)
{
    if(!_command){
       commandSet[0]->send(server);
       return;
    }
    int num = _command->getNum();
    if(num == 0){
        QByteArray res = findCommands(getrgu2).last()->getResult();
        if(res[0] != 1){
            QMessageBox::information(nullptr, tr("Ошибка"), tr("РГУ не вверхнем положении"));
            return;
        }
        uint16_t fq = percents * 20;
        QByteArray ba = QByteArray::fromRawData((char*)(&fq), sizeof(uint16_t));
        findCommands(setpuw).last()->setSendData(ba);
        commandSet[1]->send(server);
    } else if(num == 1){
        commandSet[2]->send(server);
        return;
    } else if(num == 2){
        if(commandSet[num]->getErrors() == 0){
            server->addState(spr_state_pitatel_on);
        }
        emit commandComplite(this);
    }
}
