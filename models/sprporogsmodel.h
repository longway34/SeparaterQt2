#ifndef SPRPOROGSMODEL_H
#define SPRPOROGSMODEL_H

#include <QObject>
#include "_types.h"
#include "models/isprmodeldata.h"
#include "variables/sprvariable.h"
#include "variables/sprenumvariable.h"

class SPRPorogsModel : public ISPRModelData
{
protected:
    SPREnumVariable<TypeConditions> *conditions;
    SPRVariable<uint> *threads;
    uint row;
public:
    SPRVariable<double> ***porogs;
    QList<IModelVariable*> allPorogs;

    SPRPorogsModel(QObject *parent);
    SPRPorogsModel(QDomDocument *_doc, uint _row, ISPRModelData *parent = nullptr);
    virtual ~SPRPorogsModel();
    void setThreads(SPRVariable<uint> *value);
    void setConditions(SPREnumVariable<TypeConditions> *value);
    SPREnumVariable<TypeConditions> *getConditions() const;
    SPRVariable<uint> *getThreads() const;
    QList<IModelVariable *> getAllPorogs() const;
};

#endif // SPRPOROGSMODEL_H
