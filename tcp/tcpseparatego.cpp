#include "tcp/tcpseparatego.h"
#include "tcp/tcpgetspectrumsgistogramms.h"


SPRMainModel *TCPSeparateGo::getModel() const
{
    return model;
}

void TCPSeparateGo::setModel(SPRMainModel *value)
{
    model = value;
    QVector<TCPCommand*> vcomm= findCommands(setGetSpectrumsGistorfamms);
    for(int i=0; i<vcomm.size();i++){
        ((TCPGetSpectrumsGistogramms*)vcomm[i])->setThreadTimer(model->getSettingsMainModel()->getThreads()->getData());
    }
}

void TCPSeparateGo::setLogWidget(TCPLogsWigtets *value)
{
    logWidget = value;
}

TCPGetSpectrumsGistogramms *TCPSeparateGo::getKspectCommand() const
{
    return kspectCommand;
}

TCPGetSpectrumsGistogramms *TCPSeparateGo::getHistCommand() const
{
    return histCommand;
}

TCPCommand *TCPSeparateGo::getGetseparCommand() const
{
    return getseparCommand;
}

TCPSeparateGo::TCPSeparateGo()
{

}

TCPSeparateGo::TCPSeparateGo(TCPLogsWigtets *log):
    TCPCommandSet(nullptr, nullptr, {}), logWidget(log), model(nullptr)
{

    command = setSeparateGo;
    setTimeOut(1000);

//    addCommand(new TCPTimeOutCommand(timeoutcommand, 1000));
    addCommand(new TCPCommand(getstate));
    getseparCommand = new TCPCommand(getsepar);
    addCommand(getseparCommand);
    kspectCommand = new TCPGetSpectrumsGistogramms(nullptr, getkspk);
    addCommand(kspectCommand);
    histCommand = new TCPGetSpectrumsGistogramms(nullptr, getgist);
    addCommand(histCommand);
//    addCommand(new TCPCommand(nocommand));
}

void TCPSeparateGo::go(TCPCommand *_command)
{
    if(!_command){
        toCount = 0;
        commandSet[0]->send(server);
        timer.start();
        return;
    } else {
//        if(_command->getCommand() == timeoutcommand){
//            toCount++; if(toCount > 1000000) toCount = 1;
//        }
        if(_command->getCommand() == getstate){
            QByteArray res = _command->getReplayData();
            memcpy(&stateResult, res.constData(), sizeof(stateResult));
            uint tick;
            if(stateResult.error == 0 && stateResult.state == 0x02){
                if(model){
                    tick = model->getSettingsControlModel()->tMeassureForData->getData();
                } else {
                    tick = DEF_SPR_CONTROL_T_MEASSURE_FOR_DATA;
                }
                if(toCount % tick == 0){
                     getseparCommand->send(server);
                }

                if(model){
                    tick = model->getSettingsControlModel()->tMeassureForSpectrum->getData();
                } else {
                    tick = DEF_SPR_CONTROL_T_MEASSURE_FOR_SPECTRUM;
                }
                if(toCount % tick == 0){
                    kspectCommand->send(server);
                }

                if(model){
                    tick = model->getSettingsControlModel()->tMeassureForHistogramm->getData();
                } else {
                    tick = DEF_SPR_CONTROL_T_MEASSURE_FOR_HISTOGRAMM;
                }
                if(toCount % tick == 0){
                    histCommand->send(server);
                }
//                commandSet[0]->send(server);
                return;
            } else {
                if(logWidget){
                    logWidget->onLogsCommand(_command, tr("Сепарация завершена..."));
                }
                timer.stop();
                server->timerStart();
                emit commandComplite(this);
            }
        }
        if(_command == getseparCommand){
            if(logWidget){
               logWidget->onLogsCommand(nullptr, "separate data ready...");
            }
            emit commandComplite(_command);
//            commandSet[0]->send(server);
            return;
        }
        if(_command == kspectCommand){
            if(logWidget){
               logWidget->onLogsCommand(nullptr,"kspectrums data ready...");
            }
//            emit commandComplite(_command);
//            commandSet[0]->send(server);
            return;
        }
        if(_command == histCommand){
            if(logWidget){
               logWidget->onLogsCommand(nullptr, "historgamms data ready...");
            }
//            emit commandComplite(_command);
//            commandSet[0]->send(server);
            return;
        }
    }
//    TCPCommandSet::go(_command);
}

void TCPSeparateGo::onTimeOut()
{
    toCount++; if(toCount > 1000000) toCount = 1;
    dtime = QDateTime::currentDateTime();
    sdt = dtime.toString("hh:mm:ss");

//    qDebug() << "next:" << sdt << "time:" << this->timeout << " parts:" << this->parts << "timer:" << timer.interval();

    findCommands(getstate).last()->send(server);
    return;
}
