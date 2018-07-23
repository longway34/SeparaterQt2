#ifndef SPRGRAPHITEM_H
#define SPRGRAPHITEM_H

#include <QObject>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_histogram.h>

#include "models/sprspectrumitemmodel.h"
#include "sprporogsmoved.h"

class SPRGraphItem
{
public:
    QwtPlot *plot;

    QwtPlotCurve *spect;
    QMap<EnumElements, QwtPlotHistogram*> zones;
    uint thread;
    SPRSpectrumItemModel *model;
    QPen defZonesPen;
    QPen selZonesPen;

    SPRGraphItem(): spect(nullptr){}
    SPRGraphItem(QwtPlotCurve *_spect, QwtPlot *_plot): spect(_spect), plot(_plot){}
    SPRGraphItem(SPRSpectrumItemModel *_model, QwtPlot *_plot): spect(nullptr), plot(_plot){
        setModelData(_model);
        thread = _model->getThread();
        defZonesPen.setWidth(1);
        defZonesPen.setStyle(Qt::SolidLine);

        selZonesPen.setWidth(2);
        selZonesPen.setStyle(Qt::DashLine);
    }

    setCursorSelectedZone(QwtPlotItem *item){
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

    bool findItemKey(QwtPlotItem *item, EnumElements *key){
        bool res = false;
        if(zones.values().contains((QwtPlotHistogram*)item)){
            res = true;
            if(key){
                *key = zones.key((QwtPlotHistogram*)item);
            }
        }
        return res;
    }

    void setVisible(bool visible, bool current, bool showZones = true){
        foreach(EnumElements el, zones.keys()){
            QColor color = model->getZones()->getElementsProperty()->getColor(el);
            QString zName = model->getZones()->getElementsProperty()->getSName(el);
            color.setAlpha(128);
            defZonesPen.setColor(color);
            zones[el]->setPen(defZonesPen);

            zones[el]->setBrush(QBrush(color));
            zones[el]->setVisible(current && showZones);
            zones[el]->setTitle(zName);
        }
        if(spect){
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
            spect->setVisible(visible);
        }
    }

    ~SPRGraphItem(){
        foreach (EnumElements el, model->getZones()->getZones().keys()) {
            zones[el]->detach();
            delete zones[el];
        }
        spect->detach();
        delete spect;
    }
protected:
    virtual void setModelData(SPRSpectrumItemModel *_model){
        model = _model;
        if(model){
            spect = new QwtPlotCurve(QString(model->getSpectrumData()->name));
            spect->setSamples(model->getSpectrumGraphics());
            spect->setPen(QPen(model->getSpectrumColor(), 0.5));
            foreach (EnumElements el, model->getZones()->getZones().keys()) {
                if(!zones.contains(el)){
                    zones[el] = nullptr;
                }
                zones[el] = new QwtPlotHistogram(model->getZones()->getZones()[el]->element->fName->getData());
                zones[el]->setSamples(model->getZonesGaphics()[el]);
                if(plot){
                    zones[el]->attach(plot);
                }
            }
            if(plot){
                spect->attach(plot);
            }
        }

    }
//        void clear(){
//            QPolygonF nullPolygon();
//            spect
//        }
};

#endif // SPRGRAPHITEM_H
