#ifndef SPRSEPARATEWIDGET_H
#define SPRSEPARATEWIDGET_H

#include "ui_sprseparatewidget.h"
#include "isprwidget.h"

#include "models/sprmainmodel.h"
#include "isprwidget.h"
#include "sprsettingsporogswidget.h"


#include "tcp/tcpstartseparate2.h"
#include "tcp/tcpteststopseparate.h"


class SPRSeparateWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRMainModel *model;
    SPRSettingsPorogsWidget *wPorogs;

    SPRSpectrumListItemsModel *kSpectrumModel;

    TCPStartSeparate2 *startSeparate;
    TCPTestStopSeparate *stopSeparate;

    TCPCommand *setSeparate;

    TCPCommandSeparatorOnFull *separatorOn;
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
    void onKSpectrumReady(TCPGetSpectrumsGistogramms *_command);
public:
    virtual void setLogWidget(TCPLogsWigtets *value);
protected slots:
    void onCommandComplite(TCPCommand *command);
    void onSeparateCommandComplite(TCPCommand *command);
    void onCommandButtomClick(bool);
    void onServerStateChange(uint32_t);
};

#endif // SPRSEPARATEWIDGET_H
