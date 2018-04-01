#ifndef IPVALIDATOR_H
#define IPVALIDATOR_H

#include <QObject>
#include <QRegExpValidator>
#include <QRegExp>

class IpValidator : public QRegExpValidator
{
private:
    QString ipRange;
    QString portRange;
public:
    IpValidator();
};

#endif // IPVALIDATOR_H
