#ifndef SPRSEPARATEGISTOGRAMMWIDGET_H
#define SPRSEPARATEGISTOGRAMMWIDGET_H

#include "ui_sprseparategistogrammwidget.h"
#include "models/sprseparatemodel.h"
#include "models/sprmainmodel.h"
#include "isprwidget.h"
#include "sprporogsmoved.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

class QwtPlotMultiBarChart;

class SPRSeparateGistogrammWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRSeparateModel *separateModel;
    SPRMainModel *model;

    QwtPlotMultiBarChart *gistogramms;
    QwtPlotCurve *graphic;
    QwtPlotCurve *porogHX; // red moved
    QwtPlotCurve *porogHXCurrent; // black states;

    SPRPorogsMoved *pm;

    int threadCurrent; // -1 all channels;

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

public slots:
    void onDblClickMouseEvent();
    void onChangeArgument(QwtPlotItem *, double value, MovedItemPosition);
    void onCompliteButtomClick(bool);
protected slots:
    virtual void onModelChanget(IModelVariable *source);

    void onSetSecectedItem(QwtPlotItem *item, MovedItemPosition);
signals:
    void changePorogsCompite();
};

#endif // SPRSEPARATEGISTOGRAMMWIDGET_H
