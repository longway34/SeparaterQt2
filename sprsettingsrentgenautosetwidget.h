#ifndef SPRSETTINGSRENTGENAUTOSETWIDGET_H
#define SPRSETTINGSRENTGENAUTOSETWIDGET_H

#include "ui_sprsettingsrentgenautosetwidget.h"
#include "models/sprsettingsrentgenmodel.h"
#include "isprwidget.h"

class SPRSettingsRentgenAutosetWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRSettingsRentgenModel *model;

    QButtonGroup *bgTypeSetting;

public:
    explicit SPRSettingsRentgenAutosetWidget(QWidget *parent = 0);

    ISPRModelData *setModel(SPRSettingsRentgenModel *value);

private:
    Ui::SPRSettingsRentgenAutosetWidget ui;

public:
    virtual ISPRModelData *getModel();

public slots:
    virtual void widgetsShow();

// ISPRWidget interface
protected slots:
//    virtual void viewChange(QTableWidget *, int, int);
    virtual void viewChange(bool value);
    virtual void viewChange(int value);

};

#endif // SPRSETTINGSRENTGENAUTOSETWIDGET_H
