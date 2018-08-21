#ifndef SPRSETTINGSCONTROLWIDGET_H
#define SPRSETTINGSCONTROLWIDGET_H

#include "ui_sprsettingscontrolwidget.h"
#include "isprwidget.h"
#include "models/sprsettingscontrolmodel.h"
#include "isprsettingswidget.h"

class SPRSettingsControlWidget : public QWidget, public ISPRWidget, public ISPRSettingsWidget
{
    Q_OBJECT

    SPRSettingsControlModel *model;
public:
    explicit SPRSettingsControlWidget(QWidget *parent = 0);

private:
    Ui::SPRSettingsControlWidget ui;

    // ISPRWidget interface
public:
//    virtual ISPRModelData *setModelData(SPRSettingsControlModel *data);
    virtual ISPRModelData *setModelData(ISPRModelData *data);
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

    // ISPRSettingsWidget interface
public:
    virtual void setMasterMode(bool value);
};

#endif // SPRSETTINGSCONTROLWIDGET_H
