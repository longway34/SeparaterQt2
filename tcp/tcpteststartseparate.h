#ifndef TCPTESTSTARTSEPARATE_H
#define TCPTESTSTARTSEPARATE_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "tcp/tcplogswigtets.h"
#include "models/sprseparatemodel.h"
#include "models/sprmainmodel.h"
#include "tcp/tcpseparatego.h"
#include "tcp/tcpgetspectrumsgistogramms.h"
#include "tcp/tcpcommandrentgenonfull.h"
#include "tcp/tcpcommandseparatoroff.h"

class TCPTestStartSeparate : public TCPCommandSet
{
    Q_OBJECT


    SPRSeparateModel *separateModel;
    SPRMainModel *mainModel;
    int countTry;
    int numTry;

    int errorSeparateState;

    TCPSeparateGo *separateGoCommand;
    TCPCommandSeparatorOff *separatorStop;
    TCPGetSpectrumsGistogramms *getBaseSpectrumCommand;
public:
    TCPTestStartSeparate(ServerConnect *_server, SPRMainModel *_model, TCPTimeOutWigget *_widget, TCPLogsWigtets *log = nullptr);

    SPRSeparateModel *getSeparateModel() const;
    void setModeDatal(SPRMainModel *value);
    SPRMainModel *getMainModel() const;

    // TCPCommand interface
    TCPSeparateGo *getSeparateGoCommand() const;

    TCPGetSpectrumsGistogramms *getGetBaseSpectrumCommand() const;

    SPRMainModel *getModelData() const;
public slots:
    void go(TCPCommand *_command);

signals:
    void errorCommand(TCPCommand*);
    void rentgenReady(TCPCommand*);
};

#endif // TCPTESTSTARTSEPARATE_H
