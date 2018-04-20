#include "sprsettingscontrolwidget.h"

SPRSettingsControlWidget::SPRSettingsControlWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    
    QVector<QAbstractSpinBox*> les = {
        ui.leWeightAvgConcentrate, ui.leWeightAvgTail,
        ui.leTimeMeassureData, ui.leTimeMeassureHistorramm, ui.leTimeMeassureSpector,
        ui.leVEMSLess, ui.leVEMSMaxCode, ui.leVEMSBegin,
        ui.leCorrectStream
    };
    for(int i=0; i<les.size(); i++){
        connect(les[i], SIGNAL(editingFinished()), this, SLOT(viewChange()));
    }

    ui.cbControlArea->clear();
    foreach (EnumElements el, DEF_SPR_FORMULA_ELEMENTS_PROPERTY.keys()) {
        QVariant value; value.setValue<EnumElements>(el);
        ui.cbControlArea->addItem(DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el].sname +" ("+DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el].name+")", value);
    }

    connect(ui.cbControlArea, SIGNAL(currentIndexChanged(int)), this, SLOT(viewChange(int)));
    connect(ui.cbAutoControlStream, SIGNAL(toggled(bool)), this, SLOT(viewChange(bool)));
}


void SPRSettingsControlWidget::widgetsShow()
{
}

