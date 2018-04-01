#include "sprsettingsporogsmodel.h"

SPRVariable<TypeSelection> *SPRSettingsPorogsModel::getTypeSelection() const
{
    return typeSelection;
}

void SPRSettingsPorogsModel::setSelection(SPRVariable<TypeSelection> *value)
{
    setProperty("delete_selections", QVariant(false));
    typeSelection = value;
}

SPRPorogsModel *SPRSettingsPorogsModel::getPorogs() const
{
    return porogs;
}

SPRVariable<uint> *SPRSettingsPorogsModel::getThreads() const
{
    return threads;
}

void SPRSettingsPorogsModel::setThreads(SPRVariable<uint> *value)
{
    if(value){
        threads = value;
        setProperty("delete_threads", QVariant(false));
        if(porogs){
            porogs->setThreads(threads);
        }
    }
}

SPRVariable<double> *SPRSettingsPorogsModel::getPorog(int thr, int condNum) const
{
    if(porogs && thr < MAX_SPR_MAIN_THREADS && condNum < MAX_SPR_FORMULA_CONDITION){
        return porogs->porogs[thr][condNum];
    } else {
        return nullptr;
    }
}

SPRVariable<uint> *SPRSettingsPorogsModel::getTMeteringMinStone() const
{
    return tMeteringMinStone;
}

void SPRSettingsPorogsModel::setTMeteringMinStone(SPRVariable<uint> *value)
{
    tMeteringMinStone = value;
}

SPRVariable<uint> *SPRSettingsPorogsModel::getTMeteringMaxStone() const
{
    return tMeteringMaxStone;
}

void SPRSettingsPorogsModel::setTMeteringMaxStone(SPRVariable<uint> *value)
{
    tMeteringMaxStone = value;
}

SPRPorogsModel *SPRSettingsPorogsModel::getPorogs2() const
{
    return porogs2;
}

SPRSettingsPorogsModel::SPRSettingsPorogsModel(QObject *parent)
{
    typeSelection = nullptr;
    setProperty("delete_selection", QVariant(false));
    forMinStone = nullptr;
    forMaxStone = nullptr;
    xRayCorrection = nullptr;
    porogs = nullptr;
    invertSelection = nullptr;
    porogs2 = nullptr;
    invertSelection2 = nullptr;
    threads = nullptr;
    setProperty("delete_threads", QVariant(false));

    conditions = nullptr;
    setProperty("delete_conditions", QVariant(false));
}

SPRSettingsPorogsModel::SPRSettingsPorogsModel(QDomDocument *_doc, ISPRModelData *parent):
    ISPRModelData(_doc, parent)
{
    typeSelection = new SPRVariable<TypeSelection>(doc,SPR_POROGS_TYPE_SELECTION_XPATH, DEF_SPR_POROGS_TYPE_SELECTION, this);
    setProperty("delete_selection", QVariant(false));
    forMinStone = new SPRKPRVariable(doc, SPR_POROGS_FOR_MIN_STONE_XPATH, DEF_SPR_POROGS_FOR_MIN_STONE, this);
    forMaxStone = new SPRKPRVariable(doc, SPR_POROGS_FOR_MAX_STONE_XPATH, DEF_SPR_POROGS_FOR_MAX_STONE, this);
    xRayCorrection = new SPRVariable<double>(doc, SPR_POROGS_X_RAY_CORRECTION_XPATH, DEF_SPR_POROGS_X_RAY_CORRECTION, this);
    conditions = new SPRVariable<TypeConditions>(doc, SPR_POROGS_CONDITION_XPATH, DEF_SPR_FORMULA_CONDITION, this);
    threads = new SPRVariable<uint>(doc, SPR_SETTINGS_MAIN_THREADS_XPATH, DEF_SPR_MAIN_THREADS, this);
    setProperty("delete_threads", QVariant(true));
    porogs = new SPRPorogsModel(_doc, 0, parent);
    porogs->setConditions(conditions);
    porogs->setThreads(threads);
    invertSelection = new SPRVariable<bool>(doc, SPR_POROGS_INVERT_SELECTION, DEF_SPR_POROGS_INVERT_SELECTION, this);

    porogs2 = new SPRPorogsModel(_doc, 2, parent);
    porogs2->setConditions(conditions);
    porogs2->setThreads(threads);
    invertSelection2 = new SPRVariable<bool>(doc, SPR_POROGS_INVERT_SELECTION2, DEF_SPR_POROGS_INVERT_SELECTION, this);
}

SPRSettingsPorogsModel::~SPRSettingsPorogsModel()
{
    if(typeSelection && QVariant(property("delete_selection")).toBool()) delete typeSelection; typeSelection = nullptr;
    setProperty("delete_selection", QVariant(false));
    
    if(forMinStone) delete forMinStone; forMinStone = nullptr;
    if(forMaxStone) delete forMaxStone; forMaxStone = nullptr;
    if(xRayCorrection) delete xRayCorrection; xRayCorrection = nullptr;

    if(conditions && QVariant(property(("delete_conditions"))).toBool()){
        delete conditions; conditions = nullptr;
        setProperty("delwtw_conditions", QVariant(false));
    }
    if(threads && QVariant(property("delete_threads")).toBool()){
        delete threads; threads = nullptr;
        setProperty("delete_threads", QVariant(false));
    }
    if(porogs)delete porogs; porogs = nullptr;
    if(invertSelection) delete invertSelection; invertSelection = nullptr;
    if(porogs2) delete porogs2; porogs2 = nullptr;
    if(invertSelection2) delete invertSelection2; invertSelection2 = nullptr;

}

SPRVariable<TypeConditions> *SPRSettingsPorogsModel::getConditions() const
{
    return conditions;
}

void SPRSettingsPorogsModel::setConditions(SPRVariable<TypeConditions> *value)
{
    conditions = value;
    setProperty("delete_conditions", QVariant(false));
    if(porogs){
        porogs->setConditions(conditions);
    }
}
