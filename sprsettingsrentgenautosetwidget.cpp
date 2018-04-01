#include "sprsettingsrentgenautosetwidget.h"

ISPRModelData *SPRSettingsRentgenAutosetWidget::setModel(SPRSettingsRentgenModel *value)
{
    model = value;

    if(model){
        QVector<QCheckBox*> byConnectBool = {ui.cbChannel1, ui.cbChannel2, ui.cbChannel3, ui.cbChannel4, ui.cbAllChannels,
                                            ui.cbEnhansedRgime, ui.cbWithRGU};
        QVector<QSpinBox*> byConnectInt = {ui.leCodeBegin, ui.leCodeStep, ui.lePeakPosition};

        ui.cbChannel1->setChecked(model->withChannel[0]->getData());
        ui.cbChannel2->setChecked(model->withChannel[1]->getData());
        ui.cbChannel3->setChecked(model->withChannel[2]->getData());
        ui.cbChannel4->setChecked(model->withChannel[3]->getData());
        ui.cbEnhansedRgime->setChecked(false);
        ui.cbAllChannels->setChecked(false);
        ui.cbWithRGU->setChecked(model->withRGU->getData());

        for(int i=0; i<byConnectBool.size(); i++){
            connect(byConnectBool[i], SIGNAL(clicked(bool)), SLOT(viewChange(bool)));
        }
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
        widgetsShow();
    }
    return model;
}

SPRSettingsRentgenAutosetWidget::SPRSettingsRentgenAutosetWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
}


void SPRSettingsRentgenAutosetWidget::widgetsShow()
{
    QVector<QCheckBox*> chs = {ui.cbChannel1, ui.cbChannel2, ui.cbChannel3, ui.cbChannel4};
    for(int i=0; i< chs.size(); i++){
        if(i < model->getThreads()->getData()){
            chs[i]->setVisible(true);
        } else {
            chs[i]->setVisible(false);
        }
    }
}

ISPRModelData *SPRSettingsRentgenAutosetWidget::getModel()
{
    return model;
}

//void SPRSettingsRentgenAutosetWidget::viewChange(QTableWidget *, int, int)
//{
//}


void SPRSettingsRentgenAutosetWidget::viewChange(bool value)
{
    if(sender() == ui.cbAllChannels){
        QVector<QCheckBox*> chs = {ui.cbChannel1, ui.cbChannel2, ui.cbChannel3, ui.cbChannel4};
        for(int i=0; i<chs.size(); i++){
            chs[i]->setEnabled(!value);
            if(value){
                chs[i]->setChecked(true);
            }
        }
        return;
    }
    if(sender() == ui.cbChannel1){
        model->withChannel[0]->setData(value);
        return;
    }
    if(sender() == ui.cbChannel2){
        model->withChannel[1]->setData(value);
        return;
    }
    if(sender() == ui.cbChannel3){
        model->withChannel[2]->setData(value);
        return;
    }
    if(sender() == ui.cbChannel4){
        model->withChannel[3]->setData(value);
        return;
    }

    if(sender() == ui.cbWithRGU){
        model->withRGU->setData(value);
    }
    QVector<QWidget*> enh = {ui.rbSettsOnCP, ui.rbSettsOnDEUCP, ui.leCodeBegin, ui.leCodeStep};
    for(int i=0;i<enh.size(); i++){
        enh[i]->setEnabled(!value);
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
