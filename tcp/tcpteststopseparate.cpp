#include "tcpteststopseparate.h"

TCPTestStopSeparate::TCPTestStopSeparate():
    TCPCommandSet(nullptr, nullptr, {})
{
}

TCPTestStopSeparate::TCPTestStopSeparate(TCPTimeOutWigget *_widget):
    TCPCommandSet(nullptr, _widget, {})
{
    command = setTestStopSeparate;
    addCommand(new TCPCommand(stoppuw));
    addCommand(new TCPTimeOutCommand(timeoutcommand, 10, 10, _widget, tr("Останов сепаратора"),tr("Останов сепаратора")));

    char ch0 = '\0';
    addCommand(new TCPCommand(offiw));
    addCommand(new TCPCommand(stopsep));
    TCPCommand *exp = new TCPCommand(expoff); exp->setSendData(&ch0, sizeof(ch0));
    addCommand(exp);
    addCommand(new TCPCommand(offren));
    addCommand(new TCPCommand(offosw));
    addCommand(new TCPCommand(onosw));
    addCommand(new TCPCommand(offosw));
    addCommand(new TCPCommand(onosw));
    addCommand(new TCPCommand(offosw));
    addCommand(new TCPCommand(offsep));


}
