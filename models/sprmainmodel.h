#ifndef SPRMAINMODEL_H
#define SPRMAINMODEL_H

#include <QDomDocument>
//#include "models/models.h"
#include "models/sprsettingsmainmodel.h"
#include "models/sprsettingscontrolmodel.h"
#include "models/sprsettingsformulamodel.h"
#include "models/sprsettingsimsmodel.h"
#include "models/sprsettingsporogsmodel.h"
#include "models/sprsettingsrentgenmodel.h"
#include "models/sprspectrumzonestablemodel.h"
#include "models/sprspectrumlistitemsmodel.h"
#include "models/sprelementsmodel.h"

#include "tcp/ServerConnect.h"

class SPRMainModel : public ISPRModelData
{
    QDomDocument *doc;
    
    QString fName;
    QString fPath;

    ServerConnect *server;

    SPRQStringVariable *spectrumFName;


    SPRElementsModel *elements;

    SPRSettingsMainModel        *settingsMainModel;
    SPRSettingsControlModel     *settingsControlModel;
    SPRSettingsFormulaModel     *settingsFormulaModel;
    SPRSettingsIMSModel         *settingsIMSModel;
    SPRSettingsPorogsModel      *settingsPorogsModel;
    SPRSettingsRentgenModel     *settingsRentgenModel;
    SPRSpectrumZonesTableModel  *spectrumZonesTableModel;
    SPRSpectrumListItemsModel   *spectrumListItemsModel;

//    SPRSettingsRMTAutosetModel  *settingsAutoModel;
//    SPRSettingsSpectrumRangesModel *settingsSpectrumRangersModel;

public:
    SPRMainModel(QObject *parent=nullptr);
    SPRMainModel(QDomDocument *_doc, ISPRModelData *parent=nullptr);
    SPRMainModel(QString docFName, ISPRModelData *parent=nullptr);
    virtual ~SPRMainModel();

    SPRSettingsControlModel *getSettingsControlModel(){ return settingsControlModel;}
    SPRSettingsFormulaModel *getSettingsFormulaModel(){ return settingsFormulaModel;}
    SPRSettingsIMSModel *getSettingsIMSModel() { return settingsIMSModel;}
    SPRSettingsMainModel *getSettingsMainModel(){ return settingsMainModel;}
    SPRSettingsPorogsModel *getSettingsPorogsModel(){ return settingsPorogsModel;}
    SPRSettingsRentgenModel *getSettingsRentgenModel(){ return settingsRentgenModel;}
    SPRSpectrumZonesTableModel *getSpectrumZonesTableModel(){ return spectrumZonesTableModel;}
    SPRSpectrumListItemsModel *getSpectrumListItemsModel(){ return spectrumListItemsModel; }
    //    SPRSettingsRMTAutosetModel *getSettingsAutoModel() const;
//    SPRSettingsSpectrumRangesModel *getSettingsSpectrumRangesModel() const;
    
    // ISPRModelData interface
    QDomDocument *getDoc() const;
    void setDoc(QDomDocument *value);

    SPRQStringVariable *getSpectrumFName() const;
    void setSpectrumFName(SPRQStringVariable *value);
    ServerConnect *getServer() const;
    void setServer(ServerConnect *value);
};





#endif // SPRMAINMODEL_H
