#ifndef SPRCOLORVARIABLE_H
#define SPRCOLORVARIABLE_H

#include <QObject>
#include <models/sprvariable.h>
#include "models/imodelvariable.h"
#include "models/isprmodeldata.h"

class SPRColorVariable : public SPRVariable<int>
{
public:
    SPRColorVariable(QDomDocument *doc, QString xpath, QColor defValue, ISPRModelData *modelParent):
        SPRVariable<int>(doc, xpath, int(0), modelParent)
    {
        setRoot(doc, xpath, SPRVariable::toString(static_cast<uint>(defValue.rgba())), false);
        fromXML();
//        SPRVariable<uint>::setData();
    }
    virtual void setData(QColor _data){
        SPRVariable<int>::setData(static_cast<uint>(_data.rgba()));
    }
    virtual QString toString(){
        return SPRVariable<int>::toString(data);
    }
    virtual QColor getData(){
        return QColor(SPRVariable<int>::getData());
    }

    virtual void fromXML()
    {
        data = IModelVariable::getData().toInt();
    }

};

#endif // SPRCOLORVARIABLE_H
