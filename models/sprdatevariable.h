#ifndef SPRDATEVARIABLE_H
#define SPRDATEVARIABLE_H

#include <QObject>
#include <QDateTime>

#include "models/isprvariable.h"
#include "models/imodelvariable.h"

class SPRDateVariable : public ISPRVariable, public IModelVariable
{
    Q_OBJECT
    QDate data;

public:
    SPRDateVariable(QDomDocument *doc, QString xpath, QDate defValue, ISPRModelData *modelParent):
        ISPRVariable(modelParent), IModelVariable(doc, xpath, defValue.toString("dd.MM.yyyy"))
    {
        IModelVariable::setRoot(doc, xpath, defValue.toString("dd.MM.yyyy"), false);
        fromXML();
    }

    virtual void fromXML(){
        data = QDate(QDate::fromString(IModelVariable::getData(), "dd.MM.yyyy"));
    }

    // ISPRVariable interface
public:
    virtual QString toString()
    {
        return data.toString("dd.MM.yyyy");
    }

    QDate getData() const{
        return data;
    }
    virtual void setData(QDate _data){
        data = _data;
//        setData();
    }
public slots:
    virtual void setData()
    {
        IModelVariable::setData(toString());
    }
};

#endif // SPRDATEVARIABLE_H
