#include "tcpcommandrentgenonfull.h"
#include "tcp/tcpcommandrentgeron.h"


void TCPCommandSeparatorOnFull::setModelData(SPRMainModel *value)
{
    if(value){
        model =  value;

        for(int i=0; i<commandSet.size(); i++){
            if(commandSet[i]) delete commandSet[i];
        }

//        command =
        addCommand(new TCPCommand(getstate));

        addCommand(new TCPCommand(initada));
        addCommand(new TCPCommand(onsep));
        addCommand(new TCPCommand(onren));

        addCommand(new TCPCommandRentgerOn(model->getServer(), this->widget, model->getSettingsRentgenModel()));

//        addCommand(new TCPCommand(expon));
//        addCommand(new TCPCommand(offosw));
        addCommand(new TCPCommand(onosw));

//        addCommand(new TCPTimeOutCommand(timeoutcommand, 6000, 10, widget, QString(tr("Включение экспозиции...")), QString(tr("Включение экспозиции..."))));

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
    if(_command){
        if(_command->getCommand() == getstate){
            int err = _command->getErrors();
            if(err != 0){
                emit errorsCommand(_command);
                return;
            }
        }
//        if(_command->getCommand() == getren){
//            if(!isRentgenReady()){
//                emit errorsCommand(_command);
//                return;
//            }
//        }
    }
    TCPCommandSet::go(_command);
}
