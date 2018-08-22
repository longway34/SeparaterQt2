#include "tcpstartseparate2.h"
#include "tcp/tcpexpositiononoff.h"
#include "tcp/tcpcommandstartpitatel.h"
#include <QMessageBox>


TCPGetSpectrumsGistogramms *TCPStartSeparate2::getGetBaseSpectrum() const
{
    return getBaseSpectrum;
}

TCPStartSeparate2::TCPStartSeparate2(SPRMainModel *_model, TCPTimeOutWigget *_toWidget, TCPLogsWigtets *_logWidget):
    TCPCommandSet(nullptr, _toWidget, {}), model(_model),
     getBaseSpectrum(nullptr), separateGo(nullptr), separatorStop(nullptr), errorSeparateState(SPR_SEPARATE_STATE_OK)
{
    setLogWidget(_logWidget);
}

TCPGetSpectrumsGistogramms *TCPStartSeparate2::getKSpectrunsCommand(){
    if(separateGo){
        return separateGo->getKspectCommand();
    }
    return nullptr;
}

TCPGetSpectrumsGistogramms *TCPStartSeparate2::getHistogrammCommand(){
    if(separateGo){
        return separateGo->getHistCommand();
    }
    return nullptr;
}

void TCPStartSeparate2::go(TCPCommand *_command)
{
    if(model){
        if(!_command){
//            commandSet.clear();
//            clear();

            SPRThreadList lth = model->getThreadsList();

//            for(uint8_t th=0; th<model->getThreads()->getData(); th++) lth << th;
//            if(!getBaseSpectrum){
                getBaseSpectrum = new TCPGetSpectrumsGistogramms(model->getServer(), getspk, model, 1, {}, widget, getLogWidget());
//            }
            getBaseSpectrum->setThreadTimer(1, lth);
            getBaseSpectrum->setWithOffExp(false);
            getBaseSpectrum->setWithRGU(true, true);
            if(!model->getServer()->isState(spr_state_separated)){

                addCommand(getBaseSpectrum);

    //            TCPCommand *getrenCommand = getBaseSpectrum->findCommands(getren).last();

                connect(getBaseSpectrum, SIGNAL(rentgenReady(TCPCommand*)), this, SLOT(onGetRenComplite(TCPCommand*)));
                connect(getBaseSpectrum, SIGNAL(rentgenNotReady(TCPCommand*)), this, SLOT(onGetRenNotComplite(TCPCommand*)));
            }

                QByteArray separData = model->getSeparateModel()->toByteArray(&errorSeparateState);
                addCommand(new TCPCommand(setsepar))->addSendData(separData, setsepar); // 11
    //            findCommands(setsepar).last()->setSendData(separData);

            if(!model->getServer()->isState(spr_state_separated)){
                addCommand(oniw);
                addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 100, getTimeOutWidget(),
                                MSG_TIME_OUT_ON_RASKLAD, MSG_TIME_OUT_ON_RASKLAD_MSG( 2 )));

                TCPCommandStartPitatel *startPitatel = new TCPCommandStartPitatel(model->getServer(),getTimeOutWidget());
                startPitatel->setModelVariable(model->getSettingsControlModel()->VEMSBeginCode);
                addCommand(startPitatel);
                addCommand(new TCPCommand(startsep));

                addCommand(getstate);
            }
//            if(!separateGo){
                separateGo = new TCPSeparateGo(model, getLogWidget());
//            }
//            separateGo->setModel(model);

            connect(separateGo, SIGNAL(gistogrammsDataReady(TCPCommand*)), this, SLOT(onGistorgammsDataReady(TCPCommand*)));
            connect(separateGo, SIGNAL(kspectrumsDataReady(TCPCommand*)), this, SLOT(onKSpectrumsDataReady(TCPCommand*)));
            connect(separateGo, SIGNAL(separateDataReady(TCPCommand*)), this, SLOT(onSeparateDataReady(TCPCommand*)));

            addCommand(separateGo);

//            addCommand(stopsep);

//            if(!separatorStop){
                separatorStop = new TCPTestStopSeparate(getTimeOutWidget(), model);
////            }
//            addCommand(separatorStop);
//            addCommand(offren);
//            addCommand(offosw);
//            addCommand(stoppuw);
//            addCommand(offiw);

            setLogWidget(getLogWidget());

            countTry = 0; numTry = 5;

        } else {
            if(_command == getBaseSpectrum){
                QVector<TCPCommand*> lgetren = getBaseSpectrum->findCommands(getren);
                uint16_t kV, mka; uint8_t err; bool noError = true; TCPCommand *_getren = nullptr;
                for(int i=0; i<lgetren.size() && noError;i++){
                    _getren = lgetren.at(i);
                    noError &= isRentgenReady(_getren->getReplayData(), &kV, &mka, &err);
                }
                if(!noError){
                    QMessageBox::warning(nullptr, tr("Ошибка включения рентгена"), tr("Рентген не вышел в рабочий режим (%1, %2 < 0x600)\nПопробуйте еще раз...").arg(kV, 0, 16).arg(mka, 0, 16));
                    TCPCommand *stop = findCommand(separatorStop);
                    if(stop){
                        stop->send(server);
                    }
                    emit commandNotComplite(_getren);
                    return;
                }
            }
//            if(_command == separatorStop){
//                if(countTry < numTry){
//                    int res = QMessageBox::question(nullptr, tr("Неудачная попытка..."),
//                                tr("Попытаться еще разок (%1/%2)?").arg(QString::number(countTry)).arg(QString::number(numTry)));
//                    if(res == QMessageBox::StandardButton::Yes){
//                        TCPCommand *_next = findCommands(getstate).first();
//                        if(_next){
//                            _next->send(server);
//                        }
//                        return;

//                    } else {
//                        emit commandNotComplite(this); // output with error
//                        return;
//                    }
//                }
//            }
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
