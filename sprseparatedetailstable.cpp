#include "sprseparatedetailstable.h"
#include "models/sprmainmodel.h"

int SPRSeparateDetailModel::rowCount(const QModelIndex &parent) const
{
    return lastValidRows.size();
}

int SPRSeparateDetailModel::columnCount(const QModelIndex &parent) const
{
    return 8;
}

int SPRSeparateDetailModel::getScopeData() const
{
    return scopeDataRowEnd;
}

void SPRSeparateDetailModel::beginEndScopeData(bool value)
{
    if(separateModel){
        if(value){
            if(separateModel->workSeparateRows.size() > 0){
                setScopeData(separateModel->workSeparateRows[0]->id);
            } else {
                setScopeData(0);
            }
        } else {
            setScopeData(-1);
        }
    }
}

void SPRSeparateDetailModel::setScopeData(int value)
{
    if(value == 0){
        QVector<SPRWorkSeparateRow*> sw = separateModel->workSeparateRows;
        if(sw.size() > 0){
            scopeDataRowEnd = separateModel->workSeparateRows[0]->id;
        } else {
            scopeDataRowEnd = 0;
        }
    } else {
        scopeDataRowEnd = value;
    }
}

QList<int> SPRSeparateDetailModel::getVisibleThreads() const
{
    return visibleThreads;
}

void SPRSeparateDetailModel::setVisibleThreads(const QList<int> &value)
{
    visibleThreads = value;
}

int SPRSeparateDetailModel::getMinTimeScope() const
{
    return minTimeScope;
}

void SPRSeparateDetailModel::setMinTimeScope(int value)
{
    minTimeScope = value;
}

SPRSeparateDetailModel::SPRSeparateDetailModel(QObject *parent)
    :QAbstractTableModel(parent), scopeDataRowEnd(-1), separateModel(nullptr), minTimeScope(-1)
{
}

QVariant SPRSeparateDetailModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant value = QAbstractTableModel::headerData(section, orientation, role);
    QStringList hTitles;
    hTitles << QString(tr("№ п/п")) << QString(tr("Дата/время")) << QString(tr("Ручей"))
            << QString("H1") << QString("H2") << QString("H3")
            << QString(tr("t. изм(мс)"))
            << QString(tr("хвост/концентрат"));

    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return hTitles[section];
    }
    return value;
}

void SPRSeparateDetailModel::onModelChanget(IModelVariable *)
{
    getValidRows(true);
}

ISPRModelData *SPRSeparateDetailModel::setModelData(SPRSeparateModel *_model)
{
    separateModel = _model;
    if(separateModel){
        connect(separateModel, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
    }
}

SPRSeparateModel *SPRSeparateDetailModel::getModelData()
{
    return separateModel;
}

QVector<SPRWorkSeparateRow *> SPRSeparateDetailModel::getValidRows(bool now)
{
    if(!now){
        return lastValidRows;
    }
//    QVector<SPRWorkSeparateRow*> res;
    if(separateModel){
        if(getScopeData() >= 0){
            lastValidRows.clear();
            beginResetModel();
            for(int row=0; row < separateModel->workSeparateRows.size(); row++){
                if(separateModel->workSeparateRows[row]->id == getScopeData()){
                    break;
                }
                if(separateModel->workSeparateRows[row]->wcount < getMinTimeScope()){
                    continue;
                }
                SPRWorkSeparateRow *modelRow = separateModel->workSeparateRows[row];
                if(visibleThreads.contains(modelRow->thread)){
                    lastValidRows.push_back(modelRow);
                }
            }
            endResetModel();
        }
    }
    return lastValidRows;
}

void SPRSeparateDetailModel::resetNow()
{
    getValidRows();
}

void SPRSeparateDetailModel::clear()
{
    if(separateModel->workSeparateRows.size() > 0){
        setScopeData(separateModel->workSeparateRows[0]->id);
    }
}

QVariant SPRSeparateDetailModel::data(const QModelIndex &index, int role) const
{
    QVariant value/* = QAbstractTableModel::data(index, role)*/;

    if(role == Qt::BackgroundColorRole){
//        QVector<SPRWorkSeparateRow*> rows;
//        rows = getValidRows();
        if(index.row() < lastValidRows.size()){
            if(lastValidRows[index.row()]->i_prd[0] >0){
                if(lastValidRows[index.row()]->i_prd[2] >0){
                   return QVariant(QColor(Qt::lightGray));
                } else {
                    return QVariant(QColor(Qt::gray));
                }
            }
        }
//        return value;
    }
    if(role == Qt::DisplayRole){
//        QVector<SPRWorkSeparateRow*> rows = getValidRows();
        if(index.row() < lastValidRows.size()){
            int col = index.column();
            int row = index.row();
            QString ret;
            switch(col){
            case 0:
                ret = QString::number(row+1);
                break;
            case 1:
                ret = lastValidRows[row]->dt.toString("dd.MM hh:mm:ss");
                break;
            case 2:
                ret = QString::number(lastValidRows[row]->thread);
                break;
            case 3:
                ret = QString::number(lastValidRows[row]->p_tkh1);
                break;
            case 4:
                ret = QString::number(lastValidRows[row]->p_tkh2);
                break;
            case 5:
                ret = QString::number(lastValidRows[row]->p_tkh3);
                break;
            case 6:
                ret = QString::number(lastValidRows[row]->wcount);
                break;
            case 7:
                {
                    bool needSpace = false;
                    if(lastValidRows[row]->i_prd[3] > 1){
                        if(lastValidRows[row]->i_prd[0] > 0){
                            ret = QString(tr("концентрат - %1;")).arg(lastValidRows[row]->i_prd[0]);
                            needSpace = true;
                        }
                        if(lastValidRows[row]->i_prd[2] > 0){
                            if(needSpace){
                                ret += " ";
                            }
                            ret += QString(tr("хвост - %1;")).arg(lastValidRows[row]->i_prd[2]);
                        }
                    } else {
                        if(lastValidRows[row]->i_prd[0] > 0){
                            ret = QString(tr("концентрат"));
                        }
                        if(lastValidRows[row]->i_prd[2] > 0){
                            ret = QString(tr("хвост"));
                        }
                    }
                }
                break;
            }
            return QVariant(ret);
        }
//        return value;
    }
    return value;
}

SPRSeparateDetailModel *SPRSeparateDetailsTable::getMyModel() const
{
    return myModel;
}

SPRSeparateDetailsTable::SPRSeparateDetailsTable(QWidget *parent)
    :QTableView(parent), ISPRWidget()
{
    myModel = new SPRSeparateDetailModel(this);
    setModel(myModel);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    setAutoFillBackground(true);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->setVisible(false);
    show();
}


void SPRSeparateDetailsTable::widgetsShow()
{
    if(myModel->getModelData()){
//        clear();
//        show();
        myModel->resetNow();
    }
}

void SPRSeparateDetailsTable::beginEndScope(bool value)
{
    if(value){
        myModel->setScopeData(value);
    }
}

ISPRModelData *SPRSeparateDetailsTable::setModelData(ISPRModelData *data)
{
    myModel->setModelData(((SPRMainModel*)data)->getSeparateModel());
//    if(data){
//        connect(data, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(widgetsShow()));
//    }
//    widgetsShow();
}

ISPRModelData *SPRSeparateDetailsTable::getModelData()
{
    return myModel->getModelData();
}

void SPRSeparateDetailsTable::clear()
{
    if(myModel->getModelData()){
        myModel->clear();
        repaint();
    }
}

void SPRSeparateDetailsTable::onModelChanget(IModelVariable *)
{
    widgetsShow();
}



