#ifndef TCPSTARTSEPARATE_H
#define TCPSTARTSEPARATE_H

#include <QObject>
#include "tcp/TCPCommandSet.h"

class TCPStartSeparate : public TCPCommandSet
{
public:
    TCPStartSeparate();
    TCPStartSeparate(ServerConnect *_server, TCPTimeOutWigget *_widget);
protected slots:
    virtual void go(TCPCommand *_command = NULL);
};

#endif // TCPSTARTSEPARATE_H
