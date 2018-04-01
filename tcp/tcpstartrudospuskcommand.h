#ifndef TCPSTARTRUDOSPUSKCOMMAND_H
#define TCPSTARTRUDOSPUSKCOMMAND_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "tcpcommandstartpitatel.h"

class TCPStartRudospuskCommand : public TCPCommandSet
{
public:
    TCPStartRudospuskCommand();
    TCPStartRudospuskCommand(ServerConnect *_server, TCPTimeOutWigget *_widget);

    // TCPCommand interface
public slots:
    virtual void go(TCPCommand *_command);
};



#endif // TCPSTARTRUDOSPUSKCOMMAND_H
