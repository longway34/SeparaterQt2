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
#include "variables/sprenumvariable.h"
//#include "models/sprmainmodel.h"


typedef enum typeSpectrumsData{
    spectrumsOnly, spectrumBase, spectrumsAll, spectrumsMixed
} SPRTypeSpectrumSet;

class SPRSpectrumListItemsModel : public ISPRModelData
{
    Q_OBJECT

    SPRVariable<uint> *threads;
    SPRQStringVariable *spectrumsfName;
    SPREnumVariable<EnumElements> *corelControlArea;

    SPRSpectrumZonesTableModel *zonesTableModel;
    SPRElementsModel *elementsProperty;
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
    SPRSpectrumItemModel *addSpect(uint8_t *buf, int bufLentgh, uint32_t _timeScope_in_ms, SPRTypeSpectrumSet typeData = spectrumsOnly, int numTh = -1, QString _formatName="");
    void setSpectrums();

    QVector<SPRSpectrumItemModel*> *unionModels(){
        spectrumsModelAll = spectrumsModelBase + spectrumsModel;
        return &spectrumsModelAll;
    }
    void recomplite(SPRSpectrumItemModel *item, typeSpectrumsData type = spectrumsOnly);
public:
    SPRSpectrumListItemsModel(): ISPRModelData(), threads(0), spectrumsfName(0), corelControlArea(nullptr), zonesTableModel(nullptr), elementsProperty(nullptr), formulas(nullptr), spectrumsModel(){}
    SPRSpectrumListItemsModel(QDomDocument *_doc, SPRElementsModel *_elements = nullptr, SPREnumVariable<EnumElements> *_controlArea = nullptr, ISPRModelData *parent = nullptr);
//    SPRSpectrumListItemsModel(SPRMainModel *_mainModel, ISPRModelData *parent = nullptr);

    QVector<SPRSpectrumItemModel *> *getSpectrumsModelBase();

    SPRSpectrumListItemsModel(SPRSpectrumZonesTableModel *_model, SPRSettingsFormulaModel *_formulas, SPRVariable<uint> *_threads, SPRQStringVariable *sfName, SPREnumVariable<EnumElements> *_controlArea, ISPRModelData *parent = nullptr);
    virtual ~SPRSpectrumListItemsModel();

    ISPRModelData *setZonesModel(SPRSpectrumZonesTableModel *_model);
    ISPRModelData *setFormulasModel(SPRSettingsFormulaModel *_formulas);
    ISPRModelData *setModel(SPRSpectrumZonesTableModel *_zones, SPRSettingsFormulaModel *_formulas, SPRVariable<uint> *_threads, SPRQStringVariable *_sfName, SPREnumVariable<EnumElements> *_controlArea);
    ISPRModelData *setModel(SPRSpectrumListItemsModel *_model){
        if(_model && _model !=this){
            zonesTableModel = _model->getZonesTableModel();
            formulas = _model->getFormulas();
            spectrumsModel = *_model->getSpectrumsModel();
            spectrumsModelBase = *_model->getSpectrumsModelBase();
            spectrumsfName = _model->getSpectrumsfName();
            threads = _model->getThreads();
            setCorelControlArea(_model->getCorelControlArea());
        }
        return this;
    }
    void removeSpectrum(int num, SPRTypeSpectrumSet typeData=spectrumsOnly){
        if(typeData == spectrumsOnly){
            if(num >=0 && num <spectrumsModel.size()){
                spectrumsModel.remove(num);
                emit modelChanget(this);
            }
        } else if(typeData == spectrumBase){
            if(num >=0 && num < spectrumsModelBase.size()){
                spectrumsModelBase.remove(num);
                emit modelChanget(this);
            }
        } else {
            if(num >=0 && num < this->unionModels()->size()){
                unionModels()->remove(num);
                emit modelChanget(this);
            }
        }
    }

//    SPRSpectrumItemModel *setSpectrumItem(SPRSpectrumItemModel* item, int th, SPRTypeSpectrumSet type = spectrumBase);

    SPRSpectrumItemModel *setSpectrumData(int num, QByteArray data, SPRTypeSpectrumSet _type = spectrumBase){
        return setSpectrumData(num, (uint8_t*)data.constData(), data.size(), _type);
    }

