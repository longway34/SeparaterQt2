#ifndef SPRSEPARATEOUTPUTMODEL_H
#define SPRSEPARATEOUTPUTMODEL_H

#include <QObject>
#include <QDateTime>
#include <QByteArray>

#include "_types.h"
#include "models/isprmodeldata.h"
#include "variables/sprvariable.h"
#include "models/sprspectrumlistitemsmodel.h"

class SPRMainModel;

class SPRWorkGistorgamm : public QObject{

public:
    qint64 dt;
    qint64 mdt;

    void init(uint8_t _thread, int _gcol = DEF_SPR_SEPARATE_GCOL){
        threadNum = (_thread >= MAX_SPR_MAIN_THREADS) ? 0 : _thread;
        gcol = _gcol < 0 ? DEF_SPR_SEPARATE_GCOL : _gcol;

        mdt = QDateTime::currentDateTime().toMSecsSinceEpoch();
        dt = mdt / 1000;
        clear();
    }
    void clear(){
        memset(gist, 0, DEF_SPR_SEPARATE_GCOL * sizeof(int));
    }
    int gist[DEF_SPR_SEPARATE_GCOL];
    int gcol;
    uint8_t threadNum;

    SPRWorkGistorgamm(uint8_t _thread, int _gcol = DEF_SPR_SEPARATE_GCOL, QObject *parent = nullptr):
        QObject(parent)
    {
        init(_thread, _gcol);
    }
    virtual ~SPRWorkGistorgamm(){}
};

class SPRWorkSeparate2 :public QObject
{
public:
    qint64 dt;
    qint64 mdt;
  struct{
      //double i_kn[MAX_CH+1];
      //double i_xw[MAX_CH+1];
      //double i_km[MAX_CH+1];
      double i_prd[MAX_SPR_MAIN_THREADS][MAX_SPR_MAIN_THREADS]; // MAX_CH = 4 = 8x4x4 = 128
      //double p_cr[MAX_CH];
      //double p_crk[MAX_CH];
      //double p_crx[MAX_CH];
      double p_prd[MAX_SPR_MAIN_THREADS][MAX_SPR_MAIN_THREADS]; // 8x4x4= 128
      double p_tk[MAX_SPR_MAIN_THREADS]; // 8x4 = 32
      double p_tkh1[MAX_SPR_MAIN_THREADS]; // 8x4 = 32
      double p_tkh2[MAX_SPR_MAIN_THREADS]; // 8x4 = 32
      double p_tkh3[MAX_SPR_MAIN_THREADS]; // 8x4 = 32
      double wcount[MAX_SPR_MAIN_THREADS]; // 8x4 = 32
      double s_rst[MAX_SPR_MAIN_THREADS][MAX_SPR_SEPARATE_TIME_SIZE_GROUPS]; // MAX_CH = 4 MAX_GR = 5 = 8x4x5 = 160
      double error ; // = 8 ///////
  } source;
  SPRWorkSeparate2(QObject *parent=nullptr):
        QObject(parent)
  {
      init();
  }
  void init(){
      clear();
      mdt = QDateTime::currentDateTime().toMSecsSinceEpoch();
      dt = mdt / 1000;
  }
  void clear(){
      memset(&source, 0, sizeof(source));
  }
  virtual ~SPRWorkSeparate2(){}
  void toDebug(){
//      uint32_t ptr = this;
      qDebug() << "time:" << dt <<"; ptr: 0x" << QString("%1").arg((uint32_t)this, 16);
      for(int i=0; i< MAX_SPR_MAIN_THREADS; i++){
          qDebug() << "i_prd: " << source.i_prd[i][0] << "; " << source.i_prd[i][1] << "; " << source.i_prd[i][2] << "; " << source.i_prd[i][3];
      }
  }
};


typedef QVector<SPRWorkGistorgamm*> OneTimeGistorgamms;

class SPRSeparateOutputModel : public ISPRModelData
{

    Q_OBJECT


    SPRWorkSeparate2* workSeparateLast;
    SPRWorkSeparate2* workSeparatePrev;

    OneTimeGistorgamms workGistogrammsLast;
    OneTimeGistorgamms workGistogrammsPrev;

    SPRThreadList workThreadList;

//    SPRSpectrumListItemsModel *kSpectrums;

    SPRMainModel *mainModel;
public:
    SPRSeparateOutputModel(SPRMainModel *_mainModel, SPRThreadList _threads, ISPRModelData *myParent);

    virtual ~SPRSeparateOutputModel();

    SPRWorkSeparate2 *addWorkSeparateData(QByteArray buf);
    SPRWorkSeparate2 *addWorkSeparateData(void *buf, int size);

    SPRWorkGistorgamm* addWorkGistogrammData(QByteArray buf, uint8_t thread);
    SPRWorkGistorgamm* addWorkGistogrammData(void *buf, int size, uint8_t _thread);


    SPRWorkSeparate2 *getWorkSeparateLast() const;

    SPRWorkSeparate2 *getWorkSeparatePrev() const;

    OneTimeGistorgamms getWorkGistogrammsLast() const;

    OneTimeGistorgamms getWorkGistogrammsPrev() const;

    SPRThreadList getWorkThreadList() const;

//    SPRSpectrumListItemsModel *getKSpectrums() const;

public slots:
    virtual void onModelChanget(IModelVariable *send);

};

#endif // SPRSEPARATEOUTPUTMODEL_H
