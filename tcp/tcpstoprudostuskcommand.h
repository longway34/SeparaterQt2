#ifndef TCPSTOPRUDOSTUSKCOMMAND_H
#define TCPSTOPRUDOSTUSKCOMMAND_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "tcp/tcpcommandstoppitatel.h"

class TCPStopRudostuskCommand : public TCPCommandSet
{
public:
    TCPStopRudostuskCommand();
    TCPStopRudostuskCommand(ServerConnect *_server, TCPTimeOutWigget *_widget);

    // TCPCommand interface
public slots:
    virtual void go(TCPCommand *_command);
};



#endif // TCPSTOPRUDOSTUSKCOMMAND_H
