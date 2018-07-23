#include "sprmainmodel.h"
#include <QDir>

void SPRMainModel::setDoc(QDomDocument *value)
{
    doc = value;

    elementsModel = new SPRElementsModel(doc, this);

    if(settingsMainModel) {delete settingsMainModel; settingsMainModel = nullptr;}
    settingsMainModel = new SPRSettingsMainModel(doc, this);

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
    settingsFormulaModel = new SPRSettingsFormulaModel(doc, elementsModel, settingsPorogsModel->getConditions(), this);
//    settingsFormulaModel->setCondition(settingsPorogsModel->getConditions());

    if(spectrumZonesTableModel) {delete spectrumZonesTableModel; spectrumZonesTableModel = nullptr;}
    spectrumZonesTableModel = new SPRSpectrumZonesTableModel(doc, elementsModel, this);
    spectrumZonesTableModel->setThreads(settingsMainModel->getThreads());

    if(spectrumListItemsModel) {delete spectrumListItemsModel; spectrumListItemsModel = nullptr;}
    spectrumListItemsModel = new SPRSpectrumListItemsModel(spectrumZonesTableModel, settingsFormulaModel, settingsMainModel->getThreads(), settingsMainModel->getSpectrumFileName(), settingsControlModel->controlArea, this);
//    spectrumListItemsModel->setSpectrumsfName(settingsMainModel->getSpectrumFileName());

    if(separateModel){delete separateModel; separateModel = nullptr;}
    separateModel = new SPRSeparateModel(doc, this);

    emit modelChanget(this);
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
    return settingsMainModel->getServer();
}

void SPRMainModel::setServer(ServerConnect *value)
{
    settingsMainModel->setServer(value);
}

SPRSeparateModel *SPRMainModel::getSeparateModel() const
{
    return separateModel;
}

void SPRMainModel::setSeparateModel(SPRSeparateModel *value)
{
    separateModel = value;
}

void SPRMainModel::setFName(const QString &value)
{

    fileName = value;
}

SPRElementsModel *SPRMainModel::getElementsModel() const
{
    return elementsModel;
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
    elementsModel(nullptr),
//    server(nullptr),
    separateModel(nullptr)
{
    setObjectName("MainModel");
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
    elementsModel(nullptr),
//    server(nullptr),
    separateModel(nullptr)
{
    setObjectName("MainModel");
    filePath = "";
    if(docFName.contains(QDir::separator())){
        QStringList fPathl = docFName.split(QDir::separator());
        while(fPathl.size()>0){
            filePath += fPathl.front(); fPathl.pop_front();
        }
    }
    bool y = QDir::setCurrent("F:\\Projects\\Separator\\");
//    QDomDocument *doc;

    QFile in(docFName);
    if(in.open(QIODevice::ReadOnly)){
        if(document.setContent(&in)){
            doc = &document;
            in.close();
        }
    }
    if(doc){
        setDoc(doc);
        setFName(docFName);
    }
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
    if(elementsModel) delete elementsModel; elementsModel = nullptr;
    if(separateModel) delete separateModel; separateModel = nullptr;
}

QDomDocument *SPRMainModel::getDoc() const
{
    return doc;
}
