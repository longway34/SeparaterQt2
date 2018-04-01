#include "sprspectrumlisttable.h"
#include <QLabel>


void SPRSpectrumListTable::connectFirstTable(FirstColumn *fc){
    connect(fc, SIGNAL(changeColor(QColor)), this, SLOT(viewChange(QColor)));
    connect(fc, SIGNAL(iAmSelected(int)), this, SLOT(viewChange(int)));
    connect(fc, SIGNAL(iAmDelete(int)), this, SLOT(onDeleteRow(int)));
    connect(this, SIGNAL(doShow()), fc, SLOT(widgetsShow()));

}
void SPRSpectrumListTable::onDeleteRow(int row){
    model->getSpectrumsModel()->remove(row);
    removeRow(row);
    storeCheckedRows = getSelectedItems();
    storeCurrentRow = currentRow();
    while(rowCount()>0) removeRow(0);
    widgetsShow();
    emit modelChanged();
}

void SPRSpectrumListTable::insertFirstColumn(SpectrumItemData *data, int row){
    FirstColumn *fc = new FirstColumn(this);
    int r = *data->red;
    int g = *data->green;
    int b = *data->blue;
    QColor col(r,g,b);
    fc->setColor(col);
    fc->setNumber(rowCount());
    fc->setProperty("row", QVariant(row));
    QVariant vtw; vtw.setValue<QTableWidget*>(this);
    fc->setProperty("tw",vtw);
    connectFirstTable(fc);
    setCellWidget(row, 0, fc);
}

void SPRSpectrumListTable::insertContentColumns(SpectrumItemData *data, int row){
    setCellMyWidget(this, row, 1, QString::number(*data->thread), false, tr("Номер ручья"));
    setCellMyWidget(this, row, 2, QString(data->name), true, tr("Номер ручья"));
    setCellMyWidget(this, row, 3, QString::number(*data->summ), false, tr("Номер ручья"));
    setCellMyWidget(this, row, 4, QString::number(*data->H1, 'f', 4), false, tr("Расчетный параметр H1"));
    setCellMyWidget(this, row, 5, QString::number(*data->H2, 'f', 4), false, tr("Расчетный параметр H2"));
    setCellMyWidget(this, row, 6, QString::number(*data->H3, 'f', 4), false, tr("Расчетный параметр H3"));
    setCellMyWidget(this, row, 7, QString::number(*data->center, 'f', 0), false, tr(""));
    QString tt; tt.sprintf("Канал максимального значения спектра (%d)", *data->peak_value);
    setCellMyWidget(this, row, 8, QString::number(*data->peak), false, tr(tt.toStdString().c_str()));
    setCellMyWidget(this, row, 9, QString::number(*data->Rs, 'f', 0), false, tr(""));
    setCellMyWidget(this, row, 10, QString::number(*data->time, 'f', 0), false, tr("Количество секунд измерения спектра"));
    QString date; date.sprintf("%02d.%02d.%4d %02d:%02d:%02d",
                               *data->day, *data->month, *data->year,
                               *data->hours, *data->min, *data->sec);

    setCellMyWidget(this, row, 11, date, false, tr("Дита и время измерения спектра"));
    setCellMyWidget(this, row, 12, QString::number(*data->Ns), false, tr("Значения спектра в зоне рассеенки"));
    setCellMyWidget(this, row, 13, QString::number(*data->Fe), false, tr("Значения спектра в зоне железа"));
    setCellMyWidget(this, row, 14, QString::number(*data->Cu), false, tr("Значения спектра в зоне меди"));
    setCellMyWidget(this, row, 15, QString::number(*data->Mo), false, tr("Значения спектра в зоне молибдена"));
    setCellMyWidget(this, row, 16, QString::number(*data->Zn), false, tr("Значения спектра в зоне цинка"));
    setCellMyWidget(this, row, 17, QString::number(*data->Mg), false, tr("Значения спектра в зоне марганца"));
}

void SPRSpectrumListTable::addRowTable(SpectrumItemData *data, int pastRow)
{
    int row = 0;
    if(pastRow >= 0 && pastRow < rowCount() - 1){
        insertRow(pastRow);
        row = pastRow + 1;
    } else {
        insertRow(this->rowCount());
        row = rowCount() - 1;
    }

    insertFirstColumn(data, row);

    insertContentColumns(data, row);
}

