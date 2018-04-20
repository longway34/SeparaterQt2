#ifndef SPRQCOLORVARIABLE_H
#define SPRQCOLORVARIABLE_H

#include <QObject>
#include <QtGui/QColor>

#include "variables/sprvariable.h"

class SPRQColorVariable: public SPRVariable<uint>
{
public:
    SPRQColorVariable(QDomDocument *parent, QString xpath, QColor defValue, IModelVariable *_mvparent=nullptr);

    QColor getData();
    void setData(const QColor &value);
    QString toString();
};

#endif // SPRQCOLORVARIABLE_H
