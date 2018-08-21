#include "sprseparatedetailstable.h"
#include "models/sprmainmodel.h"
#include <QMap>
#include <QColor>
#include <QScrollBar>

// ************************** SPRSeparateDetailsTableModel *************************
// *********************************************************************************

SPRThreadList SPRSeparateDetailsTableModel::getThreads() const
{
    return threads;
}

void SPRSeparateDetailsTableModel::setThreads(const SPRThreadList &value)
{
    threads = value;
}

QVariant SPRSeparateDetailsTableModel::data(const QModelIndex &index, int role) const
{
    QVariant value;

    if(role == Qt::TextAlignmentRole){
        value = QVariant(Qt::AlignCenter);
    }
    if(index.row() < mdata.size()){
        if(role == Qt::BackgroundColorRole){
            if(mdata[index.row()]->i_prd_Concentrale > 0){
                if(mdata[index.row()]->i_prd_Tail == 0){
                   return QVariant(QColor("#8CFFA8"));
                } else {
                    return QVariant(QColor(Qt::lightGray));
                }
            }
            return value;
        }
        if(role == Qt::DisplayRole){
            int col = index.column(), row = index.row();
            QString ret;
            switch(col){
            case 0:
                ret = QString::number(row+1);
                break;
            case 1:
                ret = QDateTime::fromSecsSinceEpoch(mdata[row]->mdt).toString("hh:mm:ss");
                break;
            case 2:
                ret = QString::number(mdata[row]->thread);
                break;
            case 3:
                ret = QString::number(mdata[row]->H1, 'f', 2);
                break;
            case 4:
                ret = QString::number(mdata[row]->H2, 'f', 2);
                break;
            case 5:
                ret = QString::number(mdata[row]->H3, 'f', 2);
                break;
            case 6:
                ret = QString::number(mdata[row]->t_scope);
                break;
            case 7:
                {
                    QString str;
                    if(mdata[row]->i_prd_Concentrale > 0){
                        str = QString(tr("концентрат"));
                        if(mdata[row]->i_prd_Concentrale > 1 || mdata[row]->i_prd_Concentrale < mdata[row]->i_prd_All){
                            str += QString(" - %1").arg(mdata[row]->i_prd_Concentrale);
                        }
                    }
                    if(mdata[row]->i_prd_Tail > 0){
                        if(str.size() > 0){ str += "; ";
                            str += QString(tr("хвост - %1")).arg(mdata[row]->i_prd_Tail);
                        } else {
                            str += QString(tr("хвост"));
                            if(mdata[row]->i_prd_Tail > 1){
                                str += QString(" - %1").arg(mdata[row]->i_prd_Tail);
                            }
                        }
                    }
                    ret = str;
                }
                break;
            }
            return QVariant(ret);
        }
    }
    return value;
}

int SPRSeparateDetailsTableModel::getMinStoneTime() const
{
    return minStoneTime;
}

void SPRSeparateDetailsTableModel::setMinStoneTime(int value)
{
    minStoneTime = value;
}

void SPRSeparateDetailsTableModel::startStopScope(bool _startStop){
    startStop = _startStop;
    if(_startStop){
        if(model){
            connect(model, SIGNAL(modelChanget(IModelVariable *)), this, SLOT(onModelChanget(IModelVariable*)));
            last = nullptr; prev = nullptr;
        }
    } else {
        if(model){
            disconnect(model, SIGNAL(modelChanget(IModelVariable *)), this, SLOT(onModelChanget(IModelVariable*)));
        }
    }
}

bool SPRSeparateDetailsTableModel::isStartingScope()
{
    return startStop;
}

void SPRSeparateDetailsTableModel::setModelData(SPRSeparateOutputModel *_model){
    model = _model;
    if(model){
        threads = model->getWorkThreadList();
    }
}

QVariant SPRSeparateDetailsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant ret = QAbstractTableModel::headerData(section, orientation, role);

    QVector<QString> hheaders = {tr("№ п/п"), tr("Время"), tr("Ручей"),
                            tr("H1"), tr("H2"), tr("H3"),
                            tr("размер (мм.)"), tr("концентрат/хвост")};

    if(role == Qt::DisplayRole){
        if(orientation == Qt::Horizontal){
            if(section < hheaders.size()){
                ret = QVariant(hheaders[section]);
            }
        }
    }
//    if(role == Qt::SizeHintRole){
//        if(orientation == Qt::Horizontal){
//            auto* p = qobject_cast<QTableView*>(QObject::parent());

//            p->horizontalHeader()->i

//            if(p != nullptr){
//                const int w = p->viewport()->size().width() - p->verticalScrollBar()->sizeHint().width();
//                QSize qs;
//                qs.setHeight(p->verticalHeader()->defaultSectionSize());
//                if(section < 6){
//                    qs.setWidth(static_cast<int>(std::max(static_cast<double>(w * 0.11), static_cast<double>(p->fontMetrics().width(hheaders[section]+"99")))));
//                    ret = QVariant(qs);
//                } else if(section < 7){
//                    qs.setWidth(w * 0.14);
//                } else {
//                    qs.setWidth(w * 0.20);
//                    ret = QVariant(qs);
//                }
//            }
//        }
//    }
    return ret;
}

