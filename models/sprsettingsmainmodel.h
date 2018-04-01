#ifndef SPRSETTINGSMAINMODEL_H
#define SPRSETTINGSMAINMODEL_H

#include <QObject>
#include "_types.h"
#include "models/isprmodeldata.h"
#include "models/sprcolorvariable.h"
#include "models/sprdatevariable.h"
#include "models/sprqstringvariable.h"
#include "models/sprvariable.h"

class SPRSettingsMainModel : public ISPRModelData
{
public:
    SPRSettingsMainModel(QObject *parent=nullptr);
    SPRSettingsMainModel(QDomDocument *_doc, ISPRModelData *parent = nullptr);
    virtual ~SPRSettingsMainModel();

    SPRQStringVariable *name;
    SPRQStringVariable *spectrumFileName;
    SPRQStringVariable *ipAddress;
    SPRVariable<uint> *ipPort;
    SPRVariable<uint> *rentgens;
    SPRVariable<uint> *ims;
    SPRVariable<uint> *threads;
//    SPRVariable<uint> *ims_row_count;
    SPRVariable<TypePRAM> *typePRAM;
    SPRVariable<TypeThermo> *typeThermo;
    SPRColorVariable *color;

//    SPRDateVariable *date;
//    SPRQStringVariable *version;

    SPRVariable<uint> *getRentgens() const;
    SPRVariable<uint> *getThreads() const;
    SPRVariable<uint> *getIms() const;
    SPRQStringVariable *getSpectrumFileName() const;
    SPRQStringVariable *getName() const;
    SPRQStringVariable *getIpAddress() const;
    void setIpAddress(SPRQStringVariable *value);
    SPRVariable<uint> *getIpPort() const;
    void setIpPort(SPRVariable<uint> *value);
    void setName(SPRQStringVariable *value);
};

#endif // SPRSETTINGSMAINMODEL_H
