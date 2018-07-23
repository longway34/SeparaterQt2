#include "sprseparatemodel.h"
//#include "models/sprmainmodel.h"

SPRSettintsSeparate *SPRSeparateModel::getSettingsSeparate()
{
    return &settingsSeparate;
}

bool SPRSeparateModel::isSeparateEmpty() const
{
    return separateStructupeEmpty;
}

void SPRSeparateModel::setSeparateEmpty(bool value)
{
    separateStructupeEmpty = value;
}

void *SPRSeparateModel::fullWorkSeparate(SPRWorkSeparate *dst, QByteArray rawData){
    return memcpy(dst, rawData.constData(), sizeof(SPRWorkSeparate));
}

void *SPRSeparateModel::fullWorkGistogramm(SPRWorkGistogrammRow *dst, QByteArray rawData)
{
//    int tst[200];
//    int size = rawData.size();
//    memcpy(tst, rawData.constData(), rawData.size() - 1);
    return memcpy(dst->gist, rawData.constData(), sizeof(dst->gist));
}

void SPRSeparateModel::addWorkSeparateDataRow(SPRWorkSeparateRow *row){
    int num = 0;
    if(workSeparateRows.size() > 0){
        num = workSeparateRows[0]->number+1;
    }
    row->number = num;
    workSeparateRows.push_front(row);
    if(workSeparateRows.size() > 20){
        SPRWorkSeparateRow *old = workSeparateRows.last();
        workSeparateRows.pop_back();
        if(old){
            delete old;
        }
    }
    emit modelChanget(this);
}

void SPRSeparateModel::addWorkGistogrammDataRow(SPRWorkGistogrammRow row){
    int num = 0;
    if(workGistogrammRows.size() >0){
        num = workGistogrammRows[0].number+1;
    }
    row.number = num;
    workGistogrammRows.push_front(row);
    if(workGistogrammRows.size() > 20){
        workGistogrammRows.pop_back();
    }
}

QVector<QVector<QwtIntervalSample>> SPRSeparateModel::getGistogrammContentData(int thread, int parts)
{
//    if(thread < 0) thread = 0;
    int thr_min = 0; int thr_max = MAX_SPR_MAIN_THREADS;
    if(thread >=0 && thread < MAX_SPR_MAIN_THREADS){
        thr_min = thread; thr_max = thread + 1;
    }
    QVector<QVector<QwtIntervalSample>> res;

    int mgcol = DEF_SPR_SEPARATE_GCOL;
    double ming1 = DEF_SPR_FORMULA_MIN, maxg1 = DEF_SPR_FORMULA_MAX;
    if(!isSeparateEmpty()){
        ming1 = settingsSeparate.ming1; maxg1 = settingsSeparate.maxg1;
        mgcol = settingsSeparate.gcol;
    }
    if(parts < 0){
        parts =mgcol;
    }
    if(parts == 0) parts = DEF_SPR_SEPARATE_GCOL;

    double w1 = qMax(ming1, maxg1) - qMin(ming1, maxg1);
    double step = w1 / parts;

    double bInterval = qMin(ming1, maxg1);

    for(int th=thr_min; th < thr_max; th++){

        double w = mgcol / parts;
        int width = (int)w; /*if((w - width) > 0.5) width++;*/

        QVector<QwtIntervalSample> int_th;
        for(int i=0; i<mgcol; i++){
            int index = i / width;
            while(int_th.size() <= index){
                int_th.push_back(QwtIntervalSample(0, bInterval, bInterval+step));
                bInterval += step;

                int_th[index].interval.setBorderFlags(QwtInterval::ExcludeMaximum);
            }
//            int val = workGistogrammCurrent[th].gist[i];
            int_th[index].value += workGistogrammCurrent[th].gist[i];
        }
        res.push_back(int_th);
    }
    return res;


}

QVector<QPointF> SPRSeparateModel::getGistogrammContentData2(int thread)
{
    QVector<QVector<QwtIntervalSample>> intRes = getGistogrammContentData(thread, -1);

    QVector<QPointF> res;
//    res.resize(intRes[0].size());

    if(intRes.size() > 0){
        for(int sample=0; sample<intRes[0].size(); sample++){
            double value = 0;
            for(int th=0; th<intRes.size(); th++){
                value += intRes[th][sample].value;
            }
            QPointF pf(intRes[0][sample].interval.minValue(), value);
            res.push_back(pf);
        }
    }
    return res;
}

