#include "tcpstartsoptestimscommand.h"

TCPCommand *TCPStartSopTestIMSCommand::getStopTestCommand() const
{
    return stopTestCommand;
}

TCPStartSopTestIMSCommand::TCPStartSopTestIMSCommand()
{
    setParams({}, 40, 1, 1);
}

TCPStartSopTestIMSCommand::TCPStartSopTestIMSCommand(ServerConnect *_server, TCPTimeOutWigget *_widget)
    :TCPCommandSet(_server, _widget, {})
{
    setParams({}, 40, 1, 1);
    addCommand(new TCPCommand(nocommand)); // 0
    addCommand(new TCPCommand(testim)); // 1
    addCommand(new TCPTimeOutCommand(timeoutcommand, timeWork, 10, widget, tr("Тест исполнительных механизмов"))); // 2
    addCommand(new TCPCommand(getsepar)); // 3
    addCommand(new TCPCommand(stoptest)); // 4

    stopTestCommand = new TCPCommand(stoptest);

}

void TCPStartSopTestIMSCommand::go(TCPCommand *_command)
{
    if(!_command){
       countIms = 0;
       commandSet[0]->send(server);
       if(widget){
           widget->setWindowModality(Qt::NonModal);
       }
       return;
    }
    int num = _command->getNum();
    if(num == 0){
        if(countIms >= ims.size()){
            emit commandComplite(this);
            return;
        }
        QByteArray param;
        uint8_t ch = ims[countIms];
        param.append(QByteArray::fromRawData((char*)&ch, sizeof(ch)));
        param.append(QByteArray::fromRawData((char*)&freq, sizeof(freq)));
        param.append(QByteArray::fromRawData((char*)&delay, sizeof(delay)));
        findCommands(testim).first()->setSendData(param);

        TCPTimeOutCommand* to = (TCPTimeOutCommand*)findCommands(timeoutcommand).first();
        to->setTimeOut(timeWork * 1000);
        if(widget){
            to->setWidgetTitleMessage(tr("Тест исполнительных механизмов"),
                                      QString(tr("Тест механизма ручья %1 из %2")).
                                      arg(QString::number(ims[countIms] + 1),
                                          QString::number(ims.size())));
            widget->onRangeChange(countIms, ims.size());
        }
        commandSet[1]->send(server);
        return;
    }
    if(num == commandSet[commandSet.size() - 1]->getNum()){ // last command
        countIms++;
        if(widget){
            widget->setWindowModality(Qt::ApplicationModal);
        }
        emit commandComplite(this);
        commandSet[0]->send(server);
        return;
    }
    if(num < commandSet.size() - 1){
        commandSet[num + 1]->send(server);
        return;
    }

}

QByteArray TCPStartSopTestIMSCommand::getReplayData(){
    return findCommands(getsepar).first()->getReplayData();
}
QByteArray TCPStartSopTestIMSCommand::getResult(){
    return findCommands(getsepar).first()->getResult();
}

void TCPStartSopTestIMSCommand::stopTest(){
    countIms = 10000;
    if(widget){
        widget->setWindowModality(Qt::ApplicationModal);
    }
    stopTestCommand->send(server);
    emit commandComplite(stopTestCommand);
    return;
}