ISPRModelData *SPRSettingsControlWidget::setModel(SPRSettingsControlModel *data)
{
    model = data;

    ui.tControl->clear();
    QStringList hTitle = {tr("Допустимо"), tr("Критично")};
    QStringList vTitle = {
      tr("Корреляция спектра"),
      tr("Скорость потока камней по ручью (шт./сек.)"),
      tr("Отклонение центра тяжести"),
      tr("Загрузка по воздуху минимум"),
      tr("Загрузка по воздуху максимум")
    };

    ui.tControl->setRowCount(5); ui.tControl->setColumnCount(2);
    ui.tControl->setHorizontalHeaderLabels(hTitle);
    ui.tControl->setVerticalHeaderLabels(vTitle);

    QLineEdit *le = setNumberCell(ui.tControl, 0, 0, model->correlSpectrumPermiss->getValue(), 0, 100, tr("Допустимое значение корреляции спектра (-1..1)"));
    le->setValidator(new QDoubleValidator(-1, 1, 2, le));
    le->setText(model->correlSpectrumPermiss->toString());
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 0, 1, model->correlSpectrumCritical->getValue(), 0, 100, tr("Критичное значение корреляции спектра (-1..1)"));
    le->setValidator(new QDoubleValidator(-1, 1, 2, le));
    le->setText(model->correlSpectrumCritical->toString());
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 1, 0, model->speedStreamPermiss->getValue(), 0, 25, tr("Допустимое значение скорости потока камней"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 1, 1, model->speedStreamCritical->getValue(), 0, 25, tr("Критичное значение скорости потока камней"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 2, 0, model->diffCenterGravityPermiss->getValue(), 0, 100, tr("Допустимое значение отклонения центра тяжести"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 2, 1, model->diffCenterGravityCritical->getValue(), 0, 100, tr("Критичное значение отклонения центра тяжести"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 3, 0, model->airLoadingMaxPermiss->getValue(), 0, 10000, tr("Допустимое максимальное значение загрузки по воздуху"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 3, 1, model->airLoadingMaxCritical->getValue(), 0, 10000, tr("Критичное максимальное значение загрузки по воздуху"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 4, 0, model->airLoadingMinPermiss->getValue(), 0, 10000, tr("Допустимое минимальное значение загрузки по воздуху"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 4, 1, model->airLoadingMinCritical->getValue(), 0, 10000, tr("Критичное минимальное значение загрузки по воздуху"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    ui.tControl->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui.tControl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
//    ui.tControl->resizeColumnsToContents();
    ui.tControl->setAlternatingRowColors(true);

    ui.leWeightAvgConcentrate->setValue(model->weightAvgStoneConcentrate->getValue());
    ui.leWeightAvgTail->setValue(model->weightAvgStoneTail->getValue());

    ui.leTimeMeassureData->setValue(model->tMeassureForData->getValue());
    ui.leTimeMeassureSpector->setValue(model->tMeassureForSpectrum->getValue());
    ui.leTimeMeassureHistorramm->setValue(model->tMeassureForHistogramm->getValue());

    ui.leCorrectStream->setValue(model->correctOptimalOreStream->getValue());
    ui.cbAutoControlStream->setChecked(model->autoOreStreamControl->getValue());

    ui.leVEMSLess->setValue(model->VEMSLevelLess->getValue());
    ui.leVEMSMaxCode->setValue(model->VEMSMaxCode->getValue());
    ui.leVEMSBegin->setValue(model->VEMSBeginCode->getValue());

    QVariant cur; cur.setValue<EnumElements>(model->controlArea->getValue());
    int curIndex = ui.cbControlArea->findData(cur);
    ui.cbControlArea->setCurrentIndex(curIndex);
    return model;
}

ISPRModelData *SPRSettingsControlWidget::getModel()
{
    return model;
}

void SPRSettingsControlWidget::viewChange()
{
    if(model){
        if(sender() == ui.leTimeMeassureData){
            model->tMeassureForData->setValue(ui.leTimeMeassureData->value());
            return;
        }
        if(sender() == ui.leTimeMeassureHistorramm){
            model->tMeassureForHistogramm->setValue(ui.leTimeMeassureHistorramm->value());
            return;
        }
        if(sender() == ui.leTimeMeassureSpector){
            model->tMeassureForSpectrum->setValue(ui.leTimeMeassureSpector->value());
            return;
        }
        if(sender() == ui.leWeightAvgConcentrate){
            model->weightAvgStoneConcentrate->setValue(ui.leWeightAvgConcentrate->value());
            return;
        }
        if(sender() == ui.leWeightAvgTail){
            model->weightAvgStoneTail->setValue(ui.leWeightAvgTail->value());
            return;
        }
        if(sender() == ui.leVEMSLess){
            model->VEMSLevelLess->setValue(ui.leVEMSLess->value());
            return;
        }
        if(sender() == ui.leVEMSMaxCode){
            model->VEMSMaxCode->setValue(ui.leVEMSMaxCode->value());
            return;
        }
        if(sender() == ui.leVEMSBegin){
            model->VEMSBeginCode->setValue(round(ui.leVEMSMaxCode->value()) * 20);
            return;
        }
        if(sender() == ui.leCorrectStream){
            model->correctOptimalOreStream->setValue(ui.leCorrectStream->value());
            return;
        }
        QLineEdit *le = (QLineEdit*)sender();
        QTableWidget *tw = le->property("tw").value<QTableWidget*>();
        if(tw == ui.tControl){
            int row = le->property("row").toInt();
            int col = le->property("col").toInt();
            if(row == 0){
                double value = le->text().toDouble();
                if(col == 0){
                    model->correlSpectrumPermiss->setValue(value);
                    return;
                }
                if(col == 1){
                    model->correlSpectrumCritical->setValue(value);
                    return;
                }
                return;
            }
            uint value = le->text().toInt();
            if(row == 1){
                if(col == 0){
                    model->speedStreamPermiss->setValue(value);
                    return;
                }
                if(col == 1){
                    model->speedStreamCritical->setValue(value);
                    return;
                }
                return;
            }
            if(row == 2){
                if(col == 0){
                    model->diffCenterGravityPermiss->setValue(value);
                    return;
                }
                if(col == 1){
                    model->diffCenterGravityCritical->setValue(value);
                    return;
                }
                return;
            }
            if(row == 3){
                if(col == 0){
                    model->airLoadingMinPermiss->setValue(value);
                    return;
                }
                if(col == 1){
                    model->airLoadingMinCritical->setValue(value);
                    return;
                }
                return;
            }
            if(row == 4){
                if(col == 0){
                    model->airLoadingMaxPermiss->setValue(value);
                    return;
                }
                if(col == 1){
                    model->airLoadingMaxCritical->setValue(value);
                    return;
                }
                return;
            }
        }
    }
}

void SPRSettingsControlWidget::viewChange(bool value)
{
    if(model){
        if(sender() == ui.cbAutoControlStream){
            model->autoOreStreamControl->setValue(value);
        }
    }
}


void SPRSettingsControlWidget::viewChange(int index)
{
    if(model){
        if(sender() == ui.cbControlArea){
            EnumElements el = ui.cbControlArea->itemData(index).value<EnumElements>();
            model->controlArea->setValue(el);
        }
    }
}

