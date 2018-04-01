#include "imodelvariable.h"


bool IModelVariable::isIAm(IModelVariable::IsIAmData *data, QString path, QDomElement el, int count)
{
    if(el.nodeName() != data->tagName){
        return false;
    } else {
        data->typeXPath = XPathValue;
    }
    if(!data->keyName.isEmpty()){
        if(!el.attributes().contains(data->keyName)){
            return false;
        } else {
            if(el.attributes().namedItem(data->keyName).nodeValue() != data->keyValue){
                return false;
            } else {
                if(data->argName.isEmpty()){
                    data->typeXPath = XPathAttributeCondition;
                } else {
                    data->typeXPath = XPathAttribute;
                }
            }
        }
    }
    if(data->count > 0 && data->count != count){
        return false;
    } else {
        if(data->argName.isEmpty()){
            return true;
        } else {
            data->typeXPath = XPathAttribute;
        }
    }
    return true;
}

bool IModelVariable::setRoot(QDomElement *element, QStringList _xpath, QString defValue, int count, bool create)
{
    // проверяем я/не я по верхушке списка
    QString snode = _xpath.front();
    IsIAmData data(snode);
    bool ret = false;
    if(isIAm(&data, snode, *element, count)){
        if(_xpath.size() <= 1){ // глубже некуда
            root = *element;
            typeXPath == XPathValue;
            if(data.typeXPath == XPathAttribute){ // тег типа tag[arg], значение в атрибуте <tag arg='val'></tag>
                attribute = element->attributes().namedItem(data.argName).toAttr();
                typeXPath = XPathAttribute;
                ret = true;
                if(attribute.value().isEmpty()){
                    attribute.setValue(defValue);
                }
            } else { // просто тег <tag>val</tag>, значение в текстовом поле
                     // или тег типа <tag key='id'>val</tag>, значение в текстовом поле
                QDomNode node = element->firstChild();
                if(!node.isCharacterData()){
                    root = root.ownerDocument().createTextNode(defValue);
                } else {
                    root = node;
                }
                ret = true;
                if(root.toCharacterData().data().isEmpty()){
                    root.toCharacterData().setData(defValue);
                }
            }
        } else { // _el - то, что надо, но надо глубже
            _xpath.pop_front();
            IsIAmData d(_xpath.front());
            QDomNodeList nl = element->elementsByTagName(d.tagName);
//            bool find = false;
            for(int i=0; i<nl.size() && !ret; i++){
                QDomElement _el = nl.at(i).toElement();
                ret = setRoot(&_el, _xpath, defValue, i, create);
            }
        }
    }
    if(!ret){
        if(create){
            bool finish = _xpath.size() > 1 ? false : true;
            QDomElement newElement = element->ownerDocument().createElement(data.tagName);
            ret = true;
            if(!data.keyName.isEmpty()){
               newElement.setAttribute(data.keyName,data.keyValue);
            }
            if(!data.argName.isEmpty()){
               newElement.setAttribute(data.argName, defValue);
               attribute = newElement.attributes().namedItem(data.argName).toAttr();
               typeXPath = XPathAttribute;
               finish = true;
            }
            root = root.appendChild(newElement);
            if(data.typeXPath != XPathAttribute){
                root = element->ownerDocument().createTextNode(defValue);
                typeXPath = XPathValue;
            } else {
                if(!finish){
                    _xpath.pop_front();
                    QDomElement _el = root.toElement();
                    setRoot(&_el, _xpath, defValue, create);
                }
            }
        }
    }
    return ret;
}

