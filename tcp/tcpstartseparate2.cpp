#include "tcpstartseparate2.h"
#include "tcp/tcpexpositiononoff.h"
#include <QMessageBox>


TCPStartSeparate2::TCPStartSeparate2(SPRMainModel *_model, TCPTimeOutWigget *_toWidget, TCPLogsWigtets *_logWidget):
    TCPCommandSet(nullptr, _toWidget, {}), model(_model), errorSeparateState(SPR_SEPARATE_STATE_OK)
{
    setLogWidget(_logWidget);
}



void TCPStartSeparate2::go(TCPCommand *_command)
{
    if(model){
        if(!_command){
            commandSet.clear();

            QList<uint8_t> lth;
            for(uint8_t th=0; th<model->getThreads()->getData(); th++) lth << th;
            getBaseSpectrum = new TCPGetSpectrumsGistogramms(model->getServer(), getspk, model, 1, {}, widget, getLogWidget());
            getBaseSpectrum->setThreadTimer(1, lth);
            getBaseSpectrum->setWithOffExp(false);
            getBaseSpectrum->setWithRGU(true, true);
            addCommand(getBaseSpectrum);

//            TCPCommand *getrenCommand = getBaseSpectrum->findCommands(getren).last();

            connect(getBaseSpectrum, SIGNAL(rentgenReady(TCPCommand*)), this, SLOT(onGetRenComplite(TCPCommand*)));
            connect(getBaseSpectrum, SIGNAL(rentgenNotReady(TCPCommand*)), this, SLOT(onGetRenNotComplite(TCPCommand*)));

            QByteArray separData = model->getSeparateModel()->toByteArray(model, &errorSeparateState);
            addCommand(new TCPCommand(setsepar))->addSendData(separData, setsepar); // 11
//            findCommands(setsepar).last()->setSendData(separData);

            addCommand(new TCPCommand(startsep));
            addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 100, getTimeOutWidget(),
                            MSG_TIME_OUT_ON_SEPAR, MSG_TIME_OUT_ON_SEPAR_MSG( 2 )));

            addCommand(getstate);

            separateGo = new TCPSeparateGo(model, getLogWidget());
//            separateGo->setModel(model);

            connect(separateGo, SIGNAL(gistogrammsDataReady(TCPCommand*)), this, SLOT(onGistorgammsDataReady(TCPCommand*)));
            connect(separateGo, SIGNAL(kspectrumsDataReady(TCPCommand*)), this, SLOT(onKSpectrumsDataReady(TCPCommand*)));
            connect(separateGo, SIGNAL(separateDataReady(TCPCommand*)), this, SLOT(onSeparateDataReady(TCPCommand*)));

            addCommand(separateGo);

            addCommand(stopsep);

            separatorStop = new TCPCommandSeparatorOff(model->getServer(), getTimeOutWidget());
            addCommand(separatorStop);
//            addCommand(offren);
//            addCommand(offosw);

            setLogWidget(getLogWidget());

            countTry = 0; numTry = 5;

        }else {
            if(_command->getCommand() == getren){
                uint16_t kV, mka; uint8_t err;
                if(!isRentgenReady(_command->getReplayData(), &kV, &mka, &err)){
                    TCPCommand *stop = findCommand(separatorStop);
                    if(stop){
                        stop->send(server);
                    }
                    return;
                }
            }
            if(_command == separatorStop){
                if(countTry < numTry){
                    int res = QMessageBox::question(nullptr, tr("Неудачная попытка..."),
                                tr("Попытаться еще разок (%1/%2)?").arg(QString::number(countTry)).arg(QString::number(numTry)));
                    if(res == QMessageBox::StandardButton::Yes){
                        TCPCommand *_next = findCommands(getstate).first();
                        if(_next){
                            _next->send(server);
                        }
                        return;

                    } else {
                        emit commandNotComplite(this); // output with error
                        return;
                    }
                }
            }
            if(_command == separateGo){
                emit commandComplite(this);
                return;
            }
        }

        TCPCommandSet::go(_command);
    }
}
void TCPStartSeparate2::onSeparateDataReady(TCPCommand *command)
{
    emit commandComplite(command);
}
void TCPStartSeparate2::onBaseSpectrumsReady(TCPCommand *command)
{
    emit commandComplite(command);
}

void TCPStartSeparate2::onGistorgammsDataReady(TCPCommand *command)
{
    emit commandComplite(command);
}
void TCPStartSeparate2::onKSpectrumsDataReady(TCPCommand *command)
{
    emit commandComplite(command);
}
void TCPStartSeparate2::onGetRenComplite(TCPCommand *command)
{
    emit commandComplite(command);
}
void TCPStartSeparate2::onGetRenNotComplite(TCPCommand *command)
{
    emit commandNotComplite(command);
}
