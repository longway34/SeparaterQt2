#include "sprsettingsrentgenmainwidget.h"

SPRSettingsRentgenMainWidget::SPRSettingsRentgenMainWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

}

ISPRModelData *SPRSettingsRentgenMainWidget::setModel(ISPRModelData *data)
{
    model = (SPRSettingsRentgenModel*)data;

    setRentrenCodesTable();
    setRentrenVATable();

    return data;
}


void SPRSettingsRentgenMainWidget::widgetsShow()
{
    for(int i=0; i < MAX_SPR_MAIN_THREADS; i++){
        if(i<model->getThreads()->getData()){
            ui.tDEUCode->setColumnHidden(i, false);
        } else {
            ui.tDEUCode->setColumnHidden(i, true);
        }
    }
    for(int i=0; i < MAX_SPR_MAIN_RENTGENS; i++){
        if(i<model->getRentgens()->getData()){
            ui.tTubeVA->setColumnHidden(i, false);
        } else {
            ui.tTubeVA->setColumnHidden(i, true);
        }
    }
}

ISPRModelData *SPRSettingsRentgenMainWidget::getModel()
{
    return model;
}

void SPRSettingsRentgenMainWidget::viewChange(QTableWidget *table, int row, int col)
{
//    if(table == ui.tTubeVA){
//        int value = ((QLineEdit*)(ui.tTubeVA->cellWidget(row, col)))->text().toInt();
//        if(row == 0){ // изиенили ток на трубе
//           model->iTubes[col]->setData(value);
//        } else if(row == 1){ // изиенили напряжение на трубе
//           model->uTubes[col]->setData(value);
//        }
//        return;
//    }
//    if(table == ui.tDEUCode){
//        int value = ((QLineEdit*)(ui.tDEUCode->cellWidget(row, col)))->text().toInt();
//        if(row == 0){ // изиенили код ДЭУ
//           model->deuCodes[col]->setData(value);
//        } else if(row == 1){ // изиенили код ЦП
//           model->cpCodes[col]->setData(value);
//        }
//        return;
//    }
}

void SPRSettingsRentgenMainWidget::setRentrenVATable()
{

//    qDebug() << "setRentrenVATable ren:" + model->rentgens->toString();
    QStringList nameV; nameV << tr("I(mkA)") << tr("U(kV)");
    ui.tTubeVA->blockSignals(true);
    ui.tTubeVA->setVerticalHeaderLabels(nameV);
    ui.tTubeVA->setColumnCount(MAX_SPR_MAIN_RENTGENS);
    ui.tTubeVA->setRowCount(2);
    QStringList nameH;
    for(qint8 i=0, k=1; i<MAX_SPR_MAIN_RENTGENS; i++, k++){
        nameH.append(QString::number(k));
//        ui.tTubeVA->verticalHeader()->setSectionResizeMode(i, QHeaderView::ResizeMode::Stretch);
        QString tt = QString(tr("Ток(mkA) для трубы %1")).arg(QString::number(k));
        QLineEdit *le = setNumberCell(ui.tTubeVA, 0, i, model->iTubes[i]->getData(), 0, 1000, tt);
        le->setProperty("table", "va");
            connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
        tt = QString(tr("Напряжение(kV) на трубе %1")).arg(QString::number(k));
        le = setNumberCell(ui.tTubeVA, 1, i, model->uTubes[i]->getData(), 0, 1000, tt);
        le->setProperty("table", "va");
            connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    }
    ui.tTubeVA->setHorizontalHeaderLabels(nameH);
    ui.tTubeVA->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui.tTubeVA->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui.tTubeVA->blockSignals(false);
}

void SPRSettingsRentgenMainWidget::setRentrenCodesTable()
{
//    qDebug() << "setRentrenCodesTable threads:" + model->threads->toString();
    ui.tDEUCode->blockSignals(true);
    ui.tDEUCode->clear();
    ui.tDEUCode->setColumnCount(MAX_SPR_MAIN_THREADS);
    ui.tDEUCode->setRowCount(2);
    QStringList nameV; nameV << tr("ДЭУ") << tr("ЦП");
    ui.tDEUCode->setVerticalHeaderLabels(nameV);
    QStringList nameH;
    for(qint8 i=0, k=1; i<MAX_SPR_MAIN_THREADS; i++, k++){
        QString tt = QString(tr("Код ДЭУ для ручья %1")).arg(QString::number(k));
//        ui.tDEUCode->verticalHeader()->setSectionResizeMode(i, QHeaderView::ResizeMode::Stretch);
        nameH.append(QString::number(k));
        QLineEdit *le = setNumberCell(ui.tDEUCode,0,i,model->deuCodes[i]->getData(),0,1000, tt);
        le->setProperty("table", "codes");
        connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

        tt = QString(tr("Код ЦП для ручья %1")).arg(QString::number(k));
        le = setNumberCell(ui.tDEUCode,1,i,model->cpCodes[i]->getData(),0,1000, tt);
        le->setProperty("table", "codes");
        connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    }
    ui.tDEUCode->setHorizontalHeaderLabels(nameH);
    ui.tDEUCode->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui.tDEUCode->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui.tDEUCode->blockSignals(false);
}

void SPRSettingsRentgenMainWidget::setThreads(SPRVariable<uint> *value)
{
    model->setThreads(value);
//    connect(model, SIGNAL(goStore()), model->threads, SLOT(setData()));
}

void SPRSettingsRentgenMainWidget::setRentgens(SPRVariable<uint> *value)
{
    model->setRentgens(value);
//    connect(model, SIGNAL(goStore()), model->rentgens, SLOT(setData()));
}


void SPRSettingsRentgenMainWidget::viewChange()
{
    QTableWidget *tw = sender()->property("tw").value<QTableWidget*>();
    int row = sender()->property("row").toInt();
    int col = sender()->property("col").toInt();
    QLineEdit *le = (QLineEdit*)sender();

    if(tw == ui.tTubeVA){
        uint value = le->text().toInt();
        if(row == 0){ // изиенили ток на трубе
           model->iTubes[col]->setData(value);
        } else if(row == 1){ // изиенили напряжение на трубе
           model->uTubes[col]->setData(value);
        }
        return;
    }
    if(tw == ui.tDEUCode){
        uint value = le->text().toInt();
        if(row == 0){ // изиенили код ДЭУ
           model->deuCodes[col]->setData(value);
        } else if(row == 1){ // изиенили код ЦП
           model->cpCodes[col]->setData(value);
        }
        return;
    }
}