void SPRSeparateModel::setWorkSeparateData(QByteArray rawData){
    fullWorkSeparate(&workSeparateReceive, rawData);

    bool needSignal = false;
    for(int th=0; th<MAX_SPR_MAIN_THREADS; th++){
        SPRWorkSeparateRow *diff = new SPRWorkSeparateRow();

        diff->p_tk = workSeparateReceive.p_tk[th]/* - workSeparateCurrent.p_tk[th]*/;
        diff->p_tkh1 = workSeparateReceive.p_tkh1[th]/* - workSeparateCurrent.p_tkh1[th]*/;
        diff->p_tkh2 = workSeparateReceive.p_tkh2[th]/* - workSeparateCurrent.p_tkh2[th]*/;
        diff->p_tkh3 = workSeparateReceive.p_tkh3[th]/* - workSeparateCurrent.p_tkh3[th]*/;
        diff->wcount = workSeparateReceive.wcount[th]/* - workSeparateCurrent.wcount[th]*/;
        for(int i=0; i<4; i++){
            diff->i_prd[i] = workSeparateReceive.i_prd[th][i] - workSeparateCurrent.i_prd[th][i];
            diff->p_prd[i] = workSeparateReceive.p_prd[th][i]/* - workSeparateCurrent.p_prd[th][i]*/;
        }
        for(int i = 0; i < 5; i++){
            diff->s_rst[i] = workSeparateReceive.s_rst[th][i] - workSeparateCurrent.s_rst[th][i];
        }
        if(!diff->isNullStucture()){
            diff->thread = th;
            diff->dt = QDateTime::currentDateTime();
            addWorkSeparateDataRow(diff);
            memcpy(&workSeparateCurrent, &workSeparateReceive, sizeof(SPRWorkSeparate));
//            workSeparateCurrent.p
            needSignal = true;
        } else {
            delete diff;
        }
    }


    if(needSignal){
        emit modelChanget(this);
    }
}

void SPRSeparateModel::setWorkGistogrammData(QByteArray rawData, int thread)
{
    fullWorkGistogramm(&workGistogrammReceive[thread], rawData);

    bool needSignal = false;
    SPRWorkGistogrammRow diff(gcol->getData(), thread);
    for(int i=0; i<gcol->getData(); i++){
        diff.gist[i] = workGistogrammReceive[thread].gist[i] - workGistogrammCurrent[thread].gist[i];
    }
    if(!diff.isNullStructure()){
        diff.thread = thread;
        diff.dt = QDateTime::currentDateTime();

        addWorkGistogrammDataRow(diff);
        memcpy(workGistogrammCurrent[thread].gist, workGistogrammReceive[thread].gist, sizeof(workGistogrammCurrent[thread].gist));
        needSignal = true;
    }
    if(needSignal){
        qDebug() << diff.toString();
        emit modelChanget(this);
    }
}

