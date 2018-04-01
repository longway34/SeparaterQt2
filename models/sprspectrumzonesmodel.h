#ifndef SPRSPECTRUMZONESMODEL_H
#define SPRSPECTRUMZONESMODEL_H

#include <QObject>
#include <QDomDocument>
#include "models/isprmodeldata.h"
#include "models/sprvariable.h"
#include "_types.h"

#define SPR_SETTINGS_SPECTRUM_RANGES_XPATH_PREFIX "SEPARATOR/SEPARATE_SETUP/OBL/CHANNEL"
#define SPR_SETTINGS_SPECTRUM_RANGES_XPATH_MIN_AGRUMENT "[Ls]"
#define SPR_SETTINGS_SPECTRUM_RANGES_XPATH_MAX_ARGUMENT "[Rs]"

typedef struct {
    SPRVariable<uint> *min;
    SPRVariable<uint> *max;
    QColor color;
    int elIndex;
} SpectorRange;

typedef QMap<EnumElements, SpectorRange> ElementsProperty;

class SPRSpectrumZonesModel : public ISPRModelData
{
public:
    int tIndex;
    ElementsProperty elements;

    SPRSpectrumZonesModel(){}
    SPRSpectrumZonesModel(QDomDocument *doc, int indexThread, const DefaultMapElements *_elProperty = nullptr);
    virtual ~SPRSpectrumZonesModel();
};

#endif // SPRSPECTRUMZONESMODEL_H
