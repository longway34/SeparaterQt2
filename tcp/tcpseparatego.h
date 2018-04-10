#ifndef TCPSEPARATEGO_H
#define TCPSEPARATEGO_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "models/sprmainmodel.h"
#include "tcp/tcplogswigtets.h"

class TCPSeparateGo : public TCPCommandSet
{
    Q_OBJECT

    SPRMainModel *model;
    TCPLogsWigtets *logWidget;
    uint32_t toCount;
public:
    TCPSeparateGo();
    TCPSeparateGo(TCPLogsWigtets *log=nullptr);

    SPRMainModel *getModel() const;
    void setModel(SPRMainModel *value);

    // TCPCommand interface
public slots:
    virtual void go(TCPCommand *_command);

signals:
    void separateDataReady(TCPCommand *command);
    void gistogrammsDataReady(TCPCommand *command);
    void kspectrumsDataReady(TCPCommand *command);
};



#endif // TCPSEPARATEGO_H
