#include "tcpstartseparate2.h"
#include "tcp/tcpexpositionoff.h"


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

            getBaseSpectrum = new TCPGetSpectrumsGistogramms(model->getServer(), getspk, model, widget, getLogWidget());
            getBaseSpectrum->setThreadTimer(model->getThreads()->getData(), 1);
            addCommand(getBaseSpectrum);

//            TCPCommand *getrenCommand = getBaseSpectrum->findCommands(getren).last();

            connect(getBaseSpectrum, SIGNAL(rentgenReady(TCPCommand*)), this, SLOT(onGetRenComplite(TCPCommand*)));
            connect(getBaseSpectrum, SIGNAL(rentgenNotReady(TCPCommand*)), this, SLOT(onGetRenNotComplite(TCPCommand*)));

            addCommand(new TCPCommand(setsepar)); // 11
            QByteArray separData = model->getSeparateModel()->toByteArray(model, &errorSeparateState);
            findCommands(setsepar).last()->setSendData(separData);

            addCommand(new TCPCommand(startsep)); // 12
            addCommand(new TCPTimeOutCommand(timeoutcommand, 2000));

            addCommand(getstate);

            separateGo = new TCPSeparateGo(getLogWidget());
            separateGo->setModel(model);

            connect(separateGo, SIGNAL(gistogrammsDataReady(TCPCommand*)), this, SLOT(onGistorgammsDataReady(TCPCommand*)));
            connect(separateGo, SIGNAL(kspectrumsDataReady(TCPCommand*)), this, SLOT(onKSpectrumsDataReady(TCPCommand*)));
            connect(separateGo, SIGNAL(separateDataReady(TCPCommand*)), this, SLOT(onSeparateDataReady(TCPCommand*)));

            addCommand(separateGo);

            addCommand(stopsep);
            addCommand(new TCPExpositionOff(getLogWidget()));
            addCommand(offren);
            addCommand(offosw);

            setLogWidget(getLogWidget());
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
