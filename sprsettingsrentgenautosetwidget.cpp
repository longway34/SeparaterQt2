#include "sprsettingsrentgenautosetwidget.h"

ISPRModelData *SPRSettingsRentgenAutosetWidget::setModelData(SPRSettingsRentgenModel *value)
{

    if(value){
        if(model){
            disconnect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        }
        model = value;
        connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));

//        QVector<QCheckBox*> byConnectBool = {ui.cbChannel1, ui.cbChannel2, ui.cbChannel3, ui.cbChannel4, ui.gbAllChannels,
//                                            ui.cbWithRGU};
        QVector<QSpinBox*> byConnectInt = {ui.leCodeBegin, ui.leCodeStep, ui.lePeakPosition};

//        ui.cbChannel1->setChecked(model->withChannel[0]->getData());
//        ui.cbChannel2->setChecked(model->withChannel[1]->getData());
//        ui.cbChannel3->setChecked(model->withChannel[2]->getData());
//        ui.cbChannel4->setChecked(model->withChannel[3]->getData());
//        ui.cbEnhansedRgime->setChecked(false);
        connect(ui.gbAllChannels, SIGNAL(toggled(bool)), this, SLOT(viewChange(bool)));
        ui.gbAllChannels->setChecked(true);
        ui.cbWithRGU->setChecked(model->withRGU->getData());

//        for(int i=0; i<byConnectBool.size(); i++){
//            connect(byConnectBool[i], SIGNAL(clicked(bool)), SLOT(viewChange(bool)));
//        }
        ui.leCodeBegin->setValue(model->codeBegin->getData());
        ui.leCodeStep->setValue(model->codeStep->getData());
        ui.lePeakPosition->setValue(model->peakPosition->getData());
        for(int i=0; i<byConnectInt.size(); i++){
            connect(byConnectInt[i], SIGNAL(valueChanged(int)), SLOT(viewChange(int)));
        }

//        connect(ui.cbChannel1, SIGNAL(clicked(bool)), this, SLOT(viewChange(bool)));
//        connect(ui.cbChannel2, SIGNAL(clicked(bool)), this, SLOT(viewChange(bool)));
//        connect(ui.cbChannel3, SIGNAL(clicked(bool)), this, SLOT(viewChange(bool)));
//        connect(ui.cbChannel4, SIGNAL(clicked(bool)), this, SLOT(viewChange(bool)));

//        connect(ui.cbAllChannels, SLOT(clicked(bool)), this, SLOT(viewChange(bool)));
        bgTypeSetting = new QButtonGroup();
        bgTypeSetting->addButton(ui.rbSettsOnCP, 0);
        bgTypeSetting->addButton(ui.rbSettsOnDEUCP, 1);
        connect(bgTypeSetting, SIGNAL(buttonClicked(int)), this, SLOT(viewChange(int)));

        ui.bStart->setEnabled(true);

        autoSettingDialog->setModelData(model);

        widgetsShow();
    }
    return model;
}

SPRSettingsRentgenAutosetWidget::SPRSettingsRentgenAutosetWidget(QWidget *parent) :
    QWidget(parent), model(nullptr)
{
    ui.setupUi(this);

    autoSettingDialog = new SPRSettingsRentgenAutoSetDialog();

    connect(ui.bStart, SIGNAL(clicked(bool)), this, SLOT(viewChange(bool)));
}

QList<int> SPRSettingsRentgenAutosetWidget::getSelectedThreads(){
    QList<int> res;
    QList<QCheckBox*> lcb = ui.gbAllChannels->findChildren<QCheckBox*>();
    for(int th=0; th<lcb.size(); th++){
        if(lcb[th]->isChecked() || ui.gbAllChannels->isChecked()){
            if(lcb[th]->property("thread").isValid()){
                res.push_back(lcb[th]->property("thread").toInt());
            } else {
                res.push_back(th);
            }
        }
    }
    return res;
}

