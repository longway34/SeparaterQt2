#include "tcpexpositiononoff.h"

SPRMainModel *TCPExpositionOnOff::getModelData() const
{
    return model;
}

void TCPExpositionOnOff::setModelData(SPRMainModel *value)
{
    model = value;
}

TCPExpositionOnOff::TCPExpositionOnOff(ServerConnect *_server, bool _onOff, bool _offIfError, SPRMainModel *_model, TCPTimeOutWigget *_toWidget, TCPLogsWigtets *_logWidget):
    TCPCommandSet(_server, _toWidget, {}), model(_model), onOff(_onOff), offIfError(_offIfError)
{
    setLogWidget(_logWidget);
}


void TCPExpositionOnOff::go(TCPCommand *_command)
{
    if(!_command){
        clear();
            addCommand(getstate);
            char ren = 0;
            if(onOff){
                if(!server->isState(spr_state_exposition_on) || server->isState(spr_state_rentgen_on_correct)){
                    addCommand(expon)->addSendData(&ren, sizeof(ren), expon);
                    addCommand(onosw)->addCommand(offosw);

                    uint32_t toExp = DEF_SPR_RENTGEN_TIME_HOT_RA;
                    if(model){
                        toExp = model->getSettingsRentgenModel()->timeHotRA->getData();
                    }
                    addCommand(new TCPTimeOutCommand(timeoutcommand, toExp * 1000 + 1000, 100, getTimeOutWidget(),
                                                     QString(tr("Включение рентгена")), QString(tr("Включение экспозиции (%1 сек.)...")).arg(toExp)));

                    addCommand(getren)->addSendData(&ren, sizeof(ren), getren);

                }
            } else {
                addCommand(expoff)->addSendData(&ren, sizeof(ren), expoff);
                addCommand(onosw)->addCommand(offosw)->addCommand(onosw)->addCommand(offosw);
            }

    } else {
        if(_command->getCommand() == getren){
            uint16_t mka, mkv; uint8_t err;
            char ren = 0;
            if(!isRentgenReady(_command->getReplayData(), &mka, &mkv, &err)){
                addCommand(expoff)->addSendData(&ren, sizeof(ren), expoff);
                addCommand(offosw)->addCommand(onosw)->addCommand(offosw);
                emit commandNotComplite(this);
                TCPCommand *off = findCommands(expoff).last();
                off->send(server);
                return;
            }
        }
    }
    EnumCommands _name = nocommand;
    if(_command)
        _name = _command->getCommand();
    TCPCommandSet::go(_command);
}
