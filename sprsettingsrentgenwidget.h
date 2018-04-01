#ifndef SPRSETTINGSRENTGENWIDGET_H
#define SPRSETTINGSRENTGENWIDGET_H

#include "ui_sprsettingsrentgenwidget.h"
#include "models/isprmodeldata.h"
#include "isprwidget.h"

class SPRSettingsRentgenWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRSettingsRentgenModel *model;

public:
    explicit SPRSettingsRentgenWidget(QWidget *parent = 0);

    ISPRModelData *setModel(SPRSettingsRentgenModel *value);

    void setThreads(SPRVariable<uint> *threads){
        model->setThreads(threads);
    }
    void setRentgens(SPRVariable<uint> *rentgens){
        model->setRentgens(rentgens);
    }
    virtual ISPRModelData *getModel()
    {
        return model;
    }
private:
    Ui::SPRSettingsRentgenWidget ui;

    // ISPRWidget interface
public slots:
    virtual void widgetsShow();

protected:
    virtual void viewChange(QTableWidget *, int, int);

signals:
    void doShow();
    void doStore();
};

#endif // SPRSETTINGSRENTGENWIDGET_H
