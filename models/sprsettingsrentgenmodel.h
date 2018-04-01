#ifndef SPRSETTINGSRENTGENMODEL_H
#define SPRSETTINGSRENTGENMODEL_H

#include <QObject>
#include "models/isprmodeldata.h"
#include "models/isprvariable.h"
#include "_types.h"
#include "models/sprvariable.h"

class SPRSettingsRentgenModel : public ISPRModelData
{
protected:
    SPRVariable<uint> *threads;
    SPRVariable<uint> *rentgens;
public:
    SPRVariable<uint> **iTubes;
    SPRVariable<uint> **uTubes;
    SPRVariable<uint> **deuCodes;
    SPRVariable<uint> **cpCodes;
    SPRVariable<uint> *timeOnRA;
    SPRVariable<uint> *timeOffRA;
    SPRVariable<uint> *timeHotRA;
    SPRVariable<uint> *diffParamAllow;
    SPRVariable<uint> *diffParamDisallow;
    SPRVariable<uint> *timeMoveRGU;
    SPRVariable<bool> *woControlPlace;
    SPRVariable<bool> **withChannel;
    SPRVariable<bool> *withRGU;

    SPRVariable<TypeAutoSettingRMT> *typeSetting;
    SPRVariable<uint> *codeBegin;
    SPRVariable<uint> *codeStep;
    SPRVariable<uint> *peakPosition;

    SPRSettingsRentgenModel(QObject *parent=nullptr);
    SPRSettingsRentgenModel(QDomDocument *_doc, ISPRModelData *parent = nullptr);
    virtual ~SPRSettingsRentgenModel();
    void setRentgens(SPRVariable<uint> *value);
    void setThreads(SPRVariable<uint> *value);
    SPRVariable<uint> *getThreads() const;
    SPRVariable<uint> *getRentgens() const;
};

#endif // SPRSETTINGSRENTGENMODEL_H
