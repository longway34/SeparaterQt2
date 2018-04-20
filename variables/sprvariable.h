#ifndef SPRVARIABLE_H
#define SPRVARIABLE_H

#include <QObject>
#include "models/imodelvariable.h"

template<class T>
class SPRVariable : public IModelVariable
{
protected:
    T data;
public:
    SPRVariable(QDomDocument *doc, QString xpath, T defValue, IModelVariable *modelParent = nullptr):
        IModelVariable(doc, xpath, QString::number(defValue), modelParent)
    {
        data = getData();
    }

    T getData(){
        double d = IModelVariable::getData().toDouble();
        return static_cast<T>(d);
    }
    void setData(T _data){
        data = _data;
        IModelVariable::setData(QString::number(data));
    }

    virtual QString toString(){
        return QString::number(getData());
    }
};

#endif // SPRVARIABLE_H
