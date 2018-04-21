#ifndef SPRENUMVARIABLE_H
#define SPRENUMVARIABLE_H

#include <QObject>
#include <variables/sprvariable.h>
#include <QColor>
#include "_types.h"

template <class T>
class SPREnumVariable : public SPRVariable<int>
{
protected:
//    T data;
public:
    SPREnumVariable(QDomDocument *parent, QString xpath, T defValue, IModelVariable *_mvparent):
        SPRVariable<int>(parent, xpath, static_cast<int>(defValue), _mvparent)
    {

    }
    T getData()
    {
        T d = static_cast<T>(SPRVariable::getData());
        return d;
    }
    void setData(const T &value)
    {
        T d = value;
        SPRVariable::setData(static_cast<int>(d));
    }

    int getIndex(){
        return SPRVariable::getData();
    }

    QString toString(){
//        return DEF_SPR_FORMULA_ELEMENTS_PROPERTY[getData()].name;
    }
};

#endif // SPRENUMVARIABLE_H
