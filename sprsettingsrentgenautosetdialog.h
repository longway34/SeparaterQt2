#ifndef SPRSETTINGSRENTGENAUTOSETDIALOG_H
#define SPRSETTINGSRENTGENAUTOSETDIALOG_H

#include "ui_sprsettingrentgenautosetdialog.h"

#include "models/sprmainmodel.h"


class SPRSettingsRentgenAutoSetDialog : public QDialog
{
    Q_OBJECT

    SPRMainModel *mainModel;
public:
    explicit SPRSettingsRentgenAutoSetDialog(QWidget *parent = 0);
    Ui::SPRSettingRentgenAutoSetDialog ui;

    SPRMainModel *getModelData();
    void setModelData(SPRMainModel *value);


public slots:
    virtual void widgetsShow();

private:


};

#endif // SPRSETTINGSRENTGENAUTOSETDIALOG_H
