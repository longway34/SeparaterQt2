#ifndef SPRBOOLEANVARIABLE_H
#define SPRBOOLEANVARIABLE_H

#include <QObject>
#include "variables/sprvariable.h"

class SPRBooleanVariable : public SPRVariable<uint>
{
public:
    SPRBooleanVariable(QDomDocument *doc, QString xpath, bool defValue, IModelVariable *modelParent = nullptr):
        SPRVariable<uint>(doc, xpath, static_cast<uint>(defValue), modelParent){}

    bool getData(){
        bool ret = SPRVariable<uint>::getData() == 0 ? false : true;
        return ret;
    }

    void setData(bool value){
        uint v = value ? 1 : 0;
        SPRVariable<uint>::setData(v);
    }
};

#endif // SPRBOOLEANVARIABLE_H
