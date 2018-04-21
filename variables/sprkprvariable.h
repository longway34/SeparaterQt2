#ifndef SPRKPRVARIABLE_H
#define SPRKPRVARIABLE_H

#include <QObject>
#include <variables/sprvariable.h>
#include "models/isprmodeldata.h"

class SPRKPRVariable : public SPRVariable<double>
{
public:
    SPRKPRVariable(QDomDocument *doc, QString xpath, double defValue, IModelVariable *modelParent=nullptr):
        SPRVariable<double>(doc, xpath, defValue, modelParent)
    {
    }

public:
    double getData(){
        double d = SPRVariable<double>::getData();
        if(d > -1e-6 && d < 1e-6) d = 1e-6;
        return 1 / d;
    }

    virtual void setData(double &value)
    {
        if(value > -1e-6 && value < 1e-6) value = 1e-6;
        value = 1 / value;
        SPRVariable<double>::setData(value);
    }

    QString toString(){
        return QString::number(getData());
    }
};

#endif // SPRKPRVARIABLE_H
