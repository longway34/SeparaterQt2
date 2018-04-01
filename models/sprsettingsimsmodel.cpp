#include "sprsettingsimsmodel.h"


SPRVariable<uint> *SPRSettingsIMSModel::getIms() const
{
    return ims;
}

void SPRSettingsIMSModel::setIms(SPRVariable<uint> *value)
{
    if(value){
        ims = value;
        setProperty("delete_ims", QVariant(false));
    }
}
SPRVariable<uint> *SPRSettingsIMSModel::getTMeteringMinStone() const
{
    return tMeteringMinStone;
}

SPRVariable<uint> *SPRSettingsIMSModel::getTMeteringMaxStone() const
{
    return tMeteringMaxStone;
}

SPRSettingsIMSModel::SPRSettingsIMSModel(){
    ims = nullptr; setProperty("delete_ims", QVariant(false));
    tMeteringMinStone = nullptr;
    tMeteringMaxStone =nullptr;
    tMeteringMaxMaxStone =nullptr;
    tDurationMinStone = nullptr;
    tDurationMaxStone = nullptr;
    tDurationMaxMaxStone = nullptr;
    tDelayMinStone = nullptr;
    tDelayMaxStone = nullptr;
    tDelayMaxMaxStone = nullptr;

    kSpeedOreA = nullptr;
    kSpeedOreB = nullptr;

    limitAbrasion = nullptr;
    blockImsParam = nullptr;
    blockImsParam2 = nullptr;

    withMaxMaxStone = nullptr;
}

SPRSettingsIMSModel::SPRSettingsIMSModel(QDomDocument *_doc, QObject *parent):
    ISPRModelData(_doc)
{
    ims = new SPRVariable<uint>(doc, SPR_SETTINGS_MAIN_IMS_XPATH, DEF_SPR_MAIN_IMS, this);
    setProperty("delete_ims", QVariant(true));

    tMeteringMinStone = new SPRVariable<uint>(doc, SPR_IMS_T_METTERING_MIN_STONE_XPATH, DEF_SPR_IMS_T_METTERING_MIN_STONE, this);
    tMeteringMaxStone = new SPRVariable<uint>(doc, SPR_IMS_T_METTERING_MAX_STONE_XPATH, DEF_SPR_IMS_T_METTERING_MAX_STONE, this);
    tMeteringMaxMaxStone = new SPRVariable<uint>(doc, SPR_IMS_T_METTERING_MAX_MAX_STONE_XPATH, DEF_SPR_IMS_T_METTERING_MAX_NAX_STONE, this);

    for(int i=0; i<DEF_SPR_IMS_PARTS_SIZE+1; i++){
        QString xpath = SPR_IMS_TIMES_METTERING_XPATH_PREFIX + QString::number(i)+"]";
        SPRVariable<uint> *var = new SPRVariable<uint>(doc, xpath, 0, this);
        timesMettering.push_back(var);
    }

    tDelayMinStone = new SPRVariable<uint>(doc, SPR_IMS_T_DELAY_MIN_STONE_XPATH, DEF_SPR_IMS_T_DELAY_MIN_STONE, this);
    tDelayMaxStone = new SPRVariable<uint>(doc, SPR_IMS_T_DELAY_MAX_STONE_XPATH, DEF_SPR_IMS_T_DELAY_MAX_STONE, this);
    tDelayMaxMaxStone = new SPRVariable<uint>(doc, SPR_IMS_T_DELAY_MAX_MAX_STONE_XPATH, DEF_SPR_IMS_T_DELAY_MAX_MAX_STONE, this);

    tDurationMinStone = new SPRVariable<uint>(doc, SPR_IMS_T_DURATION_MIN_STONE_XPATH, DEF_SPR_IMS_T_DURATION_MIN_STONE, this);
    tDurationMaxStone = new SPRVariable<uint>(doc, SPR_IMS_T_DURATION_MAX_STONE_XPATH, DEF_SPR_IMS_T_DURATION_MAX_STONE, this);
    tDurationMaxMaxStone = new SPRVariable<uint>(doc, SPR_IMS_T_DURATION_MAX_MAX_STONE_XPATH, DEF_SPR_IMS_T_DURATION_MAX_MAX_STONE, this);


    blockImsParam = new SPRVariable<uint>(doc, SPR_IMS_BLOCK_PARAM_XPATH, DEF_SPR_IMS_BLOCK_PARAM, this);
    blockImsParam2 = new SPRVariable<uint>(doc, SPR_IMS_BLOCK_PARAM2_XPATH, DEF_SPR_IMS_BLOCK_PARAM, this);
    kSpeedOreA = new SPRVariable<double>(doc, SPR_IMS_SPEAD_ORE_A, DEF_SPR_IMS_SPEAD_ORE_A, this);
    kSpeedOreB = new SPRVariable<double>(doc, SPR_IMS_SPEAD_ORE_B, DEF_SPR_IMS_SPEAD_ORE_B, this);
    limitAbrasion = new SPRVariable<qulonglong>(doc, SPR_IMS_LIMIT_ABRASION_XPATH, DEF_SPR_IMS_LIMIT_ABRASION, this);

    withMaxMaxStone = new SPRVariable<bool>(doc, SPR_IMS_WITH_MAX_STONE, DEF_SPR_IMS_WITH_MAX_STONE, this);

//    struct lineFormula lfDelay = getLineKoeff(timesMettering[0]->getData(), tDelayMinStone->getData(), timesMettering[timesMettering.size() - 1]->getData(), tDelayMaxStone->getData());
//    struct lineFormula lfDuration = getLineKoeff(timesMettering[0]->getData(), tDurationMinStone->getData(), timesMettering[timesMettering.size() - 1]->getData(), tDurationMaxStone->getData());

    for(int i=0; i<MAX_SPR_MAIN_IMS; i++){
        QString xpath = SPR_IMS_ABRASIONS_XPATH_PREFIX+QString::number(i)+"]";
        SPRVariable<qulonglong> *var = new SPRVariable<qulonglong>(doc, xpath, DEF_SPR_IMS_ABRASION, this);
        abrasions.push_back(var);
    }
    for(int i=0; i<DEF_SPR_MAIN_IMS; i++){
        QString xpath = SPR_IMS_K_DELAY_XPATH_PREFIX + QString::number(i)+"]";
        SPRVariable<double> *dvar = new SPRVariable<double>(doc, xpath, 1, this);
        kKoeffDelay.push_back(dvar);

        xpath = SPR_IMS_B_DELAY_XPATH_PREFIX + QString::number(i)+"]";
        dvar = new SPRVariable<double>(doc, xpath, 0, this);
        bKoeffDelay.push_back(dvar);

        xpath = SPR_IMS_K_DURATION_XPATH_PREFIX + QString::number(i)+"]";
        dvar = new SPRVariable<double>(doc, xpath, 1, this);
        kKoeffDuration.push_back(dvar);

        xpath = SPR_IMS_B_DURATION_XPATH_PREFIX + QString::number(i)+"]";
        dvar = new SPRVariable<double>(doc, xpath, 0, this);
        bKoeffDuration.push_back(dvar);

    }
    setTimesMeassureDelayDuration();

}

