#include "sprspectrumgraphicswidget.h"

void SPRSpectrumGraphicsWidget::setModelData(SPRSpectrumListItemsModel *value, SPRTypeSpectrumSet _typeSpectrumSet, bool _zonesShow)
{
    if(value){
        if(model){
            disconnect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        }
        model = value;
        connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));

        this->typeSpectrumSet = _typeSpectrumSet;

        spectrums = model->getSpectrumsModel(typeSpectrumSet);

//        if(_typeSpectrumSet == spectrumBase){
//            spectrums = model->getSpectrumsModelBase();
//        } else if(_typeSpectrumSet == spectrumsOnly){
//            spectrums = model->getSpectrumsModel();
//        } else {
//            spectrums = model->getSpectrumsModelAll();
//        }
        this->zonesShow = _zonesShow;

        widgetsShow();
    }
}

void SPRSpectrumGraphicsWidget::onChangeSelectedCheckedItems(QList<SPRSpectrumItemModel*> _checked, SPRSpectrumItemModel *_current){
    QList<int> selected;
    foreach(SPRSpectrumItemModel* item, _checked){
        int num = getGraphItemNumber(item);
        if(num < 0){
            continue;
        }
        selected.push_back(num);
    }

    int current = getGraphItemNumber(_current);

    onChangeSelectedCheckedItems(selected, current);
}

void SPRSpectrumGraphicsWidget::onChangeSelectedCheckedItems(QList<int> checked, int current)
{
    int size = model->getSpectrumsModelAll()->size();
    currentItem = current;
    if(checked.size() <= 0 && (currentItem >= 0 && currentItem < size)){
        visibleItems = QList<int>({currentItem});
    } else {
        visibleItems = checked;
    }

    if(currentItem >=0 && currentItem < size){
        zoomer->setCurrent(graphItems[currentItem]);
    } else {
        zoomer->setCurrent(nullptr);
    }
    setPorogsMovedItems();


    for(int i=0; i<size; i++){
        bool visible = false; bool curr = false;
        if(visibleItems.contains(i)){
            visible = true;
            if(i == currentItem || allCurrent){
               curr = true;
            }
        }
        if(i < graphItems.size() && i>=0){
            graphItems[i]->setVisible(visible, curr, zonesShow);
        }
    }
}

void SPRSpectrumGraphicsWidget::setPorogsMovedItems(){
    if(currentItem < 0){
        pm->setMovedItems({});
    } else {
        QVector<QwtPlotItem*> lpl;
        foreach(QwtPlotItem* pi, graphItems[currentItem]->zones.values()){
            lpl.push_back(pi);
        }
        pm->setMovedItems(lpl);
    }
}

void SPRSpectrumGraphicsWidget::setAllCurrent(bool value)
{
    allCurrent = value;
}

void SPRSpectrumGraphicsWidget::setZonesShow(bool value)
{
    zonesShow = value;
    widgetsShow();
}

SPRViewGraphicsMode SPRSpectrumGraphicsWidget::getGraphicsMode() const
{
    return graphicsMode;
}

void SPRSpectrumGraphicsWidget::setGraphicsMode(const SPRViewGraphicsMode &value)
{
    graphicsMode = value;
    if(model){
        QVector<SPRSpectrumItemModel*> *vect = model->getSpectrumsModel(spectrumsAll);
        double mul = 1;
        if(value == viewModeScales){
            uint32_t y_max = 0;
            for(int index=0; index<vect->size(); index++){
                uint32_t y = *vect->at(index)->getSpectrumData()->peak_value;
                if(y_max < y){
                    y_max = y;
                }
            }
            for(int index=0; index<vect->size(); index++){
                uint32_t val = *vect->at(index)->getSpectrumData()->peak_value;
                if(val > 0){
                    double mul = (double)y_max / (double)val;
                    vect->at(index)->setGraphicMode(value, mul);
                } else {
                    vect->at(index)->setGraphicMode(value, 1);
                }
            }
        } else {
            for(int index=0; index<vect->size(); index++){
                vect->at(index)->setGraphicMode(value, mul);
            }
        }
        widgetsShow();
    }
}

SPRSpectrumGraphicsWidget::SPRSpectrumGraphicsWidget(QWidget *parent) :
    QWidget(parent), model(nullptr)
{
    ui.setupUi(this);
    currentItem = -1;
    allCurrent = false;
    zonesShow = true;

    ui.canvas->setAxisScale(QwtPlot::Axis::xBottom, 0, 256, 10);
    QPalette pal = ui.canvas->palette();
    pal.setBrush(ui.canvas->foregroundRole(), QColor(Qt::white));

    ui.canvas->setPalette(pal);

    zoomer = new ScrollZoomer(ui.canvas->canvas(), ui.canvas);

    const QColor c( Qt::darkBlue );
    zoomer->setRubberBandPen( c );
    zoomer->setTrackerPen( c );

    pm = new SPRPorogsMoved(ui.canvas);
    connect(pm, SIGNAL(setSelectItem(QwtPlotItem*,MovedItemPosition)), this, SLOT(onCusorOverSelectItem(QwtPlotItem*, MovedItemPosition)));
    connect(pm, SIGNAL(changeArgumentValue(QwtPlotItem*,double,MovedItemPosition)), this, SLOT(onChangeSelectedItemValue(QwtPlotItem*,double,MovedItemPosition)));
}

void SPRSpectrumGraphicsWidget::onCusorOverSelectItem(QwtPlotItem *item, MovedItemPosition){
    if(currentItem >= 0 && currentItem < graphItems.size()){
        graphItems[currentItem]->setCursorSelectedZone(item);
    }
}

void SPRSpectrumGraphicsWidget::onChangeSelectedItemValue(QwtPlotItem *item, double distance, MovedItemPosition position)
{
    EnumElements el;
    if(graphItems[currentItem]->findItemKey(item, &el)){
        int th = graphItems[currentItem]->thread;
        SpectorRange *ranges = model->getZonesTableModel()->getElementsRanges(th, el);
        if(ranges){
            if(position == movedLeftSide || position == movedAllSides){
                SPRVariable<uint> *min = ranges->min;
                uint value = ranges->min->getData() + distance;
                min->setData(value);
            }
            if(position == movedRigthSize || position == movedAllSides){
                SPRVariable<uint> *max = ranges->max;
                uint value = ranges->max->getData() + distance;
                max->setData(value);
            }
        }
    }

}

void SPRSpectrumGraphicsWidget::widgetsShow()
{
    if(model){
        blockSignals(true);
        QList<int> oldVisible = visibleItems;
        int oldCurrent = currentItem;
        onChangeSelectedCheckedItems({}, -1);
        ui.canvas->detachItems();
        graphItems.clear();
        spectrums = model->getSpectrumsModel(typeSpectrumSet);
        for(int i=0; i<spectrums->size(); i++){
    //        if(i >= graphItems.size()){ // add new sectrum
//                GraphItem *gi = new GraphItem(spectrums->at(i), ui.canvas);
                SPRGraphItem *gi = new SPRGraphItem(spectrums->at(i), ui.canvas);
                graphItems.push_back(gi);
    //        }
        }
        onChangeSelectedCheckedItems(oldVisible, oldCurrent);
        setPorogsMovedItems();

//        QList<QwtPlotItem*> lqi;
//        foreach(int index, visibleItems){
//            if(graphItems[index]->spect->isVisible() && zonesShow){}
//        }
        blockSignals(false);
        ui.canvas->replot();
    }
}

void SPRSpectrumGraphicsWidget::onModelChanget(IModelVariable *)
{
    widgetsShow();
}
