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
    SPRSpectrumListItemsModel(ISPRModelData *parent = nullptr): zonesTableModel(nullptr), formulas(nullptr), spectrumsModel(){}
    SPRSpectrumListItemsModel(QDomDocument *_doc, ISPRModelData *parent = nullptr);
//    SPRSpectrumListItemsModel(SPRMainModel *_mainModel, ISPRModelData *parent = nullptr);

    SPRSpectrumListItemsModel(SPRSpectrumZonesTableModel *_model, SPRSettingsFormulaModel *_formulas, ISPRModelData *parent = nullptr);
    virtual ~SPRSpectrumListItemsModel();

    ISPRModelData *setZonesModel(SPRSpectrumZonesTableModel *_model);
    ISPRModelData *setFormulasModel(SPRSettingsFormulaModel *_formulas);
    ISPRModelData *setModel(SPRSpectrumZonesTableModel *_zones, SPRSettingsFormulaModel *_formulas, uint8_t* inp=nullptr, int len=DEF_SPECTRUM_DATA_BUF_LENGTH);
    ISPRModelData *setModel(SPRSpectrumListItemsModel *_model){
        if(_model && _model !=this){
            zonesTableModel = _model->getZonesTableModel();
            formulas = _model->getFormulas();
            spectrumsModel = *_model->getSpectrumsModel();
        }
    }

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
        clearSpectrums();
    }
    void *clearSpectrums();
    SPRSpectrumItemModel *setSpectrum(uint8_t *buf, int bufLentgh);

signals:
    void modelChanget();
};

#endif // SPRSPECTRUMLISTITEMSMODEL_H
