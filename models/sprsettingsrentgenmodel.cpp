#include "sprsettingsrentgenmodel.h"

void SPRSettingsRentgenModel::setRentgens(SPRVariable<uint> *value)
{
    if(value){
        if(QVariant(property("delete_rentgens")).toBool()){
            if(rentgens) delete rentgens;
            setProperty("delete_rentgens", QVariant(false));
        }
        rentgens = value;
    }
}

void SPRSettingsRentgenModel::setThreads(SPRVariable<uint> *value)
{
    if(value){
        if(QVariant(property("delete_threads")).toBool()){
            if(threads) delete threads;
            setProperty("delete_threads", QVariant(false));
        }
        threads = value;
    }
}

SPRVariable<uint> *SPRSettingsRentgenModel::getThreads() const
{
    return threads;
}

SPRVariable<uint> *SPRSettingsRentgenModel::getRentgens() const
{
    return rentgens;
}

SPRSettingsRentgenModel::SPRSettingsRentgenModel(QObject *parent)
{
    iTubes = nullptr;
    uTubes = nullptr;
    deuCodes = nullptr;
    cpCodes = nullptr;
    timeOnRA = nullptr;
    timeOffRA = nullptr;
    timeHotRA = nullptr;
    diffParamAllow = nullptr;
    diffParamDisallow = nullptr;
    timeMoveRGU = nullptr;
    woControlPlace = nullptr;
    threads = nullptr;
    setProperty("delete_threads", QVariant(false));
    rentgens = nullptr;
    setProperty("delete_rentgens", QVariant(false));
    withChannel = nullptr;
    withRGU = nullptr;
    typeSetting = nullptr;
    codeBegin = nullptr;
    codeStep = nullptr;

    peakPosition = nullptr;
}

SPRSettingsRentgenModel::SPRSettingsRentgenModel(QDomDocument *_doc, ISPRModelData *parent):
    ISPRModelData(_doc, parent)
{
    iTubes = (SPRVariable<uint>**) malloc(sizeof(SPRVariable<uint>*) * MAX_SPR_MAIN_RENTGENS);
    uTubes = (SPRVariable<uint>**) malloc(sizeof(SPRVariable<uint>*) * MAX_SPR_MAIN_RENTGENS);
    deuCodes = (SPRVariable<uint>**) malloc(sizeof(SPRVariable<uint>*) * MAX_SPR_MAIN_THREADS);
    cpCodes = (SPRVariable<uint>**) malloc(sizeof(SPRVariable<uint>*) * MAX_SPR_MAIN_THREADS);

    iTubes[0] = new SPRVariable<uint>(doc, SPR_RENTGEN_I_TUBE_0_XPATH, DEF_SPR_RENTGEN_I_TUBE,this);
    iTubes[1] = new SPRVariable<uint>(doc, SPR_RENTGEN_I_TUBE_1_XPATH, DEF_SPR_RENTGEN_I_TUBE,this);
    uTubes[0] = new SPRVariable<uint>(doc, SPR_RENTGEN_U_TUBE_0_XPATH, DEF_SPR_RENTGEN_U_TUBE,this);
    uTubes[1] = new SPRVariable<uint>(doc, SPR_RENTGEN_U_TUBE_1_XPATH, DEF_SPR_RENTGEN_U_TUBE,this);

    deuCodes[0] = new SPRVariable<uint>(doc, SPR_RENTGEN_DEU_CODE_0_XPATH, DEF_SPR_RENTGEN_DEU_CODE,this);
    deuCodes[1] = new SPRVariable<uint>(doc, SPR_RENTGEN_DEU_CODE_1_XPATH, DEF_SPR_RENTGEN_DEU_CODE,this);
    deuCodes[2] = new SPRVariable<uint>(doc, SPR_RENTGEN_DEU_CODE_2_XPATH, DEF_SPR_RENTGEN_DEU_CODE,this);
    deuCodes[3] = new SPRVariable<uint>(doc, SPR_RENTGEN_DEU_CODE_3_XPATH, DEF_SPR_RENTGEN_DEU_CODE,this);
    cpCodes[0] = new SPRVariable<uint>(doc, SPR_RENTGEN_CP_CODE_0_XPATH, DEF_SPR_RENTGEN_CP_CODE,this);
    cpCodes[1] = new SPRVariable<uint>(doc, SPR_RENTGEN_CP_CODE_1_XPATH, DEF_SPR_RENTGEN_CP_CODE,this);
    cpCodes[2] = new SPRVariable<uint>(doc, SPR_RENTGEN_CP_CODE_2_XPATH, DEF_SPR_RENTGEN_CP_CODE,this);
    cpCodes[3] = new SPRVariable<uint>(doc, SPR_RENTGEN_CP_CODE_3_XPATH, DEF_SPR_RENTGEN_CP_CODE,this);

    timeOnRA = new SPRVariable<uint>(doc, SPR_RENTGEN_TIME_ON_RA_XPATH, DEF_SPR_RENTGEN_TIME_ON_RA,this);
    timeOffRA = new SPRVariable<uint>(doc, SPR_RENTGEN_TIME_OFF_RA_XPATH, DEF_SPR_RENTGEN_TIME_OFF_RA,this);
    timeHotRA = new SPRVariable<uint>(doc, SPR_RENTGEN_TIME_HOT_RA_XPATH, DEF_SPR_RENTGEN_TIME_HOT_RA,this);

    diffParamAllow = new SPRVariable<uint>(doc, SPR_RENTGEN_DIFF_PARAM_ALLOW, DEF_SPR_RENTGEN_DIFF_PARAM_ALLOW,this);
    diffParamDisallow = new SPRVariable<uint>(doc, SPR_RENTGEN_DIFF_PARAM_DISALLOW, DEF_SPR_RENTGEN_DIFF_PARAM_DISALLOW,this);

    woControlPlace = new SPRVariable<bool>(doc, SPR_RENTGEN_WO_CONTROL_PLACE, DEF_SPR_RENTGEN_WO_CONTROL_PLACE,this);
    timeMoveRGU = new SPRVariable<uint>(doc, SPR_RENTGEN_TIME_MOVE_RGU, DEF_SPR_RENTGEN_TIME_MOVE_RGU,this);

    threads  = new SPRVariable<uint>(doc, SPR_SETTINGS_MAIN_THREADS_XPATH, DEF_SPR_MAIN_THREADS, this);
    setProperty("delete_threads", QVariant(true));
    rentgens = new SPRVariable<uint>(doc,SPR_SETTINGS_MAIN_RENTGENS_XPATH, DEF_SPR_MAIN_RENTGENS, this);
    setProperty("delete_rentgens", QVariant(true));

    QVector<QString> xpaths = {SPR_AUTUSETTINGS_WITH_CHANNEL_0, SPR_AUTUSETTINGS_WITH_CHANNEL_1, SPR_AUTUSETTINGS_WITH_CHANNEL_2, SPR_AUTUSETTINGS_WITH_CHANNEL_3,
                               SPR_AUTUSETTINGS_WITH_CHANNEL_4, SPR_AUTUSETTINGS_WITH_CHANNEL_5, SPR_AUTUSETTINGS_WITH_CHANNEL_6, SPR_AUTUSETTINGS_WITH_CHANNEL_7};

    withChannel = (SPRVariable<bool>**)malloc(sizeof(SPRVariable<bool>*) * MAX_SPR_MAIN_THREADS);
    for(int i=0; i<MAX_SPR_MAIN_THREADS; i++){
        withChannel[i] = new SPRVariable<bool>(doc, xpaths[i], DEF_SPR_AUTUSETTINGS_WITH_CHANNEL, this);
    }
    withRGU = new SPRVariable<bool>(doc, SPR_AUTOSETTINGS_WITH_RGU_XPATH, DEF_SPR_AUTOSETTINGS_WITH_RGU, this);
    typeSetting = new SPRVariable<TypeAutoSettingRMT>(doc, SPR_AUTOSETTINGS_TYPE_SETTING_XPATH, DEF_SPR_AUTOSETTINGS_TYPE_SETTING, this);
    codeBegin = new SPRVariable<uint>(doc,SPR_AUTOSETTINGS_CODE_BEGIN_XPATH, DEF_SPR_AUTOSETTINGS_CODE_BEGIN, this);
    codeStep = new SPRVariable<uint>(doc, SPR_AUTOSETTINGS_CODE_STEP_XPATH, DEF_SPR_AUTOSETTINGS_CODE_STEP, this);

    peakPosition = new SPRVariable<uint>(doc, SPR_AUTOSETTINGS_PEAC_POSITION_XPATH, DEF_SPR_AUTOSETTINGS_PEAC_POSITION, this);
}

