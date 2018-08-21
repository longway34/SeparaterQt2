#ifndef TCPAUTOSETCOMMANDGO_H
#define TCPAUTOSETCOMMANDGO_H

#include <QObject>
#include <tcp/TCPCommandSet.h>
#include "_types.h"

class TCPAutosetCommandGo : public TCPCommandSet
{

    Q_OBJECT

    SPRThreadList threads;
    QByteArray baDEU, baCP;
    uint32_t toSpk;
public:
    TCPAutosetCommandGo(ServerConnect *_server, QByteArray _deu, QByteArray _cp, SPRThreadList _threads, TCPTimeOutWigget *_toWidget, TCPLogsWigtets *_logs);

    void setDeuCpCodes(QByteArray _deu, QByteArray _cp);
    // TCPCommand interface
    SPRThreadList getThreads() const;
    void setThreads(const SPRThreadList &value);

public slots:
    virtual void go(TCPCommand *_command);
};

#endif // TCPAUTOSETCOMMANDGO_H
