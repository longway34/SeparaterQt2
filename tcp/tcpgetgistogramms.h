#ifndef TCPGETGISTOGRAMMS_H
#define TCPGETGISTOGRAMMS_H

#include <QObject>
#include "tcp/TCPCommandSet.h"

class TCPGetGistogramms : public TCPCommandSet
{
    Q_OBJECT
    
    uint threadNum;

public:
    TCPGetGistogramms();
    TCPGetGistogramms(TCPTimeOutWigget *_widget, uint _threads);
protected slots:
    virtual void go(TCPCommand *_command = NULL);
};

#endif // TCPGETGISTOGRAMMS_H
