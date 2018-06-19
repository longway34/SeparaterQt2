#include "tcpteststartseparate.h"
#include "tcp/tcpcommandrentgenonfull.h"
#include "tcp/tcpcommandseparatoronoff.h"
#include "tcp/tcpcommandrentgeron.h"
#include "tcp/tcpgetspectrumsgistogramms.h"
#include "tcp/tcpseparatego.h"
#include <QMessageBox>


TCPSeparateGo *TCPTestStartSeparate::getSeparateGoCommand() const
{
    return separateGoCommand;
}

TCPGetSpectrumsGistogramms *TCPTestStartSeparate::getGetBaseSpectrumCommand() const
{
    return getBaseSpectrumCommand;
}

TCPTestStartSeparate::TCPTestStartSeparate(ServerConnect *_server, SPRMainModel *_model, TCPTimeOutWigget *_widget, TCPLogsWigtets *log):
    TCPCommandSet(server, _widget, {}), separateModel(nullptr), mainModel(_model)
{
    command = setTestStartSeparate;
    separateModel = mainModel->getSeparateModel();
    char ch0 = '\0';
//    addCommand(new TCPCommandSeparatorOn(_server, _widget)); //0

//    addCommand(new TCPCommand(initada));
//    addCommand(new TCPCommand(onsep));
//    addCommand(new TCPCommand(onren));
    addCommand(new TCPCommandSeparatorOnFull(_server, mainModel, _widget)); //3

    addCommand(new TCPTimeOutCommand(timeoutcommand, 20000, 100, _widget, QString(tr("Включение рентгена")), QString(tr("Включение рентгена"))));
    addCommand(new TCPCommand(onosw)); // 4

    addCommand(new TCPCommand(getblk));
    addCommand(new TCPCommand(expon))->setSendData(&ch0,sizeof(ch0)); //5
    addCommand(new TCPCommand(offosw)); //5
    addCommand(new TCPCommand(onosw)); //7
    addCommand(new TCPTimeOutCommand(timeoutcommand, 7000, 100, _widget, tr("Включение экспозиции"), tr("Включение экспозиции"))); //8

    addCommand(new TCPCommand(getren))->setSendData(&ch0, sizeof(ch0)); // 9
//    addCommand(new TCPTimeOutCommand(timeoutcommand, ))


    getBaseSpectrumCommand = new TCPGetSpectrumsGistogramms(_server, getspk, mainModel, _widget, getLogWidget());
    getBaseSpectrumCommand->setThreadTimer(MAX_SPR_MAIN_THREADS, 1);
    addCommand(getBaseSpectrumCommand); // 10

    addCommand(new TCPCommand(setsepar)); // 11
    addCommand(new TCPCommand(startsep)); // 12
    addCommand(new TCPTimeOutCommand(timeoutcommand, 2000));

    separateGoCommand = new TCPSeparateGo(log);
//    separateGoCommand->setLogWidget(log);
    addCommand(separateGoCommand);


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
        if(separateModel && mainModel){
            countTry = 0; numTry = 5;
            findCommands(setsepar).first()->setSendData(separateModel->toByteArray(mainModel));
//            TCPCommandRentgerOn *reng = ((TCPCommandRentgerOn*)findCommands(setRentgenOn).last());
//            reng->setModel(separateModel->getModelData()->getSettingsRentgenModel());
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
            } else {
                emit rentgenReady(_command);
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

SPRMainModel *TCPTestStartSeparate::getModelData() const
{
    return mainModel;
}

void TCPTestStartSeparate::setModeDatal(SPRMainModel *value)
{
    if(value){
        mainModel = value;
        separateModel = mainModel->getSeparateModel();
        ((TCPSeparateGo*)findCommands(setSeparateGo).first())->setModel(mainModel);
    }
}

//SPRMainModel *TCPTestStartSeparate::getMainModel() const
//{
//    if(separateModel){
//        return separateModel->getModelData();
//    }
//    return nullptr;
//}


