#ifndef SPRSEPARATEHISTOGRAMMREPORT_H
#define SPRSEPARATEHISTOGRAMMREPORT_H

#include <QObject>
#include <QWidget>

#include "models/sprmainmodel.h"
#include "models/sprhistorymodel.h"

#include <qwt_plot.h>
#include <qwt_plot_barchart.h>

#include <qwt_scale_draw.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_multi_barchart.h>

class SPRSeparateHistogrammReport : public QwtPlot
{

    Q_OBJECT

    class DrawTitle : public QwtScaleDraw{

        bool withThreads;
        // QwtAbstractScaleDraw interface
    public:
        DrawTitle(bool _allThreads=false);
        virtual QwtText label(double v) const;
    };

//    QVector<double> series;

    SPRHistoryModel *model;
    SPRMainModel *mainModel;

    DrawTitle *drawTitle;
    SPRThreadList workThreadsSource;
    SPRThreadList threads;

    QwtPlotMarker *porogCurveCritical;
    QwtPlotMarker *porogCurvePermitt;

    QwtPlotMultiBarChart *chart;

    bool withOutPorogs;
    SPRWorkSeparateTypeData type;
//    qint64 lastTime_in_sec;

    QwtText title;

public:
    SPRSeparateHistogrammReport(QWidget *parent = nullptr);

    void setModelData(SPRMainModel *_MainModel, SPRHistoryModel *_histModel, QString _title, QColor _barColor, SPRThreadList _threads = {}, SPRWorkSeparateTypeData _type=percentConcentrate2Input, bool _woPorogs = true);
//    void drawPorogs();
public slots:
    void onModelChanget(IModelVariable *send);
protected:
//    QVector<double> getMySeries(double value, double porog1, double porog2) const;
};

#endif // SPRSEPARATEHISTOGRAMMREPORT_H
