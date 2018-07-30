#ifndef SPRGRAPHITEM_H
#define SPRGRAPHITEM_H

#include <QObject>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_histogram.h>

#include "models/sprspectrumitemmodel.h"
#include "sprporogsmoved.h"
#include "_types.h"


class SPRGraphItem
{
public:
    QwtPlot *plot;

    QwtPlotCurve *spect;
    QMap<EnumElements, QwtPlotHistogram*> zones;
//    uint thread;
    SPRSpectrumItemModel *model;
    QPen defZonesPen;
    QPen selZonesPen;

//    SPRGraphItem(): plot(nullptr), spect(nullptr){}
//    SPRGraphItem(QwtPlotCurve *_spect, QwtPlot *_plot): plot(_plot), spect(_spect) {}
    SPRGraphItem(SPRSpectrumItemModel *_model, QwtPlot *_plot);

    int getThread();
    void setCursorSelectedZone(QwtPlotItem *item);

    bool findItemKey(QwtPlotItem *item, EnumElements *key);

    void setVisible(bool visible, bool current, bool showZones = true);

    ~SPRGraphItem();
    SPRSpectrumItemModel *getModelData();
    virtual void setModelData(SPRSpectrumItemModel *_model);

protected:
};

#endif // SPRGRAPHITEM_H
