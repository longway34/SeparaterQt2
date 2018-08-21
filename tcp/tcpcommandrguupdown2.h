#ifndef TCPCOMMANDRGUUPDOWN2_H
#define TCPCOMMANDRGUUPDOWN2_H

#include <QObject>
#include "tcp/TCPCommandSet.h"

class TCPCommandRGUUpDown2 : public TCPCommandSet
{
    bool up_down;
    char sendData;
    uint8_t result;
    bool started;

    Q_OBJECT
public:
    TCPCommandRGUUpDown2(ServerConnect *_server, TCPTimeOutWigget *_towidget, bool _up_down);

    // TCPCommand interface
    bool getStarted() const;
    void setStarted(bool value);

public slots:
    virtual void go(TCPCommand *_command);
};

#endif // TCPCOMMANDRGUUPDOWN2_H