QByteArray SPRSeparateModel::toByteArray(IMainModel *_mainModel, int *errors)
{
    memset(&settingsSeparate, 0, sizeof(settingsSeparate));

    if(errors){
        *errors = SPR_SEPARATE_STATE_OK;
    }

    for(int th=0; th<MAX_SPR_MAIN_THREADS; th++){
        QMapElementsRanges elements = _mainModel->getSpectrumZonesTableModel()->getElementsRanges(th);
        //            SPRElementsModel *elProperty = mainModel->getSpectrumZonesTableModel()->getElementsProperty();
        //            QVector<int> unusedIndex;
        //            for(int i=0; i<MAX_SPR_SPECTOR_ELEMENTS; i++){
        //                unusedIndex.push_back(i);
        //            }

        foreach(EnumElements el, (elements).keys()){
            settingsSeparate.obl[th][static_cast<int>(el)].ls = (elements)[el]->min->getData();
            settingsSeparate.obl[th][static_cast<int>(el)].rs = (elements)[el]->max->getData();
            //                unusedIndex.remove(static_cast<int>(el),1);
        }
        //            for(int i=0; i<unusedIndex.size(); i++){
        //                settingsSeparate.obl[th][unusedIndex[i]].ls = 1;
        //                settingsSeparate.obl[th][unusedIndex[i]].rs = 2;
        //            }
        for(int cond=0; cond<MAX_SPR_FORMULA_CONDITION; cond++){
            SPRPorogsModel *porogs = _mainModel->getSettingsPorogsModel()->getPorogs();
            SPRPorogsModel *porogs2 = _mainModel->getSettingsPorogsModel()->getPorogs2();

            settingsSeparate.prg[th][cond] = porogs->porogs[th][cond]->getData();
            settingsSeparate.prg2[th][cond] = porogs2->porogs[th][cond]->getData();
        }
        settingsSeparate.k_im[0][th] = _mainModel->getSettingsIMSModel()->kKoeffDuration[th]->getData();
        settingsSeparate.b_im[0][th] = _mainModel->getSettingsIMSModel()->bKoeffDuration[th]->getData();
        settingsSeparate.k_zd[0][th] = _mainModel->getSettingsIMSModel()->kKoeffDelay[th]->getData();
        settingsSeparate.b_zd[0][th] = _mainModel->getSettingsIMSModel()->bKoeffDelay[th]->getData();

        SPRSpectrumItemModel *item = _mainModel->getSpectrumListItemsModel()->getSpectrumBaseItem(th);
        double _gmz;
        if(item && *item->getSpectrumData()->summ > 0){
           _gmz = item->getXRay();
        } else {
           _gmz = gmz[th]->getData();
           if(errors){
               *errors += SPR_SEPARATE_STATE_ERROR_BASE_SPACTRUE;
           }
        }
        _gmz += _mainModel->getSettingsPorogsModel()->xRayCorrection->getData();
        settingsSeparate.gmz[th] = _gmz;

        settingsSeparate.usl[th] = usl[th]->getData();

    }

    for(int i=0; i<DEF_SPR_IMS_PARTS_SIZE+1;i++){
        settingsSeparate.tiz[i] = _mainModel->getSettingsIMSModel()->timesMettering[i]->getData();
    }


    SPRSettingsFormulaModel *formulas = _mainModel->getSettingsFormulaModel();
    //        QVector<QVector<double*>> kh = {{&settingsSeparate.kh1[0], &settingsSeparate.kh1[1]},
    //                                        {&settingsSeparate.kh2[0], &settingsSeparate.kh2[1]},
    //                                        {&settingsSeparate.kh3[0], &settingsSeparate.kh3[1]}};
    //        QVector<QVector<double*>> sh = {{&settingsSeparate.sh1[0], &settingsSeparate.sh1[1], &settingsSeparate.sh1[2],
    //                                         &settingsSeparate.sh1[3], &settingsSeparate.sh1[4], &settingsSeparate.sh1[5]},
    //                                        {&settingsSeparate.sh2[0], &settingsSeparate.sh2[1], &settingsSeparate.sh2[2],
    //                                         &settingsSeparate.sh2[3], &settingsSeparate.sh2[4], &settingsSeparate.sh2[5]},
    //                                        {&settingsSeparate.sh3[0], &settingsSeparate.sh3[1], &settingsSeparate.sh3[2],
    //                                         &settingsSeparate.sh3[3], &settingsSeparate.sh3[4], &settingsSeparate.sh3[5]}};
    //        for(int f=0; f<3; f++){
    settingsSeparate.sh1[0] = formulas->itemsModel[0]->ElementUp1->getIndex(); settingsSeparate.sh1[1] = formulas->itemsModel[0]->ElementUp2->getIndex();
    settingsSeparate.sh1[2] = formulas->itemsModel[0]->ElementDown1->getIndex(); settingsSeparate.sh1[3] = formulas->itemsModel[0]->ElementDown2->getIndex();
    settingsSeparate.sh1[4] = formulas->itemsModel[0]->ElementDown3->getIndex(); settingsSeparate.sh1[5] = formulas->itemsModel[0]->ElementDown4->getIndex();

    settingsSeparate.kh1[0] = formulas->itemsModel[0]->MulUp->getData(); settingsSeparate.kh1[1] = formulas->itemsModel[0]->MulDown->getData();

    settingsSeparate.sh2[0] = formulas->itemsModel[1]->ElementUp1->getIndex(); settingsSeparate.sh2[1] = formulas->itemsModel[1]->ElementUp2->getIndex();
    settingsSeparate.sh2[2] = formulas->itemsModel[1]->ElementDown1->getIndex(); settingsSeparate.sh2[3] = formulas->itemsModel[1]->ElementDown2->getIndex();
    settingsSeparate.sh2[4] = formulas->itemsModel[1]->ElementDown3->getIndex(); settingsSeparate.sh2[5] = formulas->itemsModel[1]->ElementDown4->getIndex();

    settingsSeparate.kh2[0] = formulas->itemsModel[1]->MulUp->getData(); settingsSeparate.kh2[1] = formulas->itemsModel[1]->MulDown->getData();

    settingsSeparate.sh3[0] = formulas->itemsModel[2]->ElementUp1->getIndex(); settingsSeparate.sh3[1] = formulas->itemsModel[2]->ElementUp2->getIndex();
    settingsSeparate.sh3[2] = formulas->itemsModel[2]->ElementDown1->getIndex(); settingsSeparate.sh3[3] = formulas->itemsModel[2]->ElementDown2->getIndex();
    settingsSeparate.sh3[4] = formulas->itemsModel[2]->ElementDown3->getIndex(); settingsSeparate.sh3[5] = formulas->itemsModel[2]->ElementDown4->getIndex();

    settingsSeparate.kh3[0] = formulas->itemsModel[2]->MulUp->getData(); settingsSeparate.kh3[1] = formulas->itemsModel[2]->MulDown->getData();

    //            QVector<SPRVariable<double>*> kv = {formulas->itemsModel[f]->MulUp, formulas->itemsModel[f]->MulDown};

    //            for(int i=0; i<6; i++){
    //                *(sh[f][i]) = static_cast<double>(ev[i]->getData());
    //            }
    //            for(int i=0; i<2; i++){
    //                *(kh[f][i]) = kv[i]->getData();
    //            }
    //        }

    settingsSeparate.fh12 = static_cast<double>(_mainModel->getSettingsFormulaModel()->getConditions()->getData());

    settingsSeparate.fotb = static_cast<double>(_mainModel->getSettingsPorogsModel()->invertSelection->getData());
    settingsSeparate.fotbR2 = static_cast<double>(_mainModel->getSettingsPorogsModel()->invertSelection2->getData());

    settingsSeparate.ming1 = _mainModel->getSettingsFormulaModel()->min->getData();
    settingsSeparate.maxg1 = _mainModel->getSettingsFormulaModel()->max->getData();

    settingsSeparate.gcol = gcol->getData();
    settingsSeparate.kruch = kruch->getData();

    settingsSeparate.totb = _mainModel->getSettingsIMSModel()->blockImsParam->getData();
    settingsSeparate.totbR2 = _mainModel->getSettingsIMSModel()->blockImsParam2->getData();

    settingsSeparate.kprMin = _mainModel->getSettingsPorogsModel()->forMinStone->getData();
    settingsSeparate.kprMax = _mainModel->getSettingsPorogsModel()->forMaxStone->getData();

    settingsSeparate.alg = alg->getData();
    settingsSeparate.sep_row = sep_row->getData();

    QByteArray ret = QByteArray::fromRawData((char*)&settingsSeparate, sizeof(settingsSeparate));
    setSeparateEmpty(false);

    return ret;
}

