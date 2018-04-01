#include <QHeaderView>
#include "sprspectrumzonestable.h"
#include "sprspectrumranges.h"

SPRSpectrumZonesTable::SPRSpectrumZonesTable(QWidget *parent):
    QTableWidget(parent), model(nullptr)
{
}

void SPRSpectrumZonesTable::setModel(SPRSpectrumZonesTableModel *value)
{
    model = value;

    QStringList nameH;
    setColumnCount(model->items.size()); setRowCount(1);
    for(int i=0; i<model->items.size(); i++){
        nameH.append(tr("Канал ")+QString::number(i));
        SPRSpectrumRanges *ranges = new SPRSpectrumRanges();
        ranges->setFont(this->font());
        ranges->setModel(model->items.at(i));

        setCellWidget(0, i, ranges);
        connect(ranges, SIGNAL(tableChange(QTableWidget*,int,int)), SLOT(viewChange(QTableWidget*,int,int)));
        connect(ranges, SIGNAL(changeColor(EnumElements,QColor)), SLOT(onChangeColor(EnumElements,QColor)));
        setColumnWidth(i, ranges->sizeHint().width());
        setContentsMargins(0,0,0,0);
        setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    }
    setHddenChannels(QList<bool>({false, false, false, false}));
    setFirtChannel(0);

    setHorizontalHeaderLabels(nameH);
    verticalHeader()->setVisible(false);
    resizeRowsToContents();
}

void SPRSpectrumZonesTable::setFirtChannel(int ch){
    for(int i=0; i<columnCount(); i++){
        SPRSpectrumRanges* ranges = (SPRSpectrumRanges*)cellWidget(0, i);
        if(i == ch){
            ranges->setFirstStateView(true);
        } else {
            ranges->setFirstStateView(false);
        }
        setColumnWidth(i, ranges->sizeHint().width());
    }
    //        resizeColumnsToContents();
}

void SPRSpectrumZonesTable::setHddenChannels(QList<bool> hiddens){
    for(int i=0; i < columnCount(); i++){
        bool value = hiddens.size() > i ? hiddens[i] : true;
        setColumnHidden(i, value);
    }
}

void SPRSpectrumZonesTable::setVisibleOneChannel(int ch)
{
    QList<bool> hiddens;
    for(int i=0; i < columnCount(); i++){
        if(i == ch){
            hiddens.push_back(false);
        } else {
            hiddens.push_back(true);
        }
    }
    setFirtChannel(ch);
    setHddenChannels(hiddens);
}

//QSize SPRSpectrumRangesTable::sizeHint() const
//{
//    if(rowCount() > 0){
//        int height = QTableWidget::sizeHint().height();
//        int width = 0;
//        for(int i=0; i<columnCount(); i++){
//            if(!isColumnHidden(i)){
//                width += columnWidth(i);
//            }
//        }
//        return QSize(width,height);
//    } else {
//        QTableWidget::sizeHint();
//    }
//}

void SPRSpectrumZonesTable::widgetsShow()
{
    if(model){
        for(int i=0; i< model->items.size(); i++){
            SPRVariable<uint> *thr = model->getThreads();
            if(i < model->getThreads()->getData()){
                showColumn(i);
                //                cellWidget(0, i)->setVisible(true);
                ((SPRSpectrumRanges*)(cellWidget(0,i)))->widgetsShow();

            } else {
                hideColumn(i);
                //                cellWidget(0, i)->setVisible(false);
            }
        }
    }
}

QPoint SPRSpectrumZonesTable::getSelectedPosition()
{
    SPRSpectrumRanges* range = (SPRSpectrumRanges*)focusWidget();
    QPoint res(range->getSelectedPosition());
    res.setX(res.x() + ((SPRSpectrumZonesModel*)range->getModel())->tIndex * 2);
    return res;
}

void SPRSpectrumZonesTable::onChangeColor(EnumElements el, QColor color){
    emit changeColor(el, color);
}

ISPRModelData *SPRSpectrumZonesTable::getModel()
{
    return model;
}

void SPRSpectrumZonesTable::viewChange(QTableWidget *table, int row, int col)
{
    SPRSpectrumRanges *threadTable = (SPRSpectrumRanges*)table;
    QLineEdit *le = (QLineEdit*)threadTable->cellWidget(row, col);
    EnumElements el = le->property("element").value<EnumElements>();
    int thread = threadTable->getModel()->tIndex;

    emit this->tableChange(el, thread, col);
    emit this->tableChange(table, row, col);
}
