#include "sprsettingsrentgencontrolwidget.h"

SPRSettingsRentgenControlWidget::SPRSettingsRentgenControlWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
}


void SPRSettingsRentgenControlWidget::widgetsShow()
{
    createDEUCodesTable();
}

ISPRModelData *SPRSettingsRentgenControlWidget::setModel(SPRSettingsRentgenModel *data)
{
    if(model){
        model = data;

        ui.tDiffParams->clear();
        ui.tDiffParams->setRowCount(1);
        QStringList nameV; nameV << tr("%"); ui.tDiffParams->setVerticalHeaderLabels(nameV);

        QString tt = QString(tr("Допустимое значение отклонения параметров (0-100), (%)"));
        QLineEdit *le = setNumberCell(ui.tDiffParams, 0, 0, model->diffParamAllow->getData(), 0, 100, tt);
        connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
        tt = QString(tr("Критичное значение отклонения параметров (0-100), (%)"));
        le = setNumberCell(ui.tDiffParams, 0, 1, model->diffParamDisallow->getData(), 0, 100, tt);
        connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
        QStringList nameH; nameH << tr("Допустимое") << tr("Критичное");
        ui.tDiffParams->setHorizontalHeaderLabels(nameH);

        ui.slTimeMoveRGU->setValue(model->timeMoveRGU->getData());

        ui.tPapamsRA->clear();
        ui.tPapamsRA->setColumnCount(3);
        ui.tPapamsRA->setRowCount(1);
        QStringList hv; hv << tr("Включение") << tr("Выключение") << tr("Прогрев");
        ui.tPapamsRA->setHorizontalHeaderLabels(hv);
        QStringList hh; hh << tr("Время, (сек.)");
        ui.tPapamsRA->setVerticalHeaderLabels(hh);

        tt = tr("Время включения рентгеноского аррарата (0-60), (сек.)");
        le = setNumberCell(ui.tPapamsRA, 0, 0, model->timeOnRA->getData(), 0, 60, tt);
        connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
        tt = tr("Время выключения рентгеноского аррарата (0-60), (сек.)");
        le = setNumberCell(ui.tPapamsRA, 0, 1, model->timeOffRA->getData(), 0, 60, tt);
        connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
        tt = tr("Время прогрева рентгеноского аррарата (0-60), (сек.)");
        le = setNumberCell(ui.tPapamsRA, 0, 2, model->timeHotRA->getData(), 0, 60, tt);
        connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
        ui.tPapamsRA->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
        ui.tPapamsRA->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
        ui.tPapamsRA->blockSignals(false);

        createDEUCodesTable();

        connect(ui.cbWOControlPlace, SIGNAL(toggled(bool)), SLOT(viewChange(bool)));
        connect(ui.slTimeMoveRGU, SIGNAL(valueChanged(int)), this, SLOT(viewChange(int)));
    }
}

void SPRSettingsRentgenControlWidget::createDEUCodesTable(){
    ui.tDEUCode->blockSignals(true);
    ui.tDEUCode->clear();
    ui.tDEUCode->setColumnCount(model->getThreads()->getData());
    ui.tDEUCode->setRowCount(2);
    QStringList nameV; nameV << tr("ДЭУ") << tr("ЦП");
    ui.tDEUCode->setVerticalHeaderLabels(nameV);
    QStringList nameH;
    for(qint8 i=0, k=1; i<model->getThreads()->getData(); i++, k++){
        QString tt = QString(tr("Код ДЭУ для ручья %1")).arg(QString::number(k));
//        ui.tDEUCode->verticalHeader()->setSectionResizeMode(i, QHeaderView::ResizeMode::Stretch);
        nameH.append(QString::number(k));
        QLineEdit *le = setNumberCell(ui.tDEUCode,0,i,model->deuCodes[i]->getData(),0,1000, tt);
        connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

        tt = QString(tr("Код ЦП для ручья %1")).arg(QString::number(k));
        le = setNumberCell(ui.tDEUCode,1,i,model->cpCodes[i]->getData(),0,1000, tt);
        connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    }
    ui.tDEUCode->setHorizontalHeaderLabels(nameH);
    ui.tDEUCode->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui.tDEUCode->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui.tDEUCode->blockSignals(false);

}

//void SPRSettingsRentgenControlWidget::viewChange(QTableWidget *, int, int)
//{
//}

void SPRSettingsRentgenControlWidget::viewChange(int value)
{
    if(sender() == ui.slTimeMoveRGU){
        model->timeMoveRGU->setData(value);
    }
}

void SPRSettingsRentgenControlWidget::viewChange(bool value)
{
    if(sender() == ui.cbWOControlPlace){
        model->woControlPlace->setData(value);
        ui.slTimeMoveRGU->setEnabled(value);
    }
}

void SPRSettingsRentgenControlWidget::viewChange()
{
    QObject *tw = sender()->property("tw").value<QObject*>();
    int row = sender()->property("row").toInt();
    int col = sender()->property("col").toInt();

    QLineEdit *le = (QLineEdit*)sender();

    if(tw == ui.tDiffParams){
        if(col == 0){
            model->diffParamAllow->setData(le->text().toInt());
        } else if(col == 1){
            model->diffParamDisallow->setData(le->text().toInt());
        }
        return;
    }
    if(tw == ui.tPapamsRA){
        if(col == 0){
            model->timeOnRA->setData(le->text().toInt());
        } else if(col == 1){
            model->timeOffRA->setData(le->text().toInt());
        } else if(col == 2){
            model->timeHotRA->setData(le->text().toInt());
        }
        return;
    }
    if(tw == ui.tDEUCode){
        if(row == 0){
            model->deuCodes[col]->setData(le->text().toInt());
        } else if(row == 1){
            model->cpCodes[col]->setData(le->text().toInt());
        }
        return;
    }
}
