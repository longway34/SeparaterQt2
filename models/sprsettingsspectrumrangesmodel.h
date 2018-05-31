#ifndef SPRSETTINGSSPECTRUMRANGESMODEL_H
#define SPRSETTINGSSPECTRUMRANGESMODEL_H

#include <QObject>
#include <QDomDocument>
#include "isprmodeldata.h"
#include "models/sprspectrumzonesmodel.h"
#include "variables/sprvariable.h"

class SPRSettingsSpectrumRangesModel : public ISPRModelData
{
    SPRElementsModel *elementsProperty;
public:
    SPRVariable<uint> *threads;
    SPRSpectrumZonesModel **items;

    SPRSettingsSpectrumRangesModel(){}
    SPRSettingsSpectrumRangesModel(QDomDocument *doc, SPRElementsModel* _elementsProperty = nullptr, ISPRModelData *parent = nullptr);
    virtual ~SPRSettingsSpectrumRangesModel();
    void setThreads(SPRVariable<uint> *value);
};

#endif // SPRSETTINGSSPECTRUMRANGESMODEL_H
