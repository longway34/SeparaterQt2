#ifndef SPRSEPARATEWIDGET_H
#define SPRSEPARATEWIDGET_H

#include "ui_sprseparatewidget.h"
#include "isprwidget.h"

#include "models/sprmainmodel.h"
#include "isprwidget.h"
#include "sprsettingsporogswidget.h"
#include "models/sprhistorymodel.h"

#include "tcp/tcpstartseparate2.h"
#include "tcp/tcpteststopseparate.h"

#include "sprseparatecontrolwidget.h"

class SPRSeparateWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRMainModel *model;
    SPRHistoryModel *history;
    SPRSettingsPorogsWidget *wPorogs;

    SPRSpectrumListItemsModel *kSpectrumModel;

//    TCPStartSeparate2 *startSeparate;
//    TCPTestStopSeparate *stopSeparate;

    QDateTime lastKSpect;
    QDateTime lastGist;
    QDateTime lastGetSpect;

//    TCPGetSpectrumsGistogramms *forKSpertCommands;
//    TCPGetSpectrumsGistogramms *forHistCommands;

    TCPCommand *setSeparate;

//    TCPCommandSeparatorOnFull *separatorOn;
    TCPTimeOutWigget *toWidget;

public:
    Ui::SPRSeparateWidget ui;
    explicit SPRSeparateWidget(QWidget *parent = 0);

private:

    // ISPRWidget interface
public:
    virtual ISPRModelData *setModelData(ISPRModelData *data);
    virtual ISPRModelData *getModelData();

public slots:
    virtual void onModelChanget(IModelVariable *);
    virtual void widgetsShow();

    void onChangePororsCommand(bool);
protected:

    // ISPRWidget interface
    void onSpectrumReady(TCPGetSpectrumsGistogramms *_command);
    void setGraphicTitle();
public:
    virtual void setLogWidget(TCPLogsWigtets *value);
protected slots:
    void onCommandComplite(TCPCommand *command);
    void onSeparateCommandComplite(TCPCommand *command);
    void onCommandButtomClick(bool);
    void onServerStateChange(uint32_t);
    void onDblClickMouseEvent();
};

#endif // SPRSEPARATEWIDGET_H
