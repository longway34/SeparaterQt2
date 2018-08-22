#include "tcpcommandseparatoronfull.h"
#include "tcp/tcpcommandrentgersetup.h"
#include "tcp/tcpcommandseparatoroff.h"

void TCPCommandSeparatorOnFull::setModelData(SPRMainModel *value)
{
    if(value){
        model =  value;

//        for(int i=0; i<commandSet.size(); i++){
//            if(commandSet[i]) delete commandSet[i];
//        }
//        command =

//        addCommand(new TCPCommand(expon));

//        addCommand(new TCPCommand(getren));
    }
}

bool TCPCommandSeparatorOnFull::isRentgenReady(){
    QByteArray res = findCommands(getren).last()->getReplayData().right(4);
    uint mka, mkv;
    memcpy(&mkv, res.constData(), 2);
    memcpy(&mka, res.constData()+2, 2);
    if(mkv >= 0x0600 && mka >= 0x0600){
        return true;
    } else {
        return false;
    }

}

void TCPCommandSeparatorOnFull::setFullMode(bool value)
{
    fullMode = value;
}

TCPCommandSeparatorOnFull::TCPCommandSeparatorOnFull(ServerConnect *server, SPRMainModel *_model, TCPTimeOutWigget *_widget, bool _isFullMode):
    TCPCommandSet(server, _widget, {}), model(nullptr)
{
    command = setSeparatorOnFull;
    fullMode = _isFullMode;
    setModelData(_model);
}

void TCPCommandSeparatorOnFull::go(TCPCommand *_command)
{
    if(!_command){
        clear();

        if(model){
            addCommand(new TCPCommand(getstate));

            addCommand(new TCPCommandSeparatorOff(model->getServer(), widget));

            addCommand(new TCPCommand(initada));
            addCommand(new TCPCommand(onsep));
            if(fullMode){
                addCommand(new TCPCommand(onren));

                uint timeHotTube = model->getSettingsRentgenModel()->timeOnRA->getData();
                addCommand(new TCPTimeOutCommand(timeoutcommand, timeHotTube * 1000 + 1000, 10, widget,
                                MSG_TIME_OUT_REN_ON, MSG_TIME_OUT_REN_ON_MSG(timeHotTube)));
                addCommand(new TCPCommandRentgerSetup(model->getServer(), this->widget, model->getSettingsRentgenModel()));
            }
            addCommand(new TCPCommand(offosw));
            addCommand(new TCPCommand(onosw));


//                                             QString(tr("Включение рентгена...")),
//                                             QString(tr("Прогрев трубок (%1 секунд)...")).arg(timeHotTube)));
        }
    }/* else {
        if(_command->getCommand() == getstate){
            int err = _command->getErrors();
            if(err != 0){
                emit errorsCommand(_command);
                return;
            }
        }
    }*/
    EnumCommands _name = nocommand;
    if(_command)
        _name = _command->getCommand();

    TCPCommandSet::go(_command);
}
