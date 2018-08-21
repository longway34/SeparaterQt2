#include "sprsepratemaintable.h"

void SPRSeparateMainTableModel::setHxTable(const QVector<QVector<double> > &value)
{
    beginResetModel();
    hxTable = value;
    endResetModel();
}

void SPRSeparateMainTableModel::setHxTable()
{
    beginResetModel();
    hxTable.resize(threads.size());
    foreach(uint8_t th, threads){
        hxTable[th].fill(3, 0);
    }
    endResetModel();
}

void SPRSeparateMainTableModel::setI_prd(const QVector<QVector<double> > &value)
{
    beginResetModel();
    i_prd = value;
    endResetModel();
}
void SPRSeparateMainTableModel::setI_prd(){
    beginResetModel();
    i_prd.resize(threads.size());
    foreach(uint8_t th, threads){
        i_prd[th].fill(3, 0);
    }
    endResetModel();
}

void SPRSeparateMainTableModel::setSeparTables(const QVector<QVector<double> > _hxs, const QVector<QVector<double> > _i_prds)
{
    beginResetModel();
    hxTable = _hxs;
    i_prd = _i_prds;
    endResetModel();
}
void SPRSeparateMainTableModel::setCorrelTable(const QVector<double> &value)
{
    beginResetModel();
    correlTable = value;
    endResetModel();
}

void SPRSeparateMainTableModel::setCorrelTable(){
    beginResetModel();
    correlTable.resize(threads.size());
    correlTable.fill(0);
    endResetModel();
}

void SPRSeparateMainTableModel::onModelChanget(IModelVariable *send)
{
    if(send == historyModel || sender() == historyModel){
        beginResetModel();
        setSeparTables(historyModel->getMTableValues(lastHXData), historyModel->getMTableCounts());
        endResetModel();
        return;
    }
    if(send == this->spectrumsModel || sender() == spectrumsModel){
        QVector<double> tCorrel;
        foreach(uint8_t th, threads){
            QList<SPRSpectrumItemModel *> lspect = spectrumsModel->getSpectrumsItemByThread(th, spectrumsOnly);
            QList<SPRSpectrumItemModel *> lbase = spectrumsModel->getSpectrumsItemByThread(th, spectrumBase);

            if(lspect.size() > 0 && lbase.size() >0){
                double corr = lspect.first()->getCorrel(lbase.first());
                tCorrel.append(corr);
            }
        }
        setCorrelTable(tCorrel);
    }
}

SPRHistoryModel *SPRSeparateMainTableModel::getHistoryModel() const
{
    return historyModel;
}

void SPRSeparateMainTableModel::setHistoryModel(SPRHistoryModel *value)
{
    historyModel = value;
}

void SPRSeparateMainTableModel::setSpectrumsModel(SPRSpectrumListItemsModel *value)
{
    spectrumsModel = value;
    if(spectrumsModel){
        SPRMainModel *_main = spectrumsModel->getMainModel();
        mainModel = _main;
    }
}

SPRSeparateMainTableModel::SPRSeparateMainTableModel(SPRHistoryModel *_hist, SPRThreadList _threads, QObject *parent):
    QAbstractTableModel(parent), mainModel(nullptr)
{
    historyModel = _hist;
    setThreads(_threads);
}

int SPRSeparateMainTableModel::rowCount(const QModelIndex &) const
{
    return 4;
}

int SPRSeparateMainTableModel::columnCount(const QModelIndex &) const
{
    return threads.size();
}

QVariant SPRSeparateMainTableModel::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if(role == Qt::BackgroundColorRole){
        if(index.row() == 3){

            if(index.column() < correlTable.size()){
                if(correlTable.at(index.column()) < correlPorogPermiss && correlTable.at(index.column()) >= correlPorogCritical ){
                    res = QVariant(QColor(Qt::yellow));
                } else if(correlTable.at(index.column()) < correlPorogCritical){
                    res = QVariant(QColor(Qt::red));
                } else if(correlTable.at(index.column()) < -1){
                    res = QVariant(QColor(Qt::lightGray));
                }
            }
        }
        return res;
    }
    if(role == Qt::ForegroundRole){}
    if(role == Qt::FontRole){}
    if(role == Qt::DisplayRole){
        QString sres = "--";
        if((hxTable.size() > 0)  && (index.row() < hxTable.at(0).size())){// Концентрат, хвосты, средний
            if(index.column() < hxTable.size()){ // only one channel
                if(hxTable.at(index.column()).at(index.row()) > 0){ // data is as
                    sres = QString::number(hxTable.at(index.column()).at(index.row()),'f', 2);
                }
            } else if(index.column() == hxTable.size()){ // column "all"
                double val = 0; int count = 0;
                foreach(uint8_t th, threads) {
                    if(hxTable.at(th).at(index.row()) > 0){ // data is as
                        val += hxTable.at(index.column()).at(index.row()) * i_prd.at(index.column()).at(index.row());
                        count += i_prd.at(index.column()).at(index.row());
                    }
                    if(count > 0){
                        val = val / count;
                    }
                    if(val > 0){
                        sres = QString::number(val,'f', 2);
                    }
                }
            }
        } else if(index.row() == 3){ // correl
            if(index.column() < correlTable.size()){
                if(correlTable.at(index.column()) >= -1){
                    sres = QString::number(correlTable.at(index.column()), 'f', 2);
                }
            }
        }
        res = QVariant(sres);
        return res;
    }
    return res;
}

