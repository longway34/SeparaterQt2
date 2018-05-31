#include "sprzonesrangestablewidget.h"
#include "sprelementpropertydialog.h"
#include "sprzonesranges.h"
#include "_types.h"

#include <QMessageBox>

ISPRModelData *SPRZonesRangesTableWidget::setModelData(SPRSpectrumZonesTableModel *value)
{
    if(value){
        if(model){
            disconnect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        }
        model = value;
        connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));

        ui.tZonesTable->setModelData(model);
    }
    widgetsShow();
    return model;
}

void SPRZonesRangesTableWidget::setSelectBottonRowVisible(bool value)
{
    selectBottonRowVisible = value;
    ui.tZonesTable->setSelectButtonEnable(selectBottonRowVisible);
//    widgetsShow();
}

SPRZonesRangesTableWidget::SPRZonesRangesTableWidget(QWidget *parent) :
    QWidget(parent), model(nullptr)
{
    ui.setupUi(this);

    connect(ui.bAddElement, SIGNAL(clicked(bool)), this, SLOT(onAddClick(bool)));
    connect(ui.bEditElement, SIGNAL(clicked(bool)), this, SLOT(onEditClick(bool)));
    connect(ui.bRoundRanges, SIGNAL(clicked(bool)), this, SLOT(onRoundClick(bool)));

    connect(ui.tZonesTable, SIGNAL(gotFocus(SPRZonesRanges*,QLineEdit*)), this, SLOT(setControlEnabled(SPRZonesRanges*,QLineEdit*)));
    editControlEnable = false;
    setSelectBottonRowVisible(true);

    ui.tZonesTable->resize(QSize(this->width(), ui.tZonesTable->height()));
}

void SPRZonesRangesTableWidget::onRoundClick(bool){
    bool noErrors = false;
    EnumElements el = ui.tZonesTable->getCurrentElement(&noErrors);
    if(noErrors){
        int tIndex = ui.tZonesTable->getLastCurrentThread();
        if(tIndex < 0){
            return;
        } else {
            uint min = ui.tZonesTable->getLastCurrentTable()->getZoneModel()->getZones()[el]->min->getData();
            uint max = ui.tZonesTable->getLastCurrentTable()->getZoneModel()->getZones()[el]->max->getData();
            for(int th=0; th<model->items.size(); th++){
                model->items[th]->setMinMax(el, min, max);
            }
        }
        widgetsShow();
    }
}

void SPRZonesRangesTableWidget::onEditClick(bool){
    bool noErrors = false;
    EnumElements el = ui.tZonesTable->getCurrentElement(&noErrors);
    if(noErrors){
        SPRZonesRanges *zr = ui.tZonesTable->getLastCurrentTable();
        if(zr){
            SPRSpectrumZonesModel *mod = zr->getZoneModel();
            SPRElementPropertyDialog *dialog = new SPRElementPropertyDialog(mod, el, this);
            int res = dialog->exec();
            if(res == QDialog::DialogCode::Accepted){
                if(model){
                        EnumElements el = dialog->ui.cbElements->currentData().value<EnumElements>();
                        QColor color = dialog->ui.fc->getColor();
                        QString sName = dialog->ui.leSName->text();
                        QString fName = dialog->ui.leFName->text();

                        int min = dialog->ui.leMin->value();
                        int max = dialog->ui.leMax->value();

                        model->elementsProperty->addElement(el, sName, fName, color);
                        model->setMinMaxValues(el, min, max, mod->tIndex);
        //            SPRElementsModel *_elModel = model->elementsProperty->addElement();
                }
            }
            delete dialog;
        }
    }
}

void SPRZonesRangesTableWidget::onAddClick(bool){
    SPRElementPropertyDialog *dialog = new SPRElementPropertyDialog(model->elementsProperty, this);


    int res = dialog->exec();

    if(res == QDialog::DialogCode::Accepted){
        if(model){
                EnumElements el = dialog->ui.cbElements->currentData().value<EnumElements>();
                QColor color = dialog->ui.fc->getColor();
                QString sName = dialog->ui.leSName->text();
                QString fName = dialog->ui.leFName->text();

                int min = dialog->ui.leMin->value();
                int max = dialog->ui.leMax->value();

                model->elementsProperty->addElement(el, sName, fName, color);
                model->setMinMaxValues(el, min, max);
//            SPRElementsModel *_elModel = model->elementsProperty->addElement();
        }
    }

    delete dialog;
    widgetsShow();
}

void SPRZonesRangesTableWidget::widgetsShow()
{
    if(model){
        ui.tZonesTable->widgetsShow();
        showEdabled();
    }
}

void SPRZonesRangesTableWidget::showEdabled(){
    if(editControlEnable){
        ui.bEditElement->setEnabled(true);
        ui.bRoundRanges->setEnabled(true);
    } else {
        ui.bEditElement->setEnabled(false);
        ui.bRoundRanges->setEnabled(false);
    }
    if(model->getElementsProperty()->getElements().size() < MAX_SPR_SPECTOR_ELEMENTS){
        ui.bAddElement->setEnabled(true);
    } else {
        ui.bAddElement->setEnabled(false);
    }

}

void SPRZonesRangesTableWidget::setControlEnabled(SPRZonesRanges*, QLineEdit *_el){
//    bool res = false;
//    ui.tZonesTable->getCurrentElement(&res);
    if(_el){
        editControlEnable = true;
//        ui.bEdidElement->setEnabled(true);
//        ui.bRoundRanges->setEnabled(true);
    } else {
        if(ui.tZonesTable->getLastCurrentTable()){
            bool res = false;
            ui.tZonesTable->getLastCurrentTable()->getCurrentElement(&res);
            if(res){
                editControlEnable = true;
            }
        } else {
            editControlEnable = false;
        }
//        ui.bEdidElement->setEnabled(false);
//        ui.bRoundRanges->setEnabled(false);
    }
//    if(model->getElementsProperty()->getElements().size() < MAX_SPR_SPECTOR_ELEMENTS){
//        ui.bAddElement->setEnabled(true);
//    } else {
//        ui.bAddElement->setEnabled(false);
//    }
    showEdabled();
}

ISPRModelData *SPRZonesRangesTableWidget::getModelData()
{
    return model;
}


void SPRZonesRangesTableWidget::onModelChanget(IModelVariable *)
{
    widgetsShow();
}
