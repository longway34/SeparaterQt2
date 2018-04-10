#ifndef TCPTESTSTOPSEPARATE_H
#define TCPTESTSTOPSEPARATE_H

#include <QObject>

#include "tcp/TCPCommandSet.h"

class TCPTestStopSeparate : public TCPCommandSet
{
public:
    TCPTestStopSeparate();
    TCPTestStopSeparate(TCPTimeOutWigget *_widget);
};

#endif // TCPTESTSTOPSEPARATE_H
