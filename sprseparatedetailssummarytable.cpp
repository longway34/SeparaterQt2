#include "sprseparatedetailssummarytable.h"

SPRSeparateModel *SPRSeparateDetailSummaryModel::getModelData() const
{
    return separateModel;
}

void SPRSeparateDetailSummaryModel::setVisibleThreads(const QList<int> &value)
{
    if(value.size() > 0){
        visibleThreads = value;
    } else {
        for(int i=0; i<MAX_SPR_MAIN_THREADS; i++){
            visibleThreads.push_back(i);
        }
    }
}

void SPRSeparateDetailSummaryModel::setScopeData(int value)
{
    if(separateModel){
        if(value == 0){
            if(separateModel->workSeparateRows.size() > 0){
                scopeDataRowEnd = separateModel->workSeparateRows[0]->id;
                lastValidRowId = scopeDataRowEnd;
                return;
            }
        }
        scopeDataRowEnd = value;
        lastValidRowId = scopeDataRowEnd;
    }
}

int SPRSeparateDetailSummaryModel::getScopeData()
{
    return scopeDataRowEnd;
}

void SPRSeparateDetailSummaryModel::setMinTimeScope(int value)
{
    minTimeScope = value;
}

int SPRSeparateDetailSummaryModel::getMinTimeScope() const
{
    return minTimeScope;
}

QVector<SPRWorkSeparateRow *> SPRSeparateDetailSummaryModel::getLastValidRow(bool now)
{
    if(!now){
        return lastValidRows;
    }
//    QVector<SPRWorkSeparateRow*> res;
    if(separateModel){
        if(getScopeData() >= 0){
            lastValidRows.clear();
            beginResetModel();
            int count = 0;
            for(int row=0; row < separateModel->workSeparateRows.size(); row++){
                if(separateModel->workSeparateRows[row]->id == getScopeData()){
                    break;
                }
                if(separateModel->workSeparateRows[row]->id == lastValidRowId){
                    break;
                }
                if(separateModel->workSeparateRows[row]->wcount < getMinTimeScope()){
                    continue;
                }
                SPRWorkSeparateRow *modelRow = separateModel->workSeparateRows[row];
                if(visibleThreads.contains(modelRow->thread)){
                    lastValidRows.push_back(modelRow);
                    if(count++ == 0){
                        lastValidRowId = modelRow->id;
                    }
                    myData.addData(modelRow->p_tkh1, modelRow->p_tkh2, modelRow->p_tkh3, (int)modelRow->wcount);
                }
            }
            endResetModel();
        }
    }
    return lastValidRows;

}

SPRSeparateDetailSummaryModel::SPRSeparateDetailSummaryModel(QObject *parent):
    QAbstractTableModel(parent)
{
//    for(int i=1; i<11; i++){
//        myData.addData(i*0.1+i, i*0.1+i, i*0.1+i, i+10);

//    }

}

