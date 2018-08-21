#ifndef SPRGRAPHITEM_H
#define SPRGRAPHITEM_H

#include <QObject>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_histogram.h>

//#include "models/sprspectrumitemmodel.h"

#include "sprporogsmoved.h"
#include "_types.h"

class IModelVariable;
class SPRSpectrumItemModel;


class SPRGraphItem :public QObject
{
//    QwtPlot *plot;

    Q_OBJECT

//    uint thread;
private:
    SPRSpectrumItemModel *model;
    QPen defZonesPen;
    QPen selZonesPen;

    double modeKoeff;
//    SPRGraphItem(): plot(nullptr), spect(nullptr){}
//    SPRGraphItem(QwtPlotCurve *_spect, QwtPlot *_plot): plot(_plot), spect(_spect) {}
public:
    QMap<EnumElements, QwtPlotHistogram*> zones;


    QwtPlotCurve *spect;
    SPRGraphItem(SPRSpectrumItemModel *_model, double _modeKoeff = 1);

    int getThread();
    void setCursorSelectedZone(QwtPlotItem *item);

    bool findItemKey(QwtPlotItem *item, EnumElements *key);

    void setVisible(QwtPlot *plot, bool visible, bool current, bool showZones = true);

    ~SPRGraphItem();
    SPRSpectrumItemModel *getModelData();
    virtual void setModelData(SPRSpectrumItemModel *_model, double _modeKoeff);

    SPRSpectrumItemModel *getModel();

    double getModeKoeff();
    void setModeKoeff(double value);
    void renameGraph(QString _name = "");

public slots:
    void onModelChanget(IModelVariable* _model);
protected:
};

#endif // SPRGRAPHITEM_H
