#include "sprsettingsformulamodel.h"


SPREnumVariable<TypeConditions> *SPRSettingsFormulaModel::getConditions() const
{
    return conditions;
}

void SPRSettingsFormulaModel::setCondition(SPREnumVariable<TypeConditions> *value)
{
    if(value){
        if(QVariant(property("delete_conditions")).toBool()){
            delete conditions;
            setProperty("delete_conditions", QVariant(false));
        }
        conditions = value;
    }
}
