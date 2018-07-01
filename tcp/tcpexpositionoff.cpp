#include "tcpexpositionoff.h"

TCPExpositionOff::TCPExpositionOff(TCPLogsWigtets *logWidget):
    TCPCommandSet(nullptr, nullptr, {})
{

    char ch = 0;
    addCommand(expoff);
    findCommands(expoff).last()->setSendData(&ch, 1);
    addCommand(offosw);

    this->setLogWidget(logWidget);
}
