#include "models/sprspectrumitemmodel.h"
#include "models/sprgraphitem.h"


void spectumItemData::setData(uint8_t *inp, uint16_t inplength)
{

    if(!buf){
        bufLength = DEF_SPECTRUM_DATA_BUF_LENGTH;
        buf = (uint8_t*)malloc(bufLength);
        memset(buf, 0, bufLength);

        name = (char*)buf;
        thread = (uint32_t*)(buf + 0x28); // 40
        red = buf + 0x2C; green = buf + 0x2D; blue = buf + 0x2E; // 44 45 46
        spect = (uint16_t*)(buf + 0x30); // 48
        summ = (uint32_t*)(buf + 0x230); // 560
        peak_value = (uint32_t*)(buf + 0x234); // 564
        peak = (uint32_t*)(buf + 0x238); // 568
        Rs = (double*)(buf + 0x23C); // 572
        H1 = (double*)(buf + 0x244); H2 = (double*)(buf + 0x24C); H3 = (double*)(buf + 0x254); // 580 588 596
        time = (double*)(buf + 0x25C); // 604
        center = (double*)(buf + 0x268); // 616
//        x_ray = (double*)(buf + 0x270); // 624
        Ns = (uint32_t*)(buf + 0x278); // 632
        Fe = (uint32_t*)(buf + 0x27C); // 636
        Cu = (uint32_t*)(buf + 0x280); // 640
        Mo = (uint32_t*)(buf + 0x284); // 644
        Zn = (uint32_t*)(buf + 0x288); // 648
        Mg = (uint32_t*)(buf + 0x28C); // 652
        elementsSumm = QMap<EnumElements, uint32_t*>({
                                    {EnumElements::Ns, Ns},
                                    {EnumElements::Fe, Fe},
                                    {EnumElements::Cu, Cu},
                                    {EnumElements::Mo, Mo},
                                    {EnumElements::Zn, Zn},
                                    {EnumElements::Mg, Mg}
                                });
        year = (uint32_t*)(buf + 0x2AC); // 684
        month = (uint32_t*)(buf + 0x2B0);  // 688
        day = (uint32_t*)(buf + 0x2B4);  // 692
        hours = (uint32_t*)(buf + 0x2B8); // 696
        min = (uint32_t*)(buf + 0x2BC); // 700
        sec = (uint32_t*)(buf + 0x2C0); // 704
    }
    if(inp){
        if(inplength == bufLength){
            memcpy(buf, inp, bufLength);
            static double  ttt = 16.48;
            if(*thread >= MAX_SPR_MAIN_THREADS){
                *thread = 0;
            }
            return;
        }
        if(inplength == DEF_SPECTRUM_DATA_LENGTH_BYTE){
            memcpy(spect, inp, inplength);
            return;
        }
        qDebug() << "Error Data Length... Try " << QString::number(DEF_SPECTRUM_DATA_BUF_LENGTH) << " to all spectrum data or " << QString::number(DEF_SPECTRUM_DATA_LENGTH_BYTE) << " for spectrum only...";
    }
}

void spectumItemData::setData(QByteArray inp)
{
    setData((uint8_t*)(inp.constData()), inp.size());
}


SPRGraphItem *SPRSpectrumItemModel::getGraphic()
{
    return graphic;
}

SPRSpectrumItemModel::SPRSpectrumItemModel():thread(0), graphic(nullptr)
{
    hashKeyDebug = QString("spr_%1").arg(rand() % 100);
    zones = new SPRSpectrumZonesTableModel();
    formulas = new SPRSettingsFormulaModel();
    setProperty("delete_ranges", QVariant(true));
    setProperty("delete_formulas", QVariant(true));

    graphic = new SPRGraphItem(this);
}