QVariant SPRSeparateMainTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant res = QAbstractTableModel::headerData(section, orientation, role);
    if(role == Qt::DisplayRole){
        if(orientation == Qt::Horizontal){
            res = QVariant(QString(tr("Общий")));
            if(section < threads.size()){
                res = QVariant(QString(tr("Ручей %1")).arg(section + 1));
            }
        }
        if(orientation == Qt::Vertical){
            switch(section){
            case 0:
                res = QVariant(QString(tr("Ср. концентрат")));
                break;
            case 1:
                res = QVariant(QString(tr("Ср. хвосты")));
                break;
            case 2:
                res = QVariant(QString(tr("Средний")));
                break;
//            case 3:
//                res = QVariant(QString(tr("Загрузка")));
//                break;
//            case 4:
//                res = QVariant(QString(tr("Отклонение")));
//                break;
            case 3:
                res = QVariant(QString(tr("Корреляция")));
                break;
            }
        }
    }
    return res;
}

void SPRSeparateMainTableModel::setThreads(SPRThreadList _threads)
{
    threads = _threads;
    setHxTable();
    setI_prd();
    setCorrelTable();
}


SPRSeprateMainTable::SPRSeprateMainTable(QWidget *parent):
    QTableView(parent),
    mainModel(nullptr),
    separateModel(nullptr),
    spectrumModel(nullptr)
{
    tableModel = new SPRSeparateMainTableModel(nullptr, getAllThreadsListDefault());
//    tableModel->setHxTable(&hxTableData);

    this->setModel(tableModel);
}

SPRSeprateMainTable::~SPRSeprateMainTable(){
    if(tableModel) delete tableModel;
}

void SPRSeprateMainTable::init(SPRMainModel *_mainModel, SPRHistoryModel *_histModel, SPRSpectrumListItemsModel *_kSpectModel){
    mainModel = _mainModel;
    if(mainModel){
        tableModel->setThreads(mainModel->getThreadsList());
        tableModel->setCorrelPorogs(mainModel->getSettingsControlModel()->correlSpectrumPermiss->getData(),
                                    mainModel->getSettingsControlModel()->correlSpectrumCritical->getData(),
                                    mainModel->getSettingsControlModel()->controlArea->getData());

        connect(mainModel, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
    }
    separateModel = _histModel;
    if(separateModel){
        tableModel->setHistoryModel(_histModel);
        connect(separateModel, SIGNAL(modelChanget(IModelVariable*)), this->tableModel, SLOT(onModelChanget(IModelVariable*)));
    }
    spectrumModel = _kSpectModel;
    if(spectrumModel){
        tableModel->setSpectrumsModel(spectrumModel);
        connect(spectrumModel, SIGNAL(modelChanget(IModelVariable*)), this->tableModel, SLOT(onModelChanget(IModelVariable*)));
    }

    onModelChanget(nullptr);
}

void SPRSeprateMainTable::onModelChanget(IModelVariable *source){
    if(!source){
        return;
    } else {
        if(source == mainModel->getThreads()){
            if(source == mainModel->getThreads()){
                tableModel->setThreads(mainModel->getThreadsList());
            }

            return;
        }
        if(source == mainModel->getSettingsControlModel()->correlSpectrumCritical ||
                mainModel->getSettingsControlModel()->correlSpectrumPermiss ||
                mainModel->getSettingsControlModel()->controlArea){
            tableModel->setCorrelPorogs(mainModel->getSettingsControlModel()->correlSpectrumPermiss->getData(),
                                        mainModel->getSettingsControlModel()->correlSpectrumCritical->getData(),
                                        mainModel->getSettingsControlModel()->controlArea->getData());
        }
        if(source == separateModel || sender() == separateModel){
        }
    }
}