SPRSettingsRentgenModel::~SPRSettingsRentgenModel()
{
    if(iTubes){
        for(int i=0; i < MAX_SPR_MAIN_RENTGENS; i++){
            if(iTubes[i]) delete iTubes[i]; iTubes[i] = nullptr;
        }
        delete iTubes; iTubes = nullptr;
    }
    if(uTubes){
        for(int i=0; i < MAX_SPR_MAIN_RENTGENS; i++){
            if(uTubes[i]) delete uTubes[i]; uTubes[i] = nullptr;
        }
        delete uTubes; uTubes = nullptr;
    }
    if(deuCodes){
        for(int i=0; i< MAX_SPR_MAIN_THREADS; i++){
            if(deuCodes[i]) delete deuCodes[i]; deuCodes[i] = nullptr;
        }
        delete deuCodes;
    }
    if(cpCodes){
        for(int i=0; i< MAX_SPR_MAIN_THREADS; i++){
            if(cpCodes[i]) delete cpCodes[i]; cpCodes[i] = nullptr;
        }
        delete cpCodes;
    }

    if(timeOnRA) delete timeOnRA; timeOnRA = nullptr;
    if(timeOffRA) delete timeOffRA; timeOffRA = nullptr;
    if(timeHotRA) delete timeHotRA; timeHotRA = nullptr;

    if(diffParamAllow) delete diffParamAllow; diffParamAllow = nullptr;
    if(diffParamDisallow) delete diffParamDisallow; diffParamDisallow = nullptr;

    if(timeMoveRGU) delete timeMoveRGU; timeMoveRGU = nullptr;
    if(woControlPlace) delete woControlPlace; woControlPlace = nullptr;
    if(threads && QVariant(property("delete_threads")).toBool()) delete threads; threads = nullptr;
    setProperty("delete_threads", QVariant(false));
    if(rentgens && QVariant(property("delete_rentgens")).toBool()) delete rentgens; rentgens = nullptr;
    setProperty("delete_rentgens", QVariant(false));

    if(withChannel){
        for(int i=0; i<MAX_SPR_MAIN_THREADS; i++){
            if(withChannel[i]) delete withChannel[i]; withChannel[i] = nullptr;
        }
        delete withChannel; withChannel = nullptr;
    }

    if(withRGU) delete withRGU; withRGU = nullptr;

    if(typeSetting) delete typeSetting; typeSetting = nullptr;
    if(codeBegin) delete codeBegin; codeBegin = nullptr;
    if(codeStep) delete codeStep; codeStep = nullptr;
    if(peakPosition) delete peakPosition; peakPosition = nullptr;
}
