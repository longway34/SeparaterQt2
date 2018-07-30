#include "tcpcommandrentgenonfull.h"
#include "tcp/tcpcommandrentgeron.h"


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

TCPCommandSeparatorOnFull::TCPCommandSeparatorOnFull(ServerConnect *server, SPRMainModel *_model, TCPTimeOutWigget *_widget):
    TCPCommandSet(server, _widget, {}), model(nullptr)
{
    command = setSeparatorOnFull;
    setModelData(_model);
}

void TCPCommandSeparatorOnFull::go(TCPCommand *_command)
{
    if(!_command){
        clear();

        if(model){
            addCommand(new TCPCommand(getstate));

            addCommand(new TCPCommand(initada));
            addCommand(new TCPCommand(onsep));
            addCommand(new TCPCommand(onren));

            addCommand(new TCPCommandRentgerOn(model->getServer(), this->widget, model->getSettingsRentgenModel()));
            addCommand(new TCPCommand(offosw));
            addCommand(new TCPCommand(onosw));

            uint timeHotTube = model->getSettingsRentgenModel()->timeOnRA->getData();

            addCommand(new TCPTimeOutCommand(timeoutcommand, timeHotTube * 1000 + 1000, 10, widget,
                            MSG_TIME_OUT_REN_ON, MSG_TIME_OUT_REN_ON_MSG(timeHotTube)));
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
