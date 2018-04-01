#include "sprseparatemodel.h"

SPRSeparateModel::SPRSeparateModel(QDomDocument *_doc, ISPRModelData *parent):
    ISPRModelData(_doc, parent), mainModel(0), gmz(), gcol(nullptr), kruch(nullptr), usl(), alg(nullptr), sep_row()
{
    mainModel = new SPRMainModel(doc);
    setProperty("delete_main", QVariant(true));

    for(int i=0; i< mainModel->getSettingsMainModel()->getThreads()->getData(); i++){
        QString path = SPR_SEPARATE_GMZ_PATH_PREFIX + QString::number(i)+"]";
        SPRVariable<uint> *var = new SPRVariable<uint>(doc, path, DEF_SPR_SEPARATE_GMZ, this);
        gmz.push_back(var);

        path = SPR_SEPARATE_USL_PATH_PREFIX + QString::number(i)+"]";
        var = new SPRVariable<uint>(doc, path, DEF_SPR_SEPARATE_USL, this);
        usl.push_back(var);
    }

    gcol = new SPRVariable<uint>(doc, SPR_SEPARATE_GCOL_PATH, DEF_SPR_SEPARATE_GCOL, this);
    kruch = new SPRVariable<uint>(doc, SPR_SEPARATE_KRUCH_PATH, DEF_SPR_SEPARATE_KRUCH, this);
    alg = new SPRVariable<uint>(doc, SPR_SEPARATE_ALG_PATH, DEF_SPR_SEPARATE_ALG, this);
    sep_row = new SPRVariable<uint>(doc, SPR_SEPARATE_SEP_ROW_PATH, DEF_SPR_SEPARATE_SEP_ROW, this);

}



SPRMainModel *SPRSeparateModel::getMainModel() const
{
    return mainModel;
}

void SPRSeparateModel::setMainModel(SPRMainModel *value)
{
    mainModel = value;
    setProperty("delete_main", QVariant(false));
}

SPRSeparateModel::~SPRSeparateModel(){
    if(mainModel && property("delete_main").toBool()){
        delete mainModel; mainModel = nullptr;
    }

    gmz.clear();
    usl.clear();
    if(gcol) delete gcol; gcol = nullptr;
    if(kruch) delete kruch; kruch = nullptr;
    if(alg) delete alg; alg = nullptr;
    if(sep_row) delete sep_row; sep_row = nullptr;
}
