#ifndef TCPTESTSTOPSEPARATE_H
#define TCPTESTSTOPSEPARATE_H

#include <QObject>

#include "tcp/TCPCommandSet.h"
#include "models/sprmainmodel.h"

class TCPTestStopSeparate : public TCPCommandSet
{
    SPRMainModel *model;
public:
    TCPTestStopSeparate();
    TCPTestStopSeparate(TCPTimeOutWigget *_widget, SPRMainModel *_model = nullptr);

    void setModelData(SPRMainModel *_model);

    // TCPCommand interface
public slots:
    virtual void go(TCPCommand *_command);
};

#endif // TCPTESTSTOPSEPARATE_H
