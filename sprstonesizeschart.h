#ifndef SPRSTONESIZESCHART_H
#define SPRSTONESIZESCHART_H

#include <QObject>
#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QPieSlice>

#include "models/sprmainmodel.h"
#include "models/sprhistorymodel.h"
#include "models/sprseparatemodel.h"

QT_CHARTS_BEGIN_NAMESPACE
class QAbstractSeries;
class QPieSlice;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class PercentStoneSizesSlice : public QPieSlice
{
    Q_OBJECT

    qreal myValue;
//    uint8_t thread;
public:
    PercentStoneSizesSlice(/*uint8_t _thread, */qreal value, QString prefix, QAbstractSeries *_parentSeries)
        : /*thread(_thread),*/
          m_parentSeries(_parentSeries),
          m_prefix(prefix)
    {
        setValue(value);
        updateLabel();
        setLabelFont(QFont("Tahoma", 8));
        connect(this, &PercentStoneSizesSlice::percentageChanged, this, &PercentStoneSizesSlice::updateLabel);
        connect(this, &PercentStoneSizesSlice::hovered, this, &PercentStoneSizesSlice::showHighlight);
    }

    virtual ~PercentStoneSizesSlice(){}
    QAbstractSeries *parentSeries() const
    {
        return m_parentSeries;
    }

//    uint8_t getThread() const
//    {
//        return thread;
//    }


public Q_SLOTS:
    void updateLabel()
    {
        QString label = m_prefix;
//        if(this->value() > 1){
            label += " ";
            label += QString::number(this->value());
            label += " (";
            label += QString::number(this->percentage() * 100, 'f', 2);
            label += "%)";
//        }
        setLabel(label);
    }
    void showHighlight(bool show)
    {
        setLabelVisible(show);
        setExploded(show);
    }
private:
    QAbstractSeries *m_parentSeries;
    QString m_prefix;
};

class StoneChart : public QChart
{
    Q_OBJECT

    uint8_t currentThread;
public:
    explicit StoneChart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = nullptr): QChart(QChart::ChartTypeCartesian, parent, wFlags),
        currentThread(255), m_currentSeries(nullptr)
  {
  }

    ~StoneChart(){}
    void changeSeries(QAbstractSeries *series)
    {
        // NOTE: if the series is owned by the chart it will be deleted
        // here the "window" owns the series...
        if (m_currentSeries)
            removeSeries(m_currentSeries);
        m_currentSeries = series;
        addSeries(series);
        setTitle(series->name());
    }
    QAbstractSeries *currentSeries() const
    {
        return m_currentSeries;
    }

public Q_SLOTS:
    void handleSliceClicked(QPieSlice *slice)
    {
        PercentStoneSizesSlice *percentStoneSizesSlice = static_cast<PercentStoneSizesSlice *>(slice);
//        currentThread = percentStoneSizesSlice->getThread();
        changeSeries(percentStoneSizesSlice->parentSeries());
    }


private:
    QAbstractSeries *m_currentSeries;

};

class SPRStoneSizesChart : public QWidget
{
    Q_OBJECT

    struct title_slides_data{
        uint8_t th;
        QString title;
        QColor color;
    };

    StoneChart *chart;
    QPieSeries *allThreadsSeries;

    SPRMainModel *mainModel;
    SPRHistoryModel *historyModel;
    SPRSeparateModel *separateModel;

    SPRThreadList threads;


    struct title_slides_data titlesStonesSlides[DEF_SPR_IMS_PARTS_SIZE+1];
    QVector<struct title_slides_data> titlesThreadsSlides;
    QList<QPieSeries*> groupSeries;
    void setTitles();
public:
    explicit SPRStoneSizesChart(QWidget *parent = nullptr);

    void setModels(SPRMainModel *_mainModel, SPRHistoryModel *_historyModel=nullptr, SPRSeparateModel *_separateModel=nullptr);
    void setSeriesData();
signals:

public slots:
    void onModelChandet(IModelVariable *send);
protected:
    void initSeries();
};

#endif // SPRSTONESIZESCHART_H
