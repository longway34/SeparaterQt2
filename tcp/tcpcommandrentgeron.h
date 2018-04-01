#ifndef TCPCOMMANDRENTGERON_H
#define TCPCOMMANDRENTGERON_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "models/sprsettingsrentgenmodel.h"

class TCPCommandRentgerOn : public TCPCommandSet
{
    SPRSettingsRentgenModel *model;

    uint8_t rentgenNum;
    uint8_t rentgenCount;
public:
    TCPCommandRentgerOn();
    TCPCommandRentgerOn(ServerConnect *_server, TCPTimeOutWigget *_widget, SPRSettingsRentgenModel *_model = nullptr);


    void setModel(SPRSettingsRentgenModel *value);

protected slots:
    virtual void go(TCPCommand *_command = NULL);
};

#endif // TCPCOMMANDRENTGERON_H
