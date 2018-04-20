#ifndef IMODELVARIABLE_H
#define IMODELVARIABLE_H

#include <QObject>
#include <QStringList>
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QDomCharacterData>
#include <QDomText>
#include <QDomNode>
#include <QDebug>
#include <QByteArray>

#include "models/isprmodeldata.h"
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

    QDomDocument *doc;
    QString value;
    QDomNode xmlNode;
    IModelVariable *mvparent;

    void fromXml(){
        value = xmlNode.nodeValue();
    }
    void toXml(){
        xmlNode.setNodeValue(value);
    }

    void mconnect(IModelVariable *_mvparent){
        if(_mvparent){
            if(mvparent){
                disconnect(mvparent, SIGNAL(doStore()), this, SLOT(store()));
                disconnect(mvparent, SIGNAL(doRestore()), this, SLOT(restore()));
            }
            mvparent = _mvparent;
            connect(mvparent, SIGNAL(doStore()), this, SLOT(store()));
            connect(mvparent, SIGNAL(doRestore()), this, SLOT(restore()));
        }
    }
protected:
    IModelVariable(){}
    IModelVariable(QDomDocument *parent, IModelVariable *_mvparent=nullptr): doc(parent), mvparent(nullptr){
        mconnect(_mvparent);
    }

    IModelVariable(QDomDocument *parent, QString xpath, QString defValue, IModelVariable *_mvparent=nullptr): doc(parent), mvparent(nullptr){
        Init(parent, xpath, defValue, _mvparent);
    }
    void Init(QDomDocument *parent, QString xpath, QString defValue, IModelVariable *mvparent=nullptr);

public:

    QString getData(){
        return value;
    }
    void setData(QString _value){
        value = _value;
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
    void store(){
        toXml();
        emit doStore();
    }
    void restore(){
        fromXml();
        emit doRestore();
    }
signals:
    void doStore();
    void doRestore();
};
#endif // IMODELVARIABLE_H
