#ifndef SPRFORMULAITEMMODEL_H
#define SPRFORMULAITEMMODEL_H

#include <QObject>
#include "_types.h"
#include "models/isprmodeldata.h"
#include "models/sprvariable.h"
#include "models/sprelementvariable.h"

class SPRFormulaItemModel :public ISPRModelData

{
public:
// H(index) = (ElementUp1 + ElementUp2 * MulUp)/(ElementDown1 + MupDown * ElementDown2 * ElementDown3 / ElementDown4)
    SPRElementVariable *ElementUp1;
    SPRElementVariable *ElementUp2;
    SPRVariable<double> *MulUp;
    SPRElementVariable *ElementDown1;
    SPRElementVariable *ElementDown2;
    SPRElementVariable *ElementDown3;
    SPRElementVariable *ElementDown4;
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
