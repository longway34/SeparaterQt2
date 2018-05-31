#ifndef SPRSETTINGSCONTROLWIDGET_H
#define SPRSETTINGSCONTROLWIDGET_H

#include "ui_sprsettingscontrolwidget.h"
#include "isprwidget.h"
#include "models/sprsettingscontrolmodel.h"

class SPRSettingsControlWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRSettingsControlModel *model;
public:
    explicit SPRSettingsControlWidget(QWidget *parent = 0);

private:
    Ui::SPRSettingsControlWidget ui;

    // ISPRWidget interface
public:
    virtual ISPRModelData *setModelData(SPRSettingsControlModel *data);
    virtual ISPRModelData *getModelData();

    // ISPRWidget interface
public slots:
    virtual void viewChange();
    virtual void viewChange(bool value);
    virtual void widgetsShow();
    virtual void viewChange(int index);
    virtual void onModelChanget(IModelVariable *);



    // ISPRWidget interface
protected:
};

#endif // SPRSETTINGSCONTROLWIDGET_H
