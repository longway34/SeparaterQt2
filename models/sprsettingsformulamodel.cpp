#include "sprsettingsformulamodel.h"


SPRVariable<TypeConditions> *SPRSettingsFormulaModel::getConditions() const
{
    return conditions;
}

void SPRSettingsFormulaModel::setCondition(SPRVariable<TypeConditions> *value)
{
    if(value){
        if(QVariant(property("delete_conditions")).toBool()){
            delete conditions;
            setProperty("delete_conditions", QVariant(false));
        }
        conditions = value;
    }
}
