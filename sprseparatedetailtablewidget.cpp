#include "sprseparatedetailtablewidget.h"

SPRSeparateDetailTableWidget::SPRSeparateDetailTableWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.bBeginScope, SIGNAL(clicked(bool)), this, SLOT(onClickButtomCommand(bool)));
    connect(ui.bEndScope, SIGNAL(clicked(bool)), this, SLOT(onClickButtomCommand(bool)));
    connect(ui.bClear, SIGNAL(clicked(bool)), this, SLOT(onClickButtomCommand(bool)));
}

QList<int> SPRSeparateDetailTableWidget::getVisbleThreads(){
    QList<int> res;
    QList<QCheckBox*> cbs = ui.gbThreads->findChildren<QCheckBox*>();
    for(int i=0; i<cbs.size(); i++){
        if(cbs[i]->isChecked()){
            if(cbs[i]->property("index").isValid()){
                res.push_back(cbs[i]->property("index").value<int>());
            } else {
                res.push_back(i);
            }
        }
    }
    return res;
}

void SPRSeparateDetailTableWidget::widgetsShow()
{
    if(model){
//        QSpacerItem *spacer = ui.gbThreads->findChild("hsTreads");
        QList<QCheckBox*> cbs = ui.gbThreads->findChildren<QCheckBox*>();
        QList<bool> oldThreadList;

        foreach(QCheckBox *th, cbs){
            if(th->isChecked()) oldThreadList.push_back(true); else oldThreadList.push_back(false);
            ui.gbThreads->layout()->removeWidget(th);
            delete th;
        }
        ui.gbThreads->layout()->removeItem(ui.hsTreads);
        blockSignals(true);
        for(uint8_t i=0; i<model->getThreads()->getData(); i++){
           QCheckBox *th = new QCheckBox(QString(tr("Ручей %1")).arg(QString::number(i+1)));
           th->setObjectName(QString("cb_%1").arg(QString::number(i+1)));
           th->setProperty("index", QVariant(i));
           th->setChecked(oldThreadList[i]);
           if(model->getServer()->isState(spr_state_separated)){
               th->setEnabled(true);
           } else {
               th->setEnabled(false);
           }
           connect(th, SIGNAL(toggled(bool)), this, SLOT(onChangeThreadList(bool)));
           ui.gbThreads->layout()->addWidget(th);
        }
        blockSignals(true);
        ui.gbThreads->layout()->addItem(ui.hsTreads);

//        if(model->getServer()->isState(spr_state_separated)){
//            if(ui.tSeparateDetail->getMyModel()->getScopeData() >=0){
//                ui.bBeginScope->setEnabled(false);
//                ui.bEndScope->setEnabled(true);
//            } else {
//                ui.bBeginScope->setEnabled(true);
//                ui.bEndScope->setEnabled(false);
//            }
//            ui.leCommandTimeInterval->setEnabled(true);
//            ui.leMinimumTimeScope->setEnabled(true);
//        } else {
//            ui.bBeginScope->setEnabled(false);
//            ui.bEndScope->setEnabled(false);
//            ui.leCommandTimeInterval->setEnabled(false);
//            ui.leMinimumTimeScope->setEnabled(false);
//        }
        onChandeServerState(0);
    }

//    ui.tSeparateDetail->widgetsShow();
}

void SPRSeparateDetailTableWidget::onChandeServerState(uint32_t){
    if(model){
        QList<QCheckBox*> cbs = ui.gbThreads->findChildren<QCheckBox*>();
        foreach(QCheckBox *cb, cbs){
            if(model->getServer()->isState(spr_state_separated)){
                cb->setEnabled(true);
            } else {
                cb->setEnabled(false);
            }
        }
        if(model->getServer()->isState(spr_state_separated)){
            if(ui.tSeparateDetail->getMyModel()->getScopeData() >=0){
                ui.bBeginScope->setEnabled(false);
                ui.bEndScope->setEnabled(true);
            } else {
                ui.bBeginScope->setEnabled(true);
                ui.bEndScope->setEnabled(false);
            }
            ui.leCommandTimeInterval->setEnabled(true);
            ui.leMinimumTimeScope->setEnabled(true);
        } else {
            ui.bBeginScope->setEnabled(false);
            ui.bEndScope->setEnabled(false);
            ui.leCommandTimeInterval->setEnabled(false);
            ui.leMinimumTimeScope->setEnabled(false);
        }
    }
}

void SPRSeparateDetailTableWidget::onClickButtomCommand(bool){
    if(sender() == ui.bBeginScope){
        if(model){
            ui.tSeparateDetail->getMyModel()->setVisibleThreads(getVisbleThreads());
            ui.tSeparateDetail->getMyModel()->setMinTimeScope(ui.leMinimumTimeScope->value());
            ui.tSeparateDetail->getMyModel()->setScopeData(0);

            ui.tSeparateDetailSummary->getMyModel()->setVisibleThreads(getVisbleThreads());
            ui.tSeparateDetailSummary->getMyModel()->setMinTimeScope(ui.leMinimumTimeScope->value());
            ui.tSeparateDetailSummary->getMyModel()->setScopeData(0);
            model->getSettingsControlModel()->tMeassureForData->setData(ui.leCommandTimeInterval->value());
        }
        return;
    }
    if(sender() == ui.bEndScope){
        if(model){
            ui.tSeparateDetail->getMyModel()->setScopeData(-1);
            ui.tSeparateDetailSummary->getMyModel()->setScopeData(-1);
            model->getSettingsControlModel()->tMeassureForData->restore();
        }
    }
    if(sender() == ui.bClear){
        if(model){
            ui.tSeparateDetail->clear();
            ui.tSeparateDetailSummary->clear();
        }
    }
}

void SPRSeparateDetailTableWidget::onChangeIntValue(int){
    if(sender() == ui.leCommandTimeInterval){
        model->getSettingsControlModel()->tMeassureForData->setData(ui.leCommandTimeInterval->value());
        return;
    }
    if(sender() == ui.leMinimumTimeScope){
        ui.tSeparateDetail->getMyModel()->setMinTimeScope(ui.leMinimumTimeScope->value());
        return;
    }
}

void SPRSeparateDetailTableWidget::onChangeThreadList(bool value){
    QList<int> visible = getVisbleThreads();
    ui.tSeparateDetail->getMyModel()->setVisibleThreads(visible);
    ui.tSeparateDetail->widgetsShow();
}

ISPRModelData *SPRSeparateDetailTableWidget::setModelData(ISPRModelData *data)
{
    model = (SPRMainModel*)data;
    if(model){
        connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        connect(model->getServer(), SIGNAL(serverStateChange(uint32_t)), this, SLOT(onChandeServerState(uint32_t)));
        ui.tSeparateDetail->setModelData(model);
        ui.tSeparateDetailSummary->setModelData(model);

        widgetsShow();
    }
}

ISPRModelData *SPRSeparateDetailTableWidget::getModelData()
{
    return model;
}

void SPRSeparateDetailTableWidget::onModelChanget(IModelVariable *variable)
{
    if(model){
        if(variable == model->getSettingsControlModel()->tMeassureForData){
            blockSignals(true);
            ui.leCommandTimeInterval->setValue(model->getSettingsControlModel()->tMeassureForData->getData());
            blockSignals(false);
        }
        if(variable == model->getSettingsMainModel()->getThreads()){
            widgetsShow();
        }
    }
}
