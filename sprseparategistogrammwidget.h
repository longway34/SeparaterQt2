#ifndef SPRSEPARATEGISTOGRAMMWIDGET_H
#define SPRSEPARATEGISTOGRAMMWIDGET_H

#include "ui_sprseparategistogrammwidget.h"
#include "models/sprseparatemodel.h"
#include "isprwidget.h"

#include <qwt_plot.h>

class QwtPlotMultiBarChart;

class SPRSeparateGistogrammWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRSeparateModel *model;

    QwtPlotMultiBarChart *gistogramms;

//    QVector<QwtPlotHistogram*> gistogramms;

public:
    Ui::SPRSeparateGistogrammWidget ui;
    explicit SPRSeparateGistogrammWidget(QWidget *parent = 0);

    virtual ~SPRSeparateGistogrammWidget();
private:

    // ISPRWidget interface
public:
    virtual void widgetsShow();
    virtual ISPRModelData *setModelData(ISPRModelData *data);
    virtual ISPRModelData *getModelData();

protected slots:
    virtual void onModelChanget(IModelVariable *);
};

#endif // SPRSEPARATEGISTOGRAMMWIDGET_H
