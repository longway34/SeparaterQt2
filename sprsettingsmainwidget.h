#ifndef SPRSETTINGSMAINWIDGET_H
#define SPRSETTINGSMAINWIDGET_H

#include "ui_sprsettingsmainwidget.h"
#include "isprwidget.h"
#include "models/sprsettingsmainmodel.h"

class SPRSettingsMainWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRSettingsMainModel *model;
public:
    explicit SPRSettingsMainWidget(QWidget *parent = 0);

private:
    Ui::SPRSettingsMainWidget ui;

    // ISPRWidget interface
public:
    virtual ISPRModelData *setModel(ISPRModelData *data);
    virtual ISPRModelData *getModel();

    // ISPRWidget interface
public slots:
    virtual void widgetsShow();
protected slots:
    virtual void viewChange(int);
    virtual void viewChange();
    virtual void viewChange(bool);
signals:
    void doShow();
    void changeFileSettinds(QString);
    void changeFileSpectrum(QString);
};

#endif // SPRSETTINGSMAINWIDGET_H
