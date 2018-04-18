#ifndef SPRELEMENTSMODEL_H
#define SPRELEMENTSMODEL_H

#include <QObject>
#include "models/isprmodeldata.h"
#include "models/sprqstringvariable.h"
#include "models/sprcolorvariable.h"
#include "models/sprvariable.h"
#include "models/sprelementvariable.h"

#define SPR_SETTINGS_SPECTRUM_RANDES_SNAME_XPATH_PREFIX "SEPARATOR/PROPERTY[@CODE=RANGES_NAMES2]/VALUE[@NAME=NAME"
#define SPR_SETTINGS_SPECTRUM_RANGES_FNAME_XPATH_PREFIX "FNAME="
#define SPR_SETTINGS_SPECTRUM_RANGES_COLOR_XPATH_PREFIX "COLOR="

#define SPR_SETTINGS_SPECTRUM_RANGES_XPATH_PREFIX "SEPARATOR/PROPERTY[@CODE=RANGES_NAMES]/VALUE"


class SPRElementsModel: public ISPRModelData
{
    struct elementsModelProperty{
        SPRElementVariable *key;
        SPRQStringVariable *sName;
        SPRQStringVariable *fName;
        SPRColorVariable *color;
    };

    QVector<elementsModelProperty> elements;

public:
    SPRElementsModel(QDomDocument *doc, ISPRModelData *parent=nullptr)
        :ISPRModelData(doc, parent)
    {
        for(int num=0; num < MAX_SPR_SPECTOR_ELEMENTS; num++){

            QString SName_XPATH = SPR_SETTINGS_SPECTRUM_RANDES_SNAME_XPATH_PREFIX + QString::number(num+1) + "]";
            QString key_XPATH = SPR_SETTINGS_SPECTRUM_RANDES_SNAME_XPATH_PREFIX"[@NAME=NAME"+QString::number(num+1)+"][KEY]";
            struct elementsModelProperty el;
            EnumElements _key = (EnumElements)num;
            el.key = new SPRElementVariable(doc, key_XPATH, _key, this);

            QString fName_XPATH = SPR_SETTINGS_SPECTRUM_RANDES_SNAME_XPATH_PREFIX"[@NAME=NAME"+QString::number(num+1)+"][FNAME]";
            el.fName = new SPRQStringVariable(doc, fName_XPATH, DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el.key->getData()].name, this);


            QString color_XPATH = SPR_SETTINGS_SPECTRUM_RANDES_SNAME_XPATH_PREFIX"[@NAME=NAME"+QString::number(num+1)+"][FNAME]";
            el.color = new SPRColorVariable(doc, color_XPATH, DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el.key->getData()].color, this);

            el.sName = new SPRQStringVariable(doc, SName_XPATH, DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el.key->getData()].sname, this);
            elements.push_back(el);
        }
    }


};

#endif // SPRELEMENTSMODEL_H
