#ifndef SPRHISTORYGRAPHICSMODEL_H
#define SPRHISTORYGRAPHICSMODEL_H

#include <QObject>
#include <limits.h>

#include "models/isprmodeldata.h"
#include "models/sprseparateoutputmodel.h"

//#include "models/sprmainmodel.h"

class SPRMainModel;

#define DEF_SRP_DATAILS_ON_GRAPHICS_IN_SEC 60

typedef enum{
    lastHXData, avrHXData,
    countsAvrData, countsLastData,
    lastCorrel, avrCorrel,
    lastSizesStone, avrSizesStone,
    countInput, countConcentrate, countTail,
    countInputInSec, countConcentrateInSec, countInputInTime, countConcentrateInTime, percentConcentrate2Input, countTailInSec, countTailInTime,
    avrHXinput, avrHXconcentrate, avrHXtail,
    lastHXinput, lastHXconcentrate, lastHXtail
} SPRWorkSeparateTypeData;
/**
 * @brief The SPRHistoryGraphicsModel class
 */

//#define IND_INPUT       3
//#define IND_CONCENTRATE 0
//#define IND_TAIL        2

class SPRHistoryModel: public ISPRModelData
{
    Q_OBJECT

//    SPRSeparateModel *model;
    SPRMainModel *mainModel;
    qint64 maxTimeStore_in_sec;

    SPRSeparateOutputModel *separateModel;
    SPRSpectrumListItemsModel *spectrumModel;
    SPRWorkSeparate2 *last;
    SPRWorkSeparate2 *prev;

#define DEF_SEPARATE_TIME_AVERAGE_REPORTS   60

    class IModelData
    {
    public:
        void setTimeAverage(const qint64 &value = DEF_SEPARATE_TIME_AVERAGE_REPORTS){
            timeAverage = value;
        }

        qint64 getMdt() const {return mdt;}
        void setMdt(const qint64 &value){mdt = value;}

    protected:
        qint64 mdt;
        qint64 timeAverage;
        IModelData(){
            mdt = QDateTime::currentDateTime().toMSecsSinceEpoch();
            setTimeAverage(DEF_SEPARATE_TIME_AVERAGE_REPORTS);
        }
        double getAvarage(double lastValue, double value, qint64 valueTime){
            double res = 0;
            double d1 = static_cast<double>(timeAverage)/static_cast<double>(timeAverage+valueTime);
            double d2 = static_cast<double>(timeAverage+valueTime);
            if(d2 > 1e-6){
                res = lastValue * d1 + (value * valueTime) / d2;
            }
            return res;
        }
    };

    class MTableModelDataOnThread:public IModelData{
        qint64 dt;
        QVector<double> avrHxTable; // concentrate, tail, all
        QVector<double> lastHxTable; // concentrate, tail, all
        QVector<double> avrSizeStone; // concentrate, tail, all
        QVector<double> lastSizeStone; // concentrate, tail, all
//        QVector<double> avrCount; // concentrate, tail, all
        QVector<double> lastCount; // concentrate, tail, all
        double lastCorrel;
        double avrCorrel;
    public:
        MTableModelDataOnThread(): IModelData (), avrHxTable(3, -100), lastHxTable(3, -100),
                                                avrSizeStone(5, -100), lastSizeStone(5, -100),
                                                lastCount(3, 0),
                                                lastCorrel(-100), avrCorrel(-100){
            dt = QDateTime::currentDateTime().toSecsSinceEpoch();
        }
        void addHxCountStoneValues(QVector<double> hx, QVector<double> _sizesStone, QVector<double> _i_prd, qint64 _time_in_sec){
            for(int i=0; i<hx.size();i++){
                if(avrHxTable[i] < 0) avrHxTable[i] = hx[i];
                avrHxTable[i] = getAvarage(avrHxTable[i], hx[i], _time_in_sec);
                lastHxTable[i] = hx[i];
            }
            for(int i=0; i< _sizesStone.size(); i++){
                if(avrSizeStone[i] < 0) avrSizeStone[i] = _sizesStone[i];
                avrSizeStone[i] = getAvarage(avrSizeStone[i], _sizesStone[i], _time_in_sec);
                lastSizeStone[i] = _sizesStone[i];
            }
            if(_i_prd.size() == 3){
                lastCount = _i_prd;
            }
        }

