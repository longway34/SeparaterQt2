#ifndef SPRSETTINGSFORMULAMODEL_H
#define SPRSETTINGSFORMULAMODEL_H

#include <QObject>
#include <QVector>
#include <QDomDocument>

#include <models/isprmodeldata.h>
#include <models/sprformulaitemmodel.h>
#include <models/sprelementsmodel.h>
#include <variables/sprvariable.h>


class SPRSettingsFormulaModel : public ISPRModelData
{
protected:
    SPREnumVariable<TypeConditions> *conditions;
    SPRElementsModel *elements;
public:
    QVector<SPRFormulaItemModel*> itemsModel;
    SPRVariable<double> *min, *max;

    SPRSettingsFormulaModel(QObject *parent = nullptr)
    {
        conditions = nullptr;
        setProperty("delete_conditions", QVariant(false));

        elements = nullptr;
        setProperty("delete_elements", QVariant(false));

        min = nullptr;
        max = nullptr;

        itemsModel.clear();
    }
    SPRSettingsFormulaModel(QDomDocument *_doc, SPRElementsModel *_elements, SPREnumVariable<TypeConditions> *_conditions, ISPRModelData *parent = nullptr):
        ISPRModelData(_doc, parent), conditions(nullptr), elements(nullptr), min(nullptr), max(nullptr)
    {

        if(_elements){
            setElements(_elements);
        } else {
            elements = new SPRElementsModel(doc, this);
            setProperty("delete_elements", QVariant(true));
        }

        if(_conditions){
            setCondition(_conditions);
        } else {
            conditions = new SPREnumVariable<TypeConditions>(doc, SPR_POROGS_CONDITION_XPATH, DEF_SPR_FORMULA_CONDITION, this);
            setProperty("delete_conditions", QVariant(true));
        }

        max = new SPRVariable<double>(doc, SPR_FORMULA_MAX_XPATH, DEF_SPR_FORMULA_MAX, this);
        min = new SPRVariable<double>(doc, SPR_FORMULA_MIN_XPATH, DEF_SPR_FORMULA_MIN, this);

        for(int i=0; i<MAX_SPR_FORMULA_NUMBER; i++){
            SPRFormulaItemModel *mod = new SPRFormulaItemModel(doc, elements, i, this);
            mod->setMin(min);
            mod->setMax(max);
            itemsModel.push_back(mod);
        }
    }
    virtual ~SPRSettingsFormulaModel(){
        if(conditions && QVariant(property("delete_conditions")).toBool()) delete conditions; conditions = nullptr;
        if(elements && property("delete_elements").value<bool>()){
            delete elements; elements = nullptr;
        }
        if(min) delete min; min=nullptr;
        if(max) delete max; max=nullptr;

        itemsModel.clear();
    }
    virtual ISPRModelData *setModelData(ISPRModelData *model){
        SPRFormulaItemModel * mod = (SPRFormulaItemModel*)model;
        itemsModel[mod->index] = mod;
        connect(this, SIGNAL(doStore()), mod, SLOT(saveAs()));
        return model;
    }

    void setCondition(SPREnumVariable<TypeConditions> *value);
    SPREnumVariable<TypeConditions> *getConditions() const;
    SPRElementsModel *getElements() const;
    void setElements(SPRElementsModel *value);
};

#endif // SPRSETTINGSFORMULAMODEL_H
