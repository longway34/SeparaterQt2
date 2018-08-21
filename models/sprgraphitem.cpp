#include "models/sprgraphitem.h"
#include "models/sprspectrumitemmodel.h"


SPRSpectrumItemModel *SPRGraphItem::getModel()
{
    return model;
}

double SPRGraphItem::getModeKoeff()
{
    return modeKoeff;
}

void SPRGraphItem::setModeKoeff(double value)
{
    modeKoeff = value;
}

void SPRGraphItem::renameGraph(QString _name){
    if(_name.isEmpty()){
        if(model){
            _name = model->getSpectrumName();
        }
    }
    if(spect){
        spect->setTitle(_name);
    }
    if(model){
        foreach(QwtPlotHistogram *zone, zones.values()){
            EnumElements el = zones.key(zone);
            QString _elName = model->getZones()->getElementsProperty()->getSName(el);
            zone->setTitle(_elName);
        }
    }
}

SPRGraphItem::SPRGraphItem(SPRSpectrumItemModel *_model, double _modeKoeff): /*plot(_plot), */
    model(nullptr), spect(nullptr) {
//    modeKoeff = 1;
    setModelData(_model, _modeKoeff);
    //        thread = _model->getThread();
    defZonesPen.setWidth(1);
    defZonesPen.setStyle(Qt::SolidLine);

    selZonesPen.setWidth(2);
    selZonesPen.setStyle(Qt::DashLine);

}

int SPRGraphItem::getThread(){
    return model->getThread();
}

void SPRGraphItem::setCursorSelectedZone(QwtPlotItem *item){
    foreach(QwtPlotHistogram *zone, zones.values()){
        QColor color = zone->pen().color();
        if(item == zone){
            selZonesPen.setColor(color);
            zone->setPen(selZonesPen);
        } else {
            defZonesPen.setColor(color);
            zone->setPen(defZonesPen);
        }
    }
}

bool SPRGraphItem::findItemKey(QwtPlotItem *item, EnumElements *key){
    bool res = false;
    if(zones.values().contains((QwtPlotHistogram*)item)){
        res = true;
        if(key){
            *key = zones.key((QwtPlotHistogram*)item);
        }
    }
    return res;
}

void SPRGraphItem::setVisible(QwtPlot *plot, bool visible, bool current, bool showZones){
    foreach (EnumElements el, zones.keys()) {
        if(current && showZones){
            QColor color=model->getZones()->getElementsProperty()->getColor(el);
            color.setAlpha(128);
            defZonesPen.setColor(color);
            zones[el]->setPen(defZonesPen);
            zones[el]->setBrush(QBrush(color));
            if(!plot->itemList().contains(zones[el])){
                zones[el]->attach(plot);
            }
        } else {
            if(plot->itemList().contains(zones[el])){
                zones[el]->detach();
            }
        }
    }
    if(visible){
        QColor cpen = model->getSpectrumColor();
        if(current){
            QPen pen(cpen, 2);
            spect->setPen(pen);
            cpen.setAlpha(64);
            QBrush brush(cpen);
            spect->setBrush(brush);
        } else {
            QPen pen(cpen,0.5);
            spect->setPen(pen);
            cpen.setAlpha(0);
            spect->setBrush(QBrush(cpen));
        }
        if(!plot->itemList().contains(spect)){
            spect->attach(plot);
        }
    } else {
        if(plot->itemList().contains(spect)){
            spect->detach();
        }
    }
}

SPRGraphItem::~SPRGraphItem(){
//    if(plot){
        foreach (QwtPlotHistogram *zone, zones.values()) {
            //                plot->removeItem(zones[el]);
//            if(plot->itemList().contains(zone)) zone->detach();
            if(zone) delete zone;
        }
//        if(plot->itemList().contains(spect)) spect->detach();
        if(spect) delete spect;
//    }
}

SPRSpectrumItemModel *SPRGraphItem::getModelData()
{
    return model;
}

void SPRGraphItem::setModelData(SPRSpectrumItemModel *_model, double _modeKoeff){
    if(model && model != _model){
        disconnect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
    }

    model = _model;

    if(model){
        modeKoeff = _modeKoeff;

        onModelChanget(_model);
        connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
    }
}

void SPRGraphItem::onModelChanget(IModelVariable *_model)
{
    model = (SPRSpectrumItemModel*)_model;
    if(model){
        if(!spect){
            spect = new QwtPlotCurve(QString(model->getSpectrumName()));
        }

        QVector<QPointF> samples = model->getSpectrumGraphics();
        for(int x=0; x<samples.size(); x++){
            double y = samples[x].ry() * modeKoeff;
            samples[x].setY(y);
        }
        spect->setSamples(samples);
        spect->setPen(QPen(model->getSpectrumColor(), 0.5));
        foreach (EnumElements el, model->getZones()->getZones().keys()) {
            if(!zones[el]){
                zones[el] = new QwtPlotHistogram(model->getZones()->getZones()[el]->element->fName->getData());
            }
            QVector<QwtIntervalSample> intervals = model->getZonesGaphics()[el];
            for(int i=0; i<intervals.size(); i++){
                double value = intervals[i].value * modeKoeff;
                intervals[i].value = value;
            }
            zones[el]->setSamples(intervals);
        }
        renameGraph();
    }
}
