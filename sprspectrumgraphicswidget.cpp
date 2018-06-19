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

void SPRSpectrumGraphicsWidget::setAllCurrent(bool value)
{
    allCurrent = value;
}

void SPRSpectrumGraphicsWidget::setZonesShow(bool value)
{
    zonesShow = value;
    widgetsShow();
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
}

void SPRSpectrumGraphicsWidget::widgetsShow()
{
    if(model){
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
        onChangeSelectedCheckedItems(visibleItems, currentItem);
        ui.canvas->replot();
    }
}

void SPRSpectrumGraphicsWidget::onModelChanget(IModelVariable *)
{
    widgetsShow();
}
