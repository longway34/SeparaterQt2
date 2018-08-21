#ifndef SPRSETTINGSRENTGENMAINWIDGET_H
#define SPRSETTINGSRENTGENMAINWIDGET_H

#include <QDomDocument>

#include "ui_sprsettingsrentgenmainwidget.h"
#include "models/sprsettingsrentgenmodel.h"
#include "isprwidget.h"

#include "isprsettingswidget.h"

class SPRSettingsRentgenMainWidget : public QWidget, public ISPRWidget, public ISPRSettingsWidget
{
    Q_OBJECT

    SPRSettingsRentgenModel *model;
public:
    explicit SPRSettingsRentgenMainWidget(QWidget *parent = 0);

private:
    Ui::SPRSettingsRentgenMainWidget ui;

    // ISPRWidget interface
//    void setRentrenCodesTable();
//    void setRentrenVATable();
public:
    ISPRModelData *setModelData(ISPRModelData *data);
    virtual ISPRModelData *getModelData();

    void setThreads(SPRVariable<uint> *value);
    void setRentgens(SPRVariable<uint> *value);
public slots:
    virtual void widgetsShow();

signals:
    void doShow();

    // ISPRWidget interface
protected slots:
//    virtual void viewChange();
//    virtual void viewChange(QTableWidget *table, int row, int col);
    virtual void onModelChanget(IModelVariable *);

    // ISPRWidget interface
protected:

    // ISPRSettingsWidget interface
public:
    virtual void setMasterMode(bool value);
};

#endif // SPRSETTINGSRENTGENMAINWIDGET_H
