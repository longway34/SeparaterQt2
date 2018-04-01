#include "sprsettingsmainmodel.h"

SPRSettingsMainModel::SPRSettingsMainModel(QObject *parent)
{
    name = nullptr;
    ipAddress = nullptr;
    ipPort = nullptr;
    rentgens = nullptr;
    setProperty("delete_rentgens", QVariant(false));
    ims = nullptr;
    threads = nullptr;
    setProperty("delete_threads", QVariant(false));
//    ims_row_count = nullptr;
    typePRAM = nullptr;
    typeThermo = nullptr;
    color = nullptr;
//    date = nullptr;

}

SPRSettingsMainModel::SPRSettingsMainModel(QDomDocument *_doc, ISPRModelData *parent):
    ISPRModelData(_doc, parent)
{
    name = new SPRQStringVariable(doc, SPR_SETTINGS_MAIN_NAME_XPATH, DEF_SPR_MAIN_NAME,this);
    ipAddress = new SPRQStringVariable(doc, SPR_SETTINGS_MAIN_ADDRESS_XPATH, DEF_SPR_MAIN_ADDRESS,this);
    ipPort = new SPRVariable<uint>(doc, SPR_SETTINGS_MAIN_PORT_XPATH, DEF_SPR_MAIN_PORT,this);
    rentgens = new SPRVariable<uint>(doc, SPR_SETTINGS_MAIN_RENTGENS_XPATH, DEF_SPR_MAIN_RENTGENS,this);
    setProperty("delete_rentgens", QVariant(true));
    threads = new SPRVariable<uint>(doc,SPR_SETTINGS_MAIN_THREADS_XPATH, DEF_SPR_MAIN_THREADS, this);
    setProperty("delete_threads", QVariant(true));
    ims = new SPRVariable<uint>(doc, SPR_SETTINGS_MAIN_IMS_XPATH, DEF_SPR_MAIN_IMS,this);
//    ims_row_count = new SPRVariable<uint>(doc, SPR_SETTINGS_MAIN_IMS_ROW_COUNT_XPATH, DEF_SPR_MAIN_IMS_ROW_COUNT);
    typePRAM = new SPRVariable<TypePRAM>(doc, SPR_SETTINGS_MAIN_TYPE_PRAM_XPATH, DEF_SPR_MAIN_TYPE_PRAM,this);
    typeThermo = new SPRVariable<TypeThermo>(doc, SPR_SETTINGS_MAIN_TYPE_THERMO_XPATH, DEF_SPR_MAIN_TYPE_THERMO,this);
    color = new SPRColorVariable(doc, SPR_SETTINGS_MAIN_COLOR_XPATH, DEF_SPR_MAIN_COLOR,this);
//    date = new SPRDateVariable(doc, SPR_SETTINGS_MAIN_DATE_XPATH, DEF_SPR_MAIN_DATE);
//    version = new SPRQStringVariable(doc, SPR_SETTINGS_MAIN_VERSION_XPATH, DEF_SPR_MAIN_VERSION);
    spectrumFileName = new SPRQStringVariable(doc, SPR_SETTINGS_MAIN_SPECTRUM_FNAME_XPATH, DEF_SPR_MAIN_SPECTRUM_FNAME, this);
}

SPRSettingsMainModel::~SPRSettingsMainModel()
{
    if(name) delete name; name = nullptr;
    if(ipAddress) delete ipAddress; ipAddress = nullptr;
    if(ipPort) delete ipPort; ipPort = nullptr;
    if(rentgens && QVariant(property("delete_rentgens")).toBool()) delete rentgens; rentgens = nullptr;
    if(ims) delete ims; ims = nullptr;
//    if(ims_row_count) delete ims_row_count; ims_row_count = nullptr;
    if(typePRAM) delete typePRAM; typePRAM = nullptr;
    if(typeThermo) delete typeThermo; typeThermo = nullptr;
    if(color) delete color; color = nullptr;
    if(threads && QVariant(property("delete_threads")).toBool()) delete threads; threads = nullptr;
    setProperty("delete_threads", QVariant(false));
//    if(date) delete date; date = nullptr;

}

void SPRSettingsMainModel::setName(SPRQStringVariable *value)
{
    name = value;
}

SPRVariable<uint> *SPRSettingsMainModel::getIpPort() const
{
    return ipPort;
}

void SPRSettingsMainModel::setIpPort(SPRVariable<uint> *value)
{
    ipPort = value;
}

SPRQStringVariable *SPRSettingsMainModel::getIpAddress() const
{
    return ipAddress;
}

void SPRSettingsMainModel::setIpAddress(SPRQStringVariable *value)
{
    ipAddress = value;
}

SPRQStringVariable *SPRSettingsMainModel::getName() const
{
    return name;
}

SPRQStringVariable *SPRSettingsMainModel::getSpectrumFileName() const
{
    return spectrumFileName;
}

SPRVariable<uint> *SPRSettingsMainModel::getThreads() const
{
    return threads;
}

SPRVariable<uint> *SPRSettingsMainModel::getIms() const
{
    return ims;
}

SPRVariable<uint> *SPRSettingsMainModel::getRentgens() const
{
    return rentgens;
}
