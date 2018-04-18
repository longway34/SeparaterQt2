#include "sprspectrumlisttablewidget.h"
#include <QHeaderView>

SPRSpectrumListTableWidget::SPRSpectrumListTableWidget(QWidget *parent) :
    QWidget(parent), model(0)
{
    ui.setupUi(this);
//    ui.graphic->setCanvasBackground(QBrush(Qt::black));


    connect(ui.tListSpectrumItem, SIGNAL(cellClicked(int,int)), this, SLOT(viewChange(int,int)));
    connect(ui.tRangesChannel, SIGNAL(changeColor(EnumElements,QColor)), this, SLOT(onChangeZoneColor(EnumElements,QColor)));
    connect(ui.tRangesChannel, SIGNAL(tableChange(EnumElements,int,int)), this, SLOT(onChangeZoneRange(EnumElements,int,int)));

    ui.gbBasetSpectrums->setVisible(ui.cbBasetSpectrumVisible->isChecked());


}

ISPRModelData *SPRSpectrumListTableWidget::setModel(SPRMainModel *mainModel){
    model = mainModel;
    spectrums = mainModel->getSpectrumListItemsModel();

    ui.graphic->setModel(spectrums, spectrumsAll, true);

    ui.tListBasedSpectrumItem->setModel(spectrums, spectrumBase);
    ui.tListSpectrumItem->setModel(spectrums, spectrumsOnly);

    ui.tRangesChannel->setModel(mainModel->getSpectrumZonesTableModel());
    ui.tRangesChannel->setVisibleOneChannel(0);
    widgetsShow();
}

void SPRSpectrumListTableWidget::widgetsShow()
{

//    for(int row=0; row<ui.tListSpectrumItem->rowCount(); row++){
//        bool sel = ui.tListSpectrumItem->isSelectedItem(row);
//        if(row < grItems.size()){
//            bool current = ui.tListSpectrumItem->currentRow() == row;
//            grItems[row].setVisible(sel, current);
//        }
//    }
    ui.tListBasedSpectrumItem->widgetsShow();
    ui.tListSpectrumItem->widgetsShow();
    ui.tRangesChannel->widgetsShow();
    ui.graphic->widgetsShow();
}

void SPRSpectrumListTableWidget::onChangeSpectColor(int row)
{
    QColor color(ui.tListSpectrumItem->getColorSpectrum(row));
//    grItems[row].spect->setPen(color);
    widgetsShow();
}

SPRMainModel *SPRSpectrumListTableWidget::getModel()
{
    return model;
}

void SPRSpectrumListTableWidget::viewChange(int row, int col)
{
    int thr = ui.tListSpectrumItem->getThread(row);
    ui.tRangesChannel->setFirtChannel(thr);
    QList<bool> higgen;
    for(int ch=0; ch<spectrums->getZonesTableModel()->items.size(); ch++){
        bool val = ch == thr ? false : true;
        higgen.push_back(val);
    }

    QSize s = ui.tRangesChannel->sizeHint();
    ui.tRangesChannel->setHddenChannels(higgen);
    s = ui.tRangesChannel->sizeHint();
    ui.gbSpectrumElements->setMaximumWidth(ui.tRangesChannel->sizeHint().width());
    widgetsShow();
}


void SPRSpectrumListTableWidget::viewChange(int row)
{
    widgetsShow();
}

void SPRSpectrumListTableWidget::viewChange(bool value)
{
    if(sender() == ui.cbBasetSpectrumVisible){
        ui.gbBasetSpectrums->setVisible(value);
    }
}

void SPRSpectrumListTableWidget::onChangeZoneRange(EnumElements el, int thread, int col)
{
//    for(int i=0; i<grItems.size(); i++){
//        if(grItems[i].thread == thread){


//            double value = grItems[i].zones[el]->sample(0).value;
//            SPRSpectrumZonesModel *ranges = spectrums->getZonesTableModel()->items[thread];

//            QwtIntervalSample samp(value, ranges->elements[el].min->getData(), ranges->elements[el].max->getData());
//            grItems[i].zones[el]->setSamples(QVector<QwtIntervalSample>{samp});
//        }
//    }
    widgetsShow();
}