        void addCorrelValue(double _correl, qint64 _time_in_sec){
            if(fabs(avrCorrel) > 1) avrCorrel = _correl;
            avrCorrel = getAvarage(avrCorrel, _correl, _time_in_sec);
            lastCorrel = _correl;
        }
        double getHxValue(int num, SPRWorkSeparateTypeData type) const{
            double res = 0;
            if(num < 3){
                if(QList<SPRWorkSeparateTypeData>({avrHXinput, avrHXconcentrate, avrHXtail, avrHXData}).contains(type)){
                    res = avrHxTable[num];
                }
                if(QList<SPRWorkSeparateTypeData>({lastHXinput, lastHXconcentrate, lastHXtail, lastHXData}).contains(type)){
                    res = lastHxTable[num];
                }
                if(type == countsLastData){
                    res = lastCount[SPR_SEPARATE_OUT_INDEX_ALL];
                }
                if(type == lastSizesStone){
                    res = lastSizeStone[num];
                }
            }
            return res;
        }


        QVector<double> getHxValues(SPRWorkSeparateTypeData type) const{
            QVector<double> res(3, 0);
            for(int i=0; i<res.size(); i++){
                res[i] = getHxValue(i, type);
            }
            return  res;
        }

        QVector<double> getSizesStone() const {
            return lastSizeStone;
        }

        double getCorrelValue(SPRWorkSeparateTypeData type = SPRWorkSeparateTypeData::lastCorrel) const{
            if(type == SPRWorkSeparateTypeData::lastCorrel){
                return lastCorrel;
            } else {
                return avrCorrel;
            }
        }
    };

    class HistoryModelData :public IModelData{
    public:
        qint64 dt;

        double last_all;
        double last_concentrate;
        double last_tail;

