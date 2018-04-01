#ifndef SPRELEMENTVARIABLE_H
#define SPRELEMENTVARIABLE_H

#include <QObject>
#include "models/sprvariable.h"

class SPRElementVariable : public SPRVariable<EnumElements>
{
public:
    SPRElementVariable(QDomDocument *doc, QString xpath, EnumElements defValue, ISPRModelData *modelParent):
        SPRVariable<EnumElements>(doc, xpath, defValue, modelParent)
    {
        fromXML();
    }


    // ISPRVariable interface
public:
    virtual QString toString(){
        return DEF_SPR_FORMULA_ELEMENTS_PROPERTY[data].sname;
    }
    qint16 getIndex(){
        return DEF_SPR_FORMULA_ELEMENTS_PROPERTY[data].index;
    }
};

#endif // SPRELEMENTVARIABLE_H
