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
    spectrumListItemsModel = new SPRSpectrumListItemsModel(this, nullptr, false, this);
//    spectrumListItemsModel->setSpectrumsfName(settingsMainModel->getSpectrumFileName());

    kSpectrums = new SPRSpectrumListItemsModel(this,
                        spectrumListItemsModel->getSpectrumsModelBase(), true, nullptr);

    if(separateModel){delete separateModel; separateModel = nullptr;}
    separateModel = new SPRSeparateModel(doc, this, nullptr);

    if(separateOutputModel){delete separateOutputModel; separateOutputModel = nullptr;}
    separateOutputModel = new SPRSeparateOutputModel(this, this->getThreadsList(), nullptr);

    if(historyModel){delete historyModel; historyModel = nullptr;}
    historyModel = new SPRHistoryModel(this, kSpectrums);

    emit modelChanget(this);
}

SPRQStringVariable *SPRMainModel::getSpectrumFName()
{
    return settingsMainModel->getSpectrumFileName();
}

void SPRMainModel::setSpectrumFName(QString value)
{
    settingsMainModel->spectrumFileName->setData(value);
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

SPRSeparateOutputModel *SPRMainModel::getSeparateOutputModel() const
{
    return separateOutputModel;
}

SPRThreadList SPRMainModel::getThreadsList()
{
    SPRThreadList res;
    if(getThreads())
        for(uint8_t th = 0; th<getThreads()->getData(); th++){
            res << th;
        }
    else
        for(uint8_t th = 0; th<MAX_SPR_MAIN_THREADS; th++){
            res << th;
        }

    return res;
}

SPRSpectrumListItemsModel *SPRMainModel::getKSpectrums() const
{
    return kSpectrums;
}

SPRHistoryModel *SPRMainModel::getHistoryModel() const
{
    return historyModel;
}

SPRMainModel::SPRMainModel(QDomDocument *_doc, ISPRModelData *parent): ISPRModelData(_doc, parent),
//    spectrumFName(nullptr),
    settingsMainModel(nullptr),
    settingsControlModel(nullptr),
    settingsFormulaModel(nullptr),
    settingsIMSModel(nullptr),
    settingsPorogsModel(nullptr),
    settingsRentgenModel(nullptr),
    spectrumZonesTableModel(nullptr),
    spectrumListItemsModel(nullptr),
    kSpectrums(nullptr),
    elementsModel(nullptr),
//    server(nullptr),
    separateModel(nullptr),
    separateOutputModel(nullptr),
    historyModel(nullptr)
{
    setObjectName("MainModel");
    setDoc(_doc);
}

SPRMainModel::SPRMainModel(QString docFName, ISPRModelData *parent): ISPRModelData(docFName, parent),
//    spectrumFName(nullptr),
    settingsMainModel(nullptr),
    settingsControlModel(nullptr),
    settingsFormulaModel(nullptr),
    settingsIMSModel(nullptr),
    settingsPorogsModel(nullptr),
    settingsRentgenModel(nullptr),
    spectrumZonesTableModel(nullptr),
    spectrumListItemsModel(nullptr),
    kSpectrums(nullptr),
    elementsModel(nullptr),
//    server(nullptr),
    separateModel(nullptr),
    separateOutputModel(nullptr),
    historyModel(nullptr)

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
    if(separateOutputModel) delete separateOutputModel; separateOutputModel = nullptr;
    if(historyModel) delete historyModel; historyModel = nullptr;
//    if(spectrumFName) delete spectrumFName; spectrumFName = nullptr;
}

QDomDocument *SPRMainModel::getDoc() const
{
    return doc;
}
