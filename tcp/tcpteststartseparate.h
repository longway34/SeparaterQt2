#ifndef TCPTESTSTARTSEPARATE_H
#define TCPTESTSTARTSEPARATE_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "tcp/tcplogswigtets.h"
#include "models/sprseparatemodel.h"

class TCPTestStartSeparate : public TCPCommandSet
{
    Q_OBJECT

    SPRSeparateModel *separateModel;
    int countTry;
    int numTry;

public:
    TCPTestStartSeparate(ServerConnect *_server, TCPTimeOutWigget *_widget, TCPLogsWigtets *log = nullptr);

    SPRSeparateModel *getSeparateModel() const;
    void setSeparateModel(SPRSeparateModel *value);
    SPRMainModel *getMainModel() const;

    // TCPCommand interface
public slots:
    void go(TCPCommand *_command);
signals:
    void errorCommand(TCPCommand*);
};

#endif // TCPTESTSTARTSEPARATE_H