SPRSettingsIMSModel::~SPRSettingsIMSModel()
{
    if(ims && QVariant(property("delete_ims")).toBool()){
        delete ims; ims = nullptr;
    }

    if(tMeteringMinStone) delete tMeteringMinStone; tMeteringMinStone = nullptr;
    if(tMeteringMaxStone) delete tMeteringMaxStone; tMeteringMaxStone = nullptr;

    if(tDelayMinStone) delete tDelayMinStone; tDelayMinStone = nullptr;
    if(tDelayMaxStone) delete tDelayMaxStone; tDelayMaxStone = nullptr;

    if(tDurationMinStone) delete tDurationMinStone; tDurationMinStone = nullptr;
    if(tDurationMaxStone) delete tDurationMaxStone; tDurationMaxStone = nullptr;

    if(kSpeedOreA) delete kSpeedOreA; kSpeedOreA = nullptr;
    if(kSpeedOreB) delete kSpeedOreB; kSpeedOreB = nullptr;

    if(blockImsParam) delete blockImsParam; blockImsParam = nullptr;
    if(limitAbrasion) delete limitAbrasion; limitAbrasion = nullptr;

    if(withMaxMaxStone) delete withMaxMaxStone; withMaxMaxStone = nullptr;

    for(int i=0; i<abrasions.size(); i++){
        if(abrasions[i]) delete abrasions[i]; abrasions[i] = nullptr;
    }
    for(int i=0; i<timesMettering.size(); i++){
        delete timesMettering[i]; timesMettering[i] = nullptr;
    }
    for(int i=0; i<kKoeffDelay.size(); i++){
        delete kKoeffDelay[i]; kKoeffDelay[i] = nullptr;
    }
    for(int i=0; i<bKoeffDelay.size(); i++){
        delete bKoeffDelay[i]; bKoeffDelay[i] = nullptr;
    }
    for(int i=0; i<kKoeffDuration.size(); i++){
        delete kKoeffDuration[i]; kKoeffDuration[i] = nullptr;
    }
    for(int i=0; i<bKoeffDuration.size(); i++){
        delete bKoeffDuration[i]; bKoeffDuration[i] = nullptr;
    }
}
