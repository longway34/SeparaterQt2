#include "sprmainmodel.h"
#include <QDir>

void SPRMainModel::setDoc(QDomDocument *value)
{
    doc = value;

    if(settingsMainModel) {delete settingsMainModel; settingsMainModel = nullptr;}
    settingsMainModel = new SPRSettingsMainModel(doc, this);


    server = new ServerConnect(settingsMainModel->ipAddress->getData(), settingsMainModel->ipPort->getData());

    if(settingsPorogsModel) {delete settingsPorogsModel; settingsPorogsModel = nullptr;}
    settingsPorogsModel = new SPRSettingsPorogsModel(doc, this);
    settingsPorogsModel->setThreads(settingsMainModel->getThreads());

    if(settingsRentgenModel) {delete settingsRentgenModel; settingsRentgenModel = nullptr;}
    settingsRentgenModel = new SPRSettingsRentgenModel(doc, this);
    settingsRentgenModel->setThreads(settingsMainModel->getThreads());
    settingsRentgenModel->setRentgens(settingsMainModel->getRentgens());

    if(settingsIMSModel) {delete settingsIMSModel; settingsIMSModel = nullptr;}
    settingsIMSModel = new SPRSettingsIMSModel(doc, this);
    settingsIMSModel->setIms(settingsMainModel->getIms());

    if(settingsControlModel) {delete settingsControlModel; settingsControlModel = nullptr;}
    settingsControlModel = new SPRSettingsControlModel(doc, this);

    if(settingsFormulaModel) {delete settingsFormulaModel; settingsFormulaModel = nullptr;}
    settingsFormulaModel = new SPRSettingsFormulaModel(doc, this);
    settingsFormulaModel->setCondition(settingsPorogsModel->getConditions());

    if(spectrumZonesTableModel) {delete spectrumZonesTableModel; spectrumZonesTableModel = nullptr;}
    spectrumZonesTableModel = new SPRSpectrumZonesTableModel(doc, this);
    spectrumZonesTableModel->setThreads(settingsMainModel->getThreads());

    if(spectrumListItemsModel) {delete spectrumListItemsModel; spectrumListItemsModel = nullptr;}
    spectrumListItemsModel = new SPRSpectrumListItemsModel(doc, this);
    spectrumListItemsModel->setFormulasModel(settingsFormulaModel);
    spectrumListItemsModel->setZonesModel(spectrumZonesTableModel);
}

SPRQStringVariable *SPRMainModel::getSpectrumFName() const
{
    return spectrumFName;
}

void SPRMainModel::setSpectrumFName(SPRQStringVariable *value)
{
    spectrumFName = value;
}



ServerConnect *SPRMainModel::getServer() const
{
    return server;
}

void SPRMainModel::setServer(ServerConnect *value)
{
    server = value;
}

SPRMainModel::SPRMainModel(QDomDocument *_doc, ISPRModelData *parent): ISPRModelData(_doc, parent),
    settingsControlModel(nullptr),
    settingsFormulaModel(nullptr),
    settingsIMSModel(nullptr),
    settingsMainModel(nullptr),
    settingsPorogsModel(nullptr),
    settingsRentgenModel(nullptr),
    spectrumZonesTableModel(nullptr),
    spectrumListItemsModel(nullptr),
    server(nullptr)
{
    setDoc(_doc);
}

SPRMainModel::SPRMainModel(QString docFName, ISPRModelData *parent): ISPRModelData(docFName, parent),
    settingsControlModel(nullptr),
    settingsFormulaModel(nullptr),
    settingsIMSModel(nullptr),
    settingsMainModel(nullptr),
    settingsPorogsModel(nullptr),
    settingsRentgenModel(nullptr),
    spectrumZonesTableModel(nullptr),
    spectrumListItemsModel(nullptr),
    server(nullptr)
{
    fPath = "";
    if(docFName.contains(QDir::separator())){
        QStringList fPathl = docFName.split(QDir::separator());
        while(fPathl.size()>0){
            fPath += fPathl.front(); fPathl.pop_front();
        }
    }
    bool y = QDir::setCurrent("F:\\Projects\\Separator\\");
    QDomDocument *doc;

    QFile in(docFName);
    if(in.open(QIODevice::ReadOnly)){
        if(document.setContent(&in)){
            doc = &document;
            in.close();
        }
    }
    setDoc(doc);
}

SPRMainModel::~SPRMainModel()
{
    if(settingsControlModel) delete settingsControlModel; settingsControlModel = nullptr;
    if(settingsFormulaModel) delete settingsFormulaModel; settingsFormulaModel = nullptr;
    if(settingsIMSModel) delete settingsIMSModel; settingsIMSModel = nullptr;
    if(settingsRentgenModel) delete settingsRentgenModel; settingsRentgenModel = nullptr;
    if(spectrumZonesTableModel) delete spectrumZonesTableModel; spectrumZonesTableModel = nullptr;
    if(settingsPorogsModel) delete settingsPorogsModel; settingsPorogsModel = nullptr;
    if(settingsMainModel) delete settingsMainModel; settingsMainModel = nullptr;

}

QDomDocument *SPRMainModel::getDoc() const
{
    return doc;
}
