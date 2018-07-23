#ifndef SPRSETTINGSRENTGENAUTOSETWIDGET_H
#define SPRSETTINGSRENTGENAUTOSETWIDGET_H

#include "ui_sprsettingsrentgenautosetwidget.h"
#include "models/sprsettingsrentgenmodel.h"

#include "sprsettingsrentgenautosetdialog.h"
#include "isprwidget.h"

class SPRSettingsRentgenAutosetWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRSettingsRentgenModel *model;

    QButtonGroup *bgTypeSetting;

    SPRSettingsRentgenAutoSetDialog *autoSettingDialog;

public:
    explicit SPRSettingsRentgenAutosetWidget(QWidget *parent = 0);

    ISPRModelData *setModelData(SPRSettingsRentgenModel *value);

private:
    Ui::SPRSettingsRentgenAutosetWidget ui;

public:
    virtual ISPRModelData *getModelData();

public slots:
    virtual void widgetsShow();

// ISPRWidget interface
protected slots:
//    virtual void viewChange(QTableWidget *, int, int);
    virtual void viewChange(bool value);
    virtual void viewChange(int value);
    virtual void onModelChanget(IModelVariable *);


    // ISPRWidget interface
protected:
};

#endif // SPRSETTINGSRENTGENAUTOSETWIDGET_H
