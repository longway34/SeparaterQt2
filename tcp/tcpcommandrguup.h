#ifndef TCPCOMMANDRGUUP_H
#define TCPCOMMANDRGUUP_H

#include <QObject>
#include "tcp/TCPCommandSet.h"

class TCPCommandRGUUp : public TCPCommandSet
{
public:
    TCPCommandRGUUp();
    TCPCommandRGUUp(ServerConnect *_server, TCPTimeOutWigget *_widget);

protected slots:
    virtual void go(TCPCommand *_command = NULL);
};

#endif // TCPCOMMANDRGUUP_H