QLineEdit *SPRSpectrumListTable::setCellMyWidget(QTableWidget *table, int row, int col, QString value, bool editable, QString tt)
{
    QWidget *wid;
    QAction *actHide = new QAction(tr("Скрыть эту колонку"), this);
//    actHide->setData(col);
    connect(actHide, SIGNAL(triggered(bool)), this, SLOT(hideCol(bool)));
    QAction *actShow = new QAction(tr("Отобразить все"), this);
    connect(actShow, SIGNAL(triggered(bool)), this, SLOT(showCols(bool)));

    if(editable){
       QLineEdit *le = new QLineEdit(value);
       le->setFrame(false);
       connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
       le->setAlignment(Qt::AlignCenter); le->setTextMargins(3, 2, 3, 2);
       le->addAction(actHide); le->addAction(actShow);
       le->setContextMenuPolicy(Qt::ActionsContextMenu);
       wid = le;
    } else {
        QLabel *la = new QLabel(value);
        la->setAlignment(Qt::AlignCenter); la->setMargin(2);
//        la->addAction(actHide); la->addAction(actShow);
//        la->setContextMenuPolicy(Qt::ActionsContextMenu);
        wid = la;
    }
    QVariant vtw; vtw.setValue<QTableWidget*>(table);

    wid->setProperty("tw", vtw);
    wid->setProperty("row", QVariant(row)); wid->setProperty("col", QVariant(col));
    wid->setToolTip(tt);
    wid->setEnabled(editable);
//    le->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setCellWidget(row, col, wid);
//    cellWidget(row, col)->addAction(actHide); cellWidget(row, col)->addAction(actShow);
}

SPRSpectrumListTable::SPRSpectrumListTable(QWidget *parent): QTableWidget(parent), model(nullptr)
{
    QStringList hTitles;
    hTitles << tr("№") << tr("Ручей") << tr("Имя") << tr("I") << tr("H1") << tr("H2") << tr("H3")
            << tr("Центр") << tr("Пик") << tr("Rs") << tr("Время") << tr("Дата и время")
            << tr("Ns") << tr("Fe") << tr("Cu") << tr("Mo") << tr("Zn") << tr("Mg");

    setColumnCount(18);
    setHorizontalHeaderLabels(hTitles);
    verticalHeader()->setVisible(false);
    horizontalHeader()->setSectionsMovable(true);
    verticalHeader()->setSectionsMovable(true);

    QAction *actHide = new QAction(tr("Скрыть эту колонку"), this);
    connect(actHide, SIGNAL(triggered(bool)), this, SLOT(hideCol(bool)));
    QAction *actShow = new QAction(tr("Отобразить все"), this);
    connect(actShow, SIGNAL(triggered(bool)), this, SLOT(showCols(bool)));

    connect(this, SIGNAL(cellClicked(int,int)), this, SLOT(onCurrentPosChanged(int, int)));

    this->addAction(actHide); this->addAction(actShow);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    resizeColumnsToContents();
    resizeRowsToContents();
}

void SPRSpectrumListTable::onCurrentPosChanged(int row, int col){
    emit rowSelectedChecked(getSelectedItems(), row);
}

ISPRModelData *SPRSpectrumListTable::setModel(SPRSpectrumListItemsModel *_model, uint8_t *inp)
{
    if(_model){
        if(model != _model){
            model = _model;
            emit modelChanged();
        }
    }
    if(inp){
        addSpectrum(inp);
    }
}

ISPRModelData *SPRSpectrumListTable::addSpectrum(uint8_t *_inp, int _bufSize, int _thread)
{
    if(_thread < 0 || _thread >= MAX_SPR_MAIN_THREADS){
        if(_bufSize == DEF_SPECTRUM_DATA_BUF_LENGTH){
            spectumItemData b;
            b.setbuf(_inp);
            _thread = *b.thread;
        } else {
            _thread = 0;
        }
    }
    SPRSpectrumItemModel *mod = new SPRSpectrumItemModel(model->getZonesTableModel(), model->getFormulas(), model);
    mod->setSpectrumData(_inp, _bufSize);

    if(_bufSize == DEF_SPECTRUM_DATA_LENGTH){
        mod->getSpectrumData()->setThread(_thread);
        mod->recomplite();
    }
    addRowTable(mod->getSpectrumData());
    model->getSpectrumsModel()->push_back(mod);
    emit modelChanged();
}

