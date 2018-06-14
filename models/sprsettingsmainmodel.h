#ifndef SPRSETTINGSMAINMODEL_H
#define SPRSETTINGSMAINMODEL_H

#include <QObject>
#include "_types.h"
#include "models/isprmodeldata.h"
#include "variables/sprqcolorvariable.h"
#include "variables/sprdatetimevariable.h"
#include "variables/sprqstringvariable.h"
#include "variables/sprenumvariable.h"
#include "variables/sprvariable.h"
#include "tcp/ServerConnect.h"
#include "tcp/TCPCommand.h"

class SPRSettingsMainModel : public ISPRModelData
{
    ServerConnect *server;
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
    SPREnumVariable<TypePRAM> *typePRAM;
    SPREnumVariable<TypeThermo> *typeThermo;
    SPRQColorVariable *color;

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
    ServerConnect *getServer() const;
    void setServer(ServerConnect *value);
};

#endif // SPRSETTINGSMAINMODEL_H