void SPRSpectrumItemModel::recomplite(SPRSpectrumItemModel *baseItem)
{
    memset(spectrumData.Ns, 0, 6 * sizeof(uint32_t));
    *spectrumData.peak_value = 0;
    *spectrumData.summ = 0;
    // ***********for correlation
    foreach (EnumElements el, zones->items[*spectrumData.thread]->getZones().keys()) {
        /*spectrumData.elementsDiff[el] = 0;*/ spectrumData.elementsAverage[el] = 0;
        spectrumData.elementsSigma[el] = 0;
        spectrumData.elementsCount[el] = 0;
        spectrumData.elementsDispersion[el] = 0;
    }
    /*spectrumData.diff = 0;*/ spectrumData.dispersion = 0; spectrumData.sigma = 0;
    // ********************

    for(int i=0; i<(DEF_SPECTRUM_DATA_LENGTH); i++){
        uint32_t val = spectrumData.spect[i];
        *spectrumData.summ += val;
        foreach (EnumElements el, zones->items[*spectrumData.thread]->getZones().keys()) {
            int min = zones->items[*spectrumData.thread]->getZones()[el]->min->getData();
            int max = zones->items[*spectrumData.thread]->getZones()[el]->max->getData();
            if(i >= min && i <= max){
                (*(spectrumData.elementsSumm[el])) += val;
                spectrumData.elementsCount[el]++;
            }
            if(val > *spectrumData.peak_value){
                *spectrumData.peak = i;
                *spectrumData.peak_value = val;
            }
        }
    }
    uint RS = 0;
    int ch= *spectrumData.peak;
    int hsumm = round(((double)(*spectrumData.peak_value)) / 2.);

    while(spectrumData.spect[ch] > hsumm){
        RS++; ch--;
        if(ch < 0) break;
    }
    ch= *spectrumData.peak + 1;
    if(ch < (DEF_SPECTRUM_DATA_LENGTH)){
        while(spectrumData.spect[ch] > hsumm){
              RS++; ch++;
              if(ch >= (DEF_SPECTRUM_DATA_LENGTH)) break;
        }
    }
    *spectrumData.center = *spectrumData.summ / (DEF_SPECTRUM_DATA_LENGTH);
    *spectrumData.Rs = RS;

// for correlation
    foreach (EnumElements el, zones->items[*spectrumData.thread]->getZones().keys()) {
        if(spectrumData.elementsCount[el] > 0){
            spectrumData.elementsAverage[el] = ((double)*spectrumData.elementsSumm[el]) /
                (spectrumData.elementsCount[el]);
        } else {
            spectrumData.elementsAverage[el] = 0;
        }
    }
    spectrumData.diff.clear();
    foreach (EnumElements el, zones->items[*spectrumData.thread]->getZones().keys()) {
        spectrumData.elementsDiff[el].clear();
    }

    for(int i=0; i<(DEF_SPECTRUM_DATA_LENGTH); i++){
        uint32_t val = spectrumData.spect[i];

        double d = (val - *spectrumData.center);
        spectrumData.diff.push_back(d);
        spectrumData.dispersion += d*d;

        foreach (EnumElements el, zones->items[*spectrumData.thread]->getZones().keys()) {
            int min = zones->items[*spectrumData.thread]->getZones()[el]->min->getData();
            int max = zones->items[*spectrumData.thread]->getZones()[el]->max->getData();
            if(i >= min && i <= max){
                double ed = (val - spectrumData.elementsAverage[el]);
                spectrumData.elementsDiff[el].push_back(ed);
                spectrumData.elementsDispersion[el] += ed * ed;
            }
        }
    }

    double up = (double)(*spectrumData.elementsSumm[formulas->itemsModel[0]->ElementUp1->getData()]) +
            formulas->itemsModel[0]->MulUp->getData() * (*spectrumData.elementsSumm[formulas->itemsModel[0]->ElementUp2->getData()]);

    double down = (double)(*spectrumData.elementsSumm[formulas->itemsModel[0]->ElementDown1->getData()]) +
            formulas->itemsModel[0]->MulDown->getData() * (double)(*spectrumData.elementsSumm[formulas->itemsModel[0]->ElementDown2->getData()])
                * (double)(*spectrumData.elementsSumm[formulas->itemsModel[0]->ElementDown3->getData()])
                    / (double)(*spectrumData.elementsSumm[formulas->itemsModel[0]->ElementDown4->getData()]);
    if(down >1e-6)
        *spectrumData.H1 = up / down;
    else
        *spectrumData.H1 = 0;


    up = (double)(*spectrumData.elementsSumm[formulas->itemsModel[1]->ElementUp1->getData()]) +
            formulas->itemsModel[1]->MulUp->getData() * (double)(*spectrumData.elementsSumm[formulas->itemsModel[1]->ElementUp2->getData()]);

    down = (double)(*spectrumData.elementsSumm[formulas->itemsModel[1]->ElementDown1->getData()]) +
            formulas->itemsModel[1]->MulDown->getData() * (double)(*spectrumData.elementsSumm[formulas->itemsModel[1]->ElementDown2->getData()])
                * (double)(*spectrumData.elementsSumm[formulas->itemsModel[1]->ElementDown3->getData()])
                    / (double)(*spectrumData.elementsSumm[formulas->itemsModel[1]->ElementDown4->getData()]);
    if(down >1e-6)
        *spectrumData.H2 = up / down;
    else
        *spectrumData.H2 = 0;

    up = (double)(*spectrumData.elementsSumm[formulas->itemsModel[2]->ElementUp1->getData()]) +
            formulas->itemsModel[2]->MulUp->getData() * (double)(*spectrumData.elementsSumm[formulas->itemsModel[2]->ElementUp2->getData()]);

    down = (double)(*spectrumData.elementsSumm[formulas->itemsModel[2]->ElementDown1->getData()]) +
            formulas->itemsModel[2]->MulDown->getData() * (double)(*spectrumData.elementsSumm[formulas->itemsModel[2]->ElementDown2->getData()])
                * (double)(*spectrumData.elementsSumm[formulas->itemsModel[2]->ElementDown3->getData()])
                    / (double)(*spectrumData.elementsSumm[formulas->itemsModel[2]->ElementDown4->getData()]);
    if(down >1e-6)
        *spectrumData.H3 = up / down;
    else
        *spectrumData.H3 = 0;

// store to struct

    if(*spectrumData.time > 1e-6){
        uint32_t summ = *spectrumData.summ / *spectrumData.time;
        *spectrumData.summ = summ;

        foreach(EnumElements el, zones->items[*spectrumData.thread]->getZones().keys()){
            summ = *spectrumData.elementsSumm[el] / *spectrumData.time;
            *spectrumData.elementsSumm[el] = summ;
        }
    }
    if(baseItem){
        double correl = setCorrelData(baseItem);
    }
}

