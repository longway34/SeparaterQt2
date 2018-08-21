#ifndef SPRSETTINGSCONTROLMODEL_H
#define SPRSETTINGSCONTROLMODEL_H

#include <QObject>
#include "models/isprmodeldata.h"
#include "variables/sprvariable.h"
#include "variables/sprenumvariable.h"
#include "variables/sprbooleanvariable.h"
#include "_types.h"

class SPRSettingsControlModel : public ISPRModelData
{
public:
    SPRVariable<double> *correlSpectrumPermiss; // корреляция спектра допустимо
    SPRVariable<double> *correlSpectrumCritical; // ... критично

    SPRVariable<uint> *speedStreamPermiss;  // скорость потока камней допустимо
    SPRVariable<uint> *speedStreamCritical; //  ... критично

    SPRVariable<uint> *diffCenterGravityPermiss; // отклонение центра тяжести допустимо
    SPRVariable<uint> *diffCenterGravityCritical;// ... критично

    SPRVariable<uint> *airLoadingMinPermiss; // загрузка по воздуху
    SPRVariable<uint> *airLoadingMaxPermiss;
    SPRVariable<uint> *airLoadingMinCritical;
    SPRVariable<uint> *airLoadingMaxCritical;

    SPREnumVariable<EnumElements> *controlArea;     // область контроля

    SPRVariable<uint> *weightAvgStoneTail;
    SPRVariable<uint> *weightAvgStoneConcentrate;

    SPRVariable<uint> *tMeassureForData;
    SPRVariable<uint> *tMeassureForSpectrum;
    SPRVariable<uint> *tMeassureForHistogramm;

    SPRVariable<double> *correctOptimalOreStream;

    SPRBooleanVariable *autoOreStreamControl;

    SPRVariable<uint16_t> *VEMSLevelLess;
    SPRVariable<uint16_t> *VEMSMaxCode;
    SPRVariable<uint16_t> *VEMSBeginCode;
;

    SPRSettingsControlModel(QObject *parent = nullptr):
        correlSpectrumPermiss(nullptr),
        correlSpectrumCritical(nullptr),
        speedStreamPermiss(nullptr),
        speedStreamCritical(nullptr),
        diffCenterGravityPermiss(nullptr),
        diffCenterGravityCritical(nullptr),
        airLoadingMinPermiss(nullptr),
        airLoadingMaxPermiss(nullptr),
        airLoadingMinCritical(nullptr),
        airLoadingMaxCritical(nullptr),
        controlArea(nullptr),
        weightAvgStoneTail(nullptr),
        weightAvgStoneConcentrate(nullptr),
        tMeassureForData(nullptr),
        tMeassureForSpectrum(nullptr),
        tMeassureForHistogramm(nullptr),
        correctOptimalOreStream(nullptr),
        autoOreStreamControl(nullptr),
        VEMSLevelLess(nullptr),
        VEMSMaxCode(nullptr)
    {}
    SPRSettingsControlModel(QDomDocument *_doc, ISPRModelData *parent = nullptr);
    virtual ~SPRSettingsControlModel();
};

#endif // SPRSETTINGSCONTROLMODEL_H