QVector<SPRSeparateDetailsTableModel::mData *> SPRSeparateDetailsTableModel::diff() const{
    QVector<SPRSeparateDetailsTableModel::mData*> res;
    foreach(uint8_t th, threads){
        int idiff = static_cast<int>(last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_ALL] -
                prev->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_ALL]);
        if(idiff != 0){
            SPRSeparateDetailsTableModel::mData *_mData = new mData(th);

            _mData->H1 = last->source.p_tkh1[th];
            _mData->H2 = last->source.p_tkh2[th];
            _mData->H3 = last->source.p_tkh2[th];

            _mData->i_prd_All = idiff;
            _mData->i_prd_Concentrale = last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_CONCENTRATE] -
                    prev->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_CONCENTRATE];
            _mData->i_prd_Tail = last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_TAIL] -
                    prev->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_TAIL];

            _mData->t_scope = last->source.wcount[th] - prev->source.wcount[th];


//            qDebug() << "i_prd_all " << last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_ALL] << " prev: " << prev->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_ALL] << " = " << _mData->i_prd_All;
//            qDebug() << "i_prd_conc " << last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_CONCENTRATE] << " prev: " << prev->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_CONCENTRATE]<< " = " << _mData->i_prd_Concentrale;
//            qDebug() << "i_prd_tail " << last->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_TAIL] << " prev: " << prev->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_TAIL]<< " = " << _mData->i_prd_Tail;

//            qDebug() << "wcount " << last->source.wcount[th] << "; prev: " << prev->source.wcount[th] << " = " << _mData->t_scope;

            res.append(_mData);
        }
    }
    return res;
}

void SPRSeparateDetailsTableModel::onModelChanget(IModelVariable *send)
{
    if(send == model){
        last = model->getWorkSeparateLast();
        prev = model->getWorkSeparatePrev();

        if(mdata.size() > 0){
            if(last->mdt - mdata.first()->mdt < 1000){
                return;
            }
        }
        QVector<struct mData*> d = diff();
        if(d.size() > 0){
            beginResetModel();
            for(int row=0; row<d.size(); row++){
                mdata.push_front(d[row]);
            }
//            insertRows(0, d.size(), parent);
            endResetModel();
        }
        clearLast();
    }
}

void SPRSeparateDetailsTableModel::clear(){
    for(int row=0; row<mdata.size(); row++){
        delete mdata[row];
    }
    beginResetModel();
//    removeRows(0, mdata.size());
    mdata.clear();
    endResetModel();
    return;
}
void SPRSeparateDetailsTableModel::clearLast(){
    if(mdata.size() > 240){ // 60 sec X 4 thread
        beginResetModel();
        while(mdata.size() > 240){
            struct mData *row = mdata.takeLast();
            if(row) delete row;
        }
        endResetModel();
    }
}

// ************************** SPRSeparateDetailsTable ******************************
// *********************************************************************************


SPRSeparateDetailsTable::SPRSeparateDetailsTable(QWidget *parent)
    :QTableView(parent), ISPRWidget(), myModel(nullptr)
{

    setModel(new SPRSeparateDetailsTableModel(nullptr));


//    horizontalHeader()->set;
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    setAutoFillBackground(true);
    horizontalHeader()->setStretchLastSection(true);
//    horizontalHeader()->set
    verticalHeader()->setVisible(false);
    show();
}

SPRSeparateDetailsTable::~SPRSeparateDetailsTable()
{
    if(myModel) delete myModel;
}


void SPRSeparateDetailsTable::widgetsShow()
{
}

void SPRSeparateDetailsTable::startStopScope(bool value)
{
    if(myModel){
        myModel->startStopScope(value);
    }
}

bool SPRSeparateDetailsTable::isStartingScope()
{
    if(myModel){
        return myModel->isStartingScope();
    }
    return false;
}



void SPRSeparateDetailsTable::setMinStoneTime(int _time)
{
   if(myModel){
         myModel->setMinStoneTime(_time);
    }
}

int SPRSeparateDetailsTable::getMinStoneTime()
{
    if(myModel){
        return myModel->getMinStoneTime();
    }
}

void SPRSeparateDetailsTable::setVisibleThreads(SPRThreadList _threads)
{
    if(myModel){
        myModel->setThreads(_threads);
    }
}

SPRThreadList SPRSeparateDetailsTable::getVisibleThreads()
{
    if(myModel){
        return myModel->getThreads();
    }
    return SPRThreadList();
}

ISPRModelData *SPRSeparateDetailsTable::setModelData(SPRMainModel *data)
{
    mainModel = data;
    if(mainModel){
        if(myModel) delete myModel;

        myModel = new SPRSeparateDetailsTableModel(mainModel->getSeparateOutputModel(), this);
        setModel(myModel);

        connect(mainModel->getSeparateOutputModel(), SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        this->horizontalHeader()->resizeSections(QHeaderView::Stretch);

        horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
        horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        show();
    }
}

ISPRModelData *SPRSeparateDetailsTable::getModelData()
{
    return mainModel;
}

void SPRSeparateDetailsTable::clear()
{
    if(myModel){
        myModel->clear();
    }
}

void SPRSeparateDetailsTable::onModelChanget(IModelVariable *)
{
    if(sender() == mainModel->getSeparateOutputModel()){
//        resizeColumnsToContents();
    }
//    widgetsShow();
}

