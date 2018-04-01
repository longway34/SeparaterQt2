#ifndef SPRPOROGSMODEL_H
#define SPRPOROGSMODEL_H

#include <QObject>
#include "_types.h"
#include "models/isprmodeldata.h"
#include "models/sprvariable.h"

class SPRPorogsModel : public ISPRModelData
{
protected:
    SPRVariable<TypeConditions> *conditions;
    SPRVariable<uint> *threads;
    uint row;
public:
    SPRVariable<double> ***porogs;

    SPRPorogsModel(QObject *parent);
    SPRPorogsModel(QDomDocument *_doc, uint _row, ISPRModelData *parent = nullptr);
    virtual ~SPRPorogsModel();
    void setThreads(SPRVariable<uint> *value);
    void setConditions(SPRVariable<TypeConditions> *value);
    SPRVariable<TypeConditions> *getConditions() const;
    SPRVariable<uint> *getThreads() const;
};

#endif // SPRPOROGSMODEL_H
