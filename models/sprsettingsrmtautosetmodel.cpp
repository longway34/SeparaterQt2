#include "sprsettingsrmtautosetmodel.h"


void SPRSettingsRMTAutosetModel::setThreads(SPRVariable<uint> *value)
{
    if(value){
        if(QVariant(property("delete_threads")).toBool()){
            if(threads) delete threads;
            setProperty("delete_threads", QVariant(false));
        }
        threads = value;
    }
}

SPRSettingsRMTAutosetModel::SPRSettingsRMTAutosetModel(QDomDocument *_doc, QObject *parent):
    ISPRModelData(_doc)
{
    typeSetting = new SPRVariable<TypeAutoSettingRMT>(doc, SPR_AUTOSETTINGS_TYPE_SETTING_XPATH, DEF_SPR_AUTOSETTINGS_TYPE_SETTING, this);
    codeBegin = new SPRVariable<uint>(doc,SPR_AUTOSETTINGS_CODE_BEGIN_XPATH, DEF_SPR_AUTOSETTINGS_CODE_BEGIN, this);
    codeStep = new SPRVariable<uint>(doc, SPR_AUTOSETTINGS_CODE_STEP_XPATH, DEF_SPR_AUTOSETTINGS_CODE_STEP, this);
    withRGU = new SPRVariable<bool>(doc, SPR_AUTOSETTINGS_WITH_RGU_XPATH, DEF_SPR_AUTOSETTINGS_WITH_RGU, this);
    peakPosition = new SPRVariable<uint>(doc, SPR_AUTOSETTINGS_PEAC_POSITION_XPATH, DEF_SPR_AUTOSETTINGS_PEAC_POSITION, this);

    QVector<QString> xpaths = {SPR_AUTUSETTINGS_WITH_CHANNEL_0, SPR_AUTUSETTINGS_WITH_CHANNEL_1, SPR_AUTUSETTINGS_WITH_CHANNEL_2, SPR_AUTUSETTINGS_WITH_CHANNEL_3,
                               SPR_AUTUSETTINGS_WITH_CHANNEL_4, SPR_AUTUSETTINGS_WITH_CHANNEL_5, SPR_AUTUSETTINGS_WITH_CHANNEL_6, SPR_AUTUSETTINGS_WITH_CHANNEL_7};

    withChannel = (SPRVariable<bool>**)malloc(sizeof(SPRVariable<bool>*) * MAX_SPR_MAIN_THREADS);
    for(int i=0; i<MAX_SPR_MAIN_THREADS; i++){
        withChannel[i] = new SPRVariable<bool>(doc, xpaths[i], DEF_SPR_AUTUSETTINGS_WITH_CHANNEL, this);
    }
    threads = new SPRVariable<uint>(doc, SPR_SETTINGS_MAIN_THREADS_XPATH, DEF_SPR_MAIN_THREADS, this);
    setProperty("delete_threads", QVariant(true));
}

SPRSettingsRMTAutosetModel::~SPRSettingsRMTAutosetModel(){
    if(codeBegin) delete codeBegin; codeBegin = nullptr;
    if(codeStep) delete codeStep; codeStep = nullptr;
    //        if(enhansed) delete enhansed; enhansed = nullptr;
    if(withRGU) delete withRGU; withRGU = nullptr;
    if(peakPosition) delete peakPosition; peakPosition = nullptr;
    if(threads && QVariant(property("delete_threads")).toBool()) delete threads; threads = nullptr;
    setProperty("delete_threads", QVariant(false));

    if(withChannel){
        for(int i=0; i< MAX_SPR_MAIN_THREADS; i++){
            if(withChannel[i]) delete withChannel[i]; withChannel[i] = nullptr;
        }
        delete withChannel; withChannel = nullptr;
    }

}
