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
#include <qwt_plot_zoomer.h>

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
            setModelData(_model);
        }

        void setVisible(bool visible, bool current, bool showZones = true){
            foreach(EnumElements el, zones.keys()){
                QColor color = model->getZones()->getElementsProperty()->getColor(el);
                color.setAlpha(128);
                zones[el]->setBrush(QBrush(color));
                zones[el]->setVisible(current && showZones);
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

        ~graphItem(){
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
    } GraphItem;

    class MyZoomer: public QwtPlotZoomer
    {

        GraphItem *current;
    public:
        MyZoomer( QWidget *canvas ):
            QwtPlotZoomer( canvas ), current(nullptr)
        {
            setTrackerMode( AlwaysOn );
        }

        virtual QwtText trackerTextF( const QPointF &pos ) const
        {
            QColor bg( Qt::white );
            bg.setAlpha( 200 );
            QwtText nText;
            if(current){
                int x = pos.toPoint().rx(); int y=pos.toPoint().ry();
                nText = QString(tr("%1 (%2ch : %3)")).
                                  arg(current->model->getSpectrumName()).
                                  arg(QString::number(x)).
                                  arg(QString::number(current->spect->sample(x).toPoint().ry()));
            } else {
                nText = QwtPlotZoomer::trackerTextF( pos );
            }
//            QwtText text = QwtPlotZoomer::trackerTextF( pos );
//            text.setBackgroundBrush( QBrush( bg ) );
            nText.setBackgroundBrush(QBrush(bg));
            return nText;
        }
        void setCurrent(GraphItem *value){current = value;}
    };

    SPRSpectrumListItemsModel *model;
    QVector<SPRSpectrumItemModel*> *spectrums;
    QVector<GraphItem*> graphItems;

    MyZoomer *zoomer;

    QList<int> visibleItems;
    int currentItem;
    bool allCurrent;
    bool zonesShow;

    SPRTypeSpectrumSet typeSpectrumSet;

public:
    explicit SPRSpectrumGraphicsWidget(QWidget *parent = 0);

private:
    Ui::SPRSpectrumGraphicsWidget ui;

    // ISPRWidget interface
public:


    virtual ISPRModelData *getModelData(){return model;}
    void setModelData(SPRSpectrumListItemsModel *value, SPRTypeSpectrumSet typeSpectrumSet, bool _zonesShow);
    void setVisibleAll(){
        visibleItems.clear();
        for(int i=0; i<spectrums->size(); i++){
            visibleItems.push_back(i);
        }
    }
    void setAllCurrent(bool value);
    QwtPlot *getCanvas(){
        return ui.canvas;
    }

public slots:
    virtual void widgetsShow();
    void onChangeSelectedCheckedItems(QList<int> checked, int current);

    void setZonesShow(bool value);
    virtual void onModelChanget(IModelVariable *);

    // ISPRWidget interface

protected:
};

#endif // SPRSPECTRUMGRAPHICSWIDGET_H
