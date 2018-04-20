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
};

#endif // SPRQSTRINGVARIABLE_H
