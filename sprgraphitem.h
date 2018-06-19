#ifndef SPRGRAPHITEM_H
#define SPRGRAPHITEM_H

#include <QObject>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_histogram.h>

#include "models/sprspectrumitemmodel.h"

class SPRGraphItem
{
public:
    QwtPlot *plot;

    QwtPlotCurve *spect;
    QMap<EnumElements, QwtPlotHistogram*> zones;
    int thread;
    SPRSpectrumItemModel *model;

    SPRGraphItem(): spect(nullptr){}
    SPRGraphItem(QwtPlotCurve *_spect, QwtPlot *_plot): spect(_spect), plot(_plot){}
    SPRGraphItem(SPRSpectrumItemModel *_model, QwtPlot *_plot): spect(nullptr), plot(_plot){
        setModelData(_model);
    }

    void setVisible(bool visible, bool current, bool showZones = true){
        foreach(EnumElements el, zones.keys()){
            QColor color = model->getZones()->getElementsProperty()->getColor(el);
            QString zName = model->getZones()->getElementsProperty()->getSName(el);
            color.setAlpha(128);
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
