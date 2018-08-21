#ifndef SPRSEPARATEDETAILSUMMARYTABLE_H
#define SPRSEPARATEDETAILSUMMARYTABLE_H

#include <QObject>
#include <QWidget>

#include <QTableView>
#include <QAbstractTableModel>

#include "models/sprmainmodel.h"
#include "isprwidget.h"
#include "models/imodelvariable.h"

#include "isprwidget.h"

/**
 * @brief The SPRSeparateDetailSummaryModel class
 */

enum SPRSeparateDetailSummaryVariables{
    var_self, var_min_max, var_minimum, var_maximum, var_delta, var_average, var_sco
};

template<typename T>
struct SPRDetailSummaryStructure{
    long count;
    T self_value;
    T minimum;
    T maximum;
    double delta;
    double average;
    double sco;
    SPRDetailSummaryStructure<T>(){
        init();
    }
    void add(T value, int _count){

        self_value = value;

        if(value < minimum) minimum = value;
        if(value > maximum) maximum = value;

        double d1 = static_cast<double>(count)/static_cast<double>(count+_count);
        double d2 = static_cast<double>(count+_count);

        average = average * d1 + (value * _count) / d2;
        double _delta = fabs(static_cast<double>(value) - average);

        delta = delta * d1 + _delta / d2;

        _delta = _delta * _delta;
        sco = sqrt(sco * d1 + _delta / d2);

        count += _count;
    }
    void init(){
        count = 0; minimum = 10000; maximum = -10000; delta = 0; average = 0; sco = 0;
    }
    QString getValue(SPRSeparateDetailSummaryVariables varName) const {
        QString res = "...";
        if(count > 0){
            switch(varName){
            case var_self:
                res = QString::number(self_value);
                break;
            case var_min_max:
                res = QString("%1-%2").arg(QString::number(minimum)).arg(QString::number(maximum));
                break;
            case var_minimum:
                res = QString::number(minimum);
                break;
            case var_maximum:
                res = QString::number(maximum);
                break;
            case var_delta:
                res = QString::number(delta);
                break;
            case var_average:
                res = QString::number(average);
                break;
            case var_sco:
                res = QString::number(sco);
                break;
            }
        }
        return res;
    }
};

struct SPRSeparateDetailData{
    struct SPRDetailSummaryStructure<double> H1;
    struct SPRDetailSummaryStructure<double> H2;
    struct SPRDetailSummaryStructure<double> H3;
    struct SPRDetailSummaryStructure<int> time;
    void addData(double h1, double h2, double h3, int _time, int count){
        H1.add(h1, count);
        H2.add(h2, count); H3.add(h3, count);
        time.add(_time, count);
    }
    void clear(){
        H1.init(); H2.init(); H3.init(); time.init();
    }
};

//typedef struct spr_detail_summary_structure SPRDtailSummaryStructure;

class SPRSeparateDetailSummaryModel : public QAbstractTableModel
{
    Q_OBJECT

    struct SPRSeparateDetailData myData;
    SPRSeparateOutputModel *separateModel;
    SPRWorkSeparate2 *lastData;
    SPRWorkSeparate2 *prevData;

    SPRThreadList visibleThreads;
    int startStop;
    int minStoneTime;

    qint64 lastMdt;

public:
    SPRSeparateDetailSummaryModel(QObject *parent = nullptr);

    void setModelData(SPRSeparateOutputModel *_model);
    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &) const;
    virtual int columnCount(const QModelIndex &) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    SPRSeparateOutputModel *getModelData() const;

    void clear(){
        beginResetModel();
        myData.clear();
        endResetModel();
    }

    void setVisibleThreads(const SPRThreadList &value = SPRThreadList());

    bool isStartStopScope();
    void startStopScope(bool value);

    void setMinStoneTime(int value);

    int getMinStoneTime() const;

public slots:
    void onModelChanget(IModelVariable *);
};

/**
 * @brief The SPRSeparateDetailsSummaryTable class
 */
class SPRSeparateDetailsSummaryTable : public QTableView, public ISPRWidget
{
    Q_OBJECT


    SPRSeparateDetailSummaryModel *myModel;
    SPRSeparateOutputModel *separateModel;

public:
    SPRSeparateDetailsSummaryTable(QWidget *parent = nullptr);

    // ISPRWidget interface
public:
    virtual ISPRModelData *setModelData(ISPRModelData *data);
    virtual ISPRModelData *getModelData();

//    SPRSeparateDetailSummaryModel *getMyModel() const;
//    void setMyModel(SPRSeparateDetailSummaryModel *value);
    void clear(){
        myModel->clear();
    }

    void setVisibleThreads(const SPRThreadList &value = SPRThreadList());

    bool isStartStopScope();
    void startStopScope(bool value);

    void setMinStoneTime(int value);


public slots:
    virtual void onModelChanget(IModelVariable *);
    virtual void widgetsShow();
};

#endif // SPRSEPARATEDETAILSUMMARYTABLE_H
