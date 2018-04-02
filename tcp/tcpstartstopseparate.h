#ifndef TCPSTARTSTOPSEPARATE_H
#define TCPSTARTSTOPSEPARATE_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "models/sprmainmodel.h"

class TCPStartStopSeparate : public TCPCommandSet
{
    Q_OBJECT

    bool running;
    SPRMainModel *model;
    uint threads;
    uint rentgens;

    uint tGetSeparate;
    uint tGetSpectrum;
    uint tGetGistogramm;

    int timeOutsCount;
public:
    TCPStartStopSeparate();
    TCPStartStopSeparate(ServerConnect *_server, TCPTimeOutWigget *_widget);
    void setModel(SPRMainModel *value);

protected slots:
    virtual void go(TCPCommand *_command = NULL);
    void stopSeparate(bool){
        running = false;
    }

signals:
    void baseSpectrumsReady(TCPCommand *command);
    void spectrumsReady(TCPCommand *command);
    void gistogrammsReady(TCPCommand *command);
};

#endif // TCPSTARTSTOPSEPARATE_H
