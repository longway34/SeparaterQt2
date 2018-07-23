#ifndef SPRSPECTRUMZONESTABLEMODEL_H
#define SPRSPECTRUMZONESTABLEMODEL_H

#include <QObject>
#include <QDomDocument>

#include "models/isprmodeldata.h"
#include "models/sprelementsmodel.h"
#include "variables/sprvariable.h"
#include "models/sprspectrumzonesmodel.h"


class SPRSpectrumZonesTableModel : public ISPRModelData
{
    SPRVariable<uint> *threads;

    SPRSpectrumZonesModel *getItem(int _tIndex){
        if(_tIndex < 0 || _tIndex >= items.size()-1){
            _tIndex = 0;
        }
        return items[_tIndex];
    }
public:
    QVector<SPRSpectrumZonesModel*> items;
    SPRElementsModel *elementsProperty;

    SPRSpectrumZonesTableModel();
    SPRSpectrumZonesTableModel(QDomDocument *_doc, SPRElementsModel *elements = nullptr, ISPRModelData *parent = nullptr);
    ~SPRSpectrumZonesTableModel();

    SPRVariable<uint> *getThreads() const;
    void setThreads(SPRVariable<uint> *value);

    QMapElementsRanges getElementsRanges(uint _tIndex=-1){
        return getItem(_tIndex)->getZones();
    }
    SpectorRange* getElementsRanges(int _tIndex, EnumElements _el){
        SpectorRange* ranges = nullptr;
        QMapElementsRanges mranges = getItem(_tIndex)->getZones();
        if(mranges.contains(_el)){
            ranges = mranges[_el];
        }
        return ranges;
    }
    SPRElementsModel *getElementsProperty(){
        return elementsProperty;
    }
    void setElementsProperty(SPRElementsModel *value);

    void setMinMaxValues(EnumElements el, uint _min, uint _max, int tIndex=-1){
        if(tIndex < 0){
            for(int th=0; th<items.size(); th++){
                items[th]->setMinMax(el, _min, _max);
            }
        } else {
            if(tIndex >= items.size()-1){
                tIndex = items.size()-1;
            }
            items[tIndex]->setMinMax(el, _min, _max);
        }
    }
};

#endif // SPRSPECTRUMZONESTABLEMODEL_H