        double avr_count_all;
        double avr_count_concentrate;
        double avr_count_tail;
        qint64 time_in_sec;
        HistoryModelData(): IModelData (), avr_count_all(0), avr_count_concentrate(0), avr_count_tail(0), time_in_sec(0){
            dt = QDateTime::currentDateTime().toSecsSinceEpoch();
        }
        HistoryModelData(const HistoryModelData &o): IModelData()
        {
            dt = QDateTime::currentDateTime().toSecsSinceEpoch();
//            thread = o.thread;
            last_all = o.last_all; last_concentrate = o.last_concentrate; last_tail = o.last_tail;
            avr_count_all = o.avr_count_all; avr_count_concentrate = o.avr_count_concentrate; avr_count_tail = o.avr_count_tail;
            time_in_sec = o.time_in_sec;
        }
        HistoryModelData &operator=(const HistoryModelData &o){
            if(this != &o){
                dt = o.dt;
                mdt = o.mdt;
//                thread = o.thread;
                last_all = o.last_all; last_concentrate = o.last_concentrate; last_tail = o.last_tail;
                avr_count_all = o.avr_count_all; avr_count_concentrate = o.avr_count_concentrate; avr_count_tail = o.avr_count_tail;
                time_in_sec = o.time_in_sec;
            }
            return *this;
        }
        void addCountsRecord(uint8_t th, SPRWorkSeparate2 *last, SPRWorkSeparate2* prev, qint64 _time_in_sec = -1){
            if(_time_in_sec <= 0){
                qint64 t = (last->mdt - prev->mdt) / 1000;
                _time_in_sec = t;
            }

            if(_time_in_sec > 0){
                if(th < MAX_SPR_MAIN_THREADS){
                    last_all = last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_ALL];
                    last_concentrate = last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_CONCENTRATE];
                    last_tail = last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_TAIL];

                }
                double diff_all = last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_ALL] -
                        prev->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_ALL];

                double diff_concentrate = last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_CONCENTRATE] -
                        prev->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_CONCENTRATE];

                double diff_tail = last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_TAIL] -
                        prev->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_TAIL];

                addCountsRecord(diff_all, diff_concentrate, diff_tail, _time_in_sec);
            }
        }

        void addCountsRecord(double i_prd_all, double i_prd_concentrate, double i_prd_tail, qint64 _time_in_sec = -1){
//            double d1 = (static_cast<double>(count))

            if(_time_in_sec <= 0){
                qint64 _m = QDateTime::currentDateTime().toMSecsSinceEpoch();
                _time_in_sec = (_m - mdt) / 1000;
            }
            if(_time_in_sec >= 0){
//                time_in_sec = 60;


//                double d1 = static_cast<double>(time_in_sec)/static_cast<double>(time_in_sec+_time_in_sec);
//                double d2 = static_cast<double>(time_in_sec+_time_in_sec);

                avr_count_all = getAvarage(avr_count_all, i_prd_all, _time_in_sec);
                avr_count_concentrate = getAvarage(avr_count_concentrate, i_prd_concentrate, _time_in_sec);
                avr_count_tail = getAvarage(avr_count_tail, i_prd_tail, _time_in_sec);

//                last_all = i_prd_all;
//                last_concentrate = i_prd_concentrate;
//                last_tail = i_prd_tail;
//                if(d2 > 1e-6){
//                    avr_count_all = avr_count_all * d1 + (i_prd_all * _time_in_sec) / d2;
//                    avr_count_concentrate = avr_count_concentrate * d1 + (i_prd_concentrate * _time_in_sec) / d2;
//                    avr_count_tail = avr_count_tail * d1 + (i_prd_tail * _time_in_sec) / d2;
//                }
                time_in_sec = _time_in_sec;
            }

        }
        double getCountsValue(SPRWorkSeparateTypeData type){
            double ret = 0;
            switch (type) {
            case countInput:
                ret = last_all;
                break;
            case countConcentrate:
                ret = last_concentrate;
                break;
            case countTail:
                ret = last_tail;
                break;
            case countInputInTime:
                ret = avr_count_all;
                break;
            case countConcentrateInTime:
                ret = avr_count_concentrate;
                break;
            case percentConcentrate2Input:
                if(avr_count_all > 1e-6){
                    ret = avr_count_concentrate / avr_count_all * 100;
                }
                break;
            case countConcentrateInSec:
                ret = avr_count_all / time_in_sec;
                break;
            default:
                break;
            }
            return ret;
        }
    };

    SPRThreadList threadList;

    qint64 lastTimeUpdate;

    typedef QVector<SPRHistoryModel::HistoryModelData*> OneTimeData;
    typedef QVector<SPRHistoryModel::OneTimeData> DataVector;

    DataVector countsData;
    QVector<MTableModelDataOnThread*> mTableData;

    void clearOldRecord();
public:
//    QList<SPRWorkSeparate*> separateRows;
    SPRHistoryModel(SPRMainModel *_mainModel, SPRSpectrumListItemsModel *_spectModel, int _max_time_in_sec = 60 * 60 * 2, QObject* = nullptr);
    /**
     * @brief setMaxTimeSctore
     * @param _time_in_sec
     */
    void setMaxTimeSctore(int _time_in_sec);
    /**
     * @brief getSamples
     * @param type
     * @param _from_in_sec
     * @param _to_in_sec
     * @param _size_parts_in_sec
     * @return
     */
    void clear();
    QVector<QPointF> getSamplesToGraphic(SPRWorkSeparateTypeData type = countInputInTime, uint8_t thread = 255) const;
    QVector<double> getSamplesToHistogramms(SPRWorkSeparateTypeData type, uint8_t thread, double porog1 =-1, double porog2 =-1) const;
    QVector<QVector<double>> getMTableValues(SPRWorkSeparateTypeData type, SPRThreadList threads={}) const;
    QVector<QVector<double>> getMTableCounts(SPRThreadList threads={}) const;
    QVector<double> getMTableValue(SPRWorkSeparateTypeData type, uint8_t thread) const;
    QVector<QVector<double> > getMTableSizesStones(SPRThreadList threads={}) const;

    virtual ~SPRHistoryModel();
    SPRThreadList getThreadList() const;
    void setThreadList(const SPRThreadList &value);

    void setSpectrumModel(SPRSpectrumListItemsModel *value);
    void setMainModel(SPRMainModel *value);


    SPRWorkSeparate2 *getLast() const;

public slots:
    void onModelChange(IModelVariable*send);
//signals:
//    void modelChanget(IModelVariable*);
};

#endif // SPRHISTORYGRAPHICSMODEL_H