void SPRSeparateDetailSummaryModel::setModelData(SPRSeparateModel *_model)
{
    separateModel = _model;
    if(separateModel){
        connect(separateModel, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
    }
}
int SPRSeparateDetailSummaryModel::rowCount(const QModelIndex &parent) const
{
    return 6;
}

int SPRSeparateDetailSummaryModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant SPRSeparateDetailSummaryModel::data(const QModelIndex &index, int role) const
{
    QVariant res/* = QAbstractTableModel::data(index, role)*/;
    if(role == Qt::DisplayRole){
        QString sres;
        switch(index.row()){
        case 0: // Разброс
            switch(index.column()){
            case 0:
                sres = myData.H1.getValue(var_min_max);
                break;
            case 1:
                sres = myData.H2.getValue(var_min_max);
                break;
            case 2:
                sres = myData.H3.getValue(var_min_max);
                break;
            case 3:
                sres = myData.time.getValue(var_min_max);
                break;
            }
            break;
        case 1: // min
            switch(index.column()){
            case 0:
                sres = myData.H1.getValue(var_minimum);
                break;
            case 1:
                sres = myData.H2.getValue(var_minimum);
                break;
            case 2:
                sres = myData.H3.getValue(var_minimum);
                break;
            case 3:
                sres = myData.time.getValue(var_minimum);
                break;
            }
            break;
        case 2: // max
            switch(index.column()){
            case 0:
                sres = myData.H1.getValue(var_maximum);
                break;
            case 1:
                sres = myData.H2.getValue(var_maximum);
                break;
            case 2:
                sres = myData.H3.getValue(var_maximum);
                break;
            case 3:
                sres = myData.time.getValue(var_maximum);
                break;
            }
            break;
        case 3: // Среднее
            switch(index.column()){
            case 0:
                sres = myData.H1.getValue(var_average);
                break;
            case 1:
                sres = myData.H2.getValue(var_average);
                break;
            case 2:
                sres = myData.H3.getValue(var_average);
                break;
            case 3:
                sres = myData.time.getValue(var_average);
                break;
            }
            break;
        case 4: // Дельта
            switch(index.column()){
            case 0:
                sres = myData.H1.getValue(var_delta);
                break;
            case 1:
                sres = myData.H2.getValue(var_delta);
                break;
            case 2:
                sres = myData.H3.getValue(var_delta);
                break;
            case 3:
                sres = myData.time.getValue(var_delta);
                break;
            }
            break;
        case 5: // SCO
            switch(index.column()){
            case 0:
                sres = myData.H1.getValue(var_sco);
                break;
            case 1:
                sres = myData.H2.getValue(var_sco);
                break;
            case 2:
                sres = myData.H3.getValue(var_sco);
                break;
            case 3:
                sres = myData.time.getValue(var_sco);
                break;
            }
            break;
        }
        res = QVariant(sres);
    }
    return res;
}

QVariant SPRSeparateDetailSummaryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant res = QAbstractTableModel::headerData(section, orientation, role);
    if(role == Qt::DisplayRole){
        if(orientation == Qt::Horizontal){
            switch(section){
            case 0:
                res = QVariant(QString(tr("H1")));
                break;
            case 1:
                res = QVariant(QString(tr("H2")));
                break;
            case 2:
                res = QVariant(QString(tr("H3")));
                break;
            case 3:
                res = QVariant(QString(tr("Время изм. (мс)")));
            }
        }
        if(orientation == Qt::Vertical){
            switch(section){
            case 0:
                res = QVariant(QString(tr("Разброс")));
                break;
            case 1:
                res = QVariant(QString(tr("Min")));
                break;
            case 2:
                res = QVariant(QString(tr("Max")));
                break;
            case 3:
                res = QVariant(QString(tr("Среднее")));
                break;
            case 4:
                res = QVariant(QString(tr("Дельта")));
                break;
            case 5:
                res = QVariant(QString(tr("Ср. кв. откл.")));
                break;
            }
        }
    }
    return res;
}

void SPRSeparateDetailSummaryModel::onModelChanget(IModelVariable *model)
{
    getLastValidRow(true);
}

/***
 *
*/
SPRSeparateDetailSummaryModel *SPRSeparateDetailsSummaryTable::getMyModel() const
{
    return myModel;
}

void SPRSeparateDetailsSummaryTable::setMyModel(SPRSeparateDetailSummaryModel *value)
{
    myModel = value;
}

SPRSeparateDetailsSummaryTable::SPRSeparateDetailsSummaryTable(QWidget *parent):
    QTableView(parent), ISPRWidget(), myModel(nullptr)
{
    myModel = new SPRSeparateDetailSummaryModel(this);
    setModel(myModel);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    setAutoFillBackground(true);
    horizontalHeader()->setStretchLastSection(true);
//    verticalHeader()->setVisible(true);
    show();
}



void SPRSeparateDetailsSummaryTable::widgetsShow()
{
    show();
}

ISPRModelData *SPRSeparateDetailsSummaryTable::setModelData(ISPRModelData *data)
{
    getMyModel()->setModelData(((SPRMainModel*)data)->getSeparateModel());
    return data;
}

ISPRModelData *SPRSeparateDetailsSummaryTable::getModelData()
{
    return getMyModel()->getModelData();
}

void SPRSeparateDetailsSummaryTable::onModelChanget(IModelVariable *)
{
}

