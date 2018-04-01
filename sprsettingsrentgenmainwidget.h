#ifndef SPRSETTINGSRENTGENMAINWIDGET_H
#define SPRSETTINGSRENTGENMAINWIDGET_H

#include <QDomDocument>

#include "ui_sprsettingsrentgenmainwidget.h"
#include "models/sprsettingsrentgenmodel.h"
#include "isprwidget.h"

class SPRSettingsRentgenMainWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRSettingsRentgenModel *model;
public:
    explicit SPRSettingsRentgenMainWidget(QWidget *parent = 0);

private:
    Ui::SPRSettingsRentgenMainWidget ui;

    // ISPRWidget interface
    void setRentrenCodesTable();
    void setRentrenVATable();
public:
    ISPRModelData *setModel(ISPRModelData *data);
    virtual ISPRModelData *getModel();

    void setThreads(SPRVariable<uint> *value);
    void setRentgens(SPRVariable<uint> *value);
public slots:
    virtual void widgetsShow();

signals:
    void doShow();

    // ISPRWidget interface
protected slots:
    virtual void viewChange();
    virtual void viewChange(QTableWidget *table, int row, int col);
};

#endif // SPRSETTINGSRENTGENMAINWIDGET_H
