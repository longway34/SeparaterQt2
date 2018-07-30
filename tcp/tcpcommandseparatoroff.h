#ifndef TCPCOMMANDSEPARATORON_H
#define TCPCOMMANDSEPARATORON_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "models/sprsettingsrentgenmodel.h"

class TCPCommandSeparatorOff : public TCPCommandSet
{

public:
    TCPCommandSeparatorOff();
    TCPCommandSeparatorOff(ServerConnect *_server, TCPTimeOutWigget *_widget);

public slots:
    virtual void go(TCPCommand *_command);
};



#endif // TCPCOMMANDSEPARATORON_H
