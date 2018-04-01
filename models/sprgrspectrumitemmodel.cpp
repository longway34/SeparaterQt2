#include "sprgrspectrumitemmodel.h"

SPRGrSpectrumItemModel *SPRGrSpectrumItemModel::setSpectModel(SPRSpectrumItemModel *_spectModel)
{
    spectModel = _spectModel;
    return complite();
}

SPRGrSpectrumItemModel *SPRGrSpectrumItemModel::setRangesModel(SPRSpectrumZonesModel *_rangesModel)
{
    rangesModel = _rangesModel;
    return complite();
}

QPolygonF SPRGrSpectrumItemModel::getSamples() const
{
    return spectGraphData;
}

QMap<EnumElements, QVector<QwtIntervalSample>> SPRGrSpectrumItemModel::getIntervalSamples() const
{
    return zonesGraphData;
}

QColor SPRGrSpectrumItemModel::getColorZone(EnumElements el)
{
    return rangesModel->elements[el].color;
}

SPRGrSpectrumItemModel *SPRGrSpectrumItemModel::complite()
{
    if(rangesModel){
        zonesGraphData.clear();
        foreach (EnumElements el, rangesModel->elements.keys()) {
            double value = 0.5;
            double xmin = qreal(rangesModel->elements[el].min->getData());
            double xmax = qreal(rangesModel->elements[el].max->getData());
            QVector<QwtIntervalSample> inter;
            inter.push_back(QwtIntervalSample(value, xmin, xmax));
            zonesGraphData[el] = inter;
            value = zonesGraphData[el][zonesGraphData[el].size() - 1].value;
            value = 0.5;
        }
        spectGraphData.clear();
        for(int i=0; i < DEF_SPECTRUM_DATA_LENGTH / sizeof(uint16_t); i++){
            if(spectModel){
                uint16_t *rawSpect = spectModel->getSpectrumData()->spect;
                if(rawSpect){
                    uint16_t value = rawSpect[i];
                    foreach(EnumElements z, rangesModel->elements.keys()){
                        SpectorRange elData = rangesModel->elements[z];
                        int min = elData.min->getData(); int max = elData.max->getData();
                        if(i >= min && i < max){
                            if(zonesGraphData[z][0].value < value){
                                zonesGraphData[z][0].value = value;
                            }
                        }
                    }
                    if(value > 0){
                        int iii = 0;
                    }
                    spectGraphData.push_back(QPointF(qreal(i), value));
                }
            } else {
                spectGraphData.push_back(QPointF(qreal(i), 0));
            }
        }
    }
    return isComplite();
}

SPRGrSpectrumItemModel::SPRGrSpectrumItemModel(SPRSpectrumZonesModel *_ranges, SPRSpectrumItemModel *_spect) :
    rangesModel(_ranges), spectModel(_spect)
{
    spectGraphData.clear();
    zonesGraphData.clear();

    complite();
}
