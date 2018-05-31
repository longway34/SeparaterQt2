#ifndef SPRSEPARATEMODEL_H
#define SPRSEPARATEMODEL_H

#include <QObject>
#include <QByteArray>

#include "_types.h"
#include "models/isprmodeldata.h"
#include "variables/sprvariable.h"
#include "models/sprspectrumzonesmodel.h"
//#include "models/sprmainmodel.h"
#include "models/sprporogsmodel.h"
#include "models/imainmodel.h"



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

//    SPRMainModel *mainModel;
    SPRSettintsSeparate settingsSeparate;

    void *fullWorkSeparate(SPRWorkSeparate *dst, QByteArray rawData){
        return memcpy(dst, rawData.constData()+1, sizeof(SPRWorkSeparate));
    }
public:
//    QMap<EnumElements, SPRSpectrumZonesModel::SpectorRange> elements;
    QVector<SPRWorkSeparate*> workSeparate;

    QVector<SPRVariable<double>*> gmz;
    SPRVariable<uint> *gcol;
    SPRVariable<uint> *kruch;
    QVector<SPRVariable<uint>*> usl;
    SPRVariable<uint> *alg;
    SPRVariable<uint> *sep_row;

    SPRSeparateModel():
        ISPRModelData(), gmz(), gcol(nullptr), kruch(nullptr), usl(), alg(nullptr), sep_row()
    {
    }
    void addWorkSeparateData(QByteArray rawData){
        uint uots = rawData.size();
        uint inps = sizeof(SPRWorkSeparate);

        if(rawData.size()-1 == sizeof(SPRWorkSeparate)){
            SPRWorkSeparate *str = (SPRWorkSeparate*)malloc(sizeof(SPRWorkSeparate));
            fullWorkSeparate(str, rawData);
            workSeparate.push_front(str);
            if(workSeparate.size() > 4){
                delete workSeparate[workSeparate.size()-1];
                workSeparate.pop_back();
            }
        }
    }

    QByteArray toByteArray(IMainModel *_mainModel);

    SPRSeparateModel(QDomDocument *_doc, ISPRModelData *parent=nullptr);
//    SPRMainModel *getModelData() const;
//    void setModelData(SPRMainModel *value);
    virtual ~SPRSeparateModel();

    SPRSettintsSeparate *getSettingsSeparate();
};

#endif // SPRSEPARATEMODEL_H
