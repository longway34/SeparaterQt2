#ifndef TCPCOMMANDSEPARATORONFULL_H
#define TCPCOMMANDSEPARATORONFULL_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "models/sprmainmodel.h"

class TCPCommandSeparatorOnFull: public TCPCommandSet
{
    Q_OBJECT

    SPRMainModel *model;
    bool fullMode;
public:
    TCPCommandSeparatorOnFull(ServerConnect *server, SPRMainModel *_model=nullptr, TCPTimeOutWigget *_widget = nullptr, bool _isFullMode=true);

    void setModelData(SPRMainModel *value);

    virtual bool isRentgenReady();
    // TCPCommand interface
    void setFullMode(bool value);

public slots:
    virtual void go(TCPCommand *_command);

signals:
    void errorsCommand(TCPCommand *);
};



#endif // TCPCOMMANDSEPARATORONFULL_H
