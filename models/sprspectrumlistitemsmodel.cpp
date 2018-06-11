#include "sprspectrumlistitemsmodel.h"

static const QVector<QColor> mainColors = {Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow,
                                    Qt::darkRed, Qt::darkBlue, Qt::darkGreen, Qt::darkCyan, Qt::darkMagenta, Qt::darkYellow,
                                    Qt::gray, Qt::lightGray, Qt::white, Qt::darkGray};

QVector<SPRSpectrumItemModel *> *SPRSpectrumListItemsModel::getSpectrumsModel(SPRTypeSpectrumSet type)
{
    if(type == spectrumsOnly)
        return &spectrumsModel;
    else if(type == spectrumBase)
        return &spectrumsModelBase;
    else
        return unionModels();
}

SPRSpectrumZonesTableModel *SPRSpectrumListItemsModel::getZonesTableModel()
{
    return zonesTableModel;
}

QVector<SPRSpectrumItemModel *> *SPRSpectrumListItemsModel::getSpectrumsModelAll(){
    return unionModels();
}

void SPRSpectrumListItemsModel::setElementsProperty(SPRElementsModel *value)
{
    if(value){
        if(elementsProperty && property("delete_elementsProperty").value<bool>()){
            delete elementsProperty;
        }
        elementsProperty = value;
        setProperty("delete_elementsProperty", QVariant(false));
    }
}

void SPRSpectrumListItemsModel::setCorelControlArea(SPREnumVariable<EnumElements> *value)
{
    if(value){
        if(corelControlArea && property("delete_controlArea").value<bool>()){
            delete corelControlArea;
        }
        corelControlArea = value;
        setProperty("delete_controlArea", QVariant(false));
    }
}

SPREnumVariable<EnumElements> *SPRSpectrumListItemsModel::getCorelControlArea() const
{
    return corelControlArea;
}

void  SPRSpectrumListItemsModel::clearSpectrums(QVector<SPRSpectrumItemModel*> *model){
    for(int i=0; i<model->size(); i++){
        delete model->at(i);
    }
    model->clear();
    unionModels();
    emit modelChanget(this);
}

void  SPRSpectrumListItemsModel::clearSpectrums(){
    clearSpectrums(&spectrumsModel);
}

void SPRSpectrumListItemsModel::clearSpectrumsBase(){
    clearSpectrums(&spectrumsModelBase);
}

