#ifndef TCPTESTSTARTSEPARATE_H
#define TCPTESTSTARTSEPARATE_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "models/sprseparatemodel.h"

class TCPTestStartSeparate : public TCPCommandSet
{
    SPRSeparateModel *separateModel;

public:
    TCPTestStartSeparate(ServerConnect *_server, TCPTimeOutWigget *_widget);

    SPRSeparateModel *getSeparateModel() const;
    void setSeparateModel(SPRSeparateModel *value);
    SPRMainModel *getMainModel() const;

    // TCPCommand interface
public slots:
    void go(TCPCommand *_command);
};

#endif // TCPTESTSTARTSEPARATE_H
