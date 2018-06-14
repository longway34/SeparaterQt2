#include "sprspectrumzonesmodel.h"

SPRSpectrumZonesModel::SPRSpectrumZonesModel(QDomDocument *doc, int indexThread, SPRElementsModel *_elProperty, IModelVariable *parent):
    ISPRModelData(doc, parent), elementsProperty(nullptr)
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
