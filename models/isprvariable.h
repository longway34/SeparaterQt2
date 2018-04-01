#ifndef ISRPVARIABLE_H
#define ISRPVARIABLE_H

#include <QObject>
#include <QMap>
//#include "mainwindow.h"
#include "models/isprmodeldata.h"
#include "_types.h"

typedef enum :int {
    stringVariable = 0,
    intVariable,
    realVariable,
    enumVariable,
    qObjectVariable,
    boolVariable,
    longVariable
} TypeVariable;

//Q_DECLARE_METATYPE (TypeVariable)

static const QMap<const char*, TypeVariable> mTypeVariables =
{
    {typeid(const char*).name(), stringVariable},
    {typeid(std::string).name(), stringVariable},

    {typeid(int).name(), intVariable},
    {typeid(uint).name(), intVariable},
    {typeid(qint16).name(), intVariable},
    {typeid(quint16).name(), intVariable},
    {typeid(quint16).name(), intVariable},

    {typeid(long).name(), longVariable},
    {typeid(ulong).name(), longVariable},
    {typeid(qulonglong).name(), longVariable},
    {typeid(qlonglong).name(), longVariable},

    {typeid(qreal).name(), realVariable},
    {typeid(double).name(), realVariable},

    {typeid(TypeThermo).name(), enumVariable},
    {typeid(TypePRAM).name(), enumVariable},
    {typeid(TypeConditions).name(), enumVariable},
    {typeid(TypeSelection).name(), enumVariable},
    {typeid(EnumElements).name(), enumVariable},
    {typeid(TypeAutoSettingRMT).name(), enumVariable},

    {typeid(QColor).name(), qObjectVariable},
    {typeid(QString).name(), qObjectVariable},

    {typeid(bool).name(), boolVariable}

};

class ISPRVariable : public QObject
{
    Q_OBJECT

public:
    ISPRVariable(ISPRModelData *modelParent, QObject *parent = nullptr): QObject(parent)
    {
        if(modelParent){
            connect(modelParent, SIGNAL(goStore()), this, SLOT(setData()));
        }
    }


    virtual QString toString()=0;
    virtual QByteArray toByteArray()=0;
signals:

public slots:
    virtual void setData()=0;
    virtual void fromXML()=0;
};


#endif // ISRPVARIABLE_H
