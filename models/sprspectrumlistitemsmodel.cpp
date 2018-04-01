#include "sprspectrumlistitemsmodel.h"

QVector<SPRSpectrumItemModel *> *SPRSpectrumListItemsModel::getSpectrumsModel()
{
    return &spectrumsModel;
}

SPRSpectrumZonesTableModel *SPRSpectrumListItemsModel::getZonesTableModel()
{
    return zonesTableModel;
}

SPRSpectrumItemModel *SPRSpectrumListItemsModel::addSpectrum(uint8_t *buf, int bufLentgh)
{
    SPRSpectrumItemModel *item = new SPRSpectrumItemModel(zonesTableModel, formulas, this);
    item->setSpectrumData(buf, bufLentgh);
    spectrumsModel.push_back(item);
}

SPRSettingsFormulaModel *SPRSpectrumListItemsModel::getFormulas() const
{
    return formulas;
}

SPRSpectrumListItemsModel::SPRSpectrumListItemsModel(QDomDocument *_doc, ISPRModelData *parent): ISPRModelData(_doc, parent), zonesTableModel(nullptr)
{
    setZonesModel(new SPRSpectrumZonesTableModel(doc, parent));
    setProperty("delete_zones", QVariant(true));
}

SPRSpectrumListItemsModel::SPRSpectrumListItemsModel(SPRSpectrumZonesTableModel *_model, SPRSettingsFormulaModel *_formulas, ISPRModelData *parent): zonesTableModel(nullptr)
{
    setZonesModel(_model);
    setFormulasModel(_formulas);
    setProperty("delete_zones", QVariant(false));
    setProperty("delete_formulas", QVariant(false));
}

SPRSpectrumListItemsModel::~SPRSpectrumListItemsModel()
{
    if(zonesTableModel && property("delete_zones").value<bool>()){
        delete zonesTableModel;
    }
    if(formulas && property("delete_formulas").value<bool>()){
        delete zonesTableModel;
    }
}



ISPRModelData *SPRSpectrumListItemsModel::setZonesModel(SPRSpectrumZonesTableModel *_model)
{
    if(_model){
        if(zonesTableModel && property("delete_zones").value<bool>()){
            setProperty("delete_zones", QVariant(false));
            delete zonesTableModel;
        }
        zonesTableModel = _model;

        for(int i=0; i<spectrumsModel.size(); i++){
            uint32_t thr = *spectrumsModel[i]->getSpectrumData()->thread;
            spectrumsModel[i]->setZonesTable(_model);
        }
    }
}

ISPRModelData *SPRSpectrumListItemsModel::setFormulasModel(SPRSettingsFormulaModel *_formulas)
{
    if(_formulas){
        if(formulas && property("delete_formulas").value<bool>()){
            setProperty("delete_formulas", QVariant(false));
        }
        formulas = _formulas;

        for(int i=0; i<spectrumsModel.size(); i++){
            uint32_t thr = *spectrumsModel[i]->getSpectrumData()->thread;
            spectrumsModel[i]->setFormulas(_formulas);
        }
    }
    return this;
}

ISPRModelData *SPRSpectrumListItemsModel::setModel(SPRSpectrumZonesTableModel *_zones, SPRSettingsFormulaModel *_formulas)
{
    setZonesModel(_zones);
    setFormulasModel(_formulas);
    return this;
}
