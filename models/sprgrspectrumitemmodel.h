#ifndef SPRGRSPECTRUMITEMMODEL_H
#define SPRGRSPECTRUMITEMMODEL_H

#include <QObject>
#include <QWidget>
#include <QVector>

#include "qwt_plot_histogram.h"
#include "qwt_plot_curve.h"

#include "models/sprspectrumitemmodel.h"
#include "models/sprspectrumzonesmodel.h"
#include "models/isprmodeldata.h"
#include <QVector>

#include "_types.h"

class SPRGrSpectrumItemModel : public ISPRModelData
{
    Q_OBJECT

    SPRSpectrumZonesModel *rangesModel;
    SPRSpectrumItemModel *spectModel;

    QPolygonF spectGraphData;
    QMap<EnumElements, QVector<QwtIntervalSample>> zonesGraphData;

    SPRGrSpectrumItemModel *complite();
    SPRGrSpectrumItemModel *isComplite(){
        return zonesGraphData.size() > 0 ? this : nullptr;
    }

public:
    explicit SPRGrSpectrumItemModel(SPRSpectrumZonesModel *_ranges, SPRSpectrumItemModel *_spect = nullptr);

    SPRGrSpectrumItemModel *setSpectModel(SPRSpectrumItemModel *_spectModel);
    SPRGrSpectrumItemModel *setRangesModel(SPRSpectrumZonesModel *_rangesModel);
    QPolygonF getSamples() const;
    QMap<EnumElements, QVector<QwtIntervalSample> > getIntervalSamples() const;
    QColor getColorZone(EnumElements el);
signals:

public slots:
};

#endif // SPRGRSPECTRUMITEMMODEL_H
