#ifndef TCPCOMMANDRENTGERON_H
#define TCPCOMMANDRENTGERON_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "models/sprsettingsrentgenmodel.h"
#include "tcp/tcpcommandseparatoronfull.h"

class TCPCommandRentgerSetup : public TCPCommandSet
{
    SPRSettingsRentgenModel *model;

    uint8_t rentgenNum;
    uint8_t rentgenCount;
public:
    TCPCommandRentgerSetup();
    TCPCommandRentgerSetup(ServerConnect *_server, TCPTimeOutWigget *_widget, SPRSettingsRentgenModel *_model = nullptr);


    void setModel(SPRSettingsRentgenModel *value);

protected slots:
    virtual void go(TCPCommand *_command = NULL);
signals:
    void errorSetRentgenCommand(TCPCommand *);
};

#endif // TCPCOMMANDRENTGERON_H
