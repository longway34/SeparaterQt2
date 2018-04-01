#include "sprspectrumlisttablewidget.h"
#include <QHeaderView>

void SPRSpectrumListTableWidget::initNullGraphItems()
{
    foreach(EnumElements el, DEF_SPR_FORMULA_ELEMENTS_PROPERTY.keys()){
        QwtPlotHistogram *nullHist = new QwtPlotHistogram();
        DefaultElementsProperty ep = DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el];
        nullHist->setSamples(QVector<QwtIntervalSample>({QwtIntervalSample(0.001, ep.min, ep.max)}));
        nullHist->setPen(QPen(ep.color, 1, Qt::SolidLine));
        nullHist->setBrush(QBrush(ep.color));
        nullGrItems.zones[el] = nullHist;
        nullHist->attach(ui.qwtPlot);
    }

    ui.qwtPlot->setAxisScale(QwtPlot::xBottom, 0, MAX_SPR_SPECTOR_CHANNELS);
}

SPRSpectrumListTableWidget::SPRSpectrumListTableWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    ui.qwtPlot->setCanvasBackground(QBrush(Qt::black));


    connect(ui.tListSpectrumItem, SIGNAL(cellClicked(int,int)), this, SLOT(viewChange(int,int)));
//    connect(ui.tListSpectrumItem, SIGNAL(rowSelectedChecked(int)), this, SLOT(viewChange(int)));
    connect(ui.tRangesChannel, SIGNAL(changeColor(EnumElements,QColor)), this, SLOT(onChangeZoneColor(EnumElements,QColor)));
    connect(ui.tRangesChannel, SIGNAL(tableChange(EnumElements,int,int)), this, SLOT(onChangeZoneRange(EnumElements,int,int)));
    initNullGraphItems();

    ui.gbBasetSpectrums->setVisible(ui.cbBasetSpectrumVisible->isChecked());

//    connect(ui.cbBasetSpectrumVisible, SIGNAL(toggled(bool)), this, SLOT(viewChange(bool)));
//    ui.gbSpectrumElements->setMinimumWidth(150);

}

ISPRModelData *SPRSpectrumListTableWidget::setModel(SPRSpectrumListItemsModel *spectrumListItems, QString _fName){
    QFile in(_fName);
    if(in.open(QIODevice::ReadOnly)){
        setModel(spectrumListItems, &in);
        in.close();
    }
}

