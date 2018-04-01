#ifndef SPRSEPARATEMODEL_H
#define SPRSEPARATEMODEL_H

#include <QObject>
#include <QByteArray>

#include "_types.h"
#include "models/isprmodeldata.h"
#include "models/sprvariable.h"
#include "models/sprspectrumzonesmodel.h"
#include "models/sprmainmodel.h"



struct sobl{
    double ls;
    double rs;
};

typedef struct sep_ust{
    sobl obl[MAX_SPR_MAIN_THREADS][MAX_SPR_SPECTOR_ELEMENTS]; // пороги по областям из SPRSpectrumZonesModel
    double gmz[MAX_SPR_MAIN_THREADS]; // из собственной модели
    // H(index) = (ElementUp1 + ElementUp2 * MulUp)/(ElementDown1 + MupDown * ElementDown2 * ElementDown3 / ElementDown4)
    // из SPRSettingsFormulasModel и SPRFormulaItemModel
    double sh1[6]; // индексы суммы областей для формулы H1 (ElementXXXX)
    double sh2[6]; // индексы суммы областей для формулы H2 (ElementXXXX)
    double sh3[6]; // индексы суммы областей для формулы H3 (ElementXXXX)
    double kh1[2]; // множители для формулы H1 (MulXXXX)
    double kh2[2]; // множители для формулы H2 (MulXXXX)
    double kh3[2]; // множители для формулы H3 (MulXXXX)

    double prg[MAX_SPR_MAIN_THREADS][MAX_SPR_FORMULA_CONDITION]; // пороговые значения по 6 условиям сравнения из SPRSettingsPorogsModel и SPRPorogsModel
// для первого ряда
    double prg2[MAX_SPR_MAIN_THREADS][MAX_SPR_FORMULA_CONDITION]; // тоже для второго ряда
    double tiz[DEF_SPR_IMS_PARTS_SIZE+1]; // времена измерения камней разной крупности из SPRSettingsIMSNodel (timesMettering)
    double fh12; // проверка условий сравнения для решения 0 = H1(1 сравнение)
                                                        // 1 = H1 и H2 (4 сравнения)
                                                        // 2 = H1 и H2 и H3 (6 сравнений) из SPRSettingsFormulaModel
    double fotb; // инверсия отбора 0 или 1 из SPRSettingrPorogsModel
    double fotbR2; // тоже для второго ряда
    double maxg1;  // что то минимальное и максимальное из SPRSettingsFormulaModel
    double ming1;
    double gcol; // колличество элементов гистограммы из собственной модели 10 - 200

    double  kruch;  // нечто 1 - 4 не используется def=4
    double  usl[MAX_SPR_MAIN_THREADS]; // что то со значением 1 для парсинга исходных данных из собственной модели
    double  totb;   // срабатываний в секунду из SPRSettingsIMSModel (blockImsParam, blockImsParam2)
    double  totbR2;
    double  k_im[2][MAX_SPR_MAIN_THREADS]; // из SPRSettingsIMSModel (kKoeffDuration, bKoeffDuration)
    double  b_im[2][MAX_SPR_MAIN_THREADS];

    double  k_zd[2][MAX_SPR_MAIN_THREADS]; // из SPRSettingsIMSModel (kKoeffDelay, bKoeffDelay)
    double  b_zd[2][MAX_SPR_MAIN_THREADS];
    double  kprMin; // forMinStone, forMaxStone из SPRSettingsPorogsModel
    double  kprMax;
    double  alg; // походу ручная установка номера алгоритма из собственной модели
    double  sep_row; // похоже тоже ручная установка или по непонятному алгоритму из собственной модели
   } SPRSettintsSeparate;

typedef struct ssep_work {
  //double i_kn[MAX_CH+1];
  //double i_xw[MAX_CH+1];
  //double i_km[MAX_CH+1];
  double i_prd[MAX_SPR_MAIN_THREADS][4]; // MAX_CH = 4 = 8x4x4 = 128
  //double p_cr[MAX_CH];
  //double p_crk[MAX_CH];
  //double p_crx[MAX_CH];
  double p_prd[MAX_SPR_MAIN_THREADS][4]; // 8x4x4= 128
  double p_tk[MAX_SPR_MAIN_THREADS]; // 8x4 = 32
  double p_tkh1[MAX_SPR_MAIN_THREADS]; // 8x4 = 32
  double p_tkh2[MAX_SPR_MAIN_THREADS]; // 8x4 = 32
  double p_tkh3[MAX_SPR_MAIN_THREADS]; // 8x4 = 32
  double wcount[MAX_SPR_MAIN_THREADS]; // 8x4 = 32
  double s_rst[MAX_SPR_MAIN_THREADS][DEF_SPR_IMS_PARTS_SIZE+1]; // MAX_GR = 5 = 8x4x5 = 160
  double error ; // = 8 ///////
} SPRWorkSeparate;

