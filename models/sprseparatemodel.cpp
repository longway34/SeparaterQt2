#include "sprseparatemodel.h"
//#include "models/sprmainmodel.h"

SPRSettintsSeparate *SPRSeparateModel::getSettingsSeparate()
{
    return &settingsSeparate;
}

QByteArray SPRSeparateModel::toByteArray(IMainModel *_mainModel)
{
    memset(&settingsSeparate, 0, sizeof(settingsSeparate));

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

        settingsSeparate.gmz[th] = gmz[th]->getData();
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
    return ret;
}

SPRSeparateModel::SPRSeparateModel(QDomDocument *_doc, ISPRModelData *parent):
    ISPRModelData(_doc, parent), gmz(), gcol(nullptr), kruch(nullptr), usl(), alg(nullptr), sep_row(nullptr)
{
    //    mainModel = new SPRMainModel(doc);
//    setProperty("delete_main", QVariant(true));

    for(int i=0; i< MAX_SPR_MAIN_THREADS; i++){
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
