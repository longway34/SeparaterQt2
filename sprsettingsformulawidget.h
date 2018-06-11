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

    ISPRModelData *setModelData(ISPRModelData *model);
    ISPRModelData *setItemModel(ISPRModelData *model);
public slots:
//    void setElements(const DefaultMapElements *elements);
    void setConditions(SPREnumVariable<TypeConditions>* cond){this->model->setCondition(cond);}

    // ISPRWidget interface
public slots:
    virtual void widgetsShow();
    virtual ISPRModelData *getModelData();
    virtual void viewChange(int value);
    virtual void onModelChanget(IModelVariable *);
signals:
    void doShow();

    // ISPRWidget interface


    // ISPRWidget interface
protected:
};

#endif // SPRSETTINGSFORMULAWIDGET_H
