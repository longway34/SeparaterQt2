#ifndef SPRSPECTRUMGRAPHICSWIDGET_H
#define SPRSPECTRUMGRAPHICSWIDGET_H

#include "ui_sprspectrumgraphicswidget.h"
#include "isprwidget.h"
#include "models/sprspectrumlistitemsmodel.h"
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_grid.h>

class SPRSpectrumGraphicsWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    typedef struct graphItem{
        QwtPlot *plot;

        QwtPlotCurve *spect;
        QMap<EnumElements, QwtPlotHistogram*> zones;
        int thread;
        SPRSpectrumItemModel *model;

        graphItem(): spect(nullptr){}
        graphItem(QwtPlotCurve *_spect, QwtPlot *_plot): spect(_spect), plot(_plot){}
        graphItem(SPRSpectrumItemModel *_model, QwtPlot *_plot): spect(nullptr), plot(_plot){
            setModel(_model);

        }
        void setModel(SPRSpectrumItemModel *_model){
            model = _model;
            if(model){
                spect = new QwtPlotCurve(QString(model->getSpectrumData()->name));
                spect->setSamples(model->getSpectrumGraphics());
                spect->setPen(QPen(model->getSpectrumColor(), 0.5));
                foreach (EnumElements el, model->getZones()->elements.keys()) {
                    zones[el] = new QwtPlotHistogram(DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el].name);
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

        void setVisible(bool visible, bool current, bool showZones = true){
            foreach(EnumElements el, zones.keys()){
                QColor color = model->getZones()->elements[el].color;
                color.setAlpha(64);
                zones[el]->setBrush(QBrush(color));
                zones[el]->setVisible(current && showZones);
            }
            if(spect){
                spect->setVisible(visible);
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
            }
        }
//        void clear(){
//            QPolygonF nullPolygon();
//            spect
//        }
    } GraphItem;


    SPRSpectrumListItemsModel *model;
    QVector<GraphItem*> graphItems;

    QList<int> visibleItems;
    int currentItem;
    bool allCurrent;
    bool zonesShow;

public:
    explicit SPRSpectrumGraphicsWidget(QWidget *parent = 0);

private:
    Ui::SPRSpectrumGraphicsWidget ui;
    
    // ISPRWidget interface
public:


    virtual SPRSpectrumListItemsModel *getModel(){return model;}
    void setModel(SPRSpectrumListItemsModel *value);
    void setVisibleAll(){
        visibleItems.clear();
        for(int i=0; i<model->getSpectrumsModel()->size(); i++){
            visibleItems.push_back(i);
        }
    }
    void setAllCurrent(bool value);


public slots:
    virtual void widgetsShow();
    void onChangeSelectedCheckedItems(QList<int> checked, int current);

    void setZonesShow(bool value);
};

#endif // SPRSPECTRUMGRAPHICSWIDGET_H
