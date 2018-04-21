#ifndef SPRSETTINGSPOROGSMODEL_H
#define SPRSETTINGSPOROGSMODEL_H

#include <QObject>
#include "_types.h"
#include "models/sprporogsmodel.h"
#include "models/isprmodeldata.h"
#include "variables/sprvariable.h"
#include "variables/sprkprvariable.h"
#include "variables/sprenumvariable.h"
#include "variables/sprbooleanvariable.h"

class SPRSettingsPorogsModel : public ISPRModelData
{
    SPREnumVariable<TypeConditions> *conditions;
    SPRVariable<uint> *threads;
    SPRPorogsModel *porogs;
    SPRPorogsModel *porogs2;
    SPRVariable<uint> *tMeteringMinStone;
    SPRVariable<uint> *tMeteringMaxStone;

public:
    SPRSettingsPorogsModel(QObject *parent=nullptr);
    SPRSettingsPorogsModel(QDomDocument *_doc, ISPRModelData *parent = nullptr);
    virtual ~SPRSettingsPorogsModel();

    SPRKPRVariable *forMinStone;
    SPRKPRVariable *forMaxStone;
    SPRVariable<double> *xRayCorrection;
    SPREnumVariable<TypeSelection> *typeSelection;

    SPRBooleanVariable *invertSelection;
    SPRBooleanVariable *invertSelection2;

    SPREnumVariable<TypeConditions> *getConditions() const;
    void setConditions(SPREnumVariable<TypeConditions> *value);
    SPREnumVariable<TypeSelection> *getTypeSelection() const;
    void setSelection(SPREnumVariable<TypeSelection> *value);
    SPRPorogsModel *getPorogs() const;
    SPRPorogsModel *getPorogs2() const;
    SPRVariable<double> *getPorog(int thr, int condNum) const;
    SPRVariable<uint> *getThreads() const;
    void setThreads(SPRVariable<uint> *value);
    SPRVariable<uint> *getTMeteringMinStone() const;
    void setTMeteringMinStone(SPRVariable<uint> *value);
    SPRVariable<uint> *getTMeteringMaxStone() const;
    void setTMeteringMaxStone(SPRVariable<uint> *value);
};

#endif // SPRSETTINGSPOROGSMODEL_H
