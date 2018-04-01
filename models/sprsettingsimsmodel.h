#ifndef SPRSETTINGSIMSMODEL_H
#define SPRSETTINGSIMSMODEL_H

#include <QObject>
#include <models/isprmodeldata.h>
#include <models/sprvariable.h>

class SPRSettingsIMSModel : public ISPRModelData
{
    SPRVariable<uint> *ims;
    struct lineFormula{
        double k;
        double b;
    };

public:
    SPRVariable<uint> *tMeteringMinStone;
    SPRVariable<uint> *tMeteringMaxStone;
    SPRVariable<uint> *tMeteringMaxMaxStone;

    SPRVariable<uint> *tDelayMinStone;
    SPRVariable<uint> *tDelayMaxStone;
    SPRVariable<uint> *tDelayMaxMaxStone;

    SPRVariable<uint> *tDurationMinStone;
    SPRVariable<uint> *tDurationMaxStone;
    SPRVariable<uint> *tDurationMaxMaxStone;

    SPRVariable<bool> *withMaxMaxStone;

    SPRVariable<uint> *blockImsParam;
    SPRVariable<uint> *blockImsParam2;
    SPRVariable<double> *kSpeedOreA;
    SPRVariable<double> *kSpeedOreB;

    QVector<SPRVariable<uint>*> timesMettering;

    QVector<SPRVariable<double>*> kKoeffDelay;
    QVector<SPRVariable<double>*> bKoeffDelay;
    QVector<SPRVariable<double>*> kKoeffDuration;
    QVector<SPRVariable<double>*> bKoeffDuration;

    SPRVariable<qulonglong> *limitAbrasion;
    QVector<SPRVariable<qulonglong>*> abrasions;


    SPRSettingsIMSModel();
    SPRSettingsIMSModel(QDomDocument *_doc, QObject *parent = nullptr);
    virtual ~SPRSettingsIMSModel();

    SPRVariable<uint> *getIms() const;
    void setIms(SPRVariable<uint> *value);

    struct lineFormula getLineKoeff(double x1, double y1, double x2, double y2){
// Ax + By + C = 0
// By = -Ax - C
// y = -A/Bx -C/B;

        double A = y1 - y2;
        double B = x2 - x1;
        double C = x1 * y2 - x2 * y1;

        struct lineFormula ret;
        ret.k = -A/B;
        ret.b = -C/B;

        return ret;
    }
    void setTimesMeassureDelayDuration(uint tmin = 0xFFFF, uint tmax = 0xFFFF, uint delayMin = 0xFFFF, uint delayMax = 0xFFFF, uint durationMin = 0xFFFF, uint durationMax = 0xFFFF){
        if(tmin == 0xFFFF) tmin = tMeteringMinStone->getData();
        if(tmax == 0xFFFF) tmax = tMeteringMaxStone->getData();
        if(delayMin == 0xFFFF) delayMin = tDelayMinStone->getData();
        if(delayMax == 0xFFFF) delayMax = tDelayMaxStone->getData();
        if(durationMin == 0xFFFF) durationMin = tDurationMinStone->getData();
        if(durationMax == 0xFFFF) durationMax = tDurationMaxStone->getData();

        double t_part = (double)(tmax - tmin) / (double)(DEF_SPR_IMS_PARTS_SIZE);
        for(int i=0; i<DEF_SPR_IMS_PARTS_SIZE - 1; i++){
            double d = t_part * (double)(i) + tmin;
            timesMettering[i]->setData(d);
        }
        double d = (double)(tmax * 2);
        timesMettering[DEF_SPR_IMS_PARTS_SIZE]->setData(d);

        struct lineFormula lfDelay = getLineKoeff(tMeteringMinStone->getData(),
                                                  tDelayMinStone->getData(),
                                                  tMeteringMaxStone->getData(),
                                                  tDelayMaxStone->getData());
        struct lineFormula lfDuration = getLineKoeff(tMeteringMinStone->getData(),
                                                  tDurationMinStone->getData(),
                                                  tMeteringMaxStone->getData(),
                                                  tDurationMaxStone->getData());
        for(int i=0; i<DEF_SPR_MAIN_IMS; i++){
           kKoeffDelay[i]->setData(lfDelay.k);
           bKoeffDelay[i]->setData(lfDelay.b);
           kKoeffDuration[i]->setData(lfDuration.k);
           bKoeffDuration[i]->setData(lfDuration.b);
        }
    }

    SPRVariable<uint> *getTMeteringMinStone() const;
    SPRVariable<uint> *getTMeteringMaxStone() const;
};

#endif // SPRSETTINGSIMSMODEL_H
