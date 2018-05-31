#include "sprspectrumbasedlisttable.h"

SPRSpectrumBasedListTable::SPRSpectrumBasedListTable(QWidget *parent)
{
    QStringList hTitles;
    hTitles << tr("№") << tr("Ручей") << tr("I")
            << tr("Центр") << tr("X-Ray");

    setColumnCount(5);
    setHorizontalHeaderLabels(hTitles);
    verticalHeader()->setVisible(false);
    horizontalHeader()->setSectionsMovable(true);
    verticalHeader()->setSectionsMovable(true);
    resizeRowsToContents();
}


void SPRSpectrumBasedListTable::widgetsShow()
{
    for(int row=0; row < rowCount(); row++){
        SpectrumItemData *mod = spectrums->at(row)->getSpectrumData();
        FirstCollumn2 *fc = ((FirstCollumn2*)cellWidget(row, 0));
        QColor col(*mod->red, *mod->green, *mod->blue);
        fc->setColor(col); fc->setText(QString::number(row));
//        fc->setData(row, col);
//        fc->ui.cbSelect->setText("");

        ((QLabel*)cellWidget(row, 1))->setText(QString::number(*mod->thread));
        ((QLabel*)cellWidget(row, 2))->setText(QString::number(*mod->summ));
        ((QLabel*)cellWidget(row, 3))->setText(QString::number(*mod->center, 'f', 0));
        ((QLabel*)cellWidget(row, 4))->setText(QString::number(*mod->Rs, 'f', 0));
    }
    resizeColumnsToContents();
}

void SPRSpectrumBasedListTable::insertFirstColumn(SpectrumItemData *data, int row)
{
    SPRSpectrumListTable::insertFirstColumn(data, row);
    FirstCollumn2 *fc = (FirstCollumn2*)cellWidget(row, 0);

    fc->setDeleteVisible(false);
    fc->setSelectVisible(false);

//    fc->ui.bDelete->setVisible(false);
//    fc->ui.bColor->setEnabled(false);
//    fc->ui.cbSelect->setText("");
}

void SPRSpectrumBasedListTable::connectFirstTable(FirstCollumn2 *fc)
{
    connect(fc, SIGNAL(iAmSelected(int)), this, SLOT(viewChange(int)));
}

void SPRSpectrumBasedListTable::insertContentColumns(SpectrumItemData *data, int row)
{
    setCellMyWidget(this, row, 1, QString::number(*data->thread), false, tr("Номер ручья"));
    setCellMyWidget(this, row, 2, QString::number(*data->summ), false, tr("Сумма значений в зонах"));
    setCellMyWidget(this, row, 3, QString::number(*data->center, 'f', 0), false, tr(""));
    setCellMyWidget(this, row, 4, QString::number(*data->Rs, 'f', 0), false, tr(""));
    resizeColumnsToContents();
}
