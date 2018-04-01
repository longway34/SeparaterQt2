#ifndef SPRSETTINGSFORMULAWIDGET_H
#define SPRSETTINGSFORMULAWIDGET_H

#include <QButtonGroup>

#include "ui_sprsettingsformulawidget.h"
#include "isprwidget.h"
#include "models/sprformulaitemmodel.h"

#include "models/sprsettingsformulamodel.h"

class SPRSettingsFormulaWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRSettingsFormulaModel *model;
public:

    QButtonGroup *bgCondition;

    explicit SPRSettingsFormulaWidget(QWidget *parent = 0);
    Ui::SPRSettingsFormulaWidget ui;

    ISPRModelData *setModel(ISPRModelData *model);
    ISPRModelData *setItemModel(ISPRModelData *model);
public slots:
    void setElements(const DefaultMapElements *elements);
    void setConditions(SPRVariable<TypeConditions>* cond){this->model->setCondition(cond);}

    // ISPRWidget interface
public slots:
    virtual void widgetsShow();
    virtual ISPRModelData *getModel();
    virtual void viewChange(int value);
signals:
    void doShow();

    // ISPRWidget interface

};

#endif // SPRSETTINGSFORMULAWIDGET_H
