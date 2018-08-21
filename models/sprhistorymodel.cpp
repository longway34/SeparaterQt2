#include "models/sprhistorymodel.h"
#include "models/sprmainmodel.h"

SPRThreadList SPRHistoryModel::getThreadList() const
{
    return threadList;
}

void SPRHistoryModel::setThreadList(const SPRThreadList &value)
{
    threadList = value;
    for(int i=0; i<mTableData.size(); i++){
        if(mTableData.at(i)){
            delete mTableData.at(i);
        }
    }
    mTableData.clear();

    mTableData.resize(threadList.size());
    foreach(uint8_t th, threadList){
        mTableData[th] = new MTableModelDataOnThread();
    }
}

SPRHistoryModel::SPRHistoryModel(SPRMainModel *_mainModel, SPRSpectrumListItemsModel *_spectModel, int _max_time_in_sec, QObject *):
    ISPRModelData(), mainModel(nullptr), separateModel(nullptr)
{
    setMainModel(_mainModel);
    setThreadList(_mainModel->getThreadsList());
    setMaxTimeSctore(_max_time_in_sec);
    setSpectrumModel(_spectModel);
}

void SPRHistoryModel::setMainModel(SPRMainModel *value)
{
    mainModel = value;
    if(mainModel){
        setThreadList(mainModel->getThreadsList());

        separateModel = mainModel->getSeparateOutputModel();
        if(separateModel){
            connect(separateModel, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChange(IModelVariable*)));
        }
    }
}

void SPRHistoryModel::setMaxTimeSctore(int _time_in_sec){
    maxTimeStore_in_sec = _time_in_sec;
}

SPRHistoryModel::~SPRHistoryModel(){
    clear();
}

QVector<QPointF> SPRHistoryModel::getSamplesToGraphic(SPRWorkSeparateTypeData type, uint8_t thread) const {
    QVector<QPointF> res;
    if(separateModel){

        for(int row=0; row < countsData.size(); row++){
            QPointF point(0,0);
            qint64 time_in_sec = 0;
            double valueIn = 0; double countIn = 0; double valueCon = 0; double countCon = 0;
            if(thread > threadList.size()){ // средняя по каналам
                foreach(uint8_t th, threadList){
                    double countTemp = countsData[row][th]->getCountsValue(countInput);
                    valueIn += countsData[row][th]->getCountsValue(countInputInTime) * countTemp;
                    countIn += countTemp;
                    countTemp = countsData[row][th]->getCountsValue(countConcentrate);
                    valueCon += countsData[row][th]->getCountsValue(countConcentrateInTime) * countCon;
                    countCon += countTemp;
                    if(countsData[row][th]->dt > time_in_sec) time_in_sec = countsData[row][th]->dt;
                }
                valueIn = (countIn > 1e-6) ? valueIn / countIn : 0;
                valueCon = (countCon > 1e-6) ? valueCon / countCon: 0;
            } else { // один канал
                valueIn = countsData[row][thread]->getCountsValue(countInputInTime);
                valueCon = countsData[row][thread]->getCountsValue(countConcentrateInTime);
                time_in_sec = countsData[row][thread]->dt;
            }
            point.setX(time_in_sec);
            if(type == countInputInTime){
                point.setY(valueIn);
            } else if(type == percentConcentrate2Input){
                if(valueIn > 1e-6){
                    point.setY(valueCon / valueIn * 100);
                }
            }
            res.append(point);
        }
    }
    return  res;
}
QVector<double> SPRHistoryModel::getMTableValue(SPRWorkSeparateTypeData type, uint8_t thread) const{
    QVector<double> res;
    if(thread < threadList.size()){
        switch (type) {
        case lastHXconcentrate:
        case lastHXinput:
        case lastHXtail:
        case avrHXconcentrate:
        case avrHXinput:
        case avrHXtail:
        case avrSizesStone:
        case lastSizesStone:
        case lastHXData:
        case avrHXData:
            res = (mTableData.at(thread)->getHxValues(type));
            break;
        case lastCorrel:
        case avrCorrel:
            res.append(mTableData.at(thread)->getCorrelValue(type));
        default:
            break;
        }
    }
    return res;
}

