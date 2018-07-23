#ifndef TCPSTARTSEPARATE2_H
#define TCPSTARTSEPARATE2_H

#include <QObject>
#include "models/sprmainmodel.h"
#include "tcp/TCPCommandSet.h"
#include "tcp/tcplogswigtets.h"
#include "tcp/tcptimeoutwigget.h"
#include "tcpgetspectrumsgistogramms.h"
#include "tcp/tcpseparatego.h"


class TCPStartSeparate2 : public TCPCommandSet
{

    Q_OBJECT

    SPRMainModel *model;
    TCPGetSpectrumsGistogramms *getBaseSpectrum;
    TCPSeparateGo *separateGo;

    int errorSeparateState;

public:

    TCPStartSeparate2(SPRMainModel *_model, TCPTimeOutWigget *_toWidget=nullptr, TCPLogsWigtets *_logWidget=nullptr);



    // TCPCommand interface
public slots:
    virtual void go(TCPCommand *_command);
    void onGetRenComplite(TCPCommand *command);
    void onBaseSpectrumsReady(TCPCommand *command);
    void onGistorgammsDataReady(TCPCommand *command);
    void onSeparateDataReady(TCPCommand *command);
    void onKSpectrumsDataReady(TCPCommand *command);
    void onGetRenNotComplite(TCPCommand *command);
};

#endif // TCPSTARTSEPARATE2_H
