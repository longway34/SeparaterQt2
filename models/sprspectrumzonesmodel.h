#ifndef SPRSPECTRUMZONESMODEL_H
#define SPRSPECTRUMZONESMODEL_H

#include <QObject>
#include <QDomDocument>
#include "models/isprmodeldata.h"
#include "variables/sprvariable.h"
#include "variables/sprqstringvariable.h"
#include "variables/sprqcolorvariable.h"
#include "models/sprelementsmodel.h"

#include "_types.h"

class SpectorRange :public QObject{
public:

    SPRElementsProperty *element;
    SPRVariable<uint> *min;
    SPRVariable<uint> *max;

    SpectorRange():element(nullptr), min(nullptr), max(nullptr){
        element = new SPRElementsProperty();
        setProperty("delete_property", QVariant(true));
    }
    SpectorRange(SPRElementsProperty *prop):element(prop), min(nullptr), max(nullptr){
        setProperty("delete_property", QVariant(false));
    }
    virtual ~SpectorRange(){
        if(min) delete min;
        if(max) delete max;
        if(element && property("delete_property").value<bool>()){
            delete element;
        }
    }
};

typedef QMap<EnumElements, SpectorRange*> QMapElementsRanges;

#define SPR_SETTINGS_SPECTRUM_RANGES_XPATH_PREFIX "SEPARATOR/SEPARATE_SETUP/OBL/CHANNEL"
#define SPR_SETTINGS_SPECTRUM_RANGES_XPATH_MIN_AGRUMENT "[Ls]"
#define SPR_SETTINGS_SPECTRUM_RANGES_XPATH_MAX_ARGUMENT "[Rs]"

typedef enum typeZones{
    typeUsedZones, typeUnisedZones, typeAllZones
} TypeUsedZones;

class SPRSpectrumZonesModel : public ISPRModelData
{
    QMapElementsRanges elements;
    SPRElementsModel *elementsProperty;
public:
    int tIndex;


    QMapElementsRanges getZones(TypeUsedZones typeZones=typeUsedZones){
        QMapElementsRanges res;
        QMapElementsProperty input;
        switch (typeZones) {
        case typeUsedZones:
            input = elementsProperty->getElements();
            break;
        case typeUnisedZones:
            input = elementsProperty->getUnisedElements();
            break;
        case typeAllZones:
            input = elementsProperty->getAllElements();
        default:
            break;
        }
        foreach (EnumElements el, input.keys()) {
            res[el] = elements[el];
        }
        return res;
    }

    SPRSpectrumZonesModel(){}
    SPRSpectrumZonesModel(QDomDocument *doc, int indexThread, SPRElementsModel *_elProperty):
        ISPRModelData(doc), elementsProperty(nullptr)
    {
        tIndex = indexThread;
        if(!_elProperty){
            elementsProperty = new SPRElementsModel(doc, this);
            setProperty("delete_elProperty", QVariant(true));
        } else {
            elementsProperty = _elProperty;
            setProperty("delete_elProperty", QVariant(false));
        }

        foreach (EnumElements element, getZones(typeAllZones).keys()) {
            SpectorRange *range = new SpectorRange(elementsProperty->getElementProperty(element));
            QString xpath = SPR_SETTINGS_SPECTRUM_RANGES_XPATH_PREFIX +
                    QString("[@INDEX=")+QString::number(tIndex)+"]"+
                    "/OblName[#"+QString::number(static_cast<int>(element))+"]" +
                    SPR_SETTINGS_SPECTRUM_RANGES_XPATH_MAX_ARGUMENT;
            int def = (DEF_SPR_FORMULA_ELEMENTS_PROPERTY)[element].max;
            range->max = new SPRVariable<uint>(doc, xpath, def, this);

            xpath = SPR_SETTINGS_SPECTRUM_RANGES_XPATH_PREFIX +
                    QString("[@INDEX=")+QString::number(tIndex)+"]"+
                    "/OblName[#"+QString::number(static_cast<int>(element))+"]" +
                    SPR_SETTINGS_SPECTRUM_RANGES_XPATH_MIN_AGRUMENT;
            def = (DEF_SPR_FORMULA_ELEMENTS_PROPERTY)[element].min;
            range->min = new SPRVariable<uint>(doc, xpath, def, this);

            elements[element] = range;
        }

    }
    virtual ~SPRSpectrumZonesModel()
    {
        foreach (EnumElements elem, elements.keys()) {
            delete elements[elem]->max;
            delete elements[elem]->min;
        }
        elements.clear();
        if(elementsProperty && property("delete_elProperty").value<bool>()){
            delete elementsProperty;
        }
    }

    void deleteElement(EnumElements el){
//        if(elements.contains(el)){
//            elements.remove(el);
//        }
        elementsProperty->deleteElement(el);
        emit doStore();
    }
    void setMinMax(EnumElements el, uint min, uint max){
        if(elements.contains(el)){
            elements[el]->min->setData(min);
            elements[el]->max->setData(max);
        }
    }
    SPRElementsModel *getElementsProperty() const
    {
        return elementsProperty;
    }
};

#endif // SPRSPECTRUMZONESMODEL_H