SPRSeparateModel::SPRSeparateModel(QDomDocument *_doc, ISPRModelData *parent):
    ISPRModelData(_doc, parent), separateStructupeEmpty(true), gmz(), gcol(nullptr), kruch(nullptr), usl(), alg(nullptr), sep_row(nullptr)
{
    //    mainModel = new SPRMainModel(doc);
//    setProperty("delete_main", QVariant(true));

    for(int i=0; i< MAX_SPR_MAIN_THREADS; i++){

//        memset(workGistogrammCurrent[i].gist, 0, sizeof(workGistogrammCurrent[i].gist));

//        qDebug() << "path " << SPR_SEPARATE_GMZ_PATH_PREFIX;

        QString path = SPR_SEPARATE_GMZ_PATH_PREFIX + QString::number(i)+"]";
        SPRVariable<double> *var = new SPRVariable<double>(doc, path, DEF_SPR_SEPARATE_GMZ, this);
        gmz.push_back(var);

        path = SPR_SEPARATE_USL_PATH_PREFIX + QString::number(i)+"]";
        SPRVariable<uint> *vari = new SPRVariable<uint>(doc, path, DEF_SPR_SEPARATE_USL, this);
        usl.push_back(vari);
    }

    gcol = new SPRVariable<uint>(doc, SPR_SEPARATE_GCOL_PATH, DEF_SPR_SEPARATE_GCOL, this);
    kruch = new SPRVariable<uint>(doc, SPR_SEPARATE_KRUCH_PATH, DEF_SPR_SEPARATE_KRUCH, this);
    alg = new SPRVariable<uint>(doc, SPR_SEPARATE_ALG_PATH, DEF_SPR_SEPARATE_ALG, this);
    sep_row = new SPRVariable<uint>(doc, SPR_SEPARATE_SEP_ROW_PATH, DEF_SPR_SEPARATE_SEP_ROW, this);

}


//SPRMainModel *SPRSeparateModel::getModelData() const
//{
//    return mainModel;
//}

//void SPRSeparateModel::setModelData(SPRMainModel *value)
//{
//    mainModel = value;
//    setProperty("delete_main", QVariant(false));
//}

SPRSeparateModel::~SPRSeparateModel(){
    //    if(mainModel && property("delete_main").toBool()){
    //        delete mainModel; mainModel = nullptr;
    //    }

    gmz.clear();
    usl.clear();
    if(gcol) delete gcol; gcol = nullptr;
    if(kruch) delete kruch; kruch = nullptr;
    if(alg) delete alg; alg = nullptr;
    if(sep_row) delete sep_row; sep_row = nullptr;
}

int sgist::getGcol() const
{
    return gcol;
}

void sgist::setGcol(int value)
{
    gcol = value;
}
