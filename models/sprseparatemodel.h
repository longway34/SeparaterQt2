#ifndef SPRSEPARATEMODEL_H
#define SPRSEPARATEMODEL_H

#include <QObject>
#include <QByteArray>
#include <QDateTime>

#include "_types.h"
//#include "models/isprmodeldata.h"
#include "variables/sprvariable.h"
#include "models/sprspectrumzonesmodel.h"
//#include "models/sprmainmodel.h"
#include "models/sprporogsmodel.h"
//#include "models/imainmodel.h"

#include <qwt_interval.h>
#include <qwt_series_data.h>

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
    double maxg1;  // минимальное и максимальное параметра из SPRSettingsFormulaModel для деления на категории в гистограмме
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

class SPRWorkSeparate
{
public:
    uint64_t dt;
  struct{
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
      double s_rst[MAX_SPR_MAIN_THREADS][5]; // MAX_CH = 4 MAX_GR = 5 = 8x4x5 = 160
      double error ; // = 8 ///////
  } source;
  SPRWorkSeparate(){
      clear();
  }
  void clear(){
      for(int th=0; th<MAX_SPR_MAIN_THREADS; th++){
          for(int i=0; i<4; i++){
              source.i_prd[th][i] = 0; source.p_prd[th][i] = 0;
          }
          for(int i=0; i<5; i++){
              source.s_rst[th][i] = 0;
          }
          source.p_tk[th] = 0; source.p_tkh1[th] = 0; source.p_tkh2[th] = 0; source.p_tkh3[th] = 0;
          source.wcount[th] = 0;
      }
      source.error = 0;
  }
};

typedef struct ssep_work_row{
    int id;
    int number;
    uint64_t dt;
    int thread;
    double i_prd[4]; // MAX_CH = 4 = 8x4 = 32
    double p_prd[4]; // 8x4= 32
    double p_tk; // 8 = 8
    double p_tkh1; // 8
    double p_tkh2; // 8
    double p_tkh3; // 8
    double wcount; // 8
    double s_rst[5]; // MAX_GR = 5 = 8x5 = 40

    bool isNullStucture(){
        double summ = 0;
//        summ += p_tk + p_tkh1 + p_tkh2 + p_tkh3 + wcount;

        for(int i=0; i < 4; i++){
            summ += i_prd[i]/* + p_prd[i]*/;
        }
        for(int i=0; i<5; i++){
            summ += s_rst[i];
        }
        return fabs(summ) < 1e-6;
    }
    ssep_work_row(){
        id = rand();
        dt = QDateTime::currentDateTime().toSecsSinceEpoch();
        clear();
    }
    void clear(){
        for(int i=0; i<4; i++){
            i_prd[i] = 0; p_prd[i] = 0;
        }
        for(int i=0; i<5; i++){
            s_rst[i] = 0;
        }
        p_tk = 0; p_tkh1 = 0; p_tkh2 = 0; p_tkh3 = 0;
        wcount = 0;
    }
} SPRWorkSeparateRow;

typedef struct sgist{
    int gist[DEF_SPR_SEPARATE_GCOL];
    int gcol;
    int thread;
    int number;

    QDateTime dt;

    bool isNullStructure(){
        int summ = 0;
        for(int i=0; i < DEF_SPR_SEPARATE_GCOL; i++){
            summ += gist[i];
        }
        return summ == 0;
    }
    QString toString(){
        QString ret = "";
        if(!isNullStructure()){
            ret += QString::number(thread) + " ";
//            if(dt.isValid()){
//                ret += dt.toString("dd.MM hh:mm:ss ");
//            }
            for(int i=0; i < gcol; i++){
                if(gist[i] > 0){
                    if(i < gcol-1){
                        ret += QString("s%1=%2 ").arg(i).arg(QString::number(gist[i]));
                    } else {
                        ret += QString("???=%1").arg(gist[i]);
                    }
                }
            }
        }
        return ret;
    }
    sgist(int _gcol = DEF_SPR_SEPARATE_GCOL, int _thread = 0) {
        if(_gcol <= DEF_SPR_SEPARATE_GCOL && _gcol >= 0){
            gcol = _gcol;
        } else {
            gcol = DEF_SPR_SEPARATE_GCOL;
        }
        if(_thread >=0 && _thread < MAX_SPR_MAIN_THREADS){
            thread = _thread;
        } else {
            thread = 0;
        }
        dt=QDateTime::currentDateTime();
        clear();
    }
    void clear(){
        for(int i=0; i<DEF_SPR_SEPARATE_GCOL; i++){
            gist[i] = 0;
        }
    }


public:
    int getGcol() const;
    void setGcol(int value);
} SPRWorkGistogrammRow;

class SPRMainModel;

class SPRSeparateModel : public ISPRModelData
{

    SPRMainModel *mainModel;
    SPRSettintsSeparate settingsSeparate;

    bool separateStructupeEmpty;

    void *fullWorkSeparate(SPRWorkSeparate *dst, QByteArray rawData);
    void *fullWorkGistogramm(SPRWorkGistogrammRow *dst, QByteArray rawData);
    void addWorkSeparateDataRow(SPRWorkSeparateRow *row);
    void addWorkGistogrammDataRow(SPRWorkGistogrammRow row);

public:
//    QMap<EnumElements, SPRSpectrumZonesModel::SpectorRange> elements;
    SPRWorkSeparate workSeparateCurrent;
    SPRWorkSeparate workSeparateReceive;

    QVector<SPRWorkSeparateRow*> workSeparateRows;

    SPRWorkGistogrammRow workGistogrammCurrent[MAX_SPR_MAIN_THREADS];
    SPRWorkGistogrammRow workGistogrammReceive[MAX_SPR_MAIN_THREADS];
    QVector<SPRWorkGistogrammRow> workGistogrammRows;

    QVector<SPRVariable<double>*> gmz;
    SPRVariable<uint> *gcol;
    SPRVariable<uint> *kruch;
    QVector<SPRVariable<uint>*> usl;
    SPRVariable<uint> *alg;
    SPRVariable<uint> *sep_row;

    SPRSeparateModel():
        ISPRModelData(), gmz(), gcol(nullptr), kruch(nullptr), usl(), alg(nullptr), sep_row(), separateStructupeEmpty(true)
    {
    }

    QVector<QVector<QwtIntervalSample>> getGistogrammContentData(int thread = -1, int parts = -1);
    QVector<QPointF> getGistogrammContentData2(int thread = -1);

    void setWorkSeparateData(QByteArray rawData);

    void setWorkGistogrammData(QByteArray rawData, int thread);

    QByteArray toByteArray(int *errors);

    SPRSeparateModel(QDomDocument *_doc, SPRMainModel *_model, ISPRModelData *parent=nullptr);
//    SPRMainModel *getModelData() const;
    void setModelData(ISPRModelData *value);
    virtual ~SPRSeparateModel();

    SPRSettintsSeparate *getSettingsSeparate();
    bool isSeparateEmpty() const;
    void setSeparateEmpty(bool value);

private:
};

#endif // SPRSEPARATEMODEL_H
