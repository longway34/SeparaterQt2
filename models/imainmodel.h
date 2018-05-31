#ifndef IMAINMODEL_H
#define IMAINMODEL_H

#include <QObject>
#include <QDomDocument>

#include "models/isprmodeldata.h"
#include "models/sprsettingsmainmodel.h"
#include "models/sprsettingscontrolmodel.h"
#include "models/sprsettingsformulamodel.h"
#include "models/sprsettingsimsmodel.h"
#include "models/sprsettingsporogsmodel.h"
#include "models/sprsettingsrentgenmodel.h"
#include "models/sprspectrumzonestablemodel.h"
#include "models/sprspectrumlistitemsmodel.h"
#include "models/sprelementsmodel.h"
//#include "models/sprseparatemodel.h"

#include "variables/sprvariable.h"
#include "variables/sprenumvariable.h"

class IMainModel /*: public ISPRModelData*/
{
public:
//    IMainModel(QObject *parent=nullptr);

//    IMainModel(QDomDocument *_doc, ISPRModelData *parent):
//        ISPRModelData(doc, parent){}
//    IMainModel(QString docFName, ISPRModelData *parent):
//        ISPRModelData(docFName, parent){}

    virtual SPRSettingsControlModel *getSettingsControlModel()=0;
    virtual SPRSettingsFormulaModel *getSettingsFormulaModel()=0;
    virtual SPRSettingsIMSModel *getSettingsIMSModel()=0;
    virtual SPRSettingsMainModel *getSettingsMainModel()=0;
    virtual SPRSettingsPorogsModel *getSettingsPorogsModel()=0;
    virtual SPRSettingsRentgenModel *getSettingsRentgenModel()=0;
    virtual SPRSpectrumZonesTableModel *getSpectrumZonesTableModel()=0;
    virtual SPRSpectrumListItemsModel *getSpectrumListItemsModel()=0;
//    virtual SPRSeparateModel *getSeparateModel()=0;

    SPRVariable<uint> *getRentgens(){ return getSettingsMainModel()->getRentgens();}
    SPRVariable<uint> *getThreads(){ return getSettingsMainModel()->getThreads();}
    SPRVariable<uint> *getIms(){ return getSettingsMainModel()->getIms();}

    SPREnumVariable<TypeConditions> *getConditions(){ return getSettingsPorogsModel()->getConditions();}
    SPREnumVariable<TypeSelection> *getTypeSelection(){ return getSettingsPorogsModel()->getTypeSelection();}
};

#endif // IMAINMODEL_H
