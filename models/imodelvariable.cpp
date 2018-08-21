#include "imodelvariable.h"
#include "models/sprmainmodel.h"

/***
 * class FindData
 *
 */
void FindData::init(QStringList _xpath, QString _defValue){
    xpath = _xpath.front(); tagName = ""; keyName = ""; keyValue=""; attributeName = "";orderNum = 0;
    next = nullptr; defValue = "";
    _xpath.pop_front();
    nextXPath = _xpath;
    if(nextXPath.size() > 0){
        next = new FindData(nextXPath, _defValue);
    } else {
        defValue = _defValue;
    }
    QStringList params = QString(xpath).replace(QRegExp("('|\")"),"").replace(" ","").split(QRegExp("(\\[|\\])"),  QString::SkipEmptyParts);
//    toDebug(params);

    tagName = params[0];
    for(int i=1; i < params.size(); i++){
        if(params[i][0] == '@'){
            QStringList  atr = QString(params[i]).replace("@","").split('=');
            if(atr.size() > 1){
                keyName = atr[0]; keyValue = atr[1];
            }
        } else if(params[i][0] =='#'){
            orderNum = QString(params[i]).replace("#","").toInt();
        } else {
            attributeName = params[i];
        }
    }
//    toDebug();
}

QDomElement FindData::find(QDomNode *parent){
    bool findTag = false; bool findKey = false; bool findOrder = false; bool find = false;
    QDomElement findedElement;
    QDomNodeList ln; int findOrderNum = -1;
    if(parent->isDocument()){
        ln = parent->toDocument().childNodes();
    } else {
        ln = parent->toElement().elementsByTagName(tagName);
    }
    for(int i=0; i<ln.size() && !find;i++){
        if(ln.at(i).toElement().tagName() == tagName){
            findTag = true;
            if(!keyName.isEmpty()){
                if(ln.at(i).toElement().hasAttribute(keyName)){
                    if(ln.at(i).toElement().attribute(keyName) == keyValue){
                        findKey = true;
                        findedElement = ln.at(i).toElement();
                    }
                }
            } else {
                findKey = true;
            }
            if(findTag && findKey){
                findOrderNum = i;
            }
            if(orderNum == i){
                findedElement = ln.at(i).toElement();
                findOrder = true;
            }
        }
        find = findTag && findKey && findOrder;
    }
    if(!find){
        findedElement = create(parent, findOrderNum);
    }
    if(next){
        findedElement = next->find(&findedElement);
    } else {
        if(!attributeName.isEmpty()){
            if(!findedElement.hasAttribute(attributeName)){
                findedElement.setAttribute(attributeName, defValue);
            }
            findedNode = findedElement.attributeNode(attributeName);
        } else {
            if(!findedElement.hasChildNodes()){
                findedElement.insertBefore(findedElement.ownerDocument().createTextNode(defValue), QDomNode());
            }
            findedNode = findedElement.childNodes().at(0);
        }
    }
    return findedElement;
}

QDomElement FindData::create(QDomNode *parent, int findOrderNum){
    QDomElement res;
    while(findOrderNum < orderNum){
        res = parent->ownerDocument().createElement(tagName);
        findOrderNum++;
        parent->appendChild(res);
        if(!keyName.isEmpty()){
            res.setAttribute(keyName, keyValue);
        }
        if(findOrderNum == orderNum){
            if(!attributeName.isEmpty()){
                res.setAttribute(attributeName, defValue);
                findedNode = res.attributeNode(attributeName);
            } else {
                QDomText txt = res.insertBefore(res.ownerDocument().createTextNode(defValue),QDomNode()).toText();
                findedNode = txt;
            }
            return res;
        }
    }
    return res;
}

QDomNode FindData::getFindedNode(){
    FindData *item = this;
    while(item->next){
        item = item->next;
    }
    return item->findedNode;
}

void FindData::toDebug(QStringList params){
    if(params.size() > 0){
        qDebug() << "*******************";
        for(int i=0; i< params.size(); i++){
            qDebug() << i << " " << params[i];
        }
    } else {
        qDebug() << "tag  : "<< tagName;
        qDebug() << "key  : "<< keyName;
        qDebug() << "val  : "<< keyValue;
        qDebug() << "attr : "<< attributeName;
        qDebug() << "order: "<< orderNum;
        qDebug() << "default:" << defValue;
        qDebug() << "next : "<< next;
    }
}

/***
 * class IModelVariable
 *
 */

void IModelVariable::Init(QDomDocument *parent, QString xpath, QString defValue, IModelVariable *mvparent)
{
    mconnect(mvparent);
    QStringList p = xpath.split('/', QString::SkipEmptyParts);
    FindData *fd = new FindData(p, defValue);

    QDomElement el = fd->find(parent);
    if(!el.isNull() && el.isElement()){
        xmlNode = fd->getFindedNode();
    }
    fromXml();
    delete fd;
    //    toDebug();
}

SPRMainModel *IModelVariable::getMainModel()
{
    SPRMainModel *res = nullptr;
    if(objectName() == "MainModel"){
        return (SPRMainModel*)this;
    } else {
        if(mvparent){
            res = mvparent->getMainModel();
        } else {
            return nullptr;
        }
    }
    return res;
}


