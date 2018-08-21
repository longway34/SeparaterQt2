#include "sprseparatedetailtablewidget.h"

SPRSeparateDetailTableWidget::SPRSeparateDetailTableWidget(QWidget *parent) :
    QWidget(parent), model(nullptr), separateModel(nullptr)
{
    ui.setupUi(this);

    connect(ui.bBeginScope, SIGNAL(clicked(bool)), this, SLOT(onClickButtomCommand(bool)));
    connect(ui.bEndScope, SIGNAL(clicked(bool)), this, SLOT(onClickButtomCommand(bool)));
    connect(ui.bClear, SIGNAL(clicked(bool)), this, SLOT(onClickButtomCommand(bool)));
}

SPRThreadList SPRSeparateDetailTableWidget::getVisbleThreads(){
    SPRThreadList res;
    QList<QCheckBox*> cbs = ui.gbThreads->findChildren<QCheckBox*>();
    for(uint8_t i=0; i<cbs.size(); i++){
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
            if(ui.tSeparateDetail->isStartingScope()){
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
            ui.tSeparateDetail->setMinStoneTime(ui.leMinimumTimeScope->value());
            ui.tSeparateDetail->startStopScope(true);
//            ui.tSeparateDetail->getMyModel()->setScopeData(0);

            ui.tSeparateDetailSummary->setVisibleThreads(getVisbleThreads());
            ui.tSeparateDetailSummary->setMinStoneTime(ui.leMinimumTimeScope->value());
            ui.tSeparateDetailSummary->startStopScope(true);

            ui.bEndScope->setEnabled(true);
            model->getSettingsControlModel()->tMeassureForData->setData(ui.leCommandTimeInterval->value());
        }
        return;
    }
    if(sender() == ui.bEndScope){
        if(model){
            ui.tSeparateDetail->startStopScope(false);
            ui.tSeparateDetailSummary->startStopScope(false);
            model->getSettingsControlModel()->tMeassureForData->restore();

            ui.bEndScope->setEnabled(false);
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
        ui.tSeparateDetail->setMinStoneTime(ui.leMinimumTimeScope->value());
        return;
    }
}

void SPRSeparateDetailTableWidget::onChangeThreadList(bool value){
    SPRThreadList visible = getVisbleThreads();
    ui.tSeparateDetail->setVisibleThreads(visible);
    ui.tSeparateDetail->widgetsShow();
}

ISPRModelData *SPRSeparateDetailTableWidget::setModelData(ISPRModelData *data)
{
    model = (SPRMainModel*)data;
    if(model){
        SPRSeparateModel *_separateModel = model->getSeparateModel();
        if(separateModel != _separateModel){
//            if(separateModel){
//                disconnect(separateModel, SIGNAL(modelChanget(IModelVariable*)), this->ui.tSeparateDetail, SLOT(onModelChanget(IModelVariable*)));
//                disconnect(separateModel, SIGNAL(modelChanget(IModelVariable*)), this->ui.tSeparateDetailSummary, SLOT(onModelChanget(IModelVariable*)));
//            }
            separateModel = _separateModel;
//            connect(separateModel, SIGNAL(modelChanget(IModelVariable*)), this->ui.tSeparateDetail, SLOT(onModelChanget(IModelVariable*)));
//            connect(separateModel, SIGNAL(modelChanget(IModelVariable*)), this->ui.tSeparateDetailSummary, SLOT(onModelChanget(IModelVariable*)));
        }
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
//        if(sender() == separateModel){
//            widgetsShow();
//        }
    }
}
