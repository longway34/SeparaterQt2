#include "sprspectrumgraphicswidget.h"

void SPRSpectrumGraphicsWidget::setModelData(SPRSpectrumListItemsModel *value, SPRTypeSpectrumSet _typeSpectrumSet, bool _zonesShow, bool _enableChangeTypeSet)
{
    if(value){
        if(model){
            disconnect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        }
        model = value;
        connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));

        this->typeSpectrumSet = _typeSpectrumSet;
        this->typeSpectrumSetDef = _typeSpectrumSet;
        setEnableChangeTypeSet(_enableChangeTypeSet);

        spectSource = model->getSpectrumsItemByThread(currentThread, typeSpectrumSet);
        spectrums = &spectSource;

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

void SPRSpectrumGraphicsWidget::setVisibleAll(bool _allVisible){
    allVisible = _allVisible;
    //        visibleItems.clear();
    //        for(int i=0; i<spectrums->size(); i++){
    //            SPRGraphItem *gr = findGraphItemByModel(spectrums->at(i));
    //            if(gr){
    //                visibleItems.push_back(gr);
    //            }
    //        }
}

void SPRSpectrumGraphicsWidget::onChangeSelectedCheckedItems(QList<SPRSpectrumItemModel*> _checked, SPRSpectrumItemModel *_current){
    visibleItems.clear();
    foreach(SPRSpectrumItemModel* model, _checked){
        SPRGraphItem *gr = findGraphItemByModel(model);
        if(gr ||allCurrent){
            visibleItems.push_back(gr);
        }
    }
    //    visibleItems = _checked;
    currentItem = findGraphItemByModel(_current);

//    zoomer->setCurrent(currentItem);

//    widgetsShow();
}

//void SPRSpectrumGraphicsWidget::onChangeSelectedCheckedItems(QList<int> checked, int current)
//{
//    int size = model->getSpectrumsModelAll()->size();
//    currentItem = current;
//    if(checked.size() <= 0 && (currentItem >= 0 && currentItem < size)){
//        visibleItems = QList<int>({currentItem});
//    } else {
//        visibleItems = checked;
//    }

//    if(currentItem >=0 && currentItem < size){
//        zoomer->setCurrent(graphItems[currentItem]);
//    } else {
//        zoomer->setCurrent(nullptr);
//    }
//    setPorogsMovedItems();


//    for(int i=0; i<size; i++){
//        bool visible = false; bool curr = false;
//        if(visibleItems.contains(i)){
//            visible = true;
//            if(i == currentItem || allCurrent){
//               curr = true;
//            }
//        }
//        if(i < graphItems.size() && i>=0){
//            graphItems[i]->setVisible(visible, curr, zonesShow);
//        }
//    }
//}

void SPRSpectrumGraphicsWidget::clearPorogsMovedItems(){
    if(porogsMoved){
        if(spectrums){
            foreach(SPRSpectrumItemModel* model, *spectrums){
                foreach(QwtPlotItem *pi, model->getGraphic()->zones.values()){
//                foreach(QwtPlotItem *pi, graphItems[index]->zones.values()){
                    porogsMoved->remoteItem(pi);
                }
            }
        }
    }
}

void SPRSpectrumGraphicsWidget::setPorogsMovedItems(){
    if(porogsMoved){
        clearPorogsMovedItems();
        if(currentItem){
            QList<QwtPlotItem*> lpl;
            foreach(QwtPlotItem* pi, currentItem->zones.values()){
                lpl << pi;
            }
            porogsMoved->addMovedItems(lpl);
        }
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
        model->setGraphicMode(value);
    }
}

SPRPorogsMoved *SPRSpectrumGraphicsWidget::getPorogsMoved() const
{
    return porogsMoved;
}

void SPRSpectrumGraphicsWidget::setPorogsMoved(SPRPorogsMoved *value)
{
    porogsMoved = value;
}

bool SPRSpectrumGraphicsWidget::getEnableChangeTypeSet() const
{
    return enableChangeTypeSet;
}

