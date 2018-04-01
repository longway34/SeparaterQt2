#ifndef SPRKPRVARIABLE_H
#define SPRKPRVARIABLE_H

#include <QObject>
#include <models/sprvariable.h>
#include <models/isprvariable.h>

class SPRKPRVariable : public SPRVariable<double>
{
public:
    SPRKPRVariable(QDomDocument *doc, QString xpath, double defValue, ISPRModelData *modelParent):
        SPRVariable<double>(doc, xpath, defValue, modelParent)
    {
        fromXML();
//        SPRVariable<uint>::setData();
    }

    // ISPRVariable interface
public:
    virtual void fromXML()
    {
        SPRVariable::fromXML();
        if(data < -1e-6 || data > 1e6)
            data = 1 / data;
    }
    virtual void setData()
    {
        if(data < -1e-6 || data > 1e6)
            IModelVariable::setData(QString::number(1 / data));
        return;
    }


    // SPRVariable interface
public:
    virtual void setData(const double &value)
    {
        SPRVariable<double>::setData(value);
    }
};

#endif // SPRKPRVARIABLE_H
