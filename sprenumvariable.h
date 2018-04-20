#ifndef SPRENUMVARIABLE_H
#define SPRENUMVARIABLE_H

#include <QObject>
#include "models/imodelvariable.h"
#include "models/sprvariable.h"

template <class T>
class SPREnumVariable : public SPRVariable<int>
{
protected:
    T data;
public:
    SPREnumVariable(QDomDocument *parent, QString xpath, T defValue, IModelVariable *_mvparent):
        SPRVariable<int>(parent, xpath, static_cast<int>(defValue), _mvparent)
    {

    }
    T getData()
    {
        data = static_cast<T>(SPRVariable::getData());
        return data;
    }
    void setData(const T &value)
    {
        data = value;
        SPRVariable::setData(static_cast<int>(data));
    }

    QString toString(){
        return DEF_SPR_FORMULA_ELEMENTS_PROPERTY[getData()].name;
    }
};

#endif // SPRENUMVARIABLE_H
