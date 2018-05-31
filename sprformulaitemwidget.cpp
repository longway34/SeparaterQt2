#include "sprformulaitemwidget.h"

ISPRModelData *SPRFormulaItemWidget::setModelData(ISPRModelData *value)
{
    if(value){
        if(model){
            disconnect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        }
        model = (SPRFormulaItemModel*)value;
        connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));

        QVector<SPRFormulaElement*> elems =
                {
                                                ui.lElementUp1,
                                                ui.lElementUp2,
                                                ui.lElementDown1,
                                                ui.lElementDown2,
                                                ui.lElementDown3,
                                                ui.lElementDown4};
        QVector<SPREnumVariable<EnumElements>*> vars = {
            model->ElementUp1, model->ElementUp2,
            model->ElementDown1, model->ElementDown2,
            model->ElementDown3, model->ElementDown4
        };
        for(int i=0; i<elems.size(); i++){
            elems[i]->setElement(vars[i]);
        }
        ui.leMulUp->setValidator(new QDoubleValidator());
        ui.leMulDown->setValidator(new QDoubleValidator());
        ui.leMin->setValidator(new QDoubleValidator());
        ui.leMax->setValidator(new QDoubleValidator());


        ui.lResult->setText(QString("H")+QString::number(model->index+1));
        widgetsShow();
    }
    return model;
}

void SPRFormulaItemWidget::viewChange()
{
    double val = ((QLineEdit*)sender())->text().toDouble();
    if(sender() == ui.leMulUp){
        model->MulUp->setData(val);
        return;
    }
    if(sender() == ui.leMulDown){
        model->MulDown->setData(val);
        return;
    }
    if(sender() == ui.leMin){
        model->min->setData(val);
        if(model->min->getData() > model->max->getData()){
            model->max->setData(model->min->getData());
            widgetsShow();
        }
        return;
    }
    if(sender() == ui.leMax){
        model->max->setData(val);
        if(model->max->getData()<model->min->getData()){
            model->min->setData(model->max->getData());
            widgetsShow();
        }
        return;
    }
}

SPRFormulaItemWidget::SPRFormulaItemWidget(QWidget *parent) :
    QWidget(parent), model(nullptr)
{
    ui.setupUi(this);
    setElements(&DEF_SPR_FORMULA_ELEMENTS_PROPERTY);

    connect(ui.leMulUp, SIGNAL(editingFinished()),SLOT(viewChange()));
    connect(ui.leMulDown, SIGNAL(editingFinished()),SLOT(viewChange()));
    connect(ui.leMin, SIGNAL(editingFinished()),SLOT(viewChange()));
    connect(ui.leMax, SIGNAL(editingFinished()),SLOT(viewChange()));
}

void SPRFormulaItemWidget::setElements(const DefaultMapElements *elements)
{
    QList<SPRFormulaElement*> list =
            QList<SPRFormulaElement*>({
                                            ui.lElementUp1,
                                            ui.lElementUp2,
                                            ui.lElementDown1,
                                            ui.lElementDown2,
                                            ui.lElementDown3,
                                            ui.lElementDown4});
//    QVector<SPRElementVariable *
    QListIterator<SPRFormulaElement*> it(list);
    while(it.hasNext()){
        it.next()->setElements(elements);
    };
}


void SPRFormulaItemWidget::widgetsShow()
{
    if(model){
        ui.lElementUp1->setCurrentElement(model->ElementUp1->getData());
        ui.lElementUp2->setCurrentElement(model->ElementUp2->getData());
        ui.lElementDown1->setCurrentElement(model->ElementDown1->getData());
        ui.lElementDown2->setCurrentElement(model->ElementDown2->getData());
        ui.lElementDown3->setCurrentElement(model->ElementDown3->getData());
        ui.lElementDown4->setCurrentElement(model->ElementDown4->getData());

        ui.leMulUp->setText(model->MulUp->toString());
        ui.leMulDown->setText(model->MulDown->toString());
        ui.leMin->setText(model->min->toString());
        ui.leMax->setText(model->max->toString());
    }
}


ISPRModelData *SPRFormulaItemWidget::getModelData()
{
    return model;
}


void SPRFormulaItemWidget::onModelChanget(IModelVariable *)
{
    widgetsShow();
}
