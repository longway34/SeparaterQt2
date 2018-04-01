#ifndef SPRSETTINGSRENTGENCONTROLWIDGET_H
#define SPRSETTINGSRENTGENCONTROLWIDGET_H

#include "ui_sprsettingsrentgencontrolwidget.h"
#include "models/sprsettingsrentgenmodel.h"
#include "isprwidget.h"

class SPRSettingsRentgenControlWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRSettingsRentgenModel *model;
public:
    explicit SPRSettingsRentgenControlWidget(QWidget *parent = 0);

private:
    Ui::SPRSettingsRentgenControlWidget ui;

    // ISPRWidget interface
public:
    virtual ISPRModelData *setModel(SPRSettingsRentgenModel *data);
    virtual ISPRModelData *getModel()
    {
        return model;
    }

    void setThreads(SPRVariable<uint> *threads){
        model->setThreads(threads);
        createDEUCodesTable();
    }

protected:
    void createDEUCodesTable();

    // ISPRWidget interface
protected slots:
//    virtual void viewChange(QTableWidget *, int, int);
    virtual void viewChange(int value);
    virtual void viewChange(bool value);
    virtual void viewChange();
    virtual void widgetsShow();



    // ISPRWidget interface
public:
};



#endif // SPRSETTINGSRENTGENCONTROLWIDGET_H