void SPRSpectrumListItemsModel::saveAs(QString fname)
{
    if(fname == ""){
        if(spectrumsfName){
            fname = spectrumsfName->getData();
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

SPRSpectrumItemModel *SPRSpectrumListItemsModel::addSpect(uint8_t *buf, int bufLentgh, SPRTypeSpectrumSet typeData, int numTh, QString pref){
    SPRSpectrumItemModel *item = new SPRSpectrumItemModel(zonesTableModel, formulas, this);
    item->setSpectrumData(buf, bufLentgh);

    QVector<SPRSpectrumItemModel*> *model = getSpectrumsModel(typeData);

    if(bufLentgh == DEF_SPECTRUM_DATA_LENGTH_BYTE){
        item->setSpectrumThread(model->size());
        if(pref.isEmpty()){
            if(model == &spectrumsModelBase){
                pref = "bspec_";
            } else {
                pref = "spec_";
            }
        }
        item->setSpectrumName(pref+QString::number(model->size()));
        item->setSpectrumColor(mainColors[model->size() % mainColors.size()]);
        item->setSpectrumDateTime(QDateTime::currentDateTime());
        if(numTh > 0){
            item->setSpectrumThread(numTh);
        }
    }
    model->push_back(item);
    unionModels();

    if(typeData == spectrumsOnly){
        recomplite(item, spectrumsOnly);
    }

    emit modelChanget(this);
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
           if(specCount < threads->getData()){
               addSpect(buf, DEF_SPECTRUM_DATA_BUF_LENGTH, spectrumBase);
           } else {
               addSpect(buf, DEF_SPECTRUM_DATA_BUF_LENGTH, spectrumsOnly);
           }
           specCount++;
       }
       in.close();
   }
   memset(buf, 0, DEF_SPECTRUM_DATA_LENGTH_BYTE);
   while (specCount < threads->getData()) {
       addSpect(buf, DEF_SPECTRUM_DATA_LENGTH_BYTE, spectrumBase);
//       SPRSpectrumItemModel *item = new SPRSpectrumItemModel(zonesTableModel, formulas, this);
//       item->setSpectrumName(QString("bspect_%1").arg(specCount));
//       item->setSpectrumThread(specCount);
//       item->setSpectrumDateTime(QDateTime::currentDateTime());
//       spectrumsModelBase.push_back(item);
       specCount++;
   }

   emit modelChanget(this);
   //   store(spectrumsfName->getData());
}

void SPRSpectrumListItemsModel::setSpectrums()
{
    QString fName = DEF_SPR_MAIN_SETTINGS_FNAME+DEF_SPR_MAIN_SPECTRUM_FNAME_SUFFIX;
    if(spectrumsfName){
        fName = spectrumsfName->getData();
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

SPRSpectrumListItemsModel::SPRSpectrumListItemsModel(QDomDocument *_doc, SPRElementsModel *_elements, SPREnumVariable<EnumElements> *_controlArea, ISPRModelData *parent): ISPRModelData(_doc, parent), threads(0), spectrumsfName(nullptr), corelControlArea(nullptr), zonesTableModel(nullptr), formulas(nullptr), spectrumsModel()
{
    threads = new SPRVariable<uint>(doc, SPR_SETTINGS_MAIN_THREADS_XPATH, DEF_SPR_MAIN_THREADS, parent);
    setProperty("delete_threads", QVariant(true));

    spectrumsfName = new SPRQStringVariable(doc, SPR_SETTINGS_MAIN_SPECTRUM_FNAME_XPATH, QString(DEF_SPR_MAIN_SETTINGS_FNAME)+QString(DEF_SPR_MAIN_SPECTRUM_FNAME_SUFFIX), parent);
    setProperty("delete_spectrumFName", QVariant(true));

    if(_controlArea){
        setCorelControlArea(_controlArea);
    } else {
        corelControlArea = new SPREnumVariable<EnumElements>(doc, SPR_CONTROL_CONTROL_AREA_XPATH, DEF_SPR_CONTROL_CONTROL_AREA, this);
        setProperty("delete_controlArea", QVariant(true));
    }

    if(_elements){
        setElementsProperty(_elements);
    } else {
        elementsProperty = new SPRElementsModel(doc, this);
        setProperty("delete_elementsProperty", QVariant(true));
    }

    setZonesModel(new SPRSpectrumZonesTableModel(doc, elementsProperty, parent));
    setProperty("delete_zones", QVariant(true));

    setFormulasModel(new SPRSettingsFormulaModel(doc, elementsProperty, nullptr, parent));
    setProperty("delete_formulas", QVariant(true));

    setSpectrums();
}


SPRSpectrumListItemsModel::SPRSpectrumListItemsModel(SPRSpectrumZonesTableModel *_model, SPRSettingsFormulaModel *_formulas, SPRVariable<uint> *_threads, SPRQStringVariable *sfName, SPREnumVariable<EnumElements> *_controlArea, ISPRModelData *parent): threads(0), spectrumsfName(nullptr), corelControlArea(nullptr), zonesTableModel(nullptr), formulas(nullptr), spectrumsModel()
{
    setThreads(_threads);
    setSpectrumsfName(sfName);

    if(_controlArea){
        setCorelControlArea(_controlArea);
    } else {
        corelControlArea = new SPREnumVariable<EnumElements>(doc, SPR_CONTROL_CONTROL_AREA_XPATH, DEF_SPR_CONTROL_CONTROL_AREA, this);
        setProperty("delete_controlArea", QVariant(true));
    }
    setZonesModel(_model);
    setFormulasModel(_formulas);
    setElementsProperty(_model->elementsProperty);

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
    if(corelControlArea && property("delete_controlArea").value<bool>()){
        delete corelControlArea;
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
        setElementsProperty(_model->getElementsProperty());

        for(int i=0; i<spectrumsModel.size(); i++){
            spectrumsModel[i]->setZonesTable(_model);
        }
        for(int i=0; i<spectrumsModelBase.size(); i++){
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

ISPRModelData *SPRSpectrumListItemsModel::setModel(SPRSpectrumZonesTableModel *_zones, SPRSettingsFormulaModel *_formulas, SPRVariable<uint> *_threads, SPRQStringVariable *_sfName, SPREnumVariable<EnumElements> *_controlArea)
{
    setZonesModel(_zones);
    setFormulasModel(_formulas);
    setSpectrumsfName(_sfName);
    setThreads(_threads);
    setElementsProperty(_zones->elementsProperty);

    setCorelControlArea(_controlArea);

    clearSpectrums();
    return this;
}



void SPRSpectrumListItemsModel::recomplite(SPRSpectrumItemModel *item, typeSpectrumsData type)
{
    if(item){

        uint thr = item->getSpectrumThread();
//        SPRSpectrumItemModel* spect = getSpectrumItem(row % spectrumsModel.size());
        SPRSpectrumItemModel* base = getBaseItemForChannel(thr);


        item->recomplite();
        if(base){
            base->recomplite();

            if(corelControlArea){
                item->getCorrel(base, true, corelControlArea->getData());
            } else {
                item->getCorrel(base);
            }
        }
    }
}

void SPRSpectrumListItemsModel::recomplite(){
    QVector<SPRSpectrumItemModel*> *model = getSpectrumsModel();
    for(int i=0; i<model->size(); i++){
        recomplite(model->at(i));
    }
}

SPRSpectrumItemModel *SPRSpectrumListItemsModel::setSpectrumData(int num, uint8_t *buf, int bufLen, SPRTypeSpectrumSet _type)
{
    SPRSpectrumItemModel *spec;
    if(_type == spectrumBase){
        spec = getSpectrumBaseItem(num);
        if(spec){
            QColor col;
            if(bufLen == DEF_SPECTRUM_DATA_LENGTH_BYTE){
                col = spec->getSpectrumColor();
            }
            spec->setSpectrumData(buf, bufLen);
            if(bufLen == DEF_SPECTRUM_DATA_LENGTH_BYTE){
                spec->setSpectrumColor(col);
            }
            emit modelChanget(this);
        }
    }
    return spec;
}