void SPRSettingsRentgenAutosetWidget::widgetsShow()
{
    if(model){
        blockSignals(true);
        bool allChSelected = true;
        for(int th=0; th<model->getThreads()->getData(); th++){
            if(!model->withChannel[th]->getData()){
                allChSelected = false;
                break;
            }
        }
        ui.gbAllChannels->setChecked(allChSelected);
        QList<int> thSelected = getSelectedThreads();
//        QSpacerItem *ui.chSpacer;
        QList<QCheckBox*> lcb = ui.wAllChannels->findChildren<QCheckBox*>();
//        QWidget *wCh = ui.gbAllChannels->findChildren<QWidget*>().first();
        QGridLayout *lau = (QGridLayout*)ui.wAllChannels->layout();
        foreach(QCheckBox* ch, lcb){
            lau->removeWidget(ch);
            disconnect(ch, SIGNAL(toggled(bool)), this, SLOT(viewChange(bool)));
            delete ch;
        }
//        lau->removeItem(ui.chSpacer);

        Qt::Alignment al = lau->alignment();

        ui.lePeakPosition->setValue(model->peakPosition->getData());

        int threads = model->getThreads()->getData();
//        int spanSpacer = threads > 4 ? 2 : 1;
        for(int th=0; th<threads; th++){
            int row = th % 4; int col = th > 4 ? 1 : 0;
            QCheckBox *cb = new QCheckBox(QString(tr("%1 Канал")).arg(th+1));
            cb->setObjectName(QString("cbThread%1").arg(th));
            QVariant prop; prop.setValue<int>(th);
            cb->setProperty("thread", prop);

//            cb->setChecked(model->withChannel[th]->getData());
            if(thSelected.contains(th) || ui.gbAllChannels->isChecked()){
                cb->setChecked(true);
            } else {
                cb->setChecked(false);
            }
            lau->addWidget(cb, row, col);
            connect(cb, SIGNAL(toggled(bool)), this, SLOT(viewChange(bool)));
        }
//        lau->addItem(ui.chSpacer, 4, 0, 1, spanSpacer);

//        QVector<QCheckBox*> chs = {ui.cbChannel1, ui.cbChannel2, ui.cbChannel3, ui.cbChannel4};
//        for(int i=0; i< chs.size(); i++){
//            if(i < model->getThreads()->getData()){
//                chs[i]->setVisible(true);
//            } else {
//                chs[i]->setVisible(false);
//            }
//        }
        blockSignals(false);
    }
}

ISPRModelData *SPRSettingsRentgenAutosetWidget::getModelData()
{
    return model;
}

//void SPRSettingsRentgenAutosetWidget::viewChange(QTableWidget *, int, int)
//{
//}


void SPRSettingsRentgenAutosetWidget::viewChange(bool value)
{
    if(model){
        if(sender() == ui.gbAllChannels){
            QList<QCheckBox*> chs = ui.gbAllChannels->findChildren<QCheckBox*>();
            for(int i=0; i<chs.size(); i++){
//                chs[i]->setEnabled(!value);
                if(value){
                    chs[i]->setChecked(true);
                }
            }
            ui.wAllChannels->setEnabled(!value);
            return;
        }
//        if(sender()->objectName().startsWith("cbThread", Qt::CaseInsensitive)){
//            if(sender()->property("thread").isValid()){
//                int th = sender()->property("thread").value<int>();
//                if(th < 0 || th > MAX_SPR_MAIN_THREADS){
//                    return;
//                }
//                blockSignals(true);
////                model->withChannel[th]->setData(value);
//                blockSignals(false);
//            }
//            return;
//        }

        if(sender() == ui.cbWithRGU){
            model->withRGU->setData(value);
            return;
        }
//        QVector<QWidget*> enh = {ui.leCodeBegin, ui.leCodeStep};
//        for(int i=0;i<enh.size(); i++){
//            enh[i]->setEnabled(!value);
//        }

        if(sender() == ui.bStart){
            QList<int> threads = getSelectedThreads();

            autoSettingDialog->setThreads(threads);
            autoSettingDialog->widgetsShow();
            autoSettingDialog->show();
        }
    }
}


void SPRSettingsRentgenAutosetWidget::viewChange(int value)
{
    if(sender() == ui.leCodeBegin){
        model->codeBegin->setData(value);
        return;
    }
    if(sender() == ui.leCodeStep){
        model->codeStep->setData(value);
        return;
    }
    if(sender() == ui.lePeakPosition){
        model->peakPosition->setData(value);
        return;
    }

    if(sender() == bgTypeSetting){
        TypeAutoSettingRMT as = static_cast<TypeAutoSettingRMT>(value);
        model->typeSetting->setData(as);
        return;
    }
}


void SPRSettingsRentgenAutosetWidget::onModelChanget(IModelVariable *)
{
    widgetsShow();
}
