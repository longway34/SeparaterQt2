#ifndef TCPEXPOSITIONOFF_H
#define TCPEXPOSITIONOFF_H

#include <QObject>

#include "tcp/TCPCommandSet.h"
#include "tcp/tcplogswigtets.h"

class TCPExpositionOff : public TCPCommandSet
{
    Q_OBJECT

public:
    TCPExpositionOff(TCPLogsWigtets *logWidget);
};

#endif // TCPEXPOSITIONOFF_H
