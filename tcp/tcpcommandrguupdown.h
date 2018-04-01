#ifndef TCPCOMMANDRGUUPDOWN_H
#define TCPCOMMANDRGUUPDOWN_H

#include <QObject>
#include "tcp/TCPCommandSet.h"

class TCPCommandRGUUpDown : public TCPCommandSet
{
    bool upDown;
    uint result;
    bool finish;
public:
    TCPCommandRGUUpDown();
    TCPCommandRGUUpDown(ServerConnect *_server, TCPTimeOutWigget *_widget, bool _upDown);

    void setFinish(bool value);
    bool getFinish() const;
public slots:
    void finishedNow(){
        setFinish(true);
    }

protected slots:
    virtual void go(TCPCommand *_command = NULL);
};

#endif // TCPCOMMANDRGUUPDOWN_H
