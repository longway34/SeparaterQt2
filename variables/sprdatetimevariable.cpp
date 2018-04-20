#include "variables/sprdatetimevariable.h"

SPRDateTimeVariable::SPRDateTimeVariable(QDomDocument *parent, QString xpath, QDateTime defValue, IModelVariable *_mvparent, QString _format):
    IModelVariable(parent, xpath, defValue.toString(_format), _mvparent), format(_format)
{
    data = getData();
}

QDateTime SPRDateTimeVariable::getData()
{
    QString dstr = IModelVariable::getData();
    data = QDateTime::fromString(dstr, format);
    return data;
}

void SPRDateTimeVariable::setData(const QDateTime &value, QString _format)
{
    data = value;
    QString frm = _format.isEmpty() ? getFormat() : _format;
    IModelVariable::setData(data.toString(frm));
}

QString SPRDateTimeVariable::getFormat() const
{
    return format;
}

void SPRDateTimeVariable::setFormat(const QString &value)
{
    format = value;
}


QString SPRDateTimeVariable::toString(QString _frm){
    QString frm = _frm.isEmpty() ? getFormat() : _frm;
    QString ret = getData().toString(frm);
    return ret;
}
