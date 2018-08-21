#ifndef SPRHITORYGRAPHICS_H
#define SPRHITORYGRAPHICS_H

#include <QObject>
#include <QWidget>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>

#include "models/sprseparatemodel.h"
#include "models/sprmainmodel.h"

#include "models/sprhistorymodel.h"

/**
 * @brief The TimeScaleDraw class
 */
class TimeScaleDraw: public QwtScaleDraw
{
public:
    TimeScaleDraw(  )
//        baseTime( base )
    {
    }
    virtual QwtText label( double v ) const
    {
        QDateTime time = QDateTime::fromSecsSinceEpoch( static_cast<int>( v ) );
        QwtText txt = QwtText(time.toString("hh:mm"));
        QFont sys = QFont("Tahoma", 8);
        txt.setFont(sys);
        return txt;
    }
private:
//    QTime baseTime;
};
/**
 * @brief The SPRHitoryGraphics class
 */
class SPRHitoryGraphics: public QwtPlot
{
    Q_OBJECT

    class HistoryGraphic {
        uint8_t thread;
        QwtPlotCurve *curve;
        QVector<QPointF> curveData;
        QColor color;
        QString name;


    public:
        HistoryGraphic(uint8_t _th, QString _name, QColor _color): thread(_th), curve(nullptr), name(_name), color(_color){
            curve = new QwtPlotCurve(_name);
            QPen pen(_color);
            curve->setPen(pen);
        }
        virtual ~HistoryGraphic(){
            if(curve){
                delete curve;
            }
        }
        QwtPlotCurve *getGraphic(){
            return curve;
        }
        uint8_t getThread(){
            return thread;
        }
        void setThread(const uint8_t &value){
            thread = value;
        }
        QVector<QPointF> getGraphicData(){
            return curveData;
        }
        void setGraphicData(const QVector<QPointF> &value){
            curveData = value;
            if(curve){
                curve->setSamples(curveData);
            }
        }
        void setColor(const QColor &value){
            color = value;
        }
        void setName(const QString &value){
            name = value;
        }
        QString getName(){
            return name;
        }
        QColor getColor(){
            return color;
        }
    };

public:

    QList<HistoryGraphic *> lstGraphics;
//    QVector<QVector<QPointF>> curvesData;

    SPRThreadList threads;
    HistoryGraphic *current;
    QString titleFormat;

    SPRHistoryModel *model;

    SPRWorkSeparateTypeData type;

    QwtPlotGrid *grid;
    QwtLegend *legend;
    TimeScaleDraw *timeDraw;

    qint64 interval_axis_scale_in_sec;
    qint64 size_parts_in_sec;

    QList<QwtPlotItem *> getGraphics(int thread =-1);

public:
    virtual bool eventFilter(QObject *object, QEvent * event);

    SPRHitoryGraphics(QWidget *parent=nullptr);

    virtual ~SPRHitoryGraphics();

    void init(SPRThreadList _thread, SPRHistoryModel *_model, SPRWorkSeparateTypeData _type, QString _titleFormat = "", qint64 _interval_in_sec = 60 * 60 * 2);
    void setThreadsVisible(HistoryGraphic *_current);


    void setGraphicTitle();
public slots:
    void onDblClick();

    void onModelChange(IModelVariable*);

signals:
    void dblClickMouseEvent();
protected:
    QList<QwtPlotItem *> getGraphics(HistoryGraphic *_current);
};

#endif // SPRHITORYGRAPHICS_H
