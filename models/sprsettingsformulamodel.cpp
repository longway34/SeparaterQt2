#include "sprsettingsformulamodel.h"


SPREnumVariable<TypeConditions> *SPRSettingsFormulaModel::getConditions() const
{
    return conditions;
}

SPRElementsModel *SPRSettingsFormulaModel::getElements() const
{
    return elements;
}

void SPRSettingsFormulaModel::setElements(SPRElementsModel *value)
{
    if(value != elements){
        if(elements && property("delete_elements").value<bool>()){
            delete elements;
        }
    }
    elements = value;
    setProperty("delete_elements", QVariant(false));
}

void SPRSettingsFormulaModel::setCondition(SPREnumVariable<TypeConditions> *value)
{
    if(value){
        if(QVariant(property("delete_conditions")).value<bool>()){
            delete conditions;
            setProperty("delete_conditions", QVariant(false));
        }
        conditions = value;
    }
}
