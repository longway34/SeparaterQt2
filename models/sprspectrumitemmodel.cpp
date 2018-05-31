#include "sprspectrumitemmodel.h"

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

void SPRSpectrumItemModel::recomplite()
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
            if(i >= min && i < max){
                (*(spectrumData.elementsSumm[el])) += val;
                spectrumData.elementsCount[el]++;
            }
            if(val > *spectrumData.peak_value){
                *spectrumData.peak = i;
                *spectrumData.peak_value = val;
            }
        }
    }
    uint RS = 0; int ch= *spectrumData.peak; int hsumm = round(((double)(*spectrumData.peak_value)) / 2.);

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
            if(i >= min && i < max){
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
}

double SPRSpectrumItemModel::getCorrel(SPRSpectrumItemModel *ather, bool elementsOnly, EnumElements _elements)
{
    if(ather){
        double covr=0;
        double sqrSummDisp = 0;
        spectrumData.correl = 0;
        double correl = 0;
        if(!elementsOnly){
            for(int i=0; i<this->spectrumData.diff.size() && i<ather->spectrumData.diff.size();i++){
                covr += ather->spectrumData.diff[i] * this->spectrumData.diff[i];
            }
            double sdiff = ather->spectrumData.dispersion * this->spectrumData.dispersion;
            sqrSummDisp = sqrt(sdiff);
        } else {
            for(int i=0; i<spectrumData.elementsCount[_elements] && i<ather->spectrumData.elementsCount[_elements]; i++){
                covr += ather->spectrumData.elementsDiff[_elements][i] * this->spectrumData.elementsDiff[_elements][i];
            }
            double sdiff = ather->spectrumData.elementsDispersion[_elements] * this->spectrumData.elementsDispersion[_elements];
            sqrSummDisp = sqrt(sdiff);
        }
        if(sqrSummDisp > 1e-6)
            correl = covr / sqrSummDisp;

        spectrumData.correl = correl;
        return correl;
    }
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
    *spectrumData.time = (double)_time_in_ms;
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

void SPRSpectrumItemModel::setSpectrumData(uint8_t *buf, uint16_t len)
{
    spectrumData.setData(buf, len);
    if(len == DEF_SPECTRUM_DATA_LENGTH_BYTE){
        recomplite();
    }
}

SPRSpectrumItemModel::SPRSpectrumItemModel()
{
    zones = new SPRSpectrumZonesTableModel();
    formulas = new SPRSettingsFormulaModel();
    setProperty("delete_ranges", QVariant(true));
    setProperty("delete_formulas", QVariant(true));
}


SPRSpectrumItemModel::SPRSpectrumItemModel(QDomDocument *_doc, int _index, ISPRModelData *parent) : ISPRModelData(_doc, parent)
{
    setZonesTable(new SPRSpectrumZonesTableModel(doc));
    setFormulas(new SPRSettingsFormulaModel(doc));
    *spectrumData.thread = _index;
    setProperty("delete_ranges", QVariant(true));
    setProperty("delete_formulas", QVariant(true));
}

SPRSpectrumItemModel::SPRSpectrumItemModel(SPRSpectrumZonesTableModel *_ranges, SPRSettingsFormulaModel *_formulas, ISPRModelData *parent) : ISPRModelData(nullptr, parent)
{
    setZonesTable(_ranges);
    setFormulas(_formulas);
    setProperty("delete_ranges", QVariant(false));
    setProperty("delete_formulas", QVariant(false));
}


