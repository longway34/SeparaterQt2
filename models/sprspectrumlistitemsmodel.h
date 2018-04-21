#ifndef SPRSPECTRUMLISTITEMSMODEL_H
#define SPRSPECTRUMLISTITEMSMODEL_H

#include <QObject>
#include <QDomDocument>

#include "isprmodeldata.h"
//#include "models/models.h"
#include "models/sprspectrumzonestablemodel.h"
#include "models/sprsettingsformulamodel.h"
#include "models/sprspectrumitemmodel.h"
#include "variables/sprqstringvariable.h"
//#include "models/sprmainmodel.h"


typedef enum typeSpectrumsData{
    spectrumsOnly, spectrumBase, spectrumsAll
} SPRTypeSpectrumSet;

class SPRSpectrumListItemsModel : public ISPRModelData
{
    Q_OBJECT

    SPRVariable<uint> *threads;
    SPRQStringVariable *spectrumsfName;

    SPRSpectrumZonesTableModel *zonesTableModel;
    SPRSettingsFormulaModel *formulas;
    QVector<SPRSpectrumItemModel*> spectrumsModel;
    QVector<SPRSpectrumItemModel*> spectrumsModelBase;
    QVector<SPRSpectrumItemModel*> spectrumsModelAll;
protected:
    SPRSpectrumItemModel *getItem(int index, QVector<SPRSpectrumItemModel*> *model){
        if(index < 0 || index >= model->size()){
            return nullptr;
        } else {
            return model->at(index);
        }
    }

    void clearSpectrums(QVector<SPRSpectrumItemModel *> *model);
    SPRSpectrumItemModel *addSpect(uint8_t *buf, int bufLentgh, QVector<SPRSpectrumItemModel *> *model);
    void setSpectrums();

    void unionModels(){
        spectrumsModelAll = spectrumsModelBase + spectrumsModel;
    }
public:
    SPRSpectrumListItemsModel(): ISPRModelData(), threads(0), spectrumsfName(0), zonesTableModel(nullptr), formulas(nullptr), spectrumsModel(){}
    SPRSpectrumListItemsModel(QDomDocument *_doc, ISPRModelData *parent = nullptr);
//    SPRSpectrumListItemsModel(SPRMainModel *_mainModel, ISPRModelData *parent = nullptr);

    QVector<SPRSpectrumItemModel *> *getSpectrumsModelBase();

    SPRSpectrumListItemsModel(SPRSpectrumZonesTableModel *_model, SPRSettingsFormulaModel *_formulas, SPRVariable<uint> *_threads, SPRQStringVariable *sfName, ISPRModelData *parent = nullptr);
    virtual ~SPRSpectrumListItemsModel();

    ISPRModelData *setZonesModel(SPRSpectrumZonesTableModel *_model);
    ISPRModelData *setFormulasModel(SPRSettingsFormulaModel *_formulas);
    ISPRModelData *setModel(SPRSpectrumZonesTableModel *_zones, SPRSettingsFormulaModel *_formulas, SPRVariable<uint> *_threads, SPRQStringVariable *_sfName);
    ISPRModelData *setModel(SPRSpectrumListItemsModel *_model){
        if(_model && _model !=this){
            zonesTableModel = _model->getZonesTableModel();
            formulas = _model->getFormulas();
            spectrumsModel = *_model->getSpectrumsModel();
            spectrumsModelBase = *_model->getSpectrumsModelBase();
            spectrumsfName = _model->getSpectrumsfName();
            threads = _model->getThreads();
        }
        return this;
    }

    SPRSpectrumItemModel *setSpectrumData(int num, QByteArray data, SPRTypeSpectrumSet _type = spectrumBase){
        return setSpectrumData(num, (uint8_t*)data.constData(), data.size(), _type);
    }

    SPRSpectrumItemModel *setSpectrumData(int num, uint8_t *buf, int bufLen, SPRTypeSpectrumSet _type=spectrumBase);

    QVector<SPRSpectrumItemModel *> *getSpectrumsModel();


    SPRSpectrumItemModel *getSpectrumItem(int index){
        return getItem(index, &spectrumsModel);
    }

    SPRSpectrumItemModel *getSpectrumBaseItem(int index){
        return getItem(index, &spectrumsModelBase);
    }

    SPRSpectrumZonesTableModel *getZonesTableModel();
    
    void addSpectrums(QString fName);
    SPRSpectrumItemModel *addSpectrum(uint8_t *buf, int bufLentgh){
        return addSpect(buf, bufLentgh, &spectrumsModel);
    }
    SPRSpectrumItemModel *addSpectrum(QByteArray buf){
        return addSpectrum((uint8_t*)buf.constData(), buf.size());
    }
    SPRSpectrumItemModel *addSpectrumBase(uint8_t *buf, int bufLentgh){
        return addSpect(buf, bufLentgh, &spectrumsModelBase);
    }
    SPRSpectrumItemModel *addSpectrumBase(QByteArray buf){
        return addSpectrumBase((uint8_t*)buf.constData(), buf.size());
    }

    SPRSettingsFormulaModel *getFormulas() const;
    void clearGraphicsItemModel(){
        clearSpectrums();
    }
    void clearSpectrums();
    void clearSpectrumsBase();

    SPRQStringVariable *getSpectrumsfName() const;

    void setSpectrumsfName(SPRQStringVariable *value);

    SPRVariable<uint> *getThreads() const;
    void setThreads(SPRVariable<uint> *value);

    QVector<SPRSpectrumItemModel *> *getSpectrumsModelAll();

signals:
    void modelChanget();

public slots:
    virtual void saveAs(QString fname="");

};

#endif // SPRSPECTRUMLISTITEMSMODEL_H
