#include "sprspectrumlisttable.h"
#include <QLabel>
#include "_types.h"

void SPRSpectrumListTable::connectFirstTable(FirstCollumn2 *fc){
    connect(fc, SIGNAL(colorChanged(QColor)), this, SLOT(viewChange(QColor)));
    connect(fc, SIGNAL(selectedRow(bool,int)), this, SLOT(onRowSelect(bool,int)));
    connect(fc, SIGNAL(deleteRow(int)), this, SLOT(onDeleteRow(int)));
    connect(this, SIGNAL(doShow()), fc, SLOT(widgetsShow()));
}

void SPRSpectrumListTable::onRowSelect(bool checked, int row){
    if(checked){
        storeCurrentItem = spectrums->at(row);

        setCurrentCell(row, columnCount()-1);
        currentItem()->setSelected(true);
    }
    emit rowSelectedChecked3(getSelectedItems(), spectrums->at(row));
}
void SPRSpectrumListTable::onDeleteRow(int row){
//    if(typeData == spectrumsOnly){
//        int baseVectorSize = model->getSpectrumsModel(spectrumBase)->size();
//        row += baseVectorSize;
//    }
    int vectorSize = model->getSpectrumsModel(typeData)->size();
    if(row >= 0 && row < vectorSize){
        model->removeSpectrum(spectrums->at(row));
        if(spectrums->at(row) == storeCurrentItem){
            storeCurrentItem = nullptr;
        }
        removeRow(row);
        storeCheckedItems = getSelectedItems();
    }
//    while(rowCount()>0) removeRow(0);
//    widgetsShow();
}

void SPRSpectrumListTable::insertFirstColumn(SpectrumItemData *data, int row){
    FirstCollumn2 *fc = new FirstCollumn2(this);
    int r = *data->red;
    int g = *data->green;
    int b = *data->blue;

    QColor col = data->getColor();
    fc->setColor(col);
    fc->setText(QString::number(rowCount()));
    fc->setProperty("row", QVariant(row));
    fc->setRow(row);
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

SPRTypeSpectrumSet SPRSpectrumListTable::getTypeData() const
{
    return typeData;
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

SPRSpectrumListTable::SPRSpectrumListTable(QWidget *parent):
    QTableWidget(parent), model(nullptr)
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
//    resizeColumnsToContents();
    resizeRowsToContents();
}

void SPRSpectrumListTable::onCurrentPosChanged(int row, int col){
    storeCurrentItem = model->getSpectrumItem(row, typeData);
    emit rowSelectedChecked3(getSelectedItems(), spectrums->at(row));
}

ISPRModelData *SPRSpectrumListTable::setModelData(SPRSpectrumListItemsModel *_model, SPRTypeSpectrumSet _type)
{
    if(model != _model){
        if(model){
            disconnect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        }
        model = _model;
        connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));

        this->typeData = _type;
        spectrums = model->getSpectrumsModel(typeData);
//        if(typeData == spectrumBase){
//            spectrums = model->getSpectrumsModelBase();
//        } else if(typeData == spectrumsOnly){
//            spectrums = model->getSpectrumsModel();
//        } else {
//            spectrums = model->getSpectrumsModelAll();
//        }

    }
    return model;
}

void SPRSpectrumListTable::resizeEvent(QResizeEvent *event)
{
    QTableWidget::resizeEvent(event);
    int newWidth = event->size().width();

    int countVisibleCollumns = 0;
    for(int col=0; col<columnCount(); col++){
        if(!isColumnHidden(col)) countVisibleCollumns++;
    }

    for(int col=0; col<columnCount(); col++){
        if(!isColumnHidden(col)){
            int widthPart = newWidth / countVisibleCollumns;
            int colWidth = getMaxColumnWidth(col);
            if(colWidth > widthPart || col == 0 || widthPart < 0){
                widthPart = colWidth;
            }
            horizontalHeader()->resizeSection(col, widthPart);
            newWidth -= widthPart;
            countVisibleCollumns--;
        }
    }
}

int SPRSpectrumListTable::getMaxColumnWidth(int col)
{
    int res = 0;
    if(col < columnCount()){
        res = fontMetrics().width(horizontalHeaderItem(col)->text()+"99");
        for(int row=0; row<rowCount();row++){
            int rowSize = 0;
            QWidget *wid = cellWidget(row, col);
            if(wid){
                if(col == 0){
                    rowSize = cellWidget(row, col)->size().width();
                } else if(col == 2){
                    rowSize = fontMetrics().width(((QLineEdit*)cellWidget(row, col))->text());
                } else {
                    rowSize = fontMetrics().width(((QLabel*)cellWidget(row, col))->text());
                }
                if(res < rowSize){
                    res = rowSize;
                }
            }
        }
    }
    return res;
}

void SPRSpectrumListTable::widgetsShow()
{

    if(model){

        spectrums = model->getSpectrumsModel(typeData);
        storeCheckedItems = getSelectedItems();
//        storeCurrentItem = model->getSpectrumItem(currentRow(), typeData);

        while(rowCount() > 0) this->removeRow(0);

        for(int row=0; row<spectrums->size(); row++){
            SpectrumItemData *mod = spectrums->at(row)->getSpectrumData();
            int rc = rowCount();
            if(row > rc-1){
                addRowTable(mod);
            }
    //    for(int row=0; row < rowCount(); row++){
            FirstCollumn2 *fc = ((FirstCollumn2*)cellWidget(row, 0));
            QColor col(*mod->red, *mod->green, *mod->blue);
            fc->setColor(col); fc->setText(QString::number(row));
            if(storeCheckedItems.contains(spectrums->at(row))){
                fc->setSelect(true);
            }
    //        fc->setData(row, col);

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
    //    foreach (int srow, storeCheckedRows) {
    //        FirstCollumn2 *fc = ((FirstCollumn2*)cellWidget(srow, 0));
    ////        fc->ui.cbSelect->setChecked(true);
    //        fc->setSelect(true);
    //    }
        if(storeCurrentItem){
            int row = findRowByItemModel(storeCurrentItem);
            if(row >= 0 && row < rowCount()){
                setCurrentCell(row, columnCount()-1);
                currentItem()->setSelected(true);
            }
        }
    }
}

int SPRSpectrumListTable::findRowByItemModel(SPRSpectrumItemModel* mod){
    int res =-1;
    for(int row=0; row<rowCount(); row++){
        if(model->getSpectrumItem(row, typeData) == mod){
            return row;
        }
    }
    return -1;
}

void SPRSpectrumListTable::viewChange(QColor color)
{
    if(sender()->property("tw").value<QTableWidget*>() == this){
        int row = sender()->property("row").toInt();

        spectrums->at(row)->setSpectrumColor(color);

//        *spectrums->at(row)->getSpectrumData()->red = color.red();
//        *spectrums->at(row)->getSpectrumData()->green = color.green();
//        *spectrums->at(row)->getSpectrumData()->blue = color.blue();
        emit itemChangeColor(spectrums->at(row), color);
        widgetsShow();
    }
}

void SPRSpectrumListTable::viewChange()
{
    if(sender()->property("tw").value<QTableWidget*>() == this){
        int row = sender()->property("row").toInt();
        SpectrumItemData *data = spectrums->at(row)->getSpectrumData();
        const char *value = ((QLineEdit*)sender())->text().toStdString().c_str();
        strcpy(data->name, value);
        widgetsShow();
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
    emit rowSelectedChecked3(getSelectedItems(), spectrums->at(num));
}



void SPRSpectrumListTable::onModelChanget(IModelVariable *)
{
    widgetsShow();
}
