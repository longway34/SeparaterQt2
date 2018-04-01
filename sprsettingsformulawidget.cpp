#include "sprsettingsformulawidget.h"

SPRSettingsFormulaWidget::SPRSettingsFormulaWidget(QWidget *parent) :
    QWidget(parent), ISPRWidget()
{
    ui.setupUi(this);

    model = nullptr;

    bgCondition = new QButtonGroup();
    bgCondition->addButton(ui.rbH1Resume, QVariant(H1).toInt());
    bgCondition->addButton(ui.rbH2Resume, QVariant(H2).toInt());
    bgCondition->addButton(ui.rbH3Resume, QVariant(H3).toInt());

    connect(bgCondition, SIGNAL(buttonClicked(int)),SLOT(viewChange(int)));
    connect(this, SIGNAL(doShow()), ui.wFormula1, SLOT(widgetsShow()));
    connect(this, SIGNAL(doShow()), ui.wFormula2, SLOT(widgetsShow()));
    connect(this, SIGNAL(doShow()), ui.wFormula3, SLOT(widgetsShow()));


}

ISPRModelData *SPRSettingsFormulaWidget::setModel(ISPRModelData *model)
{
    QVector<SPRFormulaItemWidget*> widgets = {ui.wFormula1, ui.wFormula2, ui.wFormula3};
    SPRSettingsFormulaModel* sfm = (SPRSettingsFormulaModel*)model;
    this->model = sfm;
    for(int i=0; i< sfm->itemsModel.size();i++){
        int index = sfm->itemsModel[i]->index;
        widgets[index]->setModel(sfm->itemsModel[i]);
    }
    return model;
}

ISPRModelData *SPRSettingsFormulaWidget::setItemModel(ISPRModelData *model)
{
    QVector<SPRFormulaItemWidget*> widgets = {ui.wFormula1, ui.wFormula2, ui.wFormula3};
    SPRFormulaItemModel *fim = (SPRFormulaItemModel*)model;
    this->model->setModel(model);
    widgets[fim->index]->setModel(model);
}

void SPRSettingsFormulaWidget::setElements(const DefaultMapElements *elements)
{
    QList<SPRFormulaItemWidget*> list = QList<SPRFormulaItemWidget*>({
                                   ui.wFormula1, ui.wFormula2, ui.wFormula3
                                                            });
    QListIterator<SPRFormulaItemWidget*> it(list);
    while(it.hasNext()){
        it.next()->setElements(elements);
    }
}


void SPRSettingsFormulaWidget::widgetsShow()
{
    TypeConditions cond = model->getConditions()->getData();
    bgCondition->blockSignals(true);
    switch (cond) {
    case H1:
        ui.rbH1Resume->setChecked(true);
        break;
    case H2:
        ui.rbH2Resume->setChecked(true);
        break;
    case H3:
        ui.rbH3Resume->setChecked(true);
    default:
        break;
    }
    bgCondition->blockSignals(false);
}


ISPRModelData *SPRSettingsFormulaWidget::getModel()
{
    return model;
}

void SPRSettingsFormulaWidget::viewChange(int value)
{
    if(sender() == bgCondition){
       TypeConditions val = static_cast<TypeConditions>(value);
       if(model){
           SPRVariable<TypeConditions> *condv = model->getConditions();
            model->getConditions()->setData(val);
       }
       widgetsShow();
    }
    emit doShow();
}

