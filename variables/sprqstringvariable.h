#ifndef SPRQSTRINGVARIABLE_H
#define SPRQSTRINGVARIABLE_H

#include <QObject>
#include "models/imodelvariable.h"

class SPRQStringVariable : public IModelVariable
{
public:
    SPRQStringVariable(QDomDocument *parent, QString xpath, QString defValue, IModelVariable *_mvparent=nullptr):
        IModelVariable(parent, xpath, defValue, _mvparent)
    {

    }

    // IModelVariable interface
public:
    virtual QString getData();
    virtual void setData(QString _value);
};

#endif // SPRQSTRINGVARIABLE_H
