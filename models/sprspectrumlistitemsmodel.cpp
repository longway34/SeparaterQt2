#include "sprspectrumlistitemsmodel.h"
#include "models/sprmainmodel.h"

#include "QDebug"

static const QVector<QColor> mainColors = {Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::cyan, Qt::magenta,
                                    Qt::darkRed, Qt::darkBlue, Qt::darkGreen, Qt::darkYellow, Qt::darkCyan, Qt::darkMagenta,
                                    Qt::gray, Qt::lightGray, Qt::white, Qt::darkGray};

QList<SPRSpectrumItemModel *> *SPRSpectrumListItemsModel::getSpectrumsModel(SPRTypeSpectrumSet type)
{
    if(type == spectrumsOnly)
        return &spectrumsModel;
    else if(type == spectrumBase)
        return &spectrumsModelBase;
    else
        return unionModels();
}

SPRSpectrumItemModel *SPRSpectrumListItemsModel::getSpectrumItem(int index, SPRTypeSpectrumSet type){
    if(type == spectrumsOnly){
        return getItem(index, &spectrumsModel);
    } else if(type == spectrumBase){
        return getItem(index, &spectrumsModelBase);
    } else if(type == spectrumsAll){
        return getItem(index, unionModels());
    } else {
        return nullptr;
    }
}

QList<SPRSpectrumItemModel *> SPRSpectrumListItemsModel::getSpectrumsItemByThread(int thread, SPRTypeSpectrumSet type)
{
    QList<SPRSpectrumItemModel*> res;
    if(type == spectrumsMixed){
        type = spectrumsAll;
    }

    QList<SPRSpectrumItemModel*> *source = getSpectrumsModel(type);
    foreach(SPRSpectrumItemModel* spec, *source){
        int thr = spec->getThread();
        if(thr == thread || thread < 0){
            res << spec;
        }
    }

    return res;
}

SPRSpectrumZonesTableModel *SPRSpectrumListItemsModel::getZonesTableModel()
{
    return zonesTableModel;
}

