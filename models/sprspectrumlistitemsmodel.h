#ifndef SPRSPECTRUMLISTITEMSMODEL_H
#define SPRSPECTRUMLISTITEMSMODEL_H

#include <QObject>
#include <QDomDocument>

#include "isprmodeldata.h"
//#include "models/models.h"
#include "models/sprspectrumzonestablemodel.h"
#include "models/sprsettingsformulamodel.h"
#include "models/sprspectrumitemmodel.h"
//#include "models/sprmainmodel.h"

class SPRSpectrumListItemsModel : public ISPRModelData
{
    Q_OBJECT

    SPRSpectrumZonesTableModel *zonesTableModel;
    SPRSettingsFormulaModel *formulas;
    QVector<SPRSpectrumItemModel*> spectrumsModel;

public:
    SPRSpectrumListItemsModel(ISPRModelData *parent = nullptr): formulas(nullptr), zonesTableModel(nullptr){}
    SPRSpectrumListItemsModel(QDomDocument *_doc, ISPRModelData *parent = nullptr);
    SPRSpectrumListItemsModel(SPRSpectrumZonesTableModel *_model, SPRSettingsFormulaModel *_formulas, ISPRModelData *parent = nullptr);
    virtual ~SPRSpectrumListItemsModel();

    ISPRModelData *setZonesModel(SPRSpectrumZonesTableModel *_model);
    ISPRModelData *setFormulasModel(SPRSettingsFormulaModel *_formulas);
    ISPRModelData *setModel(SPRSpectrumZonesTableModel *_zones, SPRSettingsFormulaModel *_formulas);

    QVector<SPRSpectrumItemModel *> *getSpectrumsModel();
    SPRSpectrumItemModel *getSpectrumItem(int index){
        if(index < 0 || index >= spectrumsModel.size()){
            return nullptr;
        } else {
            return spectrumsModel[index];
        }
    }
    SPRSpectrumZonesTableModel *getZonesTableModel();
    
    SPRSpectrumItemModel *addSpectrum(uint8_t *buf, int bufLentgh);
    SPRSpectrumItemModel *addSpectrum(QByteArray buf){
        addSpectrum((uint8_t*)buf.constData(), buf.size());
    }
    SPRSettingsFormulaModel *getFormulas() const;
    void clearGraphicsItemModel(){
        spectrumsModel.clear();
    }
};

#endif // SPRSPECTRUMLISTITEMSMODEL_H