//void IModelVariable::setRoot(QDomElement *element, QStringList _xpath, QString defValue, bool create)
//{
//    // проверяем я/не я по верхушке списка
//    QString snode = _xpath.front();
//    IsIAmData data(snode);
//    QDomNodeList nl = element->elementsByTagName(data.tagName);
//    qDebug() << "el:" << element->nodeName() <<" root:" << root.nodeName() << "find:" << snode;
//    bool find = false;
//    for(int i=0; i< nl.size() && !find; i++){
//        QDomElement _el = nl.at(i).toElement();
//        qDebug() << "name:" << _el.nodeName() << " attr:" << _el.attribute("CODE");
//        if(isIAm(&data, snode, _el,  i)){ // _el - это то, что надо
//            find = true;
//            if(_xpath.size() == 1){ // глубже некуда
//                root = _el;
//                typeXPath == XPathValue;
//                if(data.typeXPath == XPathAttribute){ // тег типа tag[arg], значение в атрибуте <tag arg='val'></tag>
//                    attribute = _el.attributes().namedItem(data.argName).toAttr();
//                    typeXPath = XPathAttribute;
//                    if(attribute.value().isEmpty()){
//                        attribute.setValue(defValue);
//                    }
//                } else { // просто тег <tag>val</tag>, значение в текстовом поле
//                         // или тег типа <tag arg='id'>val</tag>, значение в текстовом поле
//                    root = _el.firstChild();
//                    if(root.toCharacterData().data().isEmpty()){
//                        root.toCharacterData().setData(defValue);
//                    }
//                }
//            } else { // _el - то, что надо, но надо глубже
//                _xpath.pop_front();
//                setRoot(&_el, _xpath, defValue);
//            }
//        }
//    }
//    if(!find){ // не нашли ничего - надо сделать
//        if(create){
//            bool finish = _xpath.size() > 1 ? false : true;
//            QDomElement newElement = element->ownerDocument().createElement(data.tagName);
//            if(!data.keyName.isEmpty()){
//               newElement.setAttribute(data.keyName,data.keyValue);
//            }
//            if(!data.argName.isEmpty()){
//               newElement.setAttribute(data.argName, defValue);
//               attribute = newElement.attributes().namedItem(data.argName).toAttr();
//               typeXPath = XPathAttribute;
//               finish = true;
//            }
//            root = root.appendChild(newElement);
//            if(data.typeXPath != XPathAttribute){
//                root = element->ownerDocument().createTextNode(defValue);
//                typeXPath = XPathValue;
//             } else {
//                if(!finish){
//                    _xpath.pop_front();
//                    QDomElement _el = root.toElement();
//                    setRoot(&_el, _xpath, defValue, create);
//                }
//             }
//         }
//    }
//}

//IModelVariable::IModelVariable()
//{
//}

IModelVariable::IModelVariable(QDomDocument *doc, QString _xpath, QString _defValue, bool create)
{
    root = doc->documentElement();
    QDomElement _el = root.toElement();
    path = _xpath.split("/", QString::SkipEmptyParts);
    setRoot(&_el, path, _defValue, create);
}

void IModelVariable::setData(QString data)
{
    if(!root.isNull()){
        if(typeXPath == XPathAttribute){
            attribute.setValue(data);
        } else {
            root.toText().setData(data);
        }
    }
}

QString IModelVariable::getData()
{
    if(!root.isNull()){
        if(typeXPath == XPathAttribute){
            return attribute.value();
        } else {
            return root.toText().data();
        }
    }
}

IModelVariable::sIsIAmData::sIsIAmData(QString path):
    count(0), tagName(""), keyName(""), keyValue(""),
    argName(""), typeXPath(XPathPath)
{
    QStringList params = QString(path).replace(QRegExp("('|\")"),"").replace(" ","").
            split(QRegExp("(\\[|\\])"),  QString::SkipEmptyParts);
    tagName=params.at(0);
    for(int i=1; i<params.size();i++){
        QString str = params.at(i);
        if(str[0] == '@'){ // ключ вида [@keyName=keyValue]
           keyName = str.replace("@","").split("=").at(0);
           keyValue = str.replace("@","").split("=").at(1);
           continue;
        }
        if(str[0] == '#'){ // счетчик вида [#0-XX]
           count = QString(str.replace("#", "")).toInt();
           continue;
        }
        argName = str; // атрибут с искомым значением вида [attr]
    }
}
