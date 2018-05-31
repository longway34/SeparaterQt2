#include "tcpcommandseparatoronoff.h"

void TCPCommandSeparatorOnOff::setModel(SPRSettingsRentgenModel *value)
{
    model = value;
}

TCPCommandSeparatorOnOff::TCPCommandSeparatorOnOff()
{

}

TCPCommandSeparatorOnOff::TCPCommandSeparatorOnOff(ServerConnect *_server, TCPTimeOutWigget *_widget, SPRSettingsRentgenModel *_model, bool _on_off)
    :TCPCommandSet(_server, _widget, {}), model(_model), on_off(_on_off)
{
    command = setSeparatorOn;
}

void TCPCommandSeparatorOnOff::go(TCPCommand *_command)
{
    if(!_command){
        for(int i=0; i<commandSet.size(); i++){
            delete commandSet[i];
        }
        commandSet.clear();
        if(on_off){
            addCommand(new TCPCommand(initada))->
            addCommand(new TCPCommand(onsep))->
            addCommand(new TCPCommand(onosw));
        } else {
            addCommand(new TCPCommand(offren))->
            addCommand(new TCPCommand(offosw))->
            addCommand(new TCPCommand(offsep));
        }
   }

   TCPCommandSet::go(_command);
}
