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
#include <qwt_legend.h>

#include "sprgraphitem.h"
#include "scrollzoomer.h"
#include "sprporogsmoved.h"

class SPRSpectrumGraphicsWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

public:

    SPRSpectrumListItemsModel *model;
    QVector<SPRSpectrumItemModel*> *spectrums;
    QVector<SPRSpectrumItemModel*> spectSource;
    QVector<SPRGraphItem*> graphItems;

    SPRViewGraphicsMode graphicsMode;

    int currentThreadIndex;
    int currentThread;
//    MyZoomer *zoomer;

    ScrollZoomer *zoomer;
    SPRPorogsMoved *porogsMoved;

    QwtLegend *legend;

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


    QList<int> visibleThreads;

    QList<SPRGraphItem*> visibleItems;
    SPRGraphItem *currentItem;
    bool allCurrent;
    bool zonesShow;
    bool enableChangeTypeSet;
    bool withLegend;
    bool allVisible;



    SPRTypeSpectrumSet typeSpectrumSet;
    SPRTypeSpectrumSet typeSpectrumSetDef;

public:
    explicit SPRSpectrumGraphicsWidget(QWidget *parent = 0);
    Ui::SPRSpectrumGraphicsWidget ui;

private:
    void clearVisibleItems(){

    }
    // ISPRWidget interface
public:

    virtual ISPRModelData *getModelData(){return model;}
    void setModelData(SPRSpectrumListItemsModel *value, SPRTypeSpectrumSet typeSpectrumSet, bool _zonesShow, bool _enableChangeTypeSet = false);
    void setVisibleAll(bool _allVisible = true){
        allVisible = _allVisible;
//        visibleItems.clear();
//        for(int i=0; i<spectrums->size(); i++){
//            SPRGraphItem *gr = findGraphItemByModel(spectrums->at(i));
//            if(gr){
//                visibleItems.push_back(gr);
//            }
//        }
    }
    void setAllCurrent(bool value);
    QwtPlot *getCanvas(){
        return ui.canvas;
    }

    SPRViewGraphicsMode getGraphicsMode() const;
    void setGraphicsMode(const SPRViewGraphicsMode &value);

    SPRPorogsMoved *getPorogsMoved() const;
    void setPorogsMoved(SPRPorogsMoved *value);

    bool getEnableChangeTypeSet() const;
    void setEnableChangeTypeSet(bool value);

    bool getWithLegend() const;
    void setWithLegend(bool value);

    QList<int> getVisibleThreads() const;
    void setVisibleThreads(const QList<int> &value);

    int getCurrentThread() const;

    void setCurrentThread(int value);

    SPRGraphItem *getCurrentItem() const;
    void setCurrentItem(SPRGraphItem *value);
    SPRGraphItem *findGraphItemByModel(SPRSpectrumItemModel *model);

public slots:
    virtual void widgetsShow();
    //    void onChangeSelectedCheckedItems(QList<int> checked, int current);

    void setZonesShow(bool value);
    virtual void onModelChanget(IModelVariable *);
    void onChangeGraphicItemsColor(SPRSpectrumItemModel* _item, QColor _color);

    // ISPRWidget interface

    void onChangeSelectedCheckedItems(QList<SPRSpectrumItemModel *> checked, SPRSpectrumItemModel *current);
protected:
    void setPorogsMovedItems();
    void clearGraphics(bool all=false);
    void clearPorogsMovedItems();
protected slots:
    void onCusorOverSelectItem(QwtPlotItem *item, MovedItemPosition);
    void onChangeSelectedItemValue(QwtPlotItem *item, double distance, MovedItemPosition position);
    void onDblClickMouse();
};

#endif // SPRSPECTRUMGRAPHICSWIDGET_H