double SPRSpectrumItemModel::setCorrelData(SPRSpectrumItemModel *ather){

    if(ather){
        double covr = 0;
        double sqrSummDisp = 0;
        spectrumData.correl = 0;
        double correl = 0;

        for(int i=0; i<this->spectrumData.diff.size() && i<ather->spectrumData.diff.size();i++){
            covr += ather->spectrumData.diff[i] * this->spectrumData.diff[i];
        }
        double sdiff = ather->spectrumData.dispersion * this->spectrumData.dispersion;
        sqrSummDisp = sqrt(sdiff);
        if(sqrSummDisp > 1e-6)
            correl = covr / sqrSummDisp;

        spectrumData.correl = correl;

        foreach(EnumElements el, zones->items[static_cast<int>(*spectrumData.thread)]->getZones().keys()){
            covr = 0;
            for(int i=0; i<spectrumData.elementsCount[el] && i<ather->spectrumData.elementsCount[el]; i++){
                covr += ather->spectrumData.elementsDiff[el][i] * this->spectrumData.elementsDiff[el][i];
            }
            double sdiff = ather->spectrumData.elementsDispersion[el] * this->spectrumData.elementsDispersion[el];
            sqrSummDisp = sqrt(sdiff);
            if(sqrSummDisp > 1e-6)
                spectrumData.elementsCorrel[el] = covr / sqrSummDisp;

        }
    }
    return spectrumData.correl;
}

