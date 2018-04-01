#ifndef SPRSETTINGSSPECTRUMRANGESMODEL_H
#define SPRSETTINGSSPECTRUMRANGESMODEL_H

#include <QObject>
#include <QDomDocument>
#include "isprmodeldata.h"
#include "models/sprspectrumzonesmodel.h"
#include "sprvariable.h"

class SPRSettingsSpectrumRangesModel : public ISPRModelData
{
public:
    SPRVariable<uint> *threads;
    SPRSpectrumZonesModel **items;

    SPRSettingsSpectrumRangesModel(){}
    SPRSettingsSpectrumRangesModel(QDomDocument *doc, ISPRModelData *parent);
    virtual ~SPRSettingsSpectrumRangesModel();
    void setThreads(SPRVariable<uint> *value);
};

#endif // SPRSETTINGSSPECTRUMRANGESMODEL_H