    SPRSpectrumItemModel *setSpectrumData(int num, uint8_t *buf, int bufLen, SPRTypeSpectrumSet _type=spectrumBase, uint32_t _timeScope_in_msec = 0, QString _formatName = "");

    QVector<SPRSpectrumItemModel *> *getSpectrumsModel(SPRTypeSpectrumSet type=spectrumsOnly);

    SPRSpectrumItemModel *getSpectrumItem(int index, SPRTypeSpectrumSet type){
        if(type == spectrumsOnly){
            return getItem(index, &spectrumsModel);
        } else if(type == spectrumBase){
            return getItem(index, &spectrumsModelBase);
        } else if(type == spectrumsAll){
            unionModels();
            return getItem(index, &spectrumsModelAll);
        } else {
            return nullptr;
        }
    }


    QVector<SPRSpectrumItemModel*> getSpectrumsItemByThread(int thread=-1, SPRTypeSpectrumSet type = spectrumsOnly);

    SPRSpectrumItemModel *getSpectrumItem(int index){
        return getItem(index, &spectrumsModel);
    }

    SPRSpectrumItemModel *getSpectrumBaseItem(int index){
        return getItem(index, &spectrumsModelBase);
    }

    SPRSpectrumZonesTableModel *getZonesTableModel();
    
    void addSpectrums(QString fName);
    SPRSpectrumItemModel *addSpectrum(uint8_t *buf, int bufLentgh, uint32_t _timeScope_in_msec, int numTh = -1, QString _formatName = ""){
        return addSpect(buf, bufLentgh, _timeScope_in_msec, spectrumsOnly, numTh, _formatName);
    }
    SPRSpectrumItemModel *addSpectrum(QByteArray buf, uint32_t _timeScope_in_ms, int numTh = -1, QString _formatName=""){
        return addSpect((uint8_t*)buf.constData(), buf.size(), _timeScope_in_ms, spectrumsOnly, numTh, _formatName);
    }
    SPRSpectrumItemModel *addSpectrumBase(uint8_t *buf, int bufLentgh, uint32_t  _timeScope, int numTh = -1, QString _formatName=""){
        return addSpect(buf, bufLentgh, _timeScope, spectrumBase, numTh, _formatName);
    }
    SPRSpectrumItemModel *addSpectrumBase(QByteArray buf, uint32_t  _timeScope, int numTh = -1, QString _formatName=""){
        return addSpectrumBase((uint8_t*)buf.constData(), buf.size(), _timeScope, numTh, _formatName);
    }

    SPRSettingsFormulaModel *getFormulas() const;
    void clearGraphicsItemModel(){
        clearSpectrums();
    }
    void clearSpectrums();
    void clearSpectrumsBase();

    SPRSpectrumItemModel *getBaseItemForChannel(int ch){
        SPRSpectrumItemModel *ret = nullptr;
        QVector<SPRSpectrumItemModel*> *model = getSpectrumsModel(spectrumBase);

        if(ch < 0 || ch > getThreads()->getData()){
            ch = getThreads()->getData() % abs(ch);
        }

        for(int i=0; i<model->size(); i++){
            if(model->at(i)->getSpectrumThread() == ch){
               ret = model->at(i);
               break;
            }
        }
        return ret;
    }


    SPRQStringVariable *getSpectrumsfName() const;

    void setSpectrumsfName(SPRQStringVariable *value);

    SPRVariable<uint> *getThreads() const;
    void setThreads(SPRVariable<uint> *value);

    QVector<SPRSpectrumItemModel *> *getSpectrumsModelAll();

    void setElementsProperty(SPRElementsModel *value);

    void setCorelControlArea(SPREnumVariable<EnumElements> *value);

    SPREnumVariable<EnumElements> *getCorelControlArea() const;

    void recomplite();
    void clearSpectrums(SPRTypeSpectrumSet type);
signals:
    //    void modelChanget();

public slots:
    virtual void saveAs(QString fname="");

};

#endif // SPRSPECTRUMLISTITEMSMODEL_H
