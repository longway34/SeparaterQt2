#include "variables/sprqstringvariable.h"



QString SPRQStringVariable::getData()
{
    QString ret = IModelVariable::getData();
//    ret = QString::fromUtf8(ret.toStdString().c_str());
//    ret = tr(ret.toStdString().c_str());
    return ret;
}

void SPRQStringVariable::setData(QString _value)
{
//    QString val = QString(_value.toUtf8());
    IModelVariable::setData(_value);
}
