#include "sprformulaitemwidget.h"

ISPRModelData *SPRFormulaItemWidget::setModel(ISPRModelData *value)
{
    model = (SPRFormulaItemModel*)value;

    QVector<SPRFormulaElement*> elems =
            {
                                            ui.lElementUp1,
                                            ui.lElementUp2,
                                            ui.lElementDown1,
                                            ui.lElementDown2,
                                            ui.lElementDown3,
                                            ui.lElementDown4};
    QVector<SPRElementVariable*> vars = {
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

    return model;
}

void SPRFormulaItemWidget::viewChange()
{
    double val = ((QLineEdit*)sender())->text().toDouble();
    if(sender() == ui.leMulUp){
        model->MulUp->setValue(val);
        return;
    }
    if(sender() == ui.leMulDown){
        model->MulDown->setValue(val);
        return;
    }
    if(sender() == ui.leMin){
        model->min->setValue(val);
        if(model->min->getValue() > model->max->getValue()){
            model->max->setValue(model->min->getValue());
            widgetsShow();
        }
        return;
    }
    if(sender() == ui.leMax){
        model->max->setValue(val);
        if(model->max->getValue()<model->min->getValue()){
            model->min->setValue(model->max->getValue());
            widgetsShow();
        }
        return;
    }
}

SPRFormulaItemWidget::SPRFormulaItemWidget(QWidget *parent) :
    QWidget(parent)
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
    ui.lElementUp1->setCurrentElement(model->ElementUp1->getValue());
    ui.lElementUp2->setCurrentElement(model->ElementUp2->getValue());
    ui.lElementDown1->setCurrentElement(model->ElementDown1->getValue());
    ui.lElementDown2->setCurrentElement(model->ElementDown2->getValue());
    ui.lElementDown3->setCurrentElement(model->ElementDown3->getValue());
    ui.lElementDown4->setCurrentElement(model->ElementDown4->getValue());

    ui.leMulUp->setText(model->MulUp->toString());
    ui.leMulDown->setText(model->MulDown->toString());
    ui.leMin->setText(model->min->toString());
    ui.leMax->setText(model->max->toString());
}


ISPRModelData *SPRFormulaItemWidget::getModel()
{
    return model;
}