double SPRSpectrumItemModel::getCorrel(SPRSpectrumItemModel *ather, bool elementsOnly, EnumElements _elements)
{
    if(ather){
//        this->recomplite();
//        ather->recomplite();

        setCorrelData(ather);

//        double covr=0;
//        double sqrSummDisp = 0;
//        spectrumData.correl = 0;
//        double correl = 0;
        if(!elementsOnly){
              return spectrumData.correl;
            //            for(int i=0; i<this->spectrumData.diff.size() && i<ather->spectrumData.diff.size();i++){
//                covr += ather->spectrumData.diff[i] * this->spectrumData.diff[i];
//            }
//            double sdiff = ather->spectrumData.dispersion * this->spectrumData.dispersion;
//            sqrSummDisp = sqrt(sdiff);
        } else {
            if(spectrumData.elementsCorrel.keys().contains(_elements)){
                return spectrumData.elementsCorrel[_elements];
            }
//            for(int i=0; i<spectrumData.elementsCount[_elements] && i<ather->spectrumData.elementsCount[_elements]; i++){
//                covr += ather->spectrumData.elementsDiff[_elements][i] * this->spectrumData.elementsDiff[_elements][i];
//            }
//            double sdiff = ather->spectrumData.elementsDispersion[_elements] * this->spectrumData.elementsDispersion[_elements];
//            sqrSummDisp = sqrt(sdiff);
        }
    }
    return 0;
}

double SPRSpectrumItemModel::getCorrel(){
    return spectrumData.correl;
}

double SPRSpectrumItemModel::getXRay()
{
    uint summ = *spectrumData.summ / 250;
    double res = summ;
    double add = 3 * sqrt(summ);
    res += add;

    return res;
}

ISPRModelData *SPRSpectrumItemModel::setModel(SPRSpectrumZonesTableModel *_ranges, SPRSettingsFormulaModel *_formulas){
    setZonesTable(_ranges);
    setFormulas(_formulas);
    recomplite();
    return this;
}

QColor SPRSpectrumItemModel::getSpectrumColor(){
    QColor ret(int(*spectrumData.red), int(*spectrumData.green), int(*spectrumData.blue));
    return ret;
}

void SPRSpectrumItemModel::setSpectrumColor(QColor color){
    *spectrumData.red = (uint8_t)color.red();
    *spectrumData.green = (uint8_t)color.green();
    *spectrumData.blue = (uint8_t)color.blue();
    //        emit modelChanget(this);
}

QString SPRSpectrumItemModel::getSpectrumName(){
    return QString(spectrumData.name);
}

void SPRSpectrumItemModel::setSpectrumName(QString name){
    setObjectName(name);
    memcpy(spectrumData.name, name.toUtf8().constData(), name.toUtf8().size());
    if(graphic){
        graphic->renameGraph(name);
    }
    //        emit modelChanget(this);
}

void SPRSpectrumItemModel::setSpectrumThread(uint th){
    *spectrumData.thread = th;
    thread = th;
}

uint SPRSpectrumItemModel::getSpectrumThread(){
    return *spectrumData.thread;
}

SPRSpectrumZonesModel *SPRSpectrumItemModel::getZones(){
    uint th = *spectrumData.thread;
    return zones->items[*spectrumData.thread];
}

void SPRSpectrumItemModel::setZonesTable(SPRSpectrumZonesTableModel *value)
{
    if(value){
        zones = value;
        setProperty("delete_ranges", QVariant(false));
        getZonesGaphics();
    }
}

QMap<EnumElements, QVector<QwtIntervalSample> > SPRSpectrumItemModel::getZonesGaphics(){
    zonesGraphData.clear();
    SPRSpectrumZonesModel *zone = zones->items[*spectrumData.thread];
    foreach (EnumElements el, zone->getZones().keys()) {

        double value = 0.5;
        for(int i=zone->getZones()[el]->min->getData(); i<zone->getZones()[el]->max->getData(); i++){
            if(value < spectrumData.spect[i]){
                value = (double(spectrumData.spect[i]));
            }
        }
        value = value;

        double xmin = qreal(zone->getZones()[el]->min->getData());
        double xmax = qreal(zone->getZones()[el]->max->getData());
        QVector<QwtIntervalSample> inter;
        inter.push_back(QwtIntervalSample(value, xmin, xmax));
        zonesGraphData[el] = inter;
    }
    return zonesGraphData;
}

QPolygonF SPRSpectrumItemModel::getSpectrumGraphics(){
    spectGraphData.clear();
    for(int i=0; i<(DEF_SPECTRUM_DATA_LENGTH); i++){
        QPointF val(qreal(i), qreal(spectrumData.spect[i]));
        spectGraphData.push_back(val);
    }
    return spectGraphData;
}

