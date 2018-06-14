#ifndef SPRSETTINGSMAINWIDGET_H
#define SPRSETTINGSMAINWIDGET_H

#include "ui_sprsettingsmainwidget.h"
#include "isprwidget.h"
#include "models/sprsettingsmainmodel.h"

class SPRSettingsMainWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRSettingsMainModel *model;

    TCPCommand *getStateCommand;

public:
    explicit SPRSettingsMainWidget(QWidget *parent = 0);

private:
    Ui::SPRSettingsMainWidget ui;

    // ISPRWidget interface
public:
    virtual ISPRModelData *setModelData(ISPRModelData *data);
    virtual ISPRModelData *getModelData();

    // ISPRWidget interface
public slots:
    virtual void widgetsShow();
protected slots:
    virtual void viewChange(int);
    virtual void viewChange();
    virtual void viewChange(bool);
    virtual void onModelChanget(IModelVariable *);
    void onServerConnectButtomClick(bool);
    void onGetStateCommandComplite(TCPCommand *command);
    void onErrorStateConnectServer(ITCPCommand *command);
signals:
    void doShow();
    void changeFileSettinds(QString);
    void changeFileSpectrum(QString);

    // ISPRWidget interface
protected:
};

#endif // SPRSETTINGSMAINWIDGET_H
