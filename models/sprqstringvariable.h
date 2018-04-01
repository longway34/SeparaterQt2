#ifndef SPRQSTRINGVARIABLE_H
#define SPRQSTRINGVARIABLE_H

#include <QObject>
#include "sprvariable.h"
#include "imodelvariable.h"
#include "models/isprmodeldata.h"

class SPRQStringVariable : public ISPRVariable, public IModelVariable
{
    QString data;
public:
//    SPRQStringVariable();
    SPRQStringVariable(QDomDocument *doc, QString xpath, QString defValue, ISPRModelData *modelParent):
        ISPRVariable(modelParent), IModelVariable(doc, xpath, defValue)
    {
        setRoot(doc, xpath, defValue, false);
        fromXML();
//        setData();
    }
    virtual void setData(QString _data){
        data = _data;
//        setData();
    }
    virtual QString getData(){
        return data;
    }


    // ISepValue interface
public:
    virtual QString toString()
    {
        return data;
    }

    // ISPRVarialse interface
public slots:
    virtual void fromXML()
    {
        data = IModelVariable::getData();
    }

    virtual void setData()
    {
        IModelVariable::setData(QString(data));
    }

    // ISPRVariable interface
public:
    virtual QByteArray toByteArray()
    {
        QByteArray::fromRawData(data.toStdString().c_str(), sizeof(data.length()));
    }
};



#endif // SPRQSTRINGVARIABLE_H
