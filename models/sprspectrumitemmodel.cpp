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
        if(inplength == DEF_SPECTRUM_DATA_LENGTH){
            memcpy(spect, inp, inplength);
            return;
        }
        qDebug() << "Error Data Length... Try " << QString::number(DEF_SPECTRUM_DATA_BUF_LENGTH) << " to all spectrum data or " << QString::number(DEF_SPECTRUM_DATA_LENGTH) << " for spectrum only...";
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
    for(int i=0; i<(DEF_SPECTRUM_DATA_LENGTH / sizeof(uint16_t)); i++){
        uint32_t val = spectrumData.spect[i];
        *spectrumData.summ = val;
        foreach (EnumElements el, zones->items[*spectrumData.thread]->elements.keys()) {
            int min = zones->items[*spectrumData.thread]->elements[el].min->getData();
            int max = zones->items[*spectrumData.thread]->elements[el].max->getData();
            if(i >= min && i < max){
                (*(spectrumData.elementsSumm[el])) += val;
            }
            if(val > *spectrumData.peak_value){
                *spectrumData.peak = i;
                *spectrumData.peak_value = val;
            }
        }
    }
    *spectrumData.center = *spectrumData.summ / (DEF_SPECTRUM_DATA_LENGTH / sizeof(uint16_t));
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
    foreach (EnumElements el, zone->elements.keys()) {

        double value = 0.5;
        for(int i=zone->elements[el].min->getData(); i<zone->elements[el].max->getData(); i++){
            if(value < spectrumData.spect[i]){
                value = (double(spectrumData.spect[i]));
            }
        }
        double xmin = qreal(zone->elements[el].min->getData());
        double xmax = qreal(zone->elements[el].max->getData());
        QVector<QwtIntervalSample> inter;
        inter.push_back(QwtIntervalSample(value, xmin, xmax));
        zonesGraphData[el] = inter;
    }
    return zonesGraphData;
}

QPolygonF SPRSpectrumItemModel::getSpectrumGraphics(){
    spectGraphData.clear();
    for(int i=0; i<DEF_SPECTRUM_DATA_LENGTH / sizeof(uint16_t); i++){
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

SpectrumItemData *SPRSpectrumItemModel::getSpectrumData()
{
    return &spectrumData;
}

void SPRSpectrumItemModel::setSpectrumData(uint8_t *buf, uint16_t len)
{
    spectrumData.setData(buf, len);
    if(len == DEF_SPECTRUM_DATA_LENGTH){
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

