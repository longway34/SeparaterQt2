#ifndef SPRFORMULAITEMMODEL_H
#define SPRFORMULAITEMMODEL_H

#include <QObject>
#include "_types.h"
#include "models/isprmodeldata.h"
#include "variables/sprvariable.h"
#include "variables/sprenumvariable.h"

class SPRFormulaItemModel :public ISPRModelData

{
public:
// H(index) = (ElementUp1 + ElementUp2 * MulUp)/(ElementDown1 + MupDown * ElementDown2 * ElementDown3 / ElementDown4)
    SPREnumVariable<EnumElements> *ElementUp1;
    SPREnumVariable<EnumElements> *ElementUp2;
    SPRVariable<double> *MulUp;
    SPREnumVariable<EnumElements> *ElementDown1;
    SPREnumVariable<EnumElements> *ElementDown2;
    SPREnumVariable<EnumElements> *ElementDown3;
    SPREnumVariable<EnumElements> *ElementDown4;
    SPRVariable<double> *MulDown;
    SPRVariable<double> *min, *max;

    int index;

    SPRFormulaItemModel(QObject *parent=nullptr);
    SPRFormulaItemModel(QDomDocument *_doc, int _index = 0, ISPRModelData *parent = nullptr);
    virtual ~SPRFormulaItemModel();
    void setMin(SPRVariable<double> *value);
    void setMax(SPRVariable<double> *value);
};

#endif // SPRFORMULAITEMMODEL_H