void SPRSpectrumItemModel::setFormulas(SPRSettingsFormulaModel *value)
{
    if(value){
        formulas = value;
        setProperty("delete_formulas", QVariant(false));
    }
}

void SPRSpectrumItemModel::setTimeScope(uint32_t _time_in_ms){
    if(spectrumData.time){
        double value = (double)_time_in_ms / 1000;
        *spectrumData.time = (double)value;
    }
}

double SPRSpectrumItemModel::getTimeScope()
{
    return *spectrumData.time;
}

void SPRSpectrumItemModel::setSpectrumDateTime(QDateTime _dateTime){
    *spectrumData.year = _dateTime.date().year();
    *spectrumData.month = _dateTime.date().month();
    *spectrumData.day = _dateTime.date().day();

    *spectrumData.hours = _dateTime.time().hour();
    *spectrumData.min = _dateTime.time().minute();
    *spectrumData.sec = _dateTime.time().second();
}

SpectrumItemData *SPRSpectrumItemModel::getSpectrumData()
{
    return &spectrumData;
}

void SPRSpectrumItemModel::setSpectrumData(uint8_t *buf, uint16_t len, SPRSpectrumItemModel *baseItem)
{
    spectrumData.setData(buf, len);
    if(len == DEF_SPECTRUM_DATA_LENGTH_BYTE){
        recomplite();
    }
    if(graphic){
        graphic->onModelChanget(this);
    }
}

uint SPRSpectrumItemModel::getThread() const
{
    return thread;
}

void SPRSpectrumItemModel::setThread(const uint &value)
{
    thread = value;
}

SPRViewGraphicsMode SPRSpectrumItemModel::getGraphicMode() const
{
    return graphicMode;
}

void SPRSpectrumItemModel::setGraphicMode(const SPRViewGraphicsMode &value, double _scaleMul)
{
    switch(value){
    case viewModeAsIs:
        _scaleMul = 1.;
        break;
    case viewModeOneSecond:
        if(*spectrumData.time > 1e-10){
            _scaleMul = 1/(*spectrumData.time);
        }
        break;
    case viewModeScales:
//        scaleMul = _scaleMul;
        break;
    }
    if(graphicMode != value){
        getGraphic()->setModelData(this, _scaleMul);
    }
    graphicMode = value;
}



SPRSpectrumItemModel::SPRSpectrumItemModel(QDomDocument *_doc, int _index, ISPRModelData *parent) : ISPRModelData(_doc, parent), graphic(nullptr)
{
    hashKeyDebug = QString("spr_%1").arg(rand() % 100);
    setZonesTable(new SPRSpectrumZonesTableModel(doc));
    setFormulas(new SPRSettingsFormulaModel(doc, new SPRElementsModel(doc, this), nullptr));
    *spectrumData.thread = _index;
    this->thread = _index;
    setProperty("delete_ranges", QVariant(true));
    setProperty("delete_formulas", QVariant(true));

    graphic = new SPRGraphItem(this);
    setGraphicMode(viewModeAsIs, 1);
}

SPRSpectrumItemModel::SPRSpectrumItemModel(SPRSpectrumZonesTableModel *_ranges, SPRSettingsFormulaModel *_formulas, uint _thread, ISPRModelData *parent) : ISPRModelData(nullptr, parent), graphic(nullptr)
{
    hashKeyDebug = QString("spr_%1").arg(rand() % 100);
    setZonesTable(_ranges);
    setFormulas(_formulas);
    if(_ranges){
        thread = _thread;
    }
    setProperty("delete_ranges", QVariant(false));
    setProperty("delete_formulas", QVariant(false));

    graphic = new SPRGraphItem(this);
    setGraphicMode(viewModeAsIs, 1);

}

SPRSpectrumItemModel::~SPRSpectrumItemModel(){
    if(zones != nullptr && QVariant(property("delete_ranges")).toBool()){
        delete zones; zones = nullptr;
        setProperty("delete_ranges", QVariant(false));
    }
    if(formulas != nullptr && QVariant(property("delete_formulas")).toBool()){
        delete formulas; formulas = nullptr;
        setProperty("delete_formulas", QVariant(false));
    }
}