void SPRSpectrumListTable::widgetsShow()
{

    for(int row=0; row<model->getSpectrumsModel()->size(); row++){
        SpectrumItemData *mod = model->getSpectrumsModel()->at(row)->getSpectrumData();
        int rc = rowCount();
        if(row > rc-1){
            addRowTable(mod);
        }
//    for(int row=0; row < rowCount(); row++){
        FirstColumn *fc = ((FirstColumn*)cellWidget(row, 0));
        QColor col(*mod->red, *mod->green, *mod->blue);
        fc->setData(row, col);

        ((QLabel*)cellWidget(row, 1))->setText(QString::number(*mod->thread));
        ((QLineEdit*)cellWidget(row, 2))->setText(QString(mod->name));
        ((QLabel*)cellWidget(row, 3))->setText(QString::number(*mod->summ));
        ((QLabel*)cellWidget(row, 4))->setText(QString::number(*mod->H1, 'f', 4));
        ((QLabel*)cellWidget(row, 5))->setText(QString::number(*mod->H2, 'f', 4));
        ((QLabel*)cellWidget(row, 6))->setText(QString::number(*mod->H3, 'f', 4));
        ((QLabel*)cellWidget(row, 7))->setText(QString::number(*mod->center, 'f', 0));
        ((QLabel*)cellWidget(row, 8))->setText(QString::number(*mod->peak));
        ((QLabel*)cellWidget(row, 9))->setText(QString::number(*mod->Rs, 'f', 0));
        ((QLabel*)cellWidget(row, 10))->setText(QString::number(*mod->time, 'f', 0));
        QString date; date.sprintf("%02d.%02d.%4d %02d:%02d:%02d",
                                   *mod->day, *mod->month, *mod->year,
                                   *mod->hours, *mod->min, *mod->sec);
        ((QLabel*)cellWidget(row, 11))->setText(date);
        ((QLabel*)cellWidget(row, 12))->setText(QString::number(*mod->Ns));
        ((QLabel*)cellWidget(row, 13))->setText(QString::number(*mod->Fe));
        ((QLabel*)cellWidget(row, 14))->setText(QString::number(*mod->Cu));
        ((QLabel*)cellWidget(row, 15))->setText(QString::number(*mod->Mo));
        ((QLabel*)cellWidget(row, 16))->setText(QString::number(*mod->Zn));
        ((QLabel*)cellWidget(row, 17))->setText(QString::number(*mod->Mg));
        resizeColumnsToContents();
    }
    foreach (int srow, storeCheckedRows) {
        FirstColumn *fc = ((FirstColumn*)cellWidget(srow, 0));
        fc->ui.cbSelect->setChecked(true);
    }
    if(storeCurrentRow >= 0 && storeCurrentRow < rowCount()){
        setCurrentCell(storeCurrentRow, 4);
    }
}

ISPRModelData *SPRSpectrumListTable::getModel()
{
    if(model->getZonesTableModel()->items.size() > 0){
        return model->getZonesTableModel()->items[0];
    }
    return nullptr;
}

void SPRSpectrumListTable::viewChange(QColor color)
{
    if(sender()->property("tw").value<QTableWidget*>() == this){
        int row = sender()->property("row").toInt();

        *model->getSpectrumsModel()->at(row)->getSpectrumData()->red = color.red();
        *model->getSpectrumsModel()->at(row)->getSpectrumData()->green = color.green();
        *model->getSpectrumsModel()->at(row)->getSpectrumData()->blue = color.blue();
//        emit rowChangeColor(row);
        emit modelChanged();
    }
}

void SPRSpectrumListTable::viewChange()
{
    if(sender()->property("tw").value<QTableWidget*>() == this){
        int row = sender()->property("row").toInt();
        SpectrumItemData *data = model->getSpectrumsModel()->at(row)->getSpectrumData();
        const char *value = ((QLineEdit*)sender())->text().toStdString().c_str();
        strcpy(data->name, value);
        emit modelChanged();
    }
}

void SPRSpectrumListTable::hideCol(bool)
{
    hideColumn(currentColumn());
}

void SPRSpectrumListTable::showCols(bool)
{
    for(int col=0; col < columnCount(); col++){
        showColumn(col);
    }

}

void SPRSpectrumListTable::viewChange(int num)
{
    emit rowSelectedChecked(getSelectedItems(), this->currentRow());
}