QVector<QVector<double>> SPRHistoryModel::getMTableSizesStones(SPRThreadList threads) const{
    QVector<QVector<double>> res;
    if(threads.size() == 0){
        threads = threadList;
    }
    foreach (uint8_t th, threads) {
        res.append(mTableData.at(th)->getSizesStone());
    }
    return res;
}

QVector<QVector<double>> SPRHistoryModel::getMTableValues(SPRWorkSeparateTypeData type, SPRThreadList threads) const{
    QVector<QVector<double>> res;
    if(threads.size() == 0){
        threads = threadList;
    }
    foreach (uint8_t th, threads) {
        res.append(getMTableValue(type, th));
    }
    return res;
}

QVector<QVector<double> > SPRHistoryModel::getMTableCounts(SPRThreadList threads) const
{
    if(threads.size() == 0){
        threads = threadList;
    }
    QVector<QVector<double>> res;
    foreach (uint8_t th, threads) {
        QVector<double> i_last(3, 0);
        if(countsData.size() > 0){
            double c_concentrate = countsData.first().at(th)->getCountsValue(countConcentrate);
            double c_tail = countsData.first().at(th)->getCountsValue(countTail);
            double c_all = countsData.first().at(th)->getCountsValue(countInput);

        const QVector<double> i_last = QVector<double>(
            {
                c_concentrate,
                c_tail,
                c_all
            });
        }
        res.append(i_last);
    }
    return res;

}

QVector<double> SPRHistoryModel::getSamplesToHistogramms(SPRWorkSeparateTypeData type, uint8_t thread, double porog1, double porog2) const{
    QVector<double> res;
    if(separateModel){
        if(countsData.size()>0){
            if(thread < threadList.size()){
                if(type == percentConcentrate2Input){
                    porog1 = -1; porog2 = -1;
                }
                double valueIn = countsData.front().at(thread)->getCountsValue(countInputInTime);
                double valueOut = countsData.front().at(thread)->getCountsValue(countConcentrateInTime);
                double value1 = 0;
                double value2 = 0, value3 = 0;
                if(type == percentConcentrate2Input){
                    value1 = valueIn > 1e-6 ? valueOut / valueIn * 100 : 0;
                } else {
                    if(valueIn > porog2){
                        value1 = porog1;
                        value2 = porog2 - porog1;
                        value3 = valueIn - porog2;
                    } else if(valueIn > porog1){
                        value1 = porog1; value2 = valueIn - porog1;
                    } else {
                        value1 = valueIn;
                    }
                }
                res = QVector<double>({value1, value2, value3});
            }
        }
    }
    return res;
}

