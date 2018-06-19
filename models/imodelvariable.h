#ifndef IMODELVARIABLE_H
#define IMODELVARIABLE_H

#include <QObject>
#include <QStringList>
#include <QtXml>
//#include <QDomDocument>
//#include <QDomElement>
//#include <QDomNode>
//#include <QDomAttr>
//#include <QDomCharacterData>
//#include <QDomText>
//#include <QDomNode>
#include <QDebug>
#include <QByteArray>

//#include "models/isprmodeldata.h"
class FindData {
    QString xpath;
    QStringList nextXPath;
    QString tagName;
    QString keyName;
    QString keyValue;
    QString attributeName;
    QDomNode findedNode;
    QString defValue;
    FindData *next;
    int orderNum;
public:
    virtual ~FindData()
    {
        if(next) {
            delete next;
        }
//        if(next) delete next;
    }
    void init(QStringList _xpath, QString _defValue);
    FindData(QStringList xpath, QString _defValue=QString()){
        init(xpath, _defValue);
    }

    QDomElement find(QDomNode *parent);

    void toDebug(QStringList params = {});

    QDomElement create(QDomNode *parent, int findOrderNum);
    QDomNode getFindedNode();
};

class IModelVariable: public QObject
{
    Q_OBJECT

    QString value;
    QDomNode xmlNode;
    IModelVariable *mvparent;

    void fromXml(){
        if(!xmlNode.isNull()){
            if(xmlNode.nodeValue() != value){
//                QString v = xmlNode.nodeValue();
//                QString v1 = QString::fromUtf8(xmlNode.nodeValue().toStdString().c_str());
                value = xmlNode.nodeValue();
//                emit modelChanget(this);
            }
        }
    }
    void toXml(){
        if(!xmlNode.isNull())
            xmlNode.setNodeValue(value);
    }

    void mconnect(IModelVariable *_mvparent){
        if(_mvparent){
            if(mvparent){
                disconnect(mvparent, SIGNAL(doStore()), this, SLOT(store()));
                disconnect(mvparent, SIGNAL(doRestore()), this, SLOT(restore()));
                disconnect(this, SIGNAL(modelChanget(IModelVariable*)), mvparent, SLOT(onModelChanget(IModelVariable*)));
            }
            mvparent = _mvparent;
            connect(mvparent, SIGNAL(doStore()), this, SLOT(store()));
            connect(mvparent, SIGNAL(doRestore()), this, SLOT(restore()));
            connect(this, SIGNAL(modelChanget(IModelVariable*)), mvparent, SLOT(onModelChanget(IModelVariable*)));
        }
    }
protected:
    QDomDocument *doc;

    IModelVariable(): mvparent(nullptr), doc(nullptr){}
    IModelVariable(QDomDocument *parent, IModelVariable *_mvparent=nullptr): doc(parent), mvparent(nullptr){
        mconnect(_mvparent);
    }

    IModelVariable(QDomDocument *parent, QString xpath, QString defValue, IModelVariable *_mvparent=nullptr): doc(parent), mvparent(nullptr){
        Init(parent, xpath, defValue, _mvparent);
    }
    void Init(QDomDocument *parent, QString xpath, QString defValue, IModelVariable *mvparent=nullptr);

public:
    void setMVParent(IModelVariable *_mvparent){
        mconnect(_mvparent);
    }

    QString getData(){
        return value;
    }
    void setData(QString _value){
        if(value != _value){
            value = _value;
            emit modelChanget(this);
        }
    }

    void toDebug(QDomNode element=QDomNode()){
        if(element.isNull()) element = xmlNode;

        if(element.isAttr()) element = element.toAttr().parentNode();
        if(element.isText()) element = element.toText().parentNode();
        if(element.isElement()){
            qDebug() << "tag    :" << element.toElement().tagName();
            QDomNamedNodeMap attrs = element.toElement().attributes();
            if(attrs.size()>0){
                qDebug() << "***********************";
                qDebug() << "tag : "<<element.toElement().tagName() << " = " << element.toElement().text();
                qDebug() << "****** atributes ******";
                for(int i=0; i<attrs.size(); i++) {
                    qDebug() << i << " " << attrs.item(i).nodeName() << " : "<< attrs.item(i).nodeValue();
                }
            }
        }
        qDebug() << "value :"<<value;
        qDebug() << "";
    }

    QString toString(){
        return getData();
    }
public slots:
    virtual void store(){
        toXml();
        emit doStore();
    }
    virtual void restore(){
        fromXml();
        emit doRestore();
    }
    virtual void onModelChanget(IModelVariable* send){
        emit modelChanget(send);
    }
signals:
    void doStore();
    void doRestore();
    void modelChanget(IModelVariable *);
};
#endif // IMODELVARIABLE_H
