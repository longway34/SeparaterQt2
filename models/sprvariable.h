#ifndef SPRVARIABLE_H
#define SPRVARIABLE_H

#include <QObject>
#include <QVariant>
#include <QColor>

#include <models/isprvariable.h>
#include <models/imodelvariable.h>

template <class T>
class SPRVariable : public ISPRVariable, public IModelVariable
{
protected:
    T data;
public:

    TypeVariable typeVariable;

    SPRVariable(QDomDocument *doc, QString xpath, T defValue, ISPRModelData *modelParent = nullptr):
        ISPRVariable(modelParent), IModelVariable(doc, xpath, QString::number(defValue))
    {
        setRoot(doc, xpath, toString(defValue), false);
        fromXML();
    }

    T getData() const
    {
        return data;
    }
    virtual void setData(const T &value)
    {
        data = value;
//        setData();
    }
    virtual QString toString(){
        return this->toString(data);
    }
    virtual QString toString(const T &value){
        if(mTypeVariables[typeid(value).name()] == realVariable) {
            return QString::number(value);
        }
        if(mTypeVariables[typeid(value).name()] == intVariable){
            return QString::number(value);
        }
        if(mTypeVariables[typeid(value).name()] == longVariable){
            return QString::number(value);
        }
        if(mTypeVariables[typeid(value).name()] == enumVariable){
            QString var = QVariant(static_cast<QVariant>(value)).toString();
            return var;
        }
        if(mTypeVariables[typeid(value).name()] == boolVariable){
            return QString::number(value ? 1 : 0);
        }

        QVariant ret = static_cast<QVariant>(value);
        return ret.toString();
    }
    // ISPRVariable interface
public:
    virtual QByteArray toByteArray()
    {
        return QByteArray::fromRawData((char *)&data, sizeof(data));
    }
public slots:
    virtual void setData(){ // установить в xml елементе
        if(mTypeVariables[typeid(data).name()] == realVariable) {
            IModelVariable::setData(QString::number(data));
            return;
        }
        if(mTypeVariables[typeid(data).name()] == intVariable){
            IModelVariable::setData(QString::number(data));
            return;
        }
        if(mTypeVariables[typeid(data).name()] == longVariable){
            IModelVariable::setData(QString::number(data));
            return;
        }
        if(mTypeVariables[typeid(data).name()] == enumVariable){
            QString var = QVariant(static_cast<QVariant>(data)).toString();
            IModelVariable::setData(var);
            return;
        }
        if(mTypeVariables[typeid(data).name()] == boolVariable){
            IModelVariable::setData(QString::number(data ? 1 : 0));
            return;
        }

        QVariant ret = static_cast<QVariant>(data);
        IModelVariable::setData(ret.toString());
        return;
    }

    // ISPRVarialse interface
public slots:
    virtual void fromXML()
    {
        if(mTypeVariables[typeid(data).name()] == intVariable){
            data = static_cast<T>(IModelVariable::getData().toInt());
            return;
        }
        if(mTypeVariables[typeid(data).name()] == longVariable){
            data = static_cast<T>(IModelVariable::getData().toLong());
            return;
        }
        if(mTypeVariables[typeid(data).name()] == enumVariable){
            data = static_cast<T>(IModelVariable::getData().toInt());
            return;
        }
        if(mTypeVariables[typeid(data).name()] == realVariable){
            data = static_cast<T>(IModelVariable::getData().toDouble());
            return;
        }
        if(mTypeVariables[typeid(data).name()] == boolVariable){
            data = static_cast<T>(IModelVariable::getData().toInt() == 0 ? false : true);
            return;
        }
    }

};




#endif // SPRVARIABLE_H
