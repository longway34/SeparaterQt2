#ifndef SPRSEPARATEDETAILSUMMARYTABLE_H
#define SPRSEPARATEDETAILSUMMARYTABLE_H

#include <QObject>
#include <QWidget>

#include <QTableView>
#include <QAbstractTableModel>

#include "models/sprmainmodel.h"
#include "isprwidget.h"
#include "models/imodelvariable.h"

/**
 * @brief The SPRSeparateDetailSummaryModel class
 */

enum SPRSeparateDetailSummaryVariables{
    var_min_max, var_minimum, var_maximum, var_delta, var_average, var_sco
};

template<typename T>
struct SPRDtailSummaryStructure{
    long count;
    T minimum;
    T maximum;
    double delta;
    double average;
    double sco;
    SPRDtailSummaryStructure<T>(){
        init();
    }
    void add(T value){
        if(value < minimum) minimum = value;
        if(value > maximum) maximum = value;

        double d1 = (double)count/(double)(count+1);
        double d2 = (double)(count+1);

        average = average * d1 + value / d2;
        T _delta = fabs(value - average);

        delta = delta * d1 + _delta / d2;

        _delta = _delta * _delta;
        sco = sqrt(sco * d1 + _delta / d2);

        count++;
    }
    void init(){
        count = 0; minimum = 10000; maximum = -10000; delta = 0; average = 0; sco = 0;
    }
    QString getValue(SPRSeparateDetailSummaryVariables varName) const {
        QString res = "...";
        if(count > 0){
            switch(varName){
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
    struct SPRDtailSummaryStructure<double> H1;
    struct SPRDtailSummaryStructure<double> H2;
    struct SPRDtailSummaryStructure<double> H3;
    struct SPRDtailSummaryStructure<int> time;
    void addData(double h1, double h2, double h3, int _time){
        H1.add(h1);
        H2.add(h2); H3.add(h3);
        time.add(_time);
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
    SPRSeparateModel *separateModel;

    QVector<SPRWorkSeparateRow*> lastValidRows;
    int lastValidRowId;
    QList<int> visibleThreads;
    int scopeDataRowEnd;
    int minTimeScope;
protected:
    QVector<SPRWorkSeparateRow *>getLastValidRow(bool now=false);

public:
    SPRSeparateDetailSummaryModel(QObject *parent = nullptr);

    void setModelData(SPRSeparateModel *_model);
    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    SPRSeparateModel *getModelData() const;

    void clear(){
        beginResetModel();
        myData.clear();
        endResetModel();
    }

    void setVisibleThreads(const QList<int> &value = QList<int>());

    int getScopeData();
    void setScopeData(int value);

    void setMinTimeScope(int value);

    int getMinTimeScope() const;

public slots:
    void onModelChanget(IModelVariable *model);
};

/**
 * @brief The SPRSeparateDetailsSummaryTable class
 */
class SPRSeparateDetailsSummaryTable : public QTableView, public ISPRWidget
{
    Q_OBJECT


    SPRSeparateDetailSummaryModel *myModel;
public:
    SPRSeparateDetailsSummaryTable(QWidget *parent = nullptr);

    // ISPRWidget interface
public:
    virtual ISPRModelData *setModelData(ISPRModelData *data);
    virtual ISPRModelData *getModelData();

    SPRSeparateDetailSummaryModel *getMyModel() const;
    void setMyModel(SPRSeparateDetailSummaryModel *value);
    void clear(){
        myModel->clear();
    }

public slots:
    virtual void onModelChanget(IModelVariable *);
    virtual void widgetsShow();
};

#endif // SPRSEPARATEDETAILSUMMARYTABLE_H