ISPRModelData *SPRSpectrumListTableWidget::setModel(SPRSpectrumListItemsModel *spectrumListItems, QFile *inp)
{
//    SPRSpectrumZonesModel *ranges = rangesModel->items[thread];

    for(int i=baseItems.size()-1; i >= 0; i--){
//        if(i < rangesModel->items.size()){
//        if(ui.tListBasedSpectrumItem->getModel(i)){
//            SPRSpectrumItemModel *itModel = ui.tListBasedSpectrumItem->getModel(i);
            foreach (EnumElements el, baseItems[i].zones.keys()) {
                baseItems[i].zones[el]->detach();
            }
//        }
        baseItems[i].spect->detach();
        baseItems.pop_back();
        ui.tListBasedSpectrumItem->removeRow(i);
    }

    for(int i=grItems.size()-1; i >= 0; i--){
//        if(i < rangesModel->items.size()){
            foreach (EnumElements el, grItems[i].zones.keys()) {
                grItems[i].zones[el]->detach();
            }
//        }
        grItems[i].spect->detach();
        grItems.pop_back();
        ui.tListSpectrumItem->removeRow(i);
    }

    rangesModel = spectrumListItems->getZonesTableModel();

    ui.tListBasedSpectrumItem->setModel(spectrumListItems);
    ui.tListSpectrumItem->setModel(spectrumListItems);
    bool res = true;
    if(rangesModel){
        int countBasedSpectrum = rangesModel->getThreads()->getData();
        if(!inp->isOpen()){
            bool res = inp->open(QIODevice::ReadOnly);
        }
        if(res){
           uint8_t buf[DEF_SPECTRUM_DATA_BUF_LENGTH];
//            if(inp->open(QIODevice::ReadOnly)){
           inp->seek(0);
           char b[2];
           inp->read(b, 2);
           while(inp->read((char*)(buf), DEF_SPECTRUM_DATA_BUF_LENGTH)){
               if(countBasedSpectrum-- > 0){
//                       ui.tListBasedSpectrumItem->setModel(buf, rangesModel);
                   ui.tListBasedSpectrumItem->addSpectrum(buf, DEF_SPECTRUM_DATA_BUF_LENGTH);
               } else {
                   ui.tListSpectrumItem->addSpectrum(buf, DEF_SPECTRUM_DATA_BUF_LENGTH);
               }
           }
//            }
        }

//        ui.tListSpectrumItem->setModel(inp, rangesModel);
        if(ui.tListSpectrumItem->columnCount() > 0){
            ui.tListSpectrumItem->selectRow(0);
        }
        ui.tRangesChannel->setModel(rangesModel);

        ui.tListBasedSpectrumItem->resizeRowsToContents();

        connect(ui.tRangesChannel, SIGNAL(changeColor(EnumElements,QColor)), this, SLOT(onChangeZoneColor(EnumElements,QColor)));
        int rc = ui.tListBasedSpectrumItem->rowCount();
        for(int row=0; row<ui.tListBasedSpectrumItem->rowCount(); row++){
            uint32_t thread = *ui.tListBasedSpectrumItem->getModel(row)->getSpectrumData()->thread;
            SPRSpectrumZonesModel *ranges = rangesModel->items[thread];
            SPRGrSpectrumItemModel *gr = new SPRGrSpectrumItemModel(ranges, ui.tListBasedSpectrumItem->getModel(row));
            QwtPlotCurve *curve = new QwtPlotCurve();

            QColor col(ui.tListSpectrumItem->getColorSpectrum(row));
            QPen pen(ui.tListSpectrumItem->getColorSpectrum(row), 1, Qt::SolidLine);
            curve->setPen(pen);
            col.setRgba(qRgba(255,255,255,64));
            QBrush brush(col);
            curve->setBrush(brush);

            GraphItem gri(curve);
            gri.thread = thread;
            foreach(EnumElements el, ranges->elements.keys()){
                QwtPlotHistogram *zonesItemHistogram = new QwtPlotHistogram();
                zonesItemHistogram->setSamples(gr->getIntervalSamples()[el]);

                QColor col(gr->getColorZone(el));
                QPen pen(col, 2, Qt::SolidLine);
                col.setAlpha(32);
                QBrush brush(col);
                zonesItemHistogram->setPen(pen);
                zonesItemHistogram->setBrush(brush);

                zonesItemHistogram->attach(ui.qwtPlot);
                gri.zones[el] = zonesItemHistogram;
            }

            baseItems.push_back(gri);
            spectBaseGrData.push_back(gr);

        }
        rc = ui.tListSpectrumItem->rowCount();
        for(int row=0; row<ui.tListSpectrumItem->rowCount(); row++){
            uint32_t thread = *ui.tListSpectrumItem->getModel(row)->getSpectrumData()->thread;

            SPRSpectrumZonesModel *ranges = rangesModel->items[thread];
            SPRGrSpectrumItemModel *gr = new SPRGrSpectrumItemModel(ranges, ui.tListSpectrumItem->getModel(row));

            QwtPlotCurve *curve = new QwtPlotCurve();
            curve->setSamples(gr->getSamples());
            QColor col(ui.tListSpectrumItem->getColorSpectrum(row));
            QPen pen(ui.tListSpectrumItem->getColorSpectrum(row), 1, Qt::SolidLine);
            curve->setPen(pen);
            col.setRgba(qRgba(255,255,255,64));
            QBrush brush(col);
            curve->setBrush(brush);


            GraphItem gri(curve);
            gri.thread = thread;

            foreach(EnumElements el, ranges->elements.keys()){
                QwtPlotHistogram *zonesItemHistogram = new QwtPlotHistogram();
                zonesItemHistogram->setSamples(gr->getIntervalSamples()[el]);

                QColor col(gr->getColorZone(el));
                QPen pen(col, 2, Qt::SolidLine);
                col.setAlpha(32);
                QBrush brush(col);
                zonesItemHistogram->setPen(pen);
                zonesItemHistogram->setBrush(brush);

                zonesItemHistogram->attach(ui.qwtPlot);
                gri.zones[el] = zonesItemHistogram;
            }

            curve->attach(ui.qwtPlot);

            grItems.push_back(gri);
            spectGrData.push_back(gr);
        }
    }

    widgetsShow();
    return rangesModel;
}

void SPRSpectrumListTableWidget::widgetsShow()
{

    for(int row=0; row<ui.tListSpectrumItem->rowCount(); row++){
        bool sel = ui.tListSpectrumItem->isSelectedItem(row);
        if(row < grItems.size()){
            bool current = ui.tListSpectrumItem->currentRow() == row;
            grItems[row].setVisible(sel, current);
        }
    }
    ui.qwtPlot->replot();
}

void SPRSpectrumListTableWidget::onChangeSpectColor(int row)
{
    QColor color(ui.tListSpectrumItem->getColorSpectrum(row));
    grItems[row].spect->setPen(color);
    widgetsShow();
}

SPRSpectrumZonesTableModel *SPRSpectrumListTableWidget::getModel()
{
    return rangesModel;
}

void SPRSpectrumListTableWidget::viewChange(int row, int col)
{
    int thr = ui.tListSpectrumItem->getThread(row);
    ui.tRangesChannel->setFirtChannel(thr);
    QList<bool> higgen;
    for(int ch=0; ch<rangesModel->items.size(); ch++){
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
    for(int i=0; i<grItems.size(); i++){
        if(grItems[i].thread == thread){


            double value = grItems[i].zones[el]->sample(0).value;
            SPRSpectrumZonesModel *ranges = getModel()->items[thread];

            QwtIntervalSample samp(value, ranges->elements[el].min->getData(), ranges->elements[el].max->getData());
            grItems[i].zones[el]->setSamples(QVector<QwtIntervalSample>{samp});
        }
    }
    widgetsShow();
}