void SPRHistoryModel::onModelChange(IModelVariable *send){
    if(separateModel && (sender() == separateModel && send == separateModel)){
        if(send == separateModel || sender() == separateModel){
            last = separateModel->getWorkSeparateLast();
            prev = separateModel->getWorkSeparatePrev();
            if(last){
                qint64 lmdt = last->mdt;
                qint64 _time_in_sec = abs(static_cast<int>(lastTimeUpdate - lmdt)) / 1000;

//                qDebug() << "mdt:" << lastTimeUpdate << "; lmdt:" << lmdt << "; time:" << _time_in_sec << "(" << abs(static_cast<int>(lastTimeUpdate - lmdt)) / 1000 << ")";
                if(_time_in_sec >= 1){
                    lastTimeUpdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
                    OneTimeData oneTime;
                    if(countsData.size() > 0){
                        oneTime = countsData.first();
                    } else {
                        oneTime.fill(new HistoryModelData(), threadList.size());
                    }
                    OneTimeData newOneTime;
                    foreach(uint8_t th, threadList){
                        HistoryModelData *newRowTh = new HistoryModelData(*oneTime.at(th));
//                        double i_prd_all = last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_ALL] - prev->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_ALL];
//                        double i_prd_conc = last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_CONCENTRATE] - prev->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_CONCENTRATE];
//                        double i_prd_tail = last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_TAIL] - prev->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_TAIL];

//                        newRowTh->addCountsRecord(i_prd_all, i_prd_conc, i_prd_tail, _time_in_sec);
                        newRowTh->addCountsRecord(th, last, prev, _time_in_sec);
                        newOneTime.append(newRowTh);

                        QVector<double> sizes(5,0);
                        for(int i=0; i<sizes.size(); i++){
                            sizes[i] = last->source.s_rst[th][i];
//                            if(i<3){
//                                i_prd[i] = last->source.p_prd[th][i];
//                            }
                        }
                        
                        QVector<double> i_prd({last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_CONCENTRATE], last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_TAIL], last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_ALL]});
                        QVector<double> p_prd({last->source.p_prd[th][SPR_SEPARATE_OUT_INDEX_CONCENTRATE], last->source.p_prd[th][SPR_SEPARATE_OUT_INDEX_TAIL], last->source.p_prd[th][SPR_SEPARATE_OUT_INDEX_ALL]});
//                        QVector<double> p_tk({last->source.p_tkh1[th], last->source.p_tkh2[th][SPR_SEPARATE_OUT_INDEX_TAIL], last->source.p_prd[th][SPR_SEPARATE_OUT_INDEX_ALL]});
                        mTableData[th]->addHxCountStoneValues(p_prd, sizes, i_prd, _time_in_sec);
                    }
                    countsData.push_front(newOneTime);

                    clearOldRecord();
//                    qDebug() << ""; qDebug() << "";
//                    qDebug() << "data.size(): "<<data.size() << "time:" << lastTimeUpdate/1000;
//                    if(data.size() > 0){
//                        for(int t=0; t< data[0].size(); t++){
//                            qDebug() << "t:" << t << " all:"<<data[0][t]->last_all << " cons:" << data[0][t]->last_concentrate << " i_tail:" << data[0][t]->last_tail;
//                        }
//                    }

                    emit modelChanget(this);
                }
            }
        }
    }
    if(spectrumModel && (sender() == spectrumModel || send == spectrumModel)){

    }
}

void SPRHistoryModel::clear(){
    while(countsData.size() > 0){
        foreach(uint8_t th, threadList){
            if(countsData.last().at(th)){
                delete  countsData.last().at(th);
            }
        }
        countsData.removeLast();
    }
    setThreadList(this->threadList);
//    while (mTableData.size() > 0) {
//        foreach(uint8_t th, threadList){
//            if(mTableData[th]){
////                delete mTableData[th];
//            }
//        }
//    }
//    mTableData.clear();

    last = nullptr; prev = nullptr;

    emit modelChanget(this);
}

void SPRHistoryModel::setSpectrumModel(SPRSpectrumListItemsModel *value)
{
    spectrumModel = value;
    if(spectrumModel){
        connect(spectrumModel, SLOT(modelChanget(ISPRModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
    }
}


SPRWorkSeparate2 *SPRHistoryModel::getLast() const
{
    return last;
}

void SPRHistoryModel::clearOldRecord()
{
    if(countsData.size() > maxTimeStore_in_sec){
        while(countsData.size() > maxTimeStore_in_sec){
            foreach (uint8_t th, threadList) {
                if(countsData.last().at(th)){
                    delete countsData.last().at(th);
                }
            }
            countsData.removeLast();
        }
        emit modelChanget(this);
    }
    if(countsData.size() > 0){
        bool finish = false;
        while(!finish && countsData.size() > 0){
            qint64 firstMdt = countsData.first().first()->getMdt();
            qint64 lastMdt = countsData.last().last()->getMdt();
            qint64 diff_in_sec = (firstMdt - lastMdt) / 1000;
            if(diff_in_sec > maxTimeStore_in_sec){
                foreach (uint8_t th, threadList) {
                    if(countsData.last().at(th)){
                        delete countsData.last().at(th);
                    }
                }
                countsData.removeLast();
            } else {
                finish = true;
            }
        }
        emit modelChanget(this);
    }
}

