#ifndef TCPCOMMANDSTARTSETPITATEL_H
#define TCPCOMMANDSTARTSETPITATEL_H

#include <QObject>
#include <tcp/TCPCommandSet.h>

class TCPCommandStartPitatel :public TCPCommandSet
{
    bool pitatelWorking;
    uint percents;
public:
    TCPCommandStartPitatel();
    TCPCommandStartPitatel(ServerConnect *_server, TCPTimeOutWigget *_widget = nullptr);

public slots:
    void setPercents(uint _persents);

    // TCPCommand interface
public slots:
    virtual void go(TCPCommand *_command);
    void setPitatelStopFlag(){pitatelWorking = false;}
};



#endif // TCPCOMMANDSTARTSETPITATEL_H
