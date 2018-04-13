#include "sprspectrumgraphicswidget.h"

void SPRSpectrumGraphicsWidget::setModel(SPRSpectrumListItemsModel *value, SPRTypeSpectrumSet _typeSpectrumSet, bool _zonesShow)
{
    if(value){
        model = value;

        this->typeSpectrumSet = _typeSpectrumSet;
        if(_typeSpectrumSet == spectrumBase){
            spectrums = model->getSpectrumsModelBase();
        } else if(_typeSpectrumSet == spectrumsOnly){
            spectrums = model->getSpectrumsModel();
        } else {
            spectrums = model->getSpectrumsModelAll();
        }
        this->zonesShow = _zonesShow;

        connect(model, SIGNAL(modelChanget()), this, SLOT(widgetsShow()));

        widgetsShow();
    }
}


void SPRSpectrumGraphicsWidget::onChangeSelectedCheckedItems(QList<int> checked, int current)
{
    visibleItems = checked;
    currentItem = current;

    for(int i=0; i<model->getSpectrumsModel()->size(); i++){
        bool visible = false; bool curr = false;
        if(visibleItems.contains(i)){
            visible = true;
            if(i == currentItem || allCurrent){
               curr = true;
            }
        }
        graphItems[i]->setVisible(visible, curr, zonesShow);
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
    QWidget(parent)
{
    ui.setupUi(this);
    currentItem = -1;
    allCurrent = false;
    zonesShow = true;

    ui.canvas->setAxisScale(QwtPlot::Axis::xBottom, 0, 256, 10);
}

void SPRSpectrumGraphicsWidget::widgetsShow()
{
    ui.canvas->detachItems();
    graphItems.clear();
    for(int i=0; i<spectrums->size(); i++){
//        if(i >= graphItems.size()){ // add new sectrum
            GraphItem *gi = new GraphItem(spectrums->at(i), ui.canvas);
            graphItems.push_back(gi);
//        }
    }
    onChangeSelectedCheckedItems(visibleItems, currentItem);
    ui.canvas->replot();
}
