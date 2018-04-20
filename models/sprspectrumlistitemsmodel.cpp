#include "sprspectrumlistitemsmodel.h"

static const QVector<QColor> mainColors = {Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow,
                                    Qt::darkRed, Qt::darkBlue, Qt::darkGreen, Qt::darkCyan, Qt::darkMagenta, Qt::darkYellow,
                                    Qt::gray, Qt::lightGray, Qt::white, Qt::darkGray};

QVector<SPRSpectrumItemModel *> *SPRSpectrumListItemsModel::getSpectrumsModel()
{
    return &spectrumsModel;
}

SPRSpectrumZonesTableModel *SPRSpectrumListItemsModel::getZonesTableModel()
{
    return zonesTableModel;
}

QVector<SPRSpectrumItemModel *> *SPRSpectrumListItemsModel::getSpectrumsModelAll(){
    return &spectrumsModelAll;
}

void  SPRSpectrumListItemsModel::clearSpectrums(QVector<SPRSpectrumItemModel*> *model){
    for(int i=0; i<model->size(); i++){
        delete model->at(i);
    }
    model->clear();
    unionModels();
}

void  SPRSpectrumListItemsModel::clearSpectrums(){
    clearSpectrums(&spectrumsModel);
}

void SPRSpectrumListItemsModel::clearSpectrumsBase(){
    clearSpectrums(&spectrumsModelBase);
}

void SPRSpectrumListItemsModel::store(QString fname)
{
    if(fname == ""){
        if(spectrumsfName){
            fname = spectrumsfName->getValue();
        } else {
            fname = QString(DEF_SPR_MAIN_SETTINGS_FNAME) + QString(DEF_SPR_MAIN_SPECTRUM_FNAME_SUFFIX);
        }
    }

    QFile out(fname);
    if(out.open(QIODevice::WriteOnly)){
        char ba[] = "10";
        out.write(ba, 2);

        for(int i=0; i<spectrumsModelBase.size(); i++){
            out.write((char*)(spectrumsModelBase[i]->getSpectrumData()->getBuf()), DEF_SPECTRUM_DATA_BUF_LENGTH);
        }
        for(int i=0; i<spectrumsModel.size(); i++){
            out.write((char*)(spectrumsModel[i]->getSpectrumData()->getBuf()), DEF_SPECTRUM_DATA_BUF_LENGTH);
        }
        out.close();
    }
}

SPRSpectrumItemModel *SPRSpectrumListItemsModel::addSpect(uint8_t *buf, int bufLentgh, QVector<SPRSpectrumItemModel*> *model){
    SPRSpectrumItemModel *item = new SPRSpectrumItemModel(zonesTableModel, formulas, this);
    item->setSpectrumData(buf, bufLentgh);
    if(bufLentgh == DEF_SPECTRUM_DATA_LENGTH_BYTE){
        item->setSpectrumThread(model->size());
        QString name;
        if(model == &spectrumsModelBase){
            name = "bspec_";
        } else {
            name = "spec_";
        }
        item->setSpectrumName(name+QString::number(model->size()));
        item->setSpectrumColor(mainColors[model->size() % mainColors.size()]);
        item->setSpectrumDateTime(QDateTime::currentDateTime());
    }
    model->push_back(item);
//    emit modelChanget();
    unionModels();
    return item;

}

SPRSettingsFormulaModel *SPRSpectrumListItemsModel::getFormulas() const
{
    return formulas;
}

void SPRSpectrumListItemsModel::addSpectrums(QString fName){
    QFile in(fName);
    int specCount = spectrumsModelBase.size();
    uint8_t buf[DEF_SPECTRUM_DATA_BUF_LENGTH];
    if(in.open(QIODevice::ReadOnly)){
       QByteArray b2 = in.read(sizeof(b2));
       while(in.read((char*)buf, DEF_SPECTRUM_DATA_BUF_LENGTH) == DEF_SPECTRUM_DATA_BUF_LENGTH){
           if(specCount < threads->getValue()){
               addSpect(buf, DEF_SPECTRUM_DATA_BUF_LENGTH, &spectrumsModelBase);
           } else {
               addSpect(buf, DEF_SPECTRUM_DATA_BUF_LENGTH, &spectrumsModel);
           }
           specCount++;
       }
       in.close();
    }
   memset(buf, 0, DEF_SPECTRUM_DATA_LENGTH_BYTE);
   while (specCount < threads->getValue()) {
       addSpect(buf, DEF_SPECTRUM_DATA_LENGTH_BYTE, &spectrumsModelBase);
//       SPRSpectrumItemModel *item = new SPRSpectrumItemModel(zonesTableModel, formulas, this);
//       item->setSpectrumName(QString("bspect_%1").arg(specCount));
//       item->setSpectrumThread(specCount);
//       item->setSpectrumDateTime(QDateTime::currentDateTime());
//       spectrumsModelBase.push_back(item);
       specCount++;
   }
   //   store(spectrumsfName->getData());
}

void SPRSpectrumListItemsModel::setSpectrums()
{
    QString fName = DEF_SPR_MAIN_SETTINGS_FNAME+DEF_SPR_MAIN_SPECTRUM_FNAME_SUFFIX;
    if(spectrumsfName){
        fName = spectrumsfName->getValue();
    }
    clearSpectrumsBase();
    clearSpectrums();
    addSpectrums(fName);
}

QVector<SPRSpectrumItemModel *> *SPRSpectrumListItemsModel::getSpectrumsModelBase()
{
    return &spectrumsModelBase;
}

