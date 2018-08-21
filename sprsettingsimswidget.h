#ifndef SPRSETTINGSIMSWIDGET_H
#define SPRSETTINGSIMSWIDGET_H

#include "ui_sprsettingsimswidget.h"
#include "isprwidget.h"
#include "models/sprsettingsimsmodel.h"
#include "isprsettingswidget.h"

#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_barchart.h>
#include <qwt_plot_zoomer.h>
#include <qwt_legend.h>
#include <qwt_plot_picker.h>

class SPRSettingsIMSWidget : public QWidget, public ISPRWidget, public ISPRSettingsWidget
{
    Q_OBJECT
    SPRSettingsIMSModel *model;

    QwtPlotPicker *pickter;
public:
    explicit SPRSettingsIMSWidget(QWidget *parent = 0);

    ISPRModelData *getModelData();
    void setIms(SPRVariable<uint> *ims){model->setIms(ims);}

private:
    Ui::SPRSettingsIMSWidget ui;

    QwtPlotCurve *curveRed;
    QwtPlotCurve *curveGreen;

    QwtPlotHistogram *gist;

    QwtPlotGrid *grid;
    QwtPlotBarChart *bars;
    QwtPlotZoomer *zoom100;
    QwtLegend *legend;
    // ISPRWidget interface
public:

    ISPRModelData *setModelData(SPRSettingsIMSModel *value);

    void repaintGraphicSetts(double);
public slots:
    virtual void viewChange();
    virtual void viewChange(bool val);
    virtual void widgetsShow();
    void onMouseMoved(QPointF point);
    virtual void onModelChanget(IModelVariable *);
signals:
    void doShow();

    // ISPRWidget interface
protected:

    // ISPRSettingsWidget interface
public:
    virtual void setMasterMode(bool value);
};

#endif // SPRSETTINGSIMSWIDGET_H