void SPRSpectrumGraphicsWidget::setEnableChangeTypeSet(bool value)
{
    enableChangeTypeSet = value;
    if(enableChangeTypeSet){
        visibleThreads.clear();
        int ths = model != nullptr ? model->getThreads()->getData() : MAX_SPR_MAIN_THREADS;
        visibleThreads.clear();

        for(int i=0; i<ths; i++){
            visibleThreads.push_back(i);
        }
    }

}

bool SPRSpectrumGraphicsWidget::getWithLegend() const
{
    return withLegend;
}

void SPRSpectrumGraphicsWidget::setWithLegend(bool value)
{
    withLegend = value;

    if(legend) delete legend;
    if(withLegend){
        legend = new QwtLegend();
    }
    ui.canvas->insertLegend(legend, QwtPlot::BottomLegend);
}

SPRThreadList SPRSpectrumGraphicsWidget::getVisibleThreads() const
{
    return visibleThreads;
}

void SPRSpectrumGraphicsWidget::setVisibleThreads(const SPRThreadList &value)
{
    visibleThreads = value;
}

int SPRSpectrumGraphicsWidget::getCurrentThread() const
{
    return currentThread;
}

void SPRSpectrumGraphicsWidget::setCurrentThread(int value)
{
    currentThread = value;
}

SPRGraphItem *SPRSpectrumGraphicsWidget::getCurrentItem() const
{
    return currentItem;
}

void SPRSpectrumGraphicsWidget::setCurrentItem(SPRGraphItem *value)
{
    currentItem = value;
}

int SPRSpectrumGraphicsWidget::getGraphItemNumber(SPRSpectrumItemModel *_mod){
    int ret = -1;
    for(int index=0; index<spectrums->size(); index++){
        if(_mod == spectrums->at(index)){
            ret = index;
            break;
        }
    }
    return ret;
}

SPRSpectrumGraphicsWidget::SPRSpectrumGraphicsWidget(QWidget *parent) :
    QWidget(parent), model(nullptr), legend(nullptr)
{
    ui.setupUi(this);
    currentItem = nullptr;
    allCurrent = false;
    allVisible = false;
    zonesShow = true;
    currentThreadIndex = -1;
    currentThread = -1;
    setEnableChangeTypeSet(false);
    withLegend = false;
    typeSpectrumSet = spectrumsOnly;
    typeSpectrumSetDef = spectrumsOnly;

    ui.canvas->setAxisScale(QwtPlot::Axis::xBottom, 0, 256, 10);
    QPalette pal = ui.canvas->palette();
    pal.setBrush(ui.canvas->foregroundRole(), QColor(Qt::white));

    ui.canvas->setPalette(pal);

    zoomer = new ScrollZoomer(ui.canvas->canvas(), ui.canvas);
    zoomer->setCurrent(&currentItem);

    const QColor c( Qt::darkBlue );
    zoomer->setRubberBandPen( c );
    zoomer->setTrackerPen( c );

    porogsMoved = new SPRPorogsMoved(ui.canvas);
    connect(porogsMoved, SIGNAL(setSelectItem(QwtPlotItem*,MovedItemPosition)), this, SLOT(onCusorOverSelectItem(QwtPlotItem*, MovedItemPosition)));
    connect(porogsMoved, SIGNAL(changeArgumentValue(QwtPlotItem*,double,MovedItemPosition)), this, SLOT(onChangeSelectedItemValue(QwtPlotItem*,double,MovedItemPosition)));
    connect(porogsMoved, SIGNAL(dblClickMouseEvent()), this, SLOT(onDblClickMouse()));

    for(int th=0; th<MAX_SPR_MAIN_THREADS; th++){
        visibleThreads.push_back(th);
    }

}

void SPRSpectrumGraphicsWidget::onCusorOverSelectItem(QwtPlotItem *item, MovedItemPosition){
    if(currentItem){
        currentItem->setCursorSelectedZone(item);
    }
}

