#ifndef SPRELEMENTSMODEL_H
#define SPRELEMENTSMODEL_H

#include <QObject>
#include "models/isprmodeldata.h"
#include "variables/sprqstringvariable.h"
#include "variables/sprqcolorvariable.h"
#include "variables/sprvariable.h"
#include "variables/sprenumvariable.h"

#include "_types.h"

typedef enum _typeElements{
    typeAllElements, typeUsedElements, typeUnisedElements
} TypeSetElements;

class SPRElementsProperty :public QObject{
public:
    SPREnumVariable<EnumElements> *key;
    SPRQStringVariable *sName;
    SPRQStringVariable *fName;
    SPRQColorVariable *color;
    SPRElementsProperty(): key(nullptr), sName(nullptr), fName(nullptr), color(nullptr)
    {}
    virtual ~SPRElementsProperty(){
        if(key) delete key;
        if(sName) delete sName;
        if(fName) delete fName;
        if(color) delete color;
    }
};

typedef QMap<EnumElements, SPRElementsProperty*> QMapElementsProperty;

class SPRElementsModel: public ISPRModelData
{

    QMapElementsProperty allElements;
    QMapElementsProperty elements;
    QMapElementsProperty unisedElements;

public:


    SPRElementsProperty *getElementProperty(EnumElements el, TypeSetElements typeSet= typeUsedElements){
        QMapElementsProperty *source;
        switch(typeSet){
        case typeAllElements:
            source = &allElements;
            break;
        case typeUsedElements:
            source = &elements;
            break;
        case typeUnisedElements:
            source = &unisedElements;
            break;
        }

        if(source->contains(el)){
            return (*source)[el];
        } else {
            return nullptr;
        }
    }

    SPRElementsModel(QDomDocument *doc, ISPRModelData *parent=nullptr)
        :ISPRModelData(doc, parent)
    {
        allElements.clear();

        for(int num=0; num < MAX_SPR_SPECTOR_ELEMENTS; num++){

            QString SName_XPATH = SPR_SETTINGS_SPECTRUM_ZONES_XPATH_PREFIX + QString::number(num+1) + "]";
            struct SPRElementsProperty *el = new SPRElementsProperty();
            EnumElements _key = static_cast<EnumElements>(num);
            QString key_XPATH = SName_XPATH + SPR_SETTINGS_SPECTRUM_ZONES_KEY_XPATH_SUFFIX;
            el->key = new SPREnumVariable<EnumElements>(doc, key_XPATH, _key, this);

            EnumElements kkk = el->key->getData();

            QString fName_XPATH = SName_XPATH + SPR_SETTINGS_SPECTRUM_ZONES_FNAME_XPATH_SUFFIX;
            el->fName = new SPRQStringVariable(doc, fName_XPATH, DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el->key->getData()].name, this);


            QString color_XPATH = SName_XPATH + SPR_SETTINGS_SPECTRUM_ZONES_COLOR_XPATH_SUFFIX;
//            el.color = new SPRQColorVariable(doc, color_XPATH, DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el.key->getData()].color, this);
            el->color = new SPRQColorVariable(doc, color_XPATH, DEF_SPR_FORMULA_ELEMENTS_PROPERTY[kkk].color, this);

            el->sName = new SPRQStringVariable(doc, SName_XPATH, DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el->key->getData()].sname, this);
            bool res;
            QString sname = el->sName->getData();
            int i = el->sName->getData().toInt(&res);
            if(!res){
                elements[el->key->getData()] = el;
            } else {
                unisedElements[el->key->getData()] = el;
            }

            allElements[el->key->getData()] = el;
        }
    }

    void deleteElement(EnumElements element);

    SPRElementsProperty *changeElementPropery(EnumElements el, QString _sName, QString _fName="", QColor _color = QColor()){
        SPRElementsProperty *nel = nullptr;
        if(allElements.contains(el)){
            nel = allElements[el];
            nel->key->setData(el);
            nel->sName->setData(_sName);
            nel->fName->setData(_fName);
            nel->color->setData(_color);
        }
        return nel;
    }

    void addElement(EnumElements elKey, QString _sName, QString _fName, QColor _color = QColor()){
        SPRElementsProperty *el = changeElementPropery(elKey, _sName, _fName, _color);
        if(el){
            if(!elements.contains(elKey)){
                elements[elKey] = el;
            }
            if(unisedElements.contains(elKey)){
                unisedElements.remove(elKey);
            }
        }
    }
    ~SPRElementsModel(){
        foreach(SPRElementsProperty *el, allElements.values()){
            if(el) delete el;
        }

        allElements.clear();
        elements.clear();
        unisedElements.clear();
    }

    SPRQColorVariable *getColorVariable(EnumElements el){
        if(elements.contains(el)){
            return elements[el]->color;
        } else {
            return nullptr;
        }
    }

    QColor getColor(EnumElements el){
        return elements[el]->color->getData();
    }
    void setColor(EnumElements el, QColor _color){
        elements[el]->color->setData(_color);
    }
    QString getSName(EnumElements el){
        return elements[el]->sName->getData();
    }
    void setSName(EnumElements el, QString _sName){
        elements[el]->sName->setData(_sName);
    }
    QString getFName(EnumElements el){
        return elements[el]->fName->getData();
    }
    void setFName(EnumElements el, QString _fName){
        elements[el]->fName->setData(_fName);
    }

    QList<EnumElements> getUsedlElementKeys(){
        return elements.keys();
    }
    QMapElementsProperty getUnisedElements(){
        return unisedElements;
    }
    QMapElementsProperty getAllElements() const
    {
        return allElements;
    }
    QMapElementsProperty getElements() const
    {
        return elements;
    }
};

#endif // SPRELEMENTSMODEL_H
