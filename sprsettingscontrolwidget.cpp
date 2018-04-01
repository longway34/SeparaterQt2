#include "sprsettingscontrolwidget.h"

SPRSettingsControlWidget::SPRSettingsControlWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    
    QVector<QAbstractSpinBox*> les = {
        ui.leWeightAvgConcentrate, ui.leWeightAvgTail,
        ui.leTimeMeassureData, ui.leTimeMeassureHistorramm, ui.leTimeMeassureSpector,
        ui.leVEMSLess, ui.leVEMSMaxCode,
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

    QLineEdit *le = setNumberCell(ui.tControl, 0, 0, model->correlSpectrumPermiss->getData(), 0, 100, tr("Допустимое значение корреляции спектра (-1..1)"));
    le->setValidator(new QDoubleValidator(-1, 1, 2, le));
    le->setText(model->correlSpectrumPermiss->toString());
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 0, 1, model->correlSpectrumCritical->getData(), 0, 100, tr("Критичное значение корреляции спектра (-1..1)"));
    le->setValidator(new QDoubleValidator(-1, 1, 2, le));
    le->setText(model->correlSpectrumCritical->toString());
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 1, 0, model->speedStreamPermiss->getData(), 0, 25, tr("Допустимое значение скорости потока камней"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 1, 1, model->speedStreamCritical->getData(), 0, 25, tr("Критичное значение скорости потока камней"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 2, 0, model->diffCenterGravityPermiss->getData(), 0, 100, tr("Допустимое значение отклонения центра тяжести"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 2, 1, model->diffCenterGravityCritical->getData(), 0, 100, tr("Критичное значение отклонения центра тяжести"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 3, 0, model->airLoadingMaxPermiss->getData(), 0, 10000, tr("Допустимое максимальное значение загрузки по воздуху"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 3, 1, model->airLoadingMaxCritical->getData(), 0, 10000, tr("Критичное максимальное значение загрузки по воздуху"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 4, 0, model->airLoadingMinPermiss->getData(), 0, 10000, tr("Допустимое минимальное значение загрузки по воздуху"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tControl, 4, 1, model->airLoadingMinCritical->getData(), 0, 10000, tr("Критичное минимальное значение загрузки по воздуху"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    ui.tControl->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui.tControl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
//    ui.tControl->resizeColumnsToContents();
    ui.tControl->setAlternatingRowColors(true);

    ui.leWeightAvgConcentrate->setValue(model->weightAvgStoneConcentrate->getData());
    ui.leWeightAvgTail->setValue(model->weightAvgStoneTail->getData());

    ui.leTimeMeassureData->setValue(model->tMeassureForData->getData());
    ui.leTimeMeassureSpector->setValue(model->tMeassureForSpectrum->getData());
    ui.leTimeMeassureHistorramm->setValue(model->tMeassureForHistogramm->getData());

    ui.leCorrectStream->setValue(model->correctOptimalOreStream->getData());
    ui.cbAutoControlStream->setChecked(model->autoOreStreamControl->getData());

    ui.leVEMSLess->setValue(model->VEMSLevelLess->getData());
    ui.leVEMSMaxCode->setValue(model->VEMSMaxCode->getData());

    QVariant cur; cur.setValue<EnumElements>(model->controlArea->getData());
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
    if(sender() == ui.leTimeMeassureData){
        model->tMeassureForData->setData(ui.leTimeMeassureData->value());
        return;
    }
    if(sender() == ui.leTimeMeassureHistorramm){
        model->tMeassureForHistogramm->setData(ui.leTimeMeassureHistorramm->value());
        return;
    }
    if(sender() == ui.leTimeMeassureSpector){
        model->tMeassureForSpectrum->setData(ui.leTimeMeassureSpector->value());
        return;
    }
    if(sender() == ui.leWeightAvgConcentrate){
        model->weightAvgStoneConcentrate->setData(ui.leWeightAvgConcentrate->value());
        return;
    }
    if(sender() == ui.leWeightAvgTail){
        model->weightAvgStoneTail->setData(ui.leWeightAvgTail->value());
        return;
    }
    if(sender() == ui.leVEMSLess){
        model->VEMSLevelLess->setData(ui.leVEMSLess->value());
        return;
    }
    if(sender() == ui.leVEMSMaxCode){
        model->VEMSMaxCode->setData(ui.leVEMSMaxCode->value());
        return;
    }
    if(sender() == ui.leCorrectStream){
        model->correctOptimalOreStream->setData(ui.leCorrectStream->value());
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
                model->correlSpectrumPermiss->setData(value);
                return;
            }
            if(col == 1){
                model->correlSpectrumCritical->setData(value);
                return;
            }
            return;
        }
        uint value = le->text().toInt();
        if(row == 1){
            if(col == 0){
                model->speedStreamPermiss->setData(value);
                return;
            }
            if(col == 1){
                model->speedStreamCritical->setData(value);
                return;
            }
            return;
        }
        if(row == 2){
            if(col == 0){
                model->diffCenterGravityPermiss->setData(value);
                return;
            }
            if(col == 1){
                model->diffCenterGravityCritical->setData(value);
                return;
            }
            return;
        }
        if(row == 3){
            if(col == 0){
                model->airLoadingMinPermiss->setData(value);
                return;
            }
            if(col == 1){
                model->airLoadingMinCritical->setData(value);
                return;
            }
            return;
        }
        if(row == 4){
            if(col == 0){
                model->airLoadingMaxPermiss->setData(value);
                return;
            }
            if(col == 1){
                model->airLoadingMaxCritical->setData(value);
                return;
            }
            return;
        }
    }
}

void SPRSettingsControlWidget::viewChange(bool value)
{
    if(sender() == ui.cbAutoControlStream){
        model->autoOreStreamControl->setData(value);
    }
}


void SPRSettingsControlWidget::viewChange(int index)
{
    if(sender() == ui.cbControlArea){
        EnumElements el = ui.cbControlArea->itemData(index).value<EnumElements>();
        model->controlArea->setData(el);
    }
}

