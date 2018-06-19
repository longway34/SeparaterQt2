#include "sprelementsmodel.h"


void SPRElementsModel::deleteElement(EnumElements element){
    SPRElementsProperty *el = changeElementPropery(element, QString::number(static_cast<int>(element)));
    if(el){
        blockSignals(true);
        if(elements.contains(element)){
            elements.remove(element);
        }
        if(unisedElements.contains(element)){
            unisedElements.remove(element);
        }
        unisedElements[element] = el;
        blockSignals(false);
        emit modelChanget(unisedElements[element]->key);
    }
}