void SPRSpectrumGraphicsWidget::onChangeSelectedItemValue(QwtPlotItem *item, double distance, MovedItemPosition position)
{
    EnumElements el;
    if(currentItem){
        if(currentItem->findItemKey(item, &el)){
            int th = currentItem->getThread();
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
    } else {
    }

}

void SPRSpectrumGraphicsWidget::onDblClickMouse()
{
    if(model){
        if(enableChangeTypeSet){
            currentThreadIndex++;
            if(currentThreadIndex >= visibleThreads.size()){
                currentThreadIndex = -1;
                currentThread = -1;
                typeSpectrumSet = typeSpectrumSetDef;
            } else {
                currentThread = visibleThreads[currentThreadIndex];
                typeSpectrumSet = spectrumsMixed;
            }
            widgetsShow();
        }
    }
}

void SPRSpectrumGraphicsWidget::hideGraphics(QwtPlotItem *graph){
    if(graph && getCanvas()->itemList().contains(graph)){
        graph->detach();
    }
}

void SPRSpectrumGraphicsWidget::hideGraphics(SPRSpectrumItemModel *item){
    if(item){
        item->getGraphic()->setVisible(getCanvas(), false, false, zonesShow);
    }
}

void SPRSpectrumGraphicsWidget::hideGraphics(QList<SPRSpectrumItemModel *> *items,  bool all){
    if(all){
        items = model->getSpectrumsModel(typeSpectrumSetDef);
    }
    foreach(SPRSpectrumItemModel *item, *items){
        hideGraphics(item);
    }
}

void SPRSpectrumGraphicsWidget::hideGraphics(SPRTypeSpectrumSet type){
    if(model){
        QList<SPRSpectrumItemModel*> *lst = model->getSpectrumsModel(type);
        hideGraphics(lst, false);
    }
}

SPRGraphItem *SPRSpectrumGraphicsWidget::findGraphItemByModel(SPRSpectrumItemModel* model){
//    foreach(SPRGraphItem *item, graphItems){
//        if(item->getModelData() == model){
//            return item;
//        }
//    }
    return model->getGraphic();
}

void SPRSpectrumGraphicsWidget::widgetsShow()
{
    if(model){
        blockSignals(true);

        hideGraphics(spectrumsAll);

//        QList<SPRSpectrumItemModel*> visibleModels;
//        foreach(SPRGraphItem* gr, visibleItems){
//            SPRSpectrumItemModel *model = gr->getModelData();
//            if(model){
//                visibleModels.push_back(model);
//            }
//        }
//        SPRSpectrumItemModel* currentModel = nullptr;
//        if(currentItem){
//            currentModel = currentItem->getModelData();
//        }
//        onChangeSelectedCheckedItems({}, nullptr);
//        ui.canvas->detachItems();
        spectSource = model->getSpectrumsItemByThread(currentThread, typeSpectrumSet);
        spectrums = &spectSource;
        QList<SPRGraphItem*> graphItems;
        foreach(SPRSpectrumItemModel* model, *spectrums){
                SPRGraphItem *gi = model->getGraphic();
                graphItems << gi;
//                if(!gi){
//                    gi = new SPRGraphItem(model, ui.canvas);
//                }
//                gi->setModelData(model);
    //        }
        }
        foreach(SPRGraphItem *graph, graphItems){
            bool visible = false;
            if(spectrums->contains(graph->getModelData())){
                if(currentItem == graph || allCurrent || allVisible){
                   visible = true;
                }
//                if(allVisible){
//                    visible = true;
//                }
                if(visibleItems.size() > 0){
                    visible = visibleItems.contains(graph);
                }
            }
            graph->setVisible(ui.canvas, visible, graph==currentItem || allCurrent, zonesShow);
        }
//        onChangeSelectedCheckedItems(visibleModels, currentModel);
        setPorogsMovedItems();
//        if(zoomer){
//            zoomer->setCurrent(currentItem);
//        }
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
    if(model){
        spectSource = model->getSpectrumsItemByThread(-1, spectrumsAll);
        if(currentItem){
            if(!spectrums->contains(currentItem->getModelData())){
                currentItem = nullptr;
            }
        }
    }

    widgetsShow();
}

void SPRSpectrumGraphicsWidget::onChangeGraphicItemsColor(SPRSpectrumItemModel *_item, QColor _color)
{
    if(spectrums){
        SPRGraphItem *graph = findGraphItemByModel(_item);
        if(graph){
            QPen pen = graph->spect->pen();
            pen.setColor(_color);
            graph->spect->setPen(pen);
        }
    }
}
