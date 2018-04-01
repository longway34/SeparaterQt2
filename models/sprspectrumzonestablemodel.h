#ifndef SPRSPECTRUMZONESTABLEMODEL_H
#define SPRSPECTRUMZONESTABLEMODEL_H

#include <QObject>
#include <QDomDocument>

#include "models/isprmodeldata.h"
#include "models/sprvariable.h"
#include "models/sprspectrumzonesmodel.h"

class SPRSpectrumZonesTableModel : public ISPRModelData
{
    SPRVariable<uint> *threads;
public:
    QVector<SPRSpectrumZonesModel*> items;

    SPRSpectrumZonesTableModel();
    SPRSpectrumZonesTableModel(QDomDocument *_doc, ISPRModelData *parent = nullptr);
    ~SPRSpectrumZonesTableModel();

    SPRVariable<uint> *getThreads() const;
    void setThreads(SPRVariable<uint> *value);

    ElementsProperty *getElements(int _tIndex=0){
        if(items.size() > 0){
            if(_tIndex < 0 || _tIndex >= items.size()-1){
                _tIndex = 0;
            }
            return &items[_tIndex]->elements;
        } else {
            return nullptr;
        }
    }
};

#endif // SPRSPECTRUMZONESTABLEMODEL_H
