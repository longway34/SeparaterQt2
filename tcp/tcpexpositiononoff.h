#ifndef TCPEXPOSITIONONOFF_H
#define TCPEXPOSITIONONOFF_H

#include <QObject>

#include "tcp/TCPCommandSet.h"
#include "tcp/tcplogswigtets.h"

#include "models/sprmainmodel.h"

class TCPExpositionOnOff : public TCPCommandSet
{
    Q_OBJECT

    SPRMainModel *model;
    bool onOff;
    bool offIfError;

public:
    TCPExpositionOnOff(ServerConnect *_server, bool _onOff, bool _offIfError=true, SPRMainModel *_model = nullptr,  TCPTimeOutWigget *_toWidget = nullptr, TCPLogsWigtets *logWidget=nullptr);
    SPRMainModel *getModelData() const;
    void setModelData(SPRMainModel *value);

    // TCPCommand interface
public slots:
    virtual void go(TCPCommand *_command);
};

#endif // TCPEXPOSITIONONOFF_H
