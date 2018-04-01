#include "sprsettingsspectrumrangesmodel.h"

void SPRSettingsSpectrumRangesModel::setThreads(SPRVariable<uint> *value)
{
    if(value){
        if(QVariant(property("delete_threads")).toBool()){
            if(threads) delete threads;
            setProperty("delete_threads", QVariant(false));
        }
        threads = value;
    }
}

SPRSettingsSpectrumRangesModel::SPRSettingsSpectrumRangesModel(QDomDocument *doc, ISPRModelData *parent):
    ISPRModelData(doc, parent), threads(nullptr), items(nullptr)
{
    items = (SPRSpectrumZonesModel**)malloc(sizeof(SPRSpectrumZonesModel*) * MAX_SPR_MAIN_THREADS);
    for(int i=0; i < MAX_SPR_MAIN_THREADS; i++){
        items[i] = new SPRSpectrumZonesModel(doc, i);
    }
    threads = new SPRVariable<uint>(doc,SPR_SETTINGS_MAIN_THREADS_XPATH, DEF_SPR_MAIN_THREADS, this);
    setProperty("delete_threads", QVariant(true));
}

SPRSettingsSpectrumRangesModel::~SPRSettingsSpectrumRangesModel()
{
    if(items){
        for(int i=0; i<MAX_SPR_MAIN_THREADS; i++){
            if(items[i]) delete items[i]; items[i] = nullptr;
        }
        delete items; items = nullptr;
    }
    if(threads && QVariant(property("delete_threads")).toBool()) delete threads; threads = nullptr;
    setProperty("delete_threads", QVariant(false));
}
