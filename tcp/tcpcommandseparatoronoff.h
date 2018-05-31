#ifndef TCPCOMMANDSEPARATORON_H
#define TCPCOMMANDSEPARATORON_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "models/sprsettingsrentgenmodel.h"

class TCPCommandSeparatorOnOff : public TCPCommandSet
{
    SPRSettingsRentgenModel *model;

    uint8_t rentgenNum;
    uint8_t rentgenCount;
    bool on_off;

public:
    TCPCommandSeparatorOnOff();
    TCPCommandSeparatorOnOff(ServerConnect *_server, TCPTimeOutWigget *_widget, SPRSettingsRentgenModel *_model = nullptr, bool _on_off = true);

    // TCPCommand interface
    void setModel(SPRSettingsRentgenModel *value);

public slots:
    virtual void go(TCPCommand *_command);
};



#endif // TCPCOMMANDSEPARATORON_H
