#ifndef TCPCOMMANDRENTGENONFULL_H
#define TCPCOMMANDRENTGENONFULL_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "models/sprmainmodel.h"

class TCPCommandSeparatorOnFull: public TCPCommandSet
{
    Q_OBJECT

    SPRMainModel *model;
public:
    TCPCommandSeparatorOnFull(ServerConnect *server, SPRMainModel *_model=nullptr, TCPTimeOutWigget *_widget = nullptr);

    void setModelData(SPRMainModel *value);

    bool isRentgenReady();
    // TCPCommand interface
public slots:
    virtual void go(TCPCommand *_command);

signals:
    void errorsCommand(TCPCommand *);
};



#endif // TCPCOMMANDRENTGENONFULL_H
