#include "variables/sprqcolorvariable.h"
#include "QtGui/QColor"

QColor SPRQColorVariable::getData()
{
    data = IModelVariable::getData().toInt();
    return QColor(data);
}

void SPRQColorVariable::setData(const QColor &value)
{

    data = value.rgb() & 0xFFFFFF;
    IModelVariable::setData(QString::number(data));
}

SPRQColorVariable::SPRQColorVariable(QDomDocument *parent, QString xpath, QColor defValue, IModelVariable *_mvparent):
    SPRVariable<uint>(parent, xpath, defValue.rgb() & 0xFFFFFF, _mvparent)
{
    data = IModelVariable::getData().toInt();
}

QString SPRQColorVariable::toString(){
    QString c = QColor(getData()).name()+"("+QString::number(data)+")";
    return c;
}
