#ifndef SPRDATETIMEVARIABLE_H
#define SPRDATETIMEVARIABLE_H

#include <QObject>
#include <QDateTime>
#include "models/imodelvariable.h"

class SPRDateTimeVariable : public IModelVariable
{
    QDateTime data;
    QString format;
public:
    SPRDateTimeVariable(QDomDocument *parent, QString xpath, QDateTime defValue, IModelVariable *_mvparent=nullptr, QString _format = "dd.MM.yyyy hh:mm:ss");

    QDateTime getData();
    void setData(const QDateTime &value, QString _format="");
    QString getFormat() const;
    void setFormat(const QString &value);
    QString toString(QString frm="");
};

#endif // SPRDATETIMEVARIABLE_H
