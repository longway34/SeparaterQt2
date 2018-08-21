#ifndef SPRSEPARATECONTROLWIDGET_H
#define SPRSEPARATECONTROLWIDGET_H

#include "ui_sprseparatecontrolwidget.h"
#include "models/sprmainmodel.h"
#include "models/sprspectrumlistitemsmodel.h"
#include "tcp/tcpstartseparate2.h"
#include "tcp/tcpteststopseparate.h"

class SPRSeparateControlWidget : public QWidget
{
    Q_OBJECT

    SPRMainModel *model;

    SPRSpectrumListItemsModel *kSpectrumModel;

    TCPStartSeparate2 *startSeparate;
    TCPTestStopSeparate *stopSeparate;
    TCPCommand *setSeparate;

    TCPTimeOutWigget *toWidget;
    TCPLogsWigtets *logWidget;

    QTimer separateTimer;
    QDateTime lastTimerUpdate;


public:
    Ui::SPRSeparateControlWidget ui;
    explicit SPRSeparateControlWidget(QWidget *parent = nullptr);

    void setModelData(SPRMainModel *_model, TCPTimeOutWigget *_toWidget, TCPLogsWigtets *_logWidget);

    SPRSpectrumListItemsModel *getKSpectrumModel() const;

    void setKSpectrumModel(SPRSpectrumListItemsModel *value);

    TCPLogsWigtets *getLogWidget() const;

    TCPTimeOutWigget *getTimeOutWidget() const;

    void setLogWidget(TCPLogsWigtets *value);

private:

public slots:
    void onServerStateChange(uint32_t);
    void onTCPCommandComplite(TCPCommand *command);
    void onTCPCommandNotComplite(TCPCommand *command);
    void onModelChanget(IModelVariable *);
    void onCommandButtomClick(bool);
    void onTimeOut();
protected:
    void onSpectrumsReady(TCPGetSpectrumsGistogramms *_command, bool isKSpectrum = true, uint32_t _time_scope_in_msec = 0);
};


#endif // SPRSEPARATECONTROLWIDGET_H