SPRQStringVariable *SPRSpectrumListItemsModel::getSpectrumsfName() const
{
    return spectrumsfName;
}

void SPRSpectrumListItemsModel::setSpectrumsfName(SPRQStringVariable *value)
{
    if(value){
        if(spectrumsfName && property("delete_spectrumFName").value<bool>()) delete spectrumsfName;
        spectrumsfName = value;
        setProperty("delete_spectrumFName", QVariant(false));
    }
}

SPRVariable<uint> *SPRSpectrumListItemsModel::getThreads() const
{
    return threads;
}

void SPRSpectrumListItemsModel::setThreads(SPRVariable<uint> *value)
{
    if(value){
        if(threads && property("delete_threads").value<bool>()) delete threads;
        threads = value;
        setProperty("delete_threads", QVariant(false));
    }
}

SPRSpectrumListItemsModel::SPRSpectrumListItemsModel(QDomDocument *_doc, ISPRModelData *parent): ISPRModelData(_doc, parent), threads(0), spectrumsfName(nullptr), zonesTableModel(nullptr), formulas(nullptr), spectrumsModel()
{
    threads = new SPRVariable<uint>(doc, SPR_SETTINGS_MAIN_THREADS_XPATH, DEF_SPR_MAIN_THREADS, parent);
    setProperty("delete_threads", QVariant(true));

    spectrumsfName = new SPRQStringVariable(doc, SPR_SETTINGS_MAIN_SPECTRUM_FNAME_XPATH, QString(DEF_SPR_MAIN_SETTINGS_FNAME)+QString(DEF_SPR_MAIN_SPECTRUM_FNAME_SUFFIX), parent);
    setProperty("delete_spectrumFName", QVariant(true));

    setZonesModel(new SPRSpectrumZonesTableModel(doc, parent));
    setProperty("delete_zones", QVariant(true));

    setFormulasModel(new SPRSettingsFormulaModel(doc, parent));
    setProperty("delete_formulas", QVariant(true));

    setSpectrums();
}


SPRSpectrumListItemsModel::SPRSpectrumListItemsModel(SPRSpectrumZonesTableModel *_model, SPRSettingsFormulaModel *_formulas, SPRVariable<uint> *_threads, SPRQStringVariable *sfName, ISPRModelData *parent): threads(0), spectrumsfName(nullptr), zonesTableModel(nullptr), formulas(nullptr), spectrumsModel()
{
    setThreads(_threads);
    setSpectrumsfName(sfName);
    setZonesModel(_model);
    setFormulasModel(_formulas);

    setSpectrums();
}

SPRSpectrumListItemsModel::~SPRSpectrumListItemsModel()
{
    if(zonesTableModel && property("delete_zones").value<bool>()){
        delete zonesTableModel;
    }
    if(formulas && property("delete_formulas").value<bool>()){
        delete zonesTableModel;
    }
    if(spectrumsfName && property("delete_spectrumFName").value<bool>()){
        delete spectrumsfName;
    }
    if(threads && property("delete_threads").value<bool>()){
        delete threads;
    }
}



ISPRModelData *SPRSpectrumListItemsModel::setZonesModel(SPRSpectrumZonesTableModel *_model)
{
    if(_model){
        if(zonesTableModel && property("delete_zones").value<bool>()){
            setProperty("delete_zones", QVariant(false));
            delete zonesTableModel;
        }
        zonesTableModel = _model;

        for(int i=0; i<spectrumsModel.size(); i++){
//            uint32_t thr = *spectrumsModel[i]->getSpectrumData()->thread;
            spectrumsModel[i]->setZonesTable(_model);
        }
        for(int i=0; i<spectrumsModelBase.size(); i++){
//            uint32_t thr = *spectrumsModelBase[i]->getSpectrumData()->thread;
            spectrumsModelBase[i]->setZonesTable(_model);
        }
    }
    return this;
}

ISPRModelData *SPRSpectrumListItemsModel::setFormulasModel(SPRSettingsFormulaModel *_formulas)
{
    if(_formulas){
        if(formulas && property("delete_formulas").value<bool>()){
            setProperty("delete_formulas", QVariant(false));
        }
        formulas = _formulas;

        for(int i=0; i<spectrumsModel.size(); i++){
//            uint32_t thr = *spectrumsModel[i]->getSpectrumData()->thread;
            spectrumsModel[i]->setFormulas(_formulas);
        }
        for(int i=0; i<spectrumsModelBase.size(); i++){
//            uint32_t thr = *spectrumsModelBase[i]->getSpectrumData()->thread;
            spectrumsModelBase[i]->setFormulas(_formulas);
        }
    }
    return this;
}

ISPRModelData *SPRSpectrumListItemsModel::setModel(SPRSpectrumZonesTableModel *_zones, SPRSettingsFormulaModel *_formulas, SPRVariable<uint> *_threads, SPRQStringVariable *_sfName)
{
    setZonesModel(_zones);
    setFormulasModel(_formulas);
    setSpectrumsfName(_sfName);
    setThreads(_threads);

    clearSpectrums();
    return this;
}

SPRSpectrumItemModel *SPRSpectrumListItemsModel::setSpectrumData(int num, uint8_t *buf, int bufLen, SPRTypeSpectrumSet _type)
{
    SPRSpectrumItemModel *spec;
    if(_type == spectrumBase){
        spec = getSpectrumBaseItem(num);
        if(spec){
            spec->setSpectrumData(buf, bufLen);
        }
    }
    return spec;
}
