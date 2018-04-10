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

TCPSeparateGo::TCPSeparateGo()
{

}

TCPSeparateGo::TCPSeparateGo(TCPLogsWigtets *log):
    TCPCommandSet(nullptr, nullptr, {}), logWidget(log), model(nullptr)
{

    command = setSeparateGo;

    addCommand(new TCPTimeOutCommand(timeoutcommand, 1000));
    addCommand(new TCPCommand(getstate));
    addCommand(new TCPCommand(getsepar));
    addCommand(new TCPGetSpectrumsGistogramms(nullptr, getkspk));
    addCommand(new TCPGetSpectrumsGistogramms(nullptr, getgist));
    addCommand(new TCPCommand(nocommand));
}

void TCPSeparateGo::go(TCPCommand *_command)
{
    if(!_command){
        toCount = 0;

        commandSet[0]->send(server);
    } else {
        if(_command->getCommand() == timeoutcommand){
            toCount++; if(toCount > 1000000) toCount = 1;
        }
        if(_command->getCommand() == getstate){
            struct {uint8_t state;
                    uint8_t error;} state;
            QByteArray res = _command->getReplayData();
            memcpy(&state, res.constData(), sizeof(state));
            uint tick;
            if(state.error == 0 && state.state == 0x02){
                if(model){
                    tick = model->getSettingsControlModel()->tMeassureForData->getData();
                } else {
                    tick = DEF_SPR_CONTROL_T_MEASSURE_FOR_DATA;
                }
                if(toCount % tick == 0){
                     findCommands(getsepar).first()->send(server);
                }

                if(model){
                    tick = model->getSettingsControlModel()->tMeassureForSpectrum->getData();
                } else {
                    tick = DEF_SPR_CONTROL_T_MEASSURE_FOR_SPECTRUM;
                }
                if(toCount % tick == 0){
                    findCommands(setGetSpectrumsGistorfamms).first()->send(server);
                }

                if(model){
                    tick = model->getSettingsControlModel()->tMeassureForHistogramm->getData();
                } else {
                    tick = DEF_SPR_CONTROL_T_MEASSURE_FOR_HISTOGRAMM;
                }
                if(toCount % tick == 0){
                    findCommands(setGetSpectrumsGistorfamms).last()->send(server);
                }
                commandSet[0]->send(server);
                return;
            } else {
                findCommands(nocommand).last()->send(server);
            }
        }
        if(_command->getCommand() == getsepar){
            if(logWidget){
               logWidget->onLogsCommand("separate data ready...");
            }
//            emit separateDataReady(_command);
            commandSet[0]->send(server);
            return;
        }
        if(_command == findCommands(setGetSpectrumsGistorfamms).first()){
//            emit kspectrumsDataReady(_command);
            if(logWidget){
               logWidget->onLogsCommand("kspectrums data ready...");
            }
            commandSet[0]->send(server);
            return;
        }
        if(_command == findCommands(setGetSpectrumsGistorfamms).last()){
//            emit gistogrammsDataReady(_command);
            if(logWidget){
               logWidget->onLogsCommand("historgamms data ready...");
            }
            commandSet[0]->send(server);
            return;
        }
    }
    TCPCommandSet::go(_command);
}
