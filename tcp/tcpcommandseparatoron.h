#ifndef TCPCOMMANDSEPARATORON_H
#define TCPCOMMANDSEPARATORON_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "models/sprsettingsrentgenmodel.h"

class TCPCommandSeparatorOn : public TCPCommandSet
{
    SPRSettingsRentgenModel *model;

    uint8_t rentgenNum;
    uint8_t rentgenCount;

public:
    TCPCommandSeparatorOn();
    TCPCommandSeparatorOn(ServerConnect *_server, TCPTimeOutWigget *_widget, SPRSettingsRentgenModel *_model = nullptr);

    // TCPCommand interface
    void setModel(SPRSettingsRentgenModel *value);

public slots:
    virtual void go(TCPCommand *_command);
};



#endif // TCPCOMMANDSEPARATORON_H