class SPRSeparateModel : public ISPRModelData
{

    SPRMainModel *mainModel;
    SPRSettintsSeparate settingsSeparate;
    SPRWorkSeparate workSeparate;

public:
//    QMap<EnumElements, SPRSpectrumZonesModel::SpectorRange> elements;

    QVector<SPRVariable<uint>*> gmz;
    SPRVariable<uint> *gcol;
    SPRVariable<uint> *kruch;
    QVector<SPRVariable<uint>*> usl;
    SPRVariable<uint> *alg;
    SPRVariable<uint> *sep_row;

    SPRSeparateModel():
        ISPRModelData(), mainModel(0), gmz(), gcol(nullptr), kruch(nullptr), usl(), alg(nullptr), sep_row()
    {
    }

    void *fullWorkSeparate(QByteArray rawData){
        return memcpy(&workSeparate, rawData.constData(), sizeof(workSeparate));
    }
    QByteArray toByteArray()
    {
        memset(&settingsSeparate, 0, sizeof(settingsSeparate));

        SPRPorogsModel *porogs = mainModel->getSettingsPorogsModel()->getPorogs();
        SPRPorogsModel *porogs2 = mainModel->getSettingsPorogsModel()->getPorogs2();
        for(int th=0; th<MAX_SPR_MAIN_THREADS; th++){
            ElementsProperty *elements = mainModel->getSpectrumZonesTableModel()->getElements(th);
            foreach(EnumElements el, (*elements).keys()){
                settingsSeparate.obl[th][(*elements)[el].elIndex].ls = (*elements)[el].min->getData();
                settingsSeparate.obl[th][(*elements)[el].elIndex].rs = (*elements)[el].max->getData();
            }
            for(int cond=0; cond<MAX_SPR_FORMULA_CONDITION; cond++){
                settingsSeparate.prg[th][cond] = porogs->porogs[th][cond]->getData();
                settingsSeparate.prg2[th][cond] = porogs2->porogs[th][cond]->getData();
            }
            settingsSeparate.k_im[0][th] = mainModel->getSettingsIMSModel()->kKoeffDuration[th]->getData();
            settingsSeparate.b_im[0][th] = mainModel->getSettingsIMSModel()->bKoeffDuration[th]->getData();
            settingsSeparate.k_zd[0][th] = mainModel->getSettingsIMSModel()->kKoeffDelay[th]->getData();
            settingsSeparate.b_zd[0][th] = mainModel->getSettingsIMSModel()->bKoeffDelay[th]->getData();

            settingsSeparate.gmz[th] = gmz[th]->getData();
            settingsSeparate.usl[th] = usl[th]->getData();

        }

        for(int i=0; i<DEF_SPR_IMS_PARTS_SIZE+1;i++){
            settingsSeparate.tiz[i] = mainModel->getSettingsIMSModel()->timesMettering[i]->getData();
        }


        SPRSettingsFormulaModel *formulas = mainModel->getSettingsFormulaModel();
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

        settingsSeparate.fh12 = static_cast<double>(mainModel->getSettingsFormulaModel()->getConditions()->getData());

        settingsSeparate.fotb = static_cast<double>(mainModel->getSettingsPorogsModel()->invertSelection->getData());
        settingsSeparate.fotbR2 = static_cast<double>(mainModel->getSettingsPorogsModel()->invertSelection2->getData());

        settingsSeparate.ming1 = mainModel->getSettingsFormulaModel()->min->getData();
        settingsSeparate.maxg1 = mainModel->getSettingsFormulaModel()->max->getData();

        settingsSeparate.gcol = gcol->getData();
        settingsSeparate.kruch = kruch->getData();

        settingsSeparate.totb = mainModel->getSettingsIMSModel()->blockImsParam->getData();
        settingsSeparate.totbR2 = mainModel->getSettingsIMSModel()->blockImsParam2->getData();

        settingsSeparate.kprMin = mainModel->getSettingsPorogsModel()->forMinStone->getData();
        settingsSeparate.kprMax = mainModel->getSettingsPorogsModel()->forMaxStone->getData();

        settingsSeparate.alg = alg->getData();
        settingsSeparate.sep_row = sep_row->getData();

        QByteArray ret = QByteArray::fromRawData((char*)&settingsSeparate, sizeof(settingsSeparate));
        return ret;
    }

    SPRSeparateModel(QDomDocument *_doc, ISPRModelData *parent=nullptr);
    SPRMainModel *getMainModel() const;
    void setMainModel(SPRMainModel *value);
    virtual ~SPRSeparateModel();

};

#endif // SPRSEPARATEMODEL_H
