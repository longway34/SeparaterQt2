#include "sprseparateoutputmodel.h"
#include "models/sprmainmodel.h"

SPRWorkSeparate2 *SPRSeparateOutputModel::getWorkSeparateLast() const
{
    return workSeparateLast;
}

SPRWorkSeparate2 *SPRSeparateOutputModel::getWorkSeparatePrev() const
{
    if(workSeparatePrev)
        return workSeparatePrev;
    else
        return workSeparateLast;
}

OneTimeGistorgamms SPRSeparateOutputModel::getWorkGistogrammsLast() const
{
    return workGistogrammsLast;
}

OneTimeGistorgamms SPRSeparateOutputModel::getWorkGistogrammsPrev() const
{
    if(workGistogrammsPrev.size() == 0)
        return workGistogrammsLast;
    else
        return workGistogrammsPrev;
}

SPRThreadList SPRSeparateOutputModel::getWorkThreadList() const
{
    return workThreadList;
}

//SPRSpectrumListItemsModel *SPRSeparateOutputModel::getKSpectrums() const
//{
//    return kSpectrums;
//}

SPRSeparateOutputModel::SPRSeparateOutputModel(SPRMainModel *_mainModel, SPRThreadList _threads, ISPRModelData *myParent):
    ISPRModelData("", myParent), workSeparateLast(nullptr), workSeparatePrev(nullptr)
{
    mainModel = _mainModel;
    if(mainModel){
//        QList<SPRSpectrumItemModel*> *bases = mainModel->getSpectrumListItemsModel()->getSpectrumsModelBase();
//        kSpectrums = new SPRSpectrumListItemsModel(mainModel, mainModel->getSpectrumListItemsModel()->getSpectrumsModel(spectrumBase) ,true, nullptr);
    }

    if(_threads.isEmpty()){
        if(mainModel){
            workThreadList = mainModel->getThreadsList();
        } else {
            workThreadList = getAllThreadsListDefault();
        }
    } else {
        workThreadList = _threads;
    }
}

SPRSeparateOutputModel::~SPRSeparateOutputModel()
{
    if(workSeparateLast) delete workSeparateLast; workSeparateLast = nullptr;
    if(workSeparatePrev) delete workSeparatePrev; workSeparatePrev = nullptr;

    for(int row=0; row<workGistogrammsLast.size(); row++){
            if(workGistogrammsLast[row]) delete workGistogrammsLast[row]; workGistogrammsLast[row] = nullptr;
    }
    workGistogrammsLast.clear();

    for(int row=0; row<workGistogrammsPrev.size(); row++){
            if(workGistogrammsPrev[row]) delete workGistogrammsPrev[row]; workGistogrammsPrev[row] = nullptr;
    }
    workGistogrammsPrev.clear();
}



SPRWorkSeparate2 *SPRSeparateOutputModel::addWorkSeparateData(QByteArray buf){
    return addWorkSeparateData((void*)buf.constData(), buf.size());
}

SPRWorkSeparate2 *SPRSeparateOutputModel::addWorkSeparateData(void *buf, int size){
    SPRWorkSeparate2 *res = nullptr;
    qint64 mdt = QDateTime::currentDateTime().toSecsSinceEpoch();
    qint64 mdtLast = 0;
    if(workSeparateLast){
        mdtLast = workSeparateLast->mdt;
    }
    if(abs(static_cast<long>(mdt - mdtLast)) >= 1000 || mdtLast < 1e-3){
        if(buf && static_cast<uint>(size) <= sizeof(workSeparateLast->source)){
            SPRWorkSeparate2 *temp = workSeparateLast; workSeparateLast = workSeparatePrev; workSeparatePrev = temp;
            if(!workSeparateLast){
                workSeparateLast = new SPRWorkSeparate2();
            } else {
                workSeparateLast->init();
            }

//            if(workSeparateLast){
//                qDebug() << ""; qDebug() << "";
//                qDebug()<< "last:";
//                workSeparateLast->toDebug();
//            }
            memcpy(&workSeparateLast->source, buf, static_cast<size_t>(size));

//            if(workSeparateLast){
//                qDebug()<< "last past:";
//                workSeparateLast->toDebug();
//            }
//            if(workSeparatePrev){
//                qDebug()<< "prev:";
//                workSeparatePrev->toDebug();
//            }
//            if(mdtLast == 0){
//                workSeparatePrev = workSeparateLast;
//            }
            res = workSeparateLast;
            emit modelChanget(this);
        }
    }
    return res;
}

SPRWorkGistorgamm *SPRSeparateOutputModel::addWorkGistogrammData(QByteArray buf, uint8_t thread)
{
    return addWorkGistogrammData((void*)buf.constData(), buf.size(), thread);
}

SPRWorkGistorgamm *SPRSeparateOutputModel::addWorkGistogrammData(void *buf, int size, uint8_t _thread){
    SPRWorkGistorgamm *res = nullptr;
    if(workThreadList.contains(_thread)){
        qint64 mdt = QDateTime::currentDateTime().toMSecsSinceEpoch();
        qint64 mdtLast = 0;
        if(workGistogrammsLast[_thread]){
            mdtLast = workGistogrammsLast[_thread]->mdt;
        }

        if(abs(static_cast<long>(mdt - mdtLast)) >= 1000){
            if(buf && size <= DEF_SPR_SEPARATE_GCOL){
                SPRWorkGistorgamm* temp = workGistogrammsLast[_thread];
                workGistogrammsLast[_thread] = workGistogrammsPrev[_thread];
                workGistogrammsPrev[_thread] = temp;
                if(!workGistogrammsLast[_thread]){
                    workGistogrammsLast[_thread] = new SPRWorkGistorgamm(_thread);
                } else {
                    workGistogrammsLast[_thread]->init(_thread);
                }
                memcpy(workGistogrammsLast[_thread]->gist, buf, static_cast<size_t>(size));
                if(mdtLast == 0){
                   workGistogrammsPrev[_thread] = workGistogrammsLast[_thread];
                }
                res = workGistogrammsLast[_thread];
                emit modelChanget(this);
            }
        }

    }
    return res;
}

void SPRSeparateOutputModel::onModelChanget(IModelVariable *)
{
}
