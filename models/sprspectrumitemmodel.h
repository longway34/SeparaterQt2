#ifndef SPRSPECTRUMITEMMODEL_H
#define SPRSPECTRUMITEMMODEL_H

#include <QObject>
#include <QPolygonF>
#include <QByteArray>
#include <QDateTime>

#include "models/isprmodeldata.h"
#include "models/sprspectrumzonestablemodel.h"
#include "models/sprsettingsformulamodel.h"

#include "qwt_interval.h"
#include "qwt_plot_histogram.h"


typedef class spectumItemData{
    uint16_t bufLength;
    uint8_t  *buf;
public:
    char *name;
    uint32_t *thread;
    uint8_t *red, *green, *blue;
    uint16_t *spect;
    uint32_t *summ;
    uint32_t *peak_value;
    uint32_t *peak;
    double *Rs;
    QMap<EnumElements, uint32_t*> elementsSumm;
    uint32_t  *Ns, *Fe, *Cu, *Mo, *Zn, *Mg;
    double *H1, *H2, *H3;
    double *time, *center, *x_ray;
    uint32_t *year, *month, *day, *hours, *min, *sec;

    QMap<EnumElements, QVector<double>> elementsDiff;
    QMap<EnumElements, double> elementsDispersion;
    QMap<EnumElements, double> elementsSigma;
    QMap<EnumElements, double> elementsAverage;
    QMap<EnumElements, double> elementsCount;

    double avr, dispersion, sigma, correl;
    QVector<double> diff;

    spectumItemData(): buf(nullptr){
        setData();
    }
    ~spectumItemData(){
//        if(buf) free(buf);
    }
    void setData(uint8_t* inp = nullptr, uint16_t inplength = DEF_SPECTRUM_DATA_BUF_LENGTH);
    void setData(QByteArray inp);
    void setThread(int _thread){*thread = _thread;}
    uint8_t *getBuf(){ return buf;}
    void setbuf(uint8_t* _buf){ buf = _buf;}
    void setColor(QColor color){
        *red = color.red();
        *green = color.green();
        *blue = color.blue();
    }
    QColor getColor(){
        return QColor(*red, *green, *blue);
    }
    bool isValidData(){
        if(*thread >= MAX_SPR_MAIN_THREADS || *thread < 0){
            return false;
        }
    }

} SpectrumItemData;

Q_DECLARE_METATYPE(SpectrumItemData)

typedef enum _viewAs{
    viewModeAsIs, viewModeOneSecond, viewModeScales
} SPRViewGraphicsMode;

class SPRSpectrumItemModel : public ISPRModelData
{
    SPRSettingsFormulaModel *formulas;
    SPRSpectrumZonesTableModel *zones;
    uint thread;
    SpectrumItemData spectrumData;
    QPolygonF spectGraphData;
    QMap<EnumElements, QVector<QwtIntervalSample>> zonesGraphData;


    SPRViewGraphicsMode graphicMode;
    double scaleMul;

public:
    SPRSpectrumItemModel();
    SPRSpectrumItemModel(QDomDocument *_doc, int _index, ISPRModelData *parent = nullptr);
    SPRSpectrumItemModel(SPRSpectrumZonesTableModel *_ranges, SPRSettingsFormulaModel *_formulas, uint _thread, ISPRModelData *parent = nullptr);

    virtual ~SPRSpectrumItemModel(){
        if(zones != nullptr && QVariant(property("delete_ranges")).toBool()){
            delete zones; zones = nullptr;
            setProperty("delete_ranges", QVariant(false));
        }
        if(formulas != nullptr && QVariant(property("delete_formulas")).toBool()){
            delete formulas; formulas = nullptr;
            setProperty("delete_formulas", QVariant(false));
        }
    }

    double getCorrel(SPRSpectrumItemModel *ather, bool elementsOnly = false, EnumElements _elements = Ns);
    double getCorrel(){
        return spectrumData.correl;
    }

    double getXRay();

    ISPRModelData *setModel(SPRSpectrumZonesTableModel *_ranges, SPRSettingsFormulaModel *_formulas){
        setZonesTable(_ranges);
        setFormulas(_formulas);
        recomplite();
        return this;
    }

    QColor getSpectrumColor(){
        QColor ret(int(*spectrumData.red), int(*spectrumData.green), int(*spectrumData.blue));
        return ret;
    }

    void setSpectrumColor(QColor color){
        *spectrumData.red = (uint8_t)color.red();
        *spectrumData.green = (uint8_t)color.green();
        *spectrumData.blue = (uint8_t)color.blue();
//        emit modelChanget(this);
    }

    QString getSpectrumName(){
        return QString(spectrumData.name);
    }

    void setSpectrumName(QString name){
        memcpy(spectrumData.name, name.toStdString().c_str(), name.length());
//        emit modelChanget(this);
    }

    void setSpectrumThread(uint th){
        *spectrumData.thread = th;
        thread = th;
    }

    uint getSpectrumThread(){
        return *spectrumData.thread;
    }

    SPRSpectrumZonesModel *getZones(){
        uint th = *spectrumData.thread;
        return zones->items[*spectrumData.thread];
    }
    void recomplite();
    void setZonesTable(SPRSpectrumZonesTableModel *value);
    void setFormulas(SPRSettingsFormulaModel *value);
    SpectrumItemData *getSpectrumData();
    void setSpectrumData(uint8_t *buf, uint16_t len = DEF_SPECTRUM_DATA_BUF_LENGTH);

    QMap<EnumElements, QVector<QwtIntervalSample>> getZonesGaphics();
    QPolygonF getSpectrumGraphics();

    void setSpectrumDateTime(QDateTime _dateTime);
    void setTimeScope(uint32_t _time_in_ms);
    double getTimeScope();
    uint getThread() const;
    void setThread(const uint &value);

    SPRViewGraphicsMode getGraphicMode() const;
    void setGraphicMode(const SPRViewGraphicsMode &value, double _scaleMul = 1);

protected:
};

#endif // SPRSPECTRUMITEMMODEL_H
