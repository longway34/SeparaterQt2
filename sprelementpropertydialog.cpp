#include "sprelementpropertydialog.h"

#include <QList>

SPRElementPropertyDialog::SPRElementPropertyDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
}

SPRElementPropertyDialog::SPRElementPropertyDialog(SPRSpectrumZonesModel *_model, EnumElements _element, QWidget *parent):
    QDialog(parent)
{
    Init(nullptr, _model, QList<EnumElements>({_element}));
}

SPRElementPropertyDialog::SPRElementPropertyDialog(SPRElementsModel *_model, QWidget *parent):
    QDialog(parent)
{
    Init(_model);
}

void SPRElementPropertyDialog::Init(SPRElementsModel *_model, SPRSpectrumZonesModel *_spzModel, QList<EnumElements> _elements){
    ui.setupUi(this);
    this->spzModel = _spzModel;
    this->elModel = _model;
    if(spzModel){
        elModel = spzModel->getElementsProperty();
    }

    this->elements = _elements;

    ui.cbElements->clear();
    if(elModel){
        QList<EnumElements> listElementsforCB = elModel->getUnisedElements().keys();
        ui.fc->setDeleteVisible(false);
        ui.fc->setSelectVisible(false);

        ui.leMin->setRange(1, 256);
        ui.leMax->setRange(1, 256);

        connect(ui.leMin, SIGNAL(valueChanged(int)), this, SLOT(onChangeMinMax(int)));
        connect(ui.leMax, SIGNAL(valueChanged(int)), this, SLOT(onChangeMinMax(int)));

        connect(ui.cbElements, SIGNAL(currentIndexChanged(int)), this, SLOT(onElementIndexChange(int)));
        if(elements.size() > 0){ // редактирование одного элемента
            listElementsforCB.clear();
            EnumElements el = elements.at(0);
            listElementsforCB.append(el);
            ui.cbElements->addItem(QString("%1(%2)").arg(elModel->getFName(el)).arg(QString::number(static_cast<int>(el))), QVariant(static_cast<int>(el)));
            ui.cbElements->setCurrentIndex(0);
            ui.cbElements->setEditable(false);


        } else { // Ищем доступные для нового элемента
//            foreach(EnumElements el, elModel->getUsedlElement()){
//                if(listElementsforCB.contains(el)){
//                    listElementsforCB.removeOne(el);
//                }
//            }
            foreach(EnumElements el, listElementsforCB){
                QString t = QString(tr("свободно(%1)")).arg(QString::number(static_cast<int>(el)));
                ui.cbElements->addItem(t, QVariant(static_cast<int>(el)));
            }
            ui.cbElements->setCurrentIndex(0);
            ui.cbElements->setEditable(true);

        }
    }
    widgetsShow();

}

void SPRElementPropertyDialog::widgetsShow(){
    EnumElements el = ui.cbElements->currentData().value<EnumElements>();

    ui.fc->setColor(DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el].color);
    ui.fc->setText(DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el].sname);

    ui.leFName->setText(DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el].name);
    ui.leSName->setText(DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el].sname);

    if(elModel){
        if(elModel->getUsedlElementKeys().contains(el)){

            ui.fc->setColor(elModel->getColor(el));
            ui.fc->setText(elModel->getSName(el));

            ui.leFName->setText(elModel->getFName(el));
            ui.leSName->setText(elModel->getSName(el));
        }
    }
    ui.fc->widgetsShow();

    ui.leMin->setValue(DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el].min);
    ui.leMax->setValue(DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el].max);

    if(spzModel){
        QList<EnumElements> kk = spzModel->getZones().keys();
        if(kk.contains(el)){
            ui.leMin->setValue(spzModel->getZones()[el]->min->getData());
            ui.leMax->setValue(spzModel->getZones()[el]->max->getData());
        }
    }
    onChangeMinMax(0);
}

void SPRElementPropertyDialog::onChangeMinMax(int){
    ui.lTitle->setText(QString("%1 (%2) %3-%4 канал").arg(ui.leFName->text()).arg(ui.leSName->text()).arg(ui.leMin->text()).arg(ui.leMax->text()));
}

void SPRElementPropertyDialog::onElementIndexChange(int){
    widgetsShow();
}
