#include "sprspectrumzonestablemodel.h"

SPRVariable<uint> *SPRSpectrumZonesTableModel::getThreads() const
{
    return threads;
}

void SPRSpectrumZonesTableModel::setThreads(SPRVariable<uint> *value)
{
    if(value){
        if(QVariant(property("delete_threads")).toBool()){
            if(threads) delete threads;
            setProperty("delete_threads", QVariant(false));
        }
        threads = value;
    }
}

SPRSpectrumZonesTableModel::SPRSpectrumZonesTableModel()
{

}

SPRSpectrumZonesTableModel::SPRSpectrumZonesTableModel(QDomDocument *_doc, ISPRModelData *parent):
    ISPRModelData(_doc, parent), threads(nullptr), items()
{

    threads = new SPRVariable<uint>(doc,SPR_SETTINGS_MAIN_THREADS_XPATH, DEF_SPR_MAIN_THREADS, this);
    setProperty("delete_threads", QVariant(true));

    for(int i=0; i<MAX_SPR_MAIN_THREADS; i++){
        items.push_back(new SPRSpectrumZonesModel(doc, i));
    }
}

SPRSpectrumZonesTableModel::~SPRSpectrumZonesTableModel()
{
    if(threads && QVariant(property("delete_threads")).toBool()){
        delete threads; threads = nullptr;
        setProperty("delete_threads", QVariant(false));
    }
}
