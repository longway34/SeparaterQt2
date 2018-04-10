#include "tcpteststartseparate.h"
#include "tcp/tcpcommandrentgeron.h"
#include "tcp/tcpcommandseparatoron.h"
#include "tcp/tcpgetspectrumsgistogramms.h"
#include "tcp/tcpseparatego.h"
#include <QMessageBox>


TCPTestStartSeparate::TCPTestStartSeparate(ServerConnect *_server, TCPTimeOutWigget *_widget, TCPLogsWigtets *log):
    TCPCommandSet(server, _widget, {}), separateModel(nullptr)
{
    command = setTestStartSeparate;
    char ch0 = '\0';
//    addCommand(new TCPCommandSeparatorOn(_server, _widget)); //0

    addCommand(new TCPCommand(initada))->addCommand(new TCPCommand(onsep));
    addCommand(new TCPCommandRentgerOn(_server, _widget)); //3

    addCommand(new TCPCommand(onosw)); // 4

    addCommand(new TCPCommand(getblk));
    addCommand(new TCPCommand(expon))->setSendData(&ch0,sizeof(ch0)); //5
    addCommand(new TCPCommand(offosw)); //5
    addCommand(new TCPCommand(onosw)); //7
    addCommand(new TCPTimeOutCommand(timeoutcommand, 7000, 100, _widget, tr("Включение экспозиции"), tr("Включение экспозиции"))); //8

    addCommand(new TCPCommand(getren))->setSendData(&ch0, sizeof(ch0)); // 9
//    addCommand(new TCPTimeOutCommand(timeoutcommand, ))


    TCPGetSpectrumsGistogramms *spect = new TCPGetSpectrumsGistogramms(_server, getspk, _widget);
    spect->setThreadTimer(MAX_SPR_MAIN_THREADS, 1);
    addCommand(spect); // 10

    addCommand(new TCPCommand(setsepar)); // 11
    addCommand(new TCPCommand(startsep)); // 12
    addCommand(new TCPTimeOutCommand(timeoutcommand, 2000));

    addCommand(new TCPSeparateGo(log));

    addCommand(new TCPCommand(nocommand));

//    addCommand(new TCPCommand(oniw)); // 13

    addCommand(new TCPCommand(expoff))->setSendData(&ch0, sizeof(ch0));
    addCommand((new TCPCommand(offosw)));
    addCommand((new TCPCommand(onosw)));
    addCommand((new TCPCommand(offosw)));
    addCommand((new TCPCommand(onosw)));
    addCommand(new TCPCommand(nocommand));
}

void TCPTestStartSeparate::go(TCPCommand *_command)
{
    if(!_command){
        if(separateModel){
            countTry = 0; numTry = 5;
            findCommands(setsepar).first()->setSendData(separateModel->toByteArray());
            TCPCommandRentgerOn *reng = ((TCPCommandRentgerOn*)findCommands(setRentgenOn).last());
            reng->setModel(separateModel->getMainModel()->getSettingsRentgenModel());
        }
    } else {
        if(_command->getCommand() == getren){
            uint16_t kV, mka;
            QByteArray res = _command->getReplayData();
            memcpy(&kV, res.constData()+1, sizeof(kV));
            memcpy(&mka, res.constData()+3, sizeof(mka));
            if(kV < 0x600 || mka < 0x600){
                emit errorCommand(_command);
                countTry++;

                TCPCommand *_next = findCommands(expoff).last();
                _next->send(server);
                return;
            }
        }
        if(_command == commandSet.last()){
            if(countTry < numTry){
                int res = QMessageBox::question(nullptr, tr("Неудачная попытка..."),
                            tr("Попытвться еще разок (%1/%2)?").arg(QString::number(countTry)).arg(QString::number(numTry)));
                if(res == QMessageBox::StandardButton::Yes){
                    TCPCommand *_next = findCommands(getblk).last();
                    _next->send(server);
                    return;

                } else {
                    emit commandComplite(nullptr); // output with error
                    return;
                }
            } else {
                emit commandComplite(nullptr); // output with error
                return;
            }
        }
        if(_command != commandSet.last() && _command->getCommand() == nocommand){
            emit commandComplite(this); // output with no errors
            return;
        }
    }
    TCPCommandSet::go(_command);
}

SPRSeparateModel *TCPTestStartSeparate::getSeparateModel() const
{
    return separateModel;
}

void TCPTestStartSeparate::setSeparateModel(SPRSeparateModel *value)
{
    separateModel = value;
    ((TCPSeparateGo*)findCommands(setSeparateGo).first())->setModel(separateModel->getMainModel());
}

SPRMainModel *TCPTestStartSeparate::getMainModel() const
{
    if(separateModel){
        return separateModel->getMainModel();
    }
    return nullptr;
}


