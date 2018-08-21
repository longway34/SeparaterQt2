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

class SPRGraphItem;

typedef enum typeSpectrumsData{
    spectrumsOnly, spectrumBase, spectrumsAll, spectrumsMixed
} SPRTypeSpectrumSet;

class SPRMainModel;

class SPRSpectrumListItemsModel : public ISPRModelData
{
    Q_OBJECT

    SPRVariable<uint> *threads;
    SPRQStringVariable *spectrumsfName;
    SPREnumVariable<EnumElements> *corelControlArea;

    SPRSpectrumZonesTableModel *zonesTableModel;
    SPRElementsModel *elementsProperty;
    SPRSettingsFormulaModel *formulas;
    QList<SPRSpectrumItemModel*> spectrumsModel;
    QList<SPRSpectrumItemModel*> spectrumsModelBase;
    QList<SPRSpectrumItemModel*> spectrumsModelAll;
protected:
    void init(){

    }


    SPRSpectrumItemModel *getItem(int index, QList<SPRSpectrumItemModel*> *model);

    void clearSpectrums(QList<SPRSpectrumItemModel *> *model);
    SPRSpectrumItemModel *addSpect(uint8_t *buf, int bufLentgh, uint32_t _timeScope_in_ms, SPRTypeSpectrumSet typeData = spectrumsOnly, int numTh = -1, QString _formatName="");
    void setSpectrums();

    QList<SPRSpectrumItemModel*> *unionModels();
    void recomplite(SPRSpectrumItemModel *item, typeSpectrumsData type = spectrumsOnly);
public:
//    SPRSpectrumListItemsModel(): ISPRModelData(), threads(0), spectrumsfName(0), corelControlArea(nullptr), zonesTableModel(nullptr), elementsProperty(nullptr), formulas(nullptr), spectrumsModel(){}
    SPRSpectrumListItemsModel(QDomDocument *_doc, SPRElementsModel *_elements = nullptr, SPREnumVariable<EnumElements> *_controlArea = nullptr, ISPRModelData *parent = nullptr);
//    SPRSpectrumListItemsModel(SPRMainModel *_mainModel, ISPRModelData *parent = nullptr);

    QList<SPRSpectrumItemModel *> *getSpectrumsModelBase();
    void setGraphicMode(SPRViewGraphicsMode mode);

    SPRSpectrumListItemsModel(SPRMainModel *_model, QList<SPRSpectrumItemModel*> *_base = nullptr, bool selfModel = true, ISPRModelData *parent = nullptr);
    virtual ~SPRSpectrumListItemsModel();

    ISPRModelData *setZonesModel(SPRSpectrumZonesTableModel *_model);
    ISPRModelData *setFormulasModel(SPRSettingsFormulaModel *_formulas);
    ISPRModelData *setModel(SPRSpectrumZonesTableModel *_zones, SPRSettingsFormulaModel *_formulas, SPRVariable<uint> *_threads, SPRQStringVariable *_sfName, SPREnumVariable<EnumElements> *_controlArea);
    ISPRModelData *setModel(SPRSpectrumListItemsModel *_model);
    void removeSpectrum(SPRSpectrumItemModel* _item);

    void removeSpectrum(int num, SPRTypeSpectrumSet typeData=spectrumsOnly);

//    SPRSpectrumItemModel *setSpectrumItem(SPRSpectrumItemModel* item, int th, SPRTypeSpectrumSet type = spectrumBase);

    SPRSpectrumItemModel *setSpectrumData(int num, QByteArray data, SPRTypeSpectrumSet _type=spectrumBase, uint32_t _timeScope_in_msec = 0, QString _formatName = "");
    SPRSpectrumItemModel *setSpectrumData(int num, uint8_t *buf, int bufLen, SPRTypeSpectrumSet _type=spectrumBase, uint32_t _timeScope_in_msec = 0, QString _formatName = "");

    QList<SPRSpectrumItemModel *> *getSpectrumsModel(SPRTypeSpectrumSet type=spectrumsOnly);

    SPRSpectrumItemModel *getSpectrumItem(int index, SPRTypeSpectrumSet type);


    QList<SPRSpectrumItemModel*> getSpectrumsItemByThread(int thread=-1, SPRTypeSpectrumSet type = spectrumsOnly);

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

    SPRSpectrumItemModel *getBaseItemForChannel(int ch);


    SPRQStringVariable *getSpectrumsfName() const;

    void setSpectrumsfName(SPRQStringVariable *value);

    SPRVariable<uint> *getThreads() const;
    void setThreads(SPRVariable<uint> *value);

    QList<SPRSpectrumItemModel *> *getSpectrumsModelAll();

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
