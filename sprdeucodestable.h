#ifndef SPRDEUCODESTABLE_H
#define SPRDEUCODESTABLE_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>

#include "isprwidget.h"
#include "models/sprsettingsrentgenmodel.h"

class SPRDEUCodesTable : public QTableWidget, public ISPRWidget
{

    Q_OBJECT

    SPRSettingsRentgenModel *model;

public:
    explicit SPRDEUCodesTable(QWidget *parent=0);

    // ISPRWidget interface
    virtual ISPRModelData *getModelData();
    virtual ISPRModelData *setModelData(SPRSettingsRentgenModel *data);
public slots:
    virtual void widgetsShow();
    virtual void viewChange();
    virtual void onModelChanget(IModelVariable *);

    // ISPRWidget interface
protected slots:

    // ISPRWidget interface
protected:
};









#endif // SPRDEUCODESTABLE_H
