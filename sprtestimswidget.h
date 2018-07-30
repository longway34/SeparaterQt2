#ifndef SPRTESTIMSWIDGET_H
#define SPRTESTIMSWIDGET_H

#include "ui_sprtestimswidget.h"
#include "models/sprmainmodel.h"
#include "isprwidget.h"
#include "tcp/TCPCommandSet.h"
#include "tcp/tcptimeoutwigget.h"
#include "tcp/tcpcommandrguup.h"
#include "tcp/tcpcommandrguupdown.h"
#include "tcp/tcpstartsoptestimscommand.h"
#include "tcp/tcpcommandstartpitatel.h"
#include "tcp/tcpcommandstoppitatel.h"
#include "tcp/tcpcommandseparatoroff.h"
#include "tcp/tcpcommandrentgeron.h"
//#include "tcp/tcpcommandgetspectrums.h"
#include "tcp/tcpgetspectrumsgistogramms.h"
#include "tcp/tcpstartrudospuskcommand.h"
#include "tcp/tcpstoprudostuskcommand.h"

#include "tcp/tcpteststopseparate.h"
#include "tcp/tcpcommandrentgenonfull.h"

class SPRTestIMSWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRMainModel *model;

public:
    explicit SPRTestIMSWidget(QWidget *parent = 0);

//    TCPCommandGetSpectrums *getSpectrumsCommand;
    TCPGetSpectrumsGistogramms *getSpectrumsCommand;

    TCPCommandSeparatorOnFull *separatorOnCommand;
    TCPTestStopSeparate *separatorOffCommand;

    TCPCommandRentgerOn *rentgenOnCommand;
    TCPCommandSet *rentgenOffCommand;

    TCPCommandSet *rguReadStateCommand;
    TCPCommandRGUUpDown *rguUpCommand;
    TCPCommandRGUUpDown *rguDownCommand;

    TCPCommandSet *thermoReadStateCommand;
    TCPCommandSet *thermoWriteStateCommand;

    TCPStartSopTestIMSCommand *startTestImsCommand;
//    TCPCommand *stopTestImsCommand;

    TCPCommandStartPitatel *commandStartPitatel;
    TCPCommandStopPitatel *commandStopPitatel;
    TCPCommand *commandChangePersentPitatel;

    TCPCommand *commandStartRasklad;
    TCPCommand *commandStopRasklad;

    TCPStartRudospuskCommand *commandStartRudostusk;
    TCPStopRudostuskCommand *commandStopRudospusk;

    TCPCommandSet *spectrumGetCommand;

    TCPTimeOutWigget *towidget;

    void setThermo(){
        TCPCommand *comm = thermoWriteStateCommand->findCommands(settemp).last();
        uint16_t temps[2] = {ui.slThermoMin->value(), ui.slThermoMax->value()};

        QByteArray ba = QByteArray::fromRawData((char*)temps, sizeof(uint16_t) * 2);
        comm->addSendData(ba);
        thermoWriteStateCommand->send(model->getServer());
    }

    void stopRGUNow(){
        rguDownCommand->setFinish(true);
        rguUpCommand->setFinish(true);
    }

private:
    Ui::SPRTestIMSWidget ui;
    QVector<QCheckBox*> vectorIms;
    QVector<QWidget*> vectorSeparatorOnEnabled;
    // ISPRWidget interface
public:
    ISPRModelData *setModelData(SPRMainModel *_model);

    virtual void widgetsShow();
    virtual ISPRModelData *getModelData();
public slots:
    void onCommandComplite(TCPCommand* _comm);
    void onCommand(bool);
//    void onServerConnectError(ITCPCommand *);
//    void onServerReadWriteError(ITCPCommand *_comm);
    void onServerConnectChangeState(uint32_t _state);

    virtual void onChangeValue(bool _val);
    virtual void onChangeValue(double _val);
    virtual void onModelChanget(IModelVariable *);

    // ISPRWidget interface
protected:

    // ISPRWidget interface
public:
    virtual void setLogWidget(TCPLogsWigtets *value);
};

#endif // SPRTESTIMSWIDGET_H
