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

#include "models/isprmodeldata.h"

class IModelVariable
{
protected:
    typedef enum : int{
        XPathPath = 0,
        XPathValue,
        XPathAttribute,
        XPathAttributeCondition
    }  TypeXPath;

    typedef struct sIsIAmData {
        int count;
        QString tagName;
        QString keyName;
        QString keyValue;
        QString argName;
        TypeXPath typeXPath;
        sIsIAmData(): count(0), tagName(""), keyName(""), keyValue(""), argName(""), typeXPath(XPathPath){}
        sIsIAmData(QString path);
    } IsIAmData;

    TypeXPath typeXPath;
    QDomAttr attribute;
    QStringList path;

    bool isIAm(IsIAmData *data, QString path,  QDomElement el, int count = 0);
    void setRoot(QDomDocument *doc, QString _xpath, QString defValue, bool create = false){
        root = doc->documentElement();
        QDomElement el = root.toElement();
        setRoot(&el, _xpath, defValue, create);
    }

    void setRoot(QDomElement *element, QString _xpath, QString defValue, bool create = false){
        path = _xpath.split(("/"), QString::SkipEmptyParts);
        setRoot(element, path, defValue, create);
    }
    bool setRoot(QDomElement *element, QStringList _xpath, QString defValue, int count = 0, bool create = false);
public:
    QDomNode root;

//    IModelVariable(){}
    IModelVariable(QDomDocument *doc, QString _xpath, QString _defValue, bool create = false);

    void setData(QString data);
    QString getData();
};

#endif // IMODELVARIABLE_H