QList<SPRSpectrumItemModel *> *SPRSpectrumListItemsModel::getSpectrumsModelAll(){
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

void SPRSpectrumListItemsModel::setGraphicMode(SPRViewGraphicsMode mode){
    QList<SPRSpectrumItemModel*> *lst = unionModels();
    double mul = 1;
    if(mode == viewModeScales){
        uint32_t y_max = 0;
        foreach(SPRSpectrumItemModel* spec, *lst){
            uint32_t y = *spec->getSpectrumData()->peak_value;
            if(y_max < y){
                y_max = y;
            }
        }
        foreach(SPRSpectrumItemModel* spec, *lst){
            uint32_t val = *spec->getSpectrumData()->peak_value;
            if(val > 0){
                double mul = (double)y_max / (double)val;
                spec->setGraphicMode(mode, mul);
            } else {
                spec->setGraphicMode(mode, 1);
            }
        }
    } else {
        foreach (SPRSpectrumItemModel *spect, *lst) {
            spect->setGraphicMode(mode, mul);
        }
    }
}

SPRSpectrumItemModel *SPRSpectrumListItemsModel::getItem(int index, QList<SPRSpectrumItemModel *> *model){
    if(index < 0 || index >= model->size()){
        return nullptr;
    } else {
        return model->at(index);
    }
}

void  SPRSpectrumListItemsModel::clearSpectrums(QList<SPRSpectrumItemModel *> *model){
    //    for(int i=0; i<model->size(); i++){
    //        delete model->at(i);
    //    }
    //    qDeleteAll(*model);
    model->clear();
    unionModels();
    emit modelChanget(this);
}

void SPRSpectrumListItemsModel::clearSpectrums(SPRTypeSpectrumSet type){
    blockSignals(true);
    if(type == spectrumBase){
        clearSpectrumsBase();
    }
    if(type == spectrumsOnly){
        clearSpectrums();
    }
    if(type == spectrumsAll){
        clearSpectrums();
        clearSpectrumsBase();
    }
    unionModels();
    blockSignals(false);
    emit modelChanget(this);
}

void  SPRSpectrumListItemsModel::clearSpectrums(){
    clearSpectrums(&spectrumsModel);
}

void SPRSpectrumListItemsModel::clearSpectrumsBase(){
    clearSpectrums(&spectrumsModelBase);
}

SPRSpectrumItemModel *SPRSpectrumListItemsModel::getBaseItemForChannel(int ch){
    SPRSpectrumItemModel *ret = nullptr;
    QList<SPRSpectrumItemModel*> *model = getSpectrumsModel(spectrumBase);

    if(ch < 0 || ch > getThreads()->getData()){
        ch = getThreads()->getData() % abs(ch);
    }

    foreach(SPRSpectrumItemModel *spec, *model){
        if(spec->getSpectrumThread() == ch){
            ret = spec;
            break;
        }
    }
    return ret;
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

SPRSpectrumItemModel *SPRSpectrumListItemsModel::addSpect(uint8_t *buf, int bufLentgh, uint32_t _timeScope_in_ms, SPRTypeSpectrumSet typeData, int numTh, QString _formatName){
    SPRSpectrumItemModel *item = new SPRSpectrumItemModel(zonesTableModel, formulas, numTh, this);
    item->setSpectrumData(buf, bufLentgh);
    if(_timeScope_in_ms == 0){
        if(typeData == spectrumsAll || typeData == spectrumsOnly){
            _timeScope_in_ms = 5000;
        } else {
            _timeScope_in_ms = 30000;
        }
    }

    QList<SPRSpectrumItemModel*> *model = getSpectrumsModel(typeData);
    if(_formatName.isEmpty()){
        if(typeData == spectrumBase){
            _formatName = QString(tr("Ручей %1(базовый)"));
        } else {
            _formatName = QString(tr("Ручей %1"));
        }
    }

    if(bufLentgh == DEF_SPECTRUM_DATA_LENGTH_BYTE){
//        item->setSpectrumThread(model->size());
        item->setSpectrumName(QString(_formatName).arg(model->size()+1));
        item->setSpectrumColor(mainColors[model->size() % mainColors.size()]);
        item->setSpectrumDateTime(QDateTime::currentDateTime());
        if(numTh >= 0 && numTh < zonesTableModel->getThreads()->getData()){
            item->setSpectrumThread(numTh);
        } else {
            item->setSpectrumThread(0);
        }
        item->setTimeScope(_timeScope_in_ms);
    } else {
        uint32_t th = *(item->getSpectrumData()->thread);
        if(th >= 0 && th < zonesTableModel->getThreads()->getData()){
            item->setSpectrumThread(th);
        } else {
            item->setSpectrumThread(0);
        }
        QString name = item->getSpectrumName();
        if(name.isEmpty()){
            item->setSpectrumName(QString(_formatName).arg(th+1));
        }
    }
    model->push_back(item);
    unionModels();

    if(typeData == spectrumsOnly){
        recomplite(item, spectrumsOnly);
        if(fabs(item->getTimeScope()) < 1e-10){
            item->setTimeScope(5000);
        }
    } else if(typeData == spectrumBase){
        if(fabs(item->getTimeScope()) < 1e-10){
            item->setTimeScope(10000);
        }
    }

    emit modelChanget(this);
    return item;

}

SPRSettingsFormulaModel *SPRSpectrumListItemsModel::getFormulas() const
{
    return formulas;
}

void SPRSpectrumListItemsModel::addSpectrums(QString fName){
//    qDebug()<<"addSpectrums: cur dir: " << QDir::current() << "; qcurrpath: "<<QDir::currentPath();

    QFile in(fName.toUtf8());
    int specCount = spectrumsModelBase.size();
    uint8_t buf[DEF_SPECTRUM_DATA_BUF_LENGTH];

    if(in.open(QIODevice::ReadOnly)){
       blockSignals(true);
       QByteArray b2 = in.read(2);
       while(in.read((char*)buf, DEF_SPECTRUM_DATA_BUF_LENGTH) == DEF_SPECTRUM_DATA_BUF_LENGTH){
           if(specCount < threads->getData()){
               addSpect(buf, DEF_SPECTRUM_DATA_BUF_LENGTH, 0, spectrumBase);
           } else {
               addSpect(buf, DEF_SPECTRUM_DATA_BUF_LENGTH, 0, spectrumsOnly);
           }
           specCount++;
       }
       in.close();
       blockSignals(false);
   }
   memset(buf, 0, DEF_SPECTRUM_DATA_LENGTH_BYTE);
   while (specCount < threads->getData()) {
       blockSignals(true);
       addSpect(buf, DEF_SPECTRUM_DATA_LENGTH_BYTE, 0, spectrumBase, specCount, QString(tr("Ручей %1(базовый)")));
//       SPRSpectrumItemModel *item = new SPRSpectrumItemModel(zonesTableModel, formulas, this);
//       item->setSpectrumName(QString("bspect_%1").arg(specCount));
//       item->setSpectrumThread(specCount);
//       item->setSpectrumDateTime(QDateTime::currentDateTime());
//       spectrumsModelBase.push_back(item);
       specCount++;
       blockSignals(false);
   }

   emit modelChanget(this);
   //   store(spectrumsfName->getData());
}

void SPRSpectrumListItemsModel::setSpectrums()
{
    QString fName = ""; //DEF_SPR_MAIN_SETTINGS_FNAME+DEF_SPR_MAIN_SPECTRUM_FNAME_SUFFIX;
    if(spectrumsfName){
        fName = spectrumsfName->getData();
    }
//    clearSpectrumsBase();
//    clearSpectrums();
    addSpectrums(fName);
}

QList<SPRSpectrumItemModel *> *SPRSpectrumListItemsModel::unionModels(){
    spectrumsModelAll = spectrumsModelBase;
    spectrumsModelAll << spectrumsModel;
    return &spectrumsModelAll;
}

QList<SPRSpectrumItemModel *> *SPRSpectrumListItemsModel::getSpectrumsModelBase()
{
    return &spectrumsModelBase;
}

SPRQStringVariable *SPRSpectrumListItemsModel::getSpectrumsfName() const
{
    return spectrumsfName;
}

void SPRSpectrumListItemsModel::setSpectrumsfName(SPRQStringVariable *value)
{
    spectrumsfName = value;
    if(value){
        if(spectrumsfName && property("delete_spectrumFName").value<bool>()) delete spectrumsfName;
        setProperty("delete_spectrumFName", QVariant(false));

//        if(zonesTableModel){
//            blockSignals(true);

////            clearSpectrumsBase();
////            clearSpectrums();

////            setSpectrums();
//        }
//        blockSignals(false);
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


SPRSpectrumListItemsModel::SPRSpectrumListItemsModel(SPRMainModel *_model, QList<SPRSpectrumItemModel*> *_base, bool selfModel, ISPRModelData *parent)
    : ISPRModelData(_model->getDoc(), parent), threads(0), spectrumsfName(nullptr), corelControlArea(nullptr), zonesTableModel(nullptr), formulas(nullptr), spectrumsModel()
{
    if(_model){
        setThreads(_model->getThreads());
        setCorelControlArea(_model->getSettingsControlModel()->controlArea);
        setZonesModel(_model->getSpectrumZonesTableModel());
        setFormulasModel(_model->getSettingsFormulaModel());
        setElementsProperty(_model->getElementsModel());

        if(!selfModel){
            setSpectrumsfName(_model->getSpectrumFName());
        } else {
            setSpectrumsfName(nullptr);
        }
        if(_base){
            spectrumsModelBase = *_base;
        }
        clearSpectrums(&spectrumsModel);
    }

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

ISPRModelData *SPRSpectrumListItemsModel::setModel(SPRSpectrumListItemsModel *_model){
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

void SPRSpectrumListItemsModel::removeSpectrum(SPRSpectrumItemModel *_item){
    if(_item){
        while(1){
            int index = spectrumsModel.indexOf(_item);
            if(index < 0) break;

            SPRSpectrumItemModel * mod = spectrumsModel.takeAt(index);
            if(mod){
                delete mod;
            }
        }
    }
    unionModels();
    emit modelChanget(this);
}

void SPRSpectrumListItemsModel::removeSpectrum(int num, SPRTypeSpectrumSet typeData){
    if(typeData == spectrumsOnly){
        if(num >=0 && num <spectrumsModel.size()){
            SPRSpectrumItemModel * mod = spectrumsModel.takeAt(num);
            if(mod){
                delete mod;
            }
            emit modelChanget(this);
        }
    } else if(typeData == spectrumBase){
        if(num >=0 && num < spectrumsModelBase.size()){
            SPRSpectrumItemModel * mod = spectrumsModelBase.takeAt(num);
            if(mod){
                delete mod;
            }
            emit modelChanget(this);
        }
    } else {
        if(num >=0 && num < this->unionModels()->size()){
            SPRSpectrumItemModel * mod = unionModels()->takeAt(num);
            if(mod){
                delete mod;
            }
            emit modelChanget(this);
        }
    }
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
    blockSignals(true);
    QList<SPRSpectrumItemModel*> *model = getSpectrumsModel(spectrumsAll);
    for(int i=0; i<model->size(); i++){
        if(i < getThreads()->getData()){
            recomplite(model->at(i), spectrumBase);
        } else {
            recomplite(model->at(i), spectrumsOnly);
        }
    }
    blockSignals(false);

    emit modelChanget(this);
}

SPRSpectrumItemModel *SPRSpectrumListItemsModel::setSpectrumData(int num, QByteArray data, SPRTypeSpectrumSet _type, uint32_t _timeScope_in_msec, QString _formatName){
    return setSpectrumData(num, (uint8_t*)data.constData(), data.size(), _type, _timeScope_in_msec, _formatName);
}

SPRSpectrumItemModel *SPRSpectrumListItemsModel::setSpectrumData(int num, uint8_t *buf, int bufLen, SPRTypeSpectrumSet _type, uint32_t _timeScope_in_msec, QString _formatName/*** "xxx %1" */)
{
    SPRSpectrumItemModel *spec = nullptr;
    if(_type == spectrumBase){
        spec = getSpectrumBaseItem(num);
    } else if(_type == spectrumsOnly){
        QList<SPRSpectrumItemModel*> lst = getSpectrumsItemByThread(num);
        if(lst.size() > 0){
            spec = lst.first();
        }
    }
    QColor col = _type == spectrumBase ? QColor(Qt::white) : mainColors[num % mainColors.size()];
    if(_formatName == ""){
        _formatName = _type == spectrumBase ? QString(tr("Ручей %1(базовый)")) : QString(tr("Ручей %1"));
    }
    if(_timeScope_in_msec == 0){
        _timeScope_in_msec = _type == spectrumBase ? 30000 : 5000;
    }
    if(spec){
        spec->setSpectrumData(buf, bufLen);

        if(bufLen == DEF_SPECTRUM_DATA_LENGTH_BYTE){
            spec->setSpectrumColor(col);
            spec->setSpectrumName(QString(_formatName).arg(num+1));
            spec->setTimeScope(_timeScope_in_msec);
            spec->setSpectrumDateTime(QDateTime::currentDateTime());
        }
    } else {
        spec = addSpectrum(buf, bufLen, _timeScope_in_msec, num, _formatName);
        spec->setSpectrumColor(col);
    }
    emit modelChanget(this);
    return spec;
}
