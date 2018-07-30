#include "tcpcommandseparatoroff.h"


TCPCommandSeparatorOff::TCPCommandSeparatorOff()
{

}

TCPCommandSeparatorOff::TCPCommandSeparatorOff(ServerConnect *_server, TCPTimeOutWigget *_widget)
    :TCPCommandSet(_server, _widget, {})
{
    command = setSeparatorOn;
}

void TCPCommandSeparatorOff::go(TCPCommand *_command)
{
    if(!_command){
        clear();

        char ren = 0;

        addCommand(expoff)->addSendData(&ren, sizeof(ren), expoff);
        addCommand(offosw)->addCommand(onosw)->addCommand(offosw)->addCommand(onosw)->
        addCommand(offren)->
        addCommand(offosw)->
        addCommand(offsep);
   }

   TCPCommandSet::go(_command);
}
