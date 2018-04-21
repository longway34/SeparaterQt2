#ifndef SPRSETTINGSRMTAUTOSETMODEL_H
#define SPRSETTINGSRMTAUTOSETMODEL_H

#include <QObject>
#include "models/isprmodeldata.h"
#include "variables/sprvariable.h"
#include "variables/sprbooleanvariable.h"
#include "variables/sprenumvariable.h"

class SPRSettingsRMTAutosetModel : public ISPRModelData
{
protected:
    SPRVariable<uint> *threads;
public:
    SPREnumVariable<TypeAutoSettingRMT> *typeSetting;
    SPRVariable<uint> *codeBegin;
    SPRVariable<uint> *codeStep;
//    SPRVariable<bool> *enhansed;
    SPRBooleanVariable *withRGU;
    SPRBooleanVariable **withChannel;
    SPRVariable<uint> *peakPosition;


    SPRSettingsRMTAutosetModel(QObject *parent = nullptr):
        codeBegin(nullptr), codeStep(nullptr), /*enhansed(nullptr),*/ withRGU(nullptr),
        withChannel(nullptr), peakPosition(nullptr), threads(nullptr){}

    SPRSettingsRMTAutosetModel(QDomDocument *_doc, QObject *parent = nullptr);
    virtual ~SPRSettingsRMTAutosetModel();
    void setThreads(SPRVariable<uint> *value);
};

#endif // SPRSETTINGSRMTAUTOSETMODEL_H
