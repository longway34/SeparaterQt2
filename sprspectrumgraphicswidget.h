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

#include "sprgraphitem.h"
#include "scrollzoomer.h"
#include "sprporogsmoved.h"

class SPRSpectrumGraphicsWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

public:
//    typedef struct graphItem{
//        QwtPlot *plot;

//        QwtPlotCurve *spect;
//        QMap<EnumElements, QwtPlotHistogram*> zones;
//        int thread;
//        SPRSpectrumItemModel *model;

//        graphItem(): spect(nullptr){}
//        graphItem(QwtPlotCurve *_spect, QwtPlot *_plot): spect(_spect), plot(_plot){}
//        graphItem(SPRSpectrumItemModel *_model, QwtPlot *_plot): spect(nullptr), plot(_plot){
//            setModelData(_model);
//        }

//        void setVisible(bool visible, bool current, bool showZones = true){
//            foreach(EnumElements el, zones.keys()){
//                QColor color = model->getZones()->getElementsProperty()->getColor(el);
//                QString zName = model->getZones()->getElementsProperty()->getSName(el);
//                color.setAlpha(128);
//                zones[el]->setBrush(QBrush(color));
//                zones[el]->setVisible(current && showZones);
//                zones[el]->setTitle(zName);
//            }
//            if(spect){
//                QColor cpen = model->getSpectrumColor();
//                if(current){
//                    QPen pen(cpen, 2);
//                    spect->setPen(pen);
//                    cpen.setAlpha(64);
//                    QBrush brush(cpen);
//                    spect->setBrush(brush);
//                } else {
//                    QPen pen(cpen,0.5);
//                    spect->setPen(pen);
//                    cpen.setAlpha(0);
//                    spect->setBrush(QBrush(cpen));
//                }
//                spect->setVisible(visible);
//            }
//        }

//        ~graphItem(){
//            foreach (EnumElements el, model->getZones()->getZones().keys()) {
//                zones[el]->detach();
//                delete zones[el];
//            }
//            spect->detach();
//            delete spect;
//        }
//    protected:
//        virtual void setModelData(SPRSpectrumItemModel *_model){
//            model = _model;
//            if(model){
//                spect = new QwtPlotCurve(QString(model->getSpectrumData()->name));
//                spect->setSamples(model->getSpectrumGraphics());
//                spect->setPen(QPen(model->getSpectrumColor(), 0.5));
//                foreach (EnumElements el, model->getZones()->getZones().keys()) {
//                    zones[el] = new QwtPlotHistogram(model->getZones()->getZones()[el]->element->fName->getData());
//                    zones[el]->setSamples(model->getZonesGaphics()[el]);
//                    if(plot){
//                        zones[el]->attach(plot);
//                    }
//                }
//                if(plot){
//                    spect->attach(plot);
//                }
//            }

//        }
////        void clear(){
////            QPolygonF nullPolygon();
////            spect
////        }
//    } GraphItem;

//    class MyZoomer: public QwtPlotZoomer
//    {

//        GraphItem *current;
//    public:
//        MyZoomer( QWidget *canvas ):
//            QwtPlotZoomer( canvas ), current(nullptr)
//        {
//            setTrackerMode( AlwaysOn );
//        }

//        virtual QwtText trackerTextF( const QPointF &pos ) const
//        {
//            QColor bg( Qt::white );
//            bg.setAlpha( 200 );
//            QwtText nText;
//            if(current){
//                int x = pos.toPoint().rx(); int y=pos.toPoint().ry();
//                size_t size = current->spect->dataSize();
//                if(x>=0 && x<current->spect->dataSize()){
//                    nText = QString(tr("%1 (%2ch : %3)")).
//                                  arg(current->model->getSpectrumName()).
//                                  arg(QString::number(x)).
//                                  arg(QString::number(current->spect->sample(x).toPoint().ry()));
//                }
//            } else {
//                nText = QwtPlotZoomer::trackerTextF( pos );
//            }
////            QwtText text = QwtPlotZoomer::trackerTextF( pos );
////            text.setBackgroundBrush( QBrush( bg ) );
//            nText.setBackgroundBrush(QBrush(bg));
//            return nText;
//        }
//        void setCurrent(GraphItem *value){current = value;}
//    };

    SPRSpectrumListItemsModel *model;
    QVector<SPRSpectrumItemModel*> *spectrums;
    QVector<SPRGraphItem*> graphItems;

    SPRViewGraphicsMode graphicsMode;

//    MyZoomer *zoomer;

    ScrollZoomer *zoomer;
    SPRPorogsMoved *pm;

    int getGraphItemNumber(SPRSpectrumItemModel* _mod){
        int ret = -1;
        for(int index=0; index<spectrums->size(); index++){
            if(_mod == spectrums->at(index)){
                ret = index;
                break;
            }
        }
        return ret;
    }


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

    SPRViewGraphicsMode getGraphicsMode() const;
    void setGraphicsMode(const SPRViewGraphicsMode &value);

public slots:
    virtual void widgetsShow();
    void onChangeSelectedCheckedItems(QList<int> checked, int current);

    void setZonesShow(bool value);
    virtual void onModelChanget(IModelVariable *);

    // ISPRWidget interface

    void onChangeSelectedCheckedItems(QList<SPRSpectrumItemModel *> checked, SPRSpectrumItemModel *current);
protected:
    void setPorogsMovedItems();
protected slots:
    void onCusorOverSelectItem(QwtPlotItem *item, MovedItemPosition);
    void onChangeSelectedItemValue(QwtPlotItem *item, double distance, MovedItemPosition position);
};

#endif // SPRSPECTRUMGRAPHICSWIDGET_H
