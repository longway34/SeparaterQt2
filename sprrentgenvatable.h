#ifndef SPRRENTGENVATABLE_H
#define SPRRENTGENVATABLE_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>

#include "isprwidget.h"
#include "models/sprsettingsrentgenmodel.h"

class SPRRentgenVATable : public QTableWidget, public ISPRWidget
{
    Q_OBJECT

    SPRSettingsRentgenModel *model;
public:
    SPRRentgenVATable(QWidget *parent=nullptr);

    // ISPRWidget interface
public:
    virtual ISPRModelData *setModelData(ISPRModelData *data);
    virtual ISPRModelData *getModelData();

protected:
    virtual void onModelChanget(IModelVariable *);

public slots:
    virtual void widgetsShow();
    virtual void viewChange();
};

#endif // SPRRENTGENVATABLE_H
