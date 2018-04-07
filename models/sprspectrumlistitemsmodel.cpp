#include "sprspectrumlistitemsmodel.h"

static const QVector<QColor> mainColors = {Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow,
                                    Qt::darkRed, Qt::darkBlue, Qt::darkGreen, Qt::darkCyan, Qt::darkMagenta, Qt::darkYellow,
                                    Qt::gray, Qt::lightGray, Qt::white, Qt::darkGray};

QVector<SPRSpectrumItemModel *> *SPRSpectrumListItemsModel::getSpectrumsModel()
{
    return &spectrumsModel;
}

SPRSpectrumZonesTableModel *SPRSpectrumListItemsModel::getZonesTableModel()
{
    return zonesTableModel;
}
void  *SPRSpectrumListItemsModel::clearSpectrums(){
    for(int i=0; i<spectrumsModel.size(); i++){
        delete spectrumsModel[i];
    }
    spectrumsModel.clear();
//    emit modelChanget();
}

SPRSpectrumItemModel *SPRSpectrumListItemsModel::setSpectrum(uint8_t *buf, int bufLentgh){
    clearSpectrums();
    addSpectrum(buf, bufLentgh);
}

SPRSpectrumItemModel *SPRSpectrumListItemsModel::addSpectrum(uint8_t *buf, int bufLentgh)
{
    SPRSpectrumItemModel *item = new SPRSpectrumItemModel(zonesTableModel, formulas, this);
    item->setSpectrumData(buf, bufLentgh);
    if(bufLentgh == DEF_SPECTRUM_DATA_LENGTH){
        item->setSpectrumThread(spectrumsModel.size());
        item->setSpectrumName("spec_"+QString::number(spectrumsModel.size()));
        item->setSpectrumColor(mainColors[spectrumsModel.size() % mainColors.size()]);
        item->setSpectrumDateTime(QDateTime::currentDateTime());
    }
    spectrumsModel.push_back(item);
//    emit modelChanget();
    return item;
}

SPRSettingsFormulaModel *SPRSpectrumListItemsModel::getFormulas() const
{
    return formulas;
}

SPRSpectrumListItemsModel::SPRSpectrumListItemsModel(QDomDocument *_doc, ISPRModelData *parent): ISPRModelData(_doc, parent), zonesTableModel(nullptr), formulas(nullptr), spectrumsModel()
{
    setZonesModel(new SPRSpectrumZonesTableModel(doc, parent));
    setProperty("delete_zones", QVariant(true));
}

//SPRSpectrumListItemsModel::SPRSpectrumListItemsModel(SPRMainModel *_mainModel, ISPRModelData *parent): ISPRModelData(_doc, parent), zonesTableModel(nullptr), formulas(nullptr), spectrumsModel()
//{
//    setZonesModel(_mainModel->getSpectrumZonesTableModel());
//    setZonesModel(_mainModel->getSettingsFormulaModel());
//    setProperty("delete_zones", QVariant(false));
//    setProperty("delete_formulas", QVariant(false));
//}

SPRSpectrumListItemsModel::SPRSpectrumListItemsModel(SPRSpectrumZonesTableModel *_model, SPRSettingsFormulaModel *_formulas, ISPRModelData *parent): zonesTableModel(nullptr), formulas(nullptr), spectrumsModel()
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

ISPRModelData *SPRSpectrumListItemsModel::setModel(SPRSpectrumZonesTableModel *_zones, SPRSettingsFormulaModel *_formulas, uint8_t *inp, int len)
{
    setZonesModel(_zones);
    setFormulasModel(_formulas);
    clearSpectrums();
    if(inp){
        addSpectrum(inp, len);
    }
    return this;
}
