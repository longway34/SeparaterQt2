#include "sprsettingsimswidget.h"

ISPRModelData *SPRSettingsIMSWidget::getModel()
{
    return model;
}

ISPRModelData *SPRSettingsIMSWidget::setModel(SPRSettingsIMSModel *value)
{
    model = value;

    ui.tMinMaxParams->clear();
    ui.tMinMaxParams->setRowCount(3);
    QStringList vHeaderTitle = {
        tr("Время измерения куска (мс)"),
        tr("Время задержки на срабатывания ИМ (мс)"),
        tr("Длительность импульса ИМ (мс)")
    };
    QStringList hHeaderTitle = {
        tr("Мин. кусок"), tr("Макс. кусок"), tr("Макс. камень")
    };
    ui.tMinMaxParams->setVerticalHeaderLabels(vHeaderTitle);
    ui.tMinMaxParams->verticalHeader()->sectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui.tMinMaxParams->setColumnCount(3);


    QLineEdit *le = setNumberCell(ui.tMinMaxParams, 0, 0, model->tMeteringMinStone->getData(), 0, 1000, tr("Время змерения для минимального куска в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    le = setNumberCell(ui.tMinMaxParams, 0, 1, model->tMeteringMaxStone->getData(), 0, 1000, tr("Время змерения для максимального куска в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    le = setNumberCell(ui.tMinMaxParams, 0, 2, model->tMeteringMaxMaxStone->getData(), 0, 1000, tr("Время змерения для максимального камня (бута) в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tMinMaxParams, 1, 0, model->tDelayMinStone->getData(), 0, 1000, tr("Время задержки на срабатывания ИМ для минимального куска в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    le = setNumberCell(ui.tMinMaxParams, 1, 1, model->tDelayMaxStone->getData(), 0, 1000, tr("Время задержки на срабатывания ИМ для максимального куска в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    le = setNumberCell(ui.tMinMaxParams, 1, 2, model->tDelayMaxMaxStone->getData(), 0, 1000, tr("Время задержки на срабатывания ИМ для максимального камня (бута) в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tMinMaxParams, 2, 0, model->tDurationMinStone->getData(), 0, 1000, tr("Длительность импульса ИМ для минимального куска в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    le = setNumberCell(ui.tMinMaxParams, 2, 1, model->tDurationMaxStone->getData(), 0, 1000, tr("Длительность импульса ИМ для максимального куска в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    le = setNumberCell(ui.tMinMaxParams, 2, 2, model->tDurationMaxMaxStone->getData(), 0, 1000, tr("Длительность импульса ИМ для максимального камня (бута) в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    if(model->withMaxMaxStone->getData()){
        ui.tMinMaxParams->showColumn(2);
    } else {
        ui.tMinMaxParams->hideColumn(2);
    }
    ui.tMinMaxParams->resizeColumnsToContents();
    ui.tMinMaxParams->resizeRowsToContents();
    ui.tMinMaxParams->setHorizontalHeaderLabels(hHeaderTitle);
    ui.tMinMaxParams->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

    ui.cbWithMaxMaxStone->setChecked(model->withMaxMaxStone->getData());
    connect(ui.cbWithMaxMaxStone, SIGNAL(clicked(bool)), this, SLOT(viewChange(bool)));

    ui.leBlockParam->setValue(model->blockImsParam->getData());
    connect(ui.leBlockParam, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    ui.leKoefA->setValue(model->kSpeedOreA->getData());
    connect(ui.leKoefA, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    ui.leKoefB->setValue(model->kSpeedOreB->getData());
    connect(ui.leKoefB, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    ui.leLimitAbrasion->setValue(model->limitAbrasion->getData());
    connect(ui.leLimitAbrasion, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    widgetsShow();

    return model;

}

void SPRSettingsIMSWidget::repaintGraphicSetts(double)
{
    double grAdd = (model->timesMettering[model->timesMettering.size()-1]->getData()/2) * 1.05;
    QVector<QPointF> grDataRed = {
        {0, (double)model->tDelayMinStone->getData()},
        {(double)model->timesMettering[0]->getData(), (double)model->tDelayMinStone->getData()},
        {(double)model->timesMettering[model->timesMettering.size()-1]->getData()/2, (double)model->tDelayMaxStone->getData()},
        {grAdd, (double)model->tDelayMaxStone->getData()}
    };

    QVector<QPointF> grDataGreen = {
        {0, (double)model->tDurationMinStone->getData()},
        {(double)model->timesMettering[0]->getData(),(double)model->tDurationMinStone->getData()},
        {(double)model->timesMettering[model->timesMettering.size()-1]->getData()/2, (double)model->tDurationMaxStone->getData()},
        {grAdd, (double)model->tDurationMaxStone->getData()}
    };

    curveRed->setSamples(grDataRed);
    curveGreen->setSamples(grDataGreen);
    ui.plotParams->setAxisScale(QwtPlot::xBottom, 0, grAdd);
    ui.plotParams->replot();


    QVector<QPointF> grDataYellow;
    for(int i=0; i < model->getIms()->getData(); i++) {
        double r = ((rand() * 10) % model->limitAbrasion->getData());
        qlonglong v = model->limitAbrasion->getData();
        double var = r / v * 100.;
        grDataYellow.push_back(QPointF(i, var));
    }
    bars->setSamples(grDataYellow);
    ui.plotStatistuka->setAxisScale(QwtPlot::yLeft, 0, 100);
    ui.plotStatistuka->replot();

}


SPRSettingsIMSWidget::SPRSettingsIMSWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    curveRed = new QwtPlotCurve(tr("Задержка"));
    curveRed->setPen(QColor(Qt::red), 2, Qt::SolidLine);
    curveRed->attach(ui.plotParams);

    curveGreen = new QwtPlotCurve(tr("Длительность"));
    curveGreen->setPen(QColor(Qt::green), 2, Qt::SolidLine);
    curveGreen->attach(ui.plotParams);

    grid = new QwtPlotGrid();
    grid->enableXMin(true);
    grid->setMajorPen(QColor(Qt::black), 0.1, Qt::DotLine);
    grid->setMinorPen(QColor(Qt::gray), 0.1, Qt::DotLine);
    grid->attach(ui.plotParams);

    gist = new QwtPlotHistogram(tr("Статистика"));
    ui.plotStatistuka->setTitle(tr("Износ ИМ (%)"));
    ui.plotStatistuka->setFont(this->font());
    grid->attach(ui.plotStatistuka);

    gist->setPen(QColor(Qt::yellow), 1, Qt::DotLine);

    bars = new QwtPlotBarChart(tr("Статистика"));
    bars->attach(ui.plotStatistuka);

    legend = new QwtLegend();
    ui.plotParams->insertLegend(legend, QwtPlot::TopLegend);

    pickter = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPicker::CrossRubberBand, QwtPicker::AlwaysOn, ui.plotParams->canvas());
}


void SPRSettingsIMSWidget::widgetsShow()
{
    repaintGraphicSetts(0);
}

void SPRSettingsIMSWidget::onMouseMoved(QPointF point)
{
    int x = 0;
}

void SPRSettingsIMSWidget::viewChange()
{
    if(sender() == ui.leBlockParam){
        model->blockImsParam->setData(ui.leBlockParam->value());
        return;
    }
    if(sender() == ui.leKoefA){
        model->kSpeedOreA->setData(ui.leKoefA->value());
        return;
    }
    if(sender() == ui.leKoefB){
        model->kSpeedOreB->setData(ui.leKoefB->value());
        return;
    }
    if(sender() == ui.leLimitAbrasion){
        model->limitAbrasion->setData(ui.leLimitAbrasion->value());
        return;
    }
    QObject *tw = sender()->property("tw").value<QObject*>();
    int row = sender()->property("row").toInt();
    int col = sender()->property("col").toInt();

    if(tw == ui.tMinMaxParams){
        QLineEdit *le = (QLineEdit*)sender();
        if(row == 0){
            if(col == 0){
                model->tMeteringMinStone->setData(le->text().toInt());
                if(model->tMeteringMinStone->getData() > model->tMeteringMaxStone->getData()){
                    model->tMeteringMaxStone->setData(model->tMeteringMinStone->getData());
                }
            }
            if(col == 1){
                model->tMeteringMaxStone->setData(le->text().toInt());
                if(model->tMeteringMinStone->getData() > model->tMeteringMaxStone->getData()){
                    model->tDurationMinStone->setData(model->tMeteringMinStone->getData());
                }
            }
            if(col == 2){
                model->tMeteringMaxMaxStone->setData(le->text().toInt());
            }
        }
        if(row == 1){
            if(col == 0){
                model->tDelayMinStone->setData(le->text().toInt());
            }
            if(col == 1){
                model->tDelayMaxStone->setData(le->text().toInt());
            }
            if(col == 2){
                model->tDelayMaxMaxStone->setData(le->text().toInt());
            }
        }
        if(row == 2){
            if(col == 0){
                model->tDurationMinStone->setData(le->text().toInt());
            }
            if(col == 1){
                model->tDurationMaxStone->setData(le->text().toInt());
            }
            if(col == 2){
                model->tDurationMaxMaxStone->setData(le->text().toInt());
            }
        }
        model->setTimesMeassureDelayDuration();
        emit doShow();

        repaintGraphicSetts(0);
    }
}

void SPRSettingsIMSWidget::viewChange(bool val)
{
    if(sender() == ui.cbWithMaxMaxStone){
        if(!val){
            ui.tMinMaxParams->hideColumn(2);
        } else {
            ui.tMinMaxParams->showColumn(2);
        }
    }
    ui.tMinMaxParams->resizeColumnsToContents();
    ui.tMinMaxParams->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
}

