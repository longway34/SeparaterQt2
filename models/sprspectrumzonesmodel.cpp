#include "sprspectrumzonesmodel.h"

SPRSpectrumZonesModel::SPRSpectrumZonesModel(QDomDocument *doc, int indexThread, const DefaultMapElements *_elProperty):
    ISPRModelData(doc)
{
    tIndex = indexThread;
    if(!_elProperty){
        _elProperty = &DEF_SPR_FORMULA_ELEMENTS_PROPERTY;
    }
    foreach (EnumElements element, _elProperty->keys()) {
        QString xpath = SPR_SETTINGS_SPECTRUM_RANGES_XPATH_PREFIX +
                QString("[@INDEX=")+QString::number(tIndex)+"]"+
                "/OblName[#"+QString::number((*_elProperty)[element].index)+"]" +
                SPR_SETTINGS_SPECTRUM_RANGES_XPATH_MAX_ARGUMENT;
        int def = (*_elProperty)[element].max;
        this->elements[element].max = new SPRVariable<uint>(doc, xpath, def, this);

        xpath = SPR_SETTINGS_SPECTRUM_RANGES_XPATH_PREFIX +
                QString("[@INDEX=")+QString::number(tIndex)+"]"+
                "/OblName[#"+QString::number((*_elProperty)[element].index)+"]" +
                SPR_SETTINGS_SPECTRUM_RANGES_XPATH_MIN_AGRUMENT;
        def = (*_elProperty)[element].min;
        this->elements[element].min = new SPRVariable<uint>(doc, xpath, def, this);
        this->elements[element].color = (*_elProperty)[element].color;

        this->elements[element].elIndex = (*_elProperty)[element].index;
    }

}

SPRSpectrumZonesModel::~SPRSpectrumZonesModel()
{
    foreach (EnumElements elem, elements.keys()) {
        delete elements[elem].max; elements[elem].max = nullptr;
        delete elements[elem].min; elements[elem].min = nullptr;
    }
    elements.clear();
}


