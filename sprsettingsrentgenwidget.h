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
    Ui::SPRSettingsRentgenWidget ui;

    explicit SPRSettingsRentgenWidget(QWidget *parent = 0);

    ISPRModelData *setModelData(SPRSettingsRentgenModel *value);

    void setThreads(SPRVariable<uint> *threads){
        model->setThreads(threads);
    }
    void setRentgens(SPRVariable<uint> *rentgens){
        model->setRentgens(rentgens);
    }
    virtual ISPRModelData *getModelData()
    {
        return model;
    }
private:

    // ISPRWidget interface
public slots:
    virtual void widgetsShow();
    virtual void onModelChanget(IModelVariable *);

protected:
    virtual void viewChange(QTableWidget *, int, int);

signals:
    void doShow();
    void doStore();

    // ISPRWidget interface
protected:
};

#endif // SPRSETTINGSRENTGENWIDGET_H
