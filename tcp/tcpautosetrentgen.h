#ifndef TCPAUTOSETRENTGEN_H
#define TCPAUTOSETRENTGEN_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "models/sprmainmodel.h"
#include "tcp/tcpcommandrentgenonfull.h"
#include "tcp/tcpcommandrguupdown2.h"

class TCPAutoSetRentgen: public TCPCommandSet
{
    Q_OBJECT

    SPRMainModel *mainModel;
    TCPCommandSeparatorOnFull *separatorOn;
    TCPCommandRGUUpDown2 *commandRGU;

    bool useRGU;
    SPRThreadList threads;

    QByteArray codesDEU;
    QByteArray codesCP;

    void settingCodes();
    void settingRentgenVA();

public:
    TCPAutoSetRentgen(SPRMainModel *_model, TCPTimeOutWigget *_toWidget);

    bool getUseRGU() const;
    void setUseRGU(bool value);

    SPRThreadList getThreads();
    void setThreads(SPRThreadList &value);
    void setCodesDEU(QByteArray &value);
    void setCodesCP(QByteArray &value);

public slots:
    virtual void go(TCPCommand *_command);
};

#endif // TCPAUTOSETRENTGEN_H
