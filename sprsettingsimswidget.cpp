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


    QLineEdit *le = setNumberCell(ui.tMinMaxParams, 0, 0, model->tMeteringMinStone->getValue(), 0, 1000, tr("Время змерения для минимального куска в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    le = setNumberCell(ui.tMinMaxParams, 0, 1, model->tMeteringMaxStone->getValue(), 0, 1000, tr("Время змерения для максимального куска в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    le = setNumberCell(ui.tMinMaxParams, 0, 2, model->tMeteringMaxMaxStone->getValue(), 0, 1000, tr("Время змерения для максимального камня (бута) в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tMinMaxParams, 1, 0, model->tDelayMinStone->getValue(), 0, 1000, tr("Время задержки на срабатывания ИМ для минимального куска в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    le = setNumberCell(ui.tMinMaxParams, 1, 1, model->tDelayMaxStone->getValue(), 0, 1000, tr("Время задержки на срабатывания ИМ для максимального куска в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    le = setNumberCell(ui.tMinMaxParams, 1, 2, model->tDelayMaxMaxStone->getValue(), 0, 1000, tr("Время задержки на срабатывания ИМ для максимального камня (бута) в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

    le = setNumberCell(ui.tMinMaxParams, 2, 0, model->tDurationMinStone->getValue(), 0, 1000, tr("Длительность импульса ИМ для минимального куска в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    le = setNumberCell(ui.tMinMaxParams, 2, 1, model->tDurationMaxStone->getValue(), 0, 1000, tr("Длительность импульса ИМ для максимального куска в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    le = setNumberCell(ui.tMinMaxParams, 2, 2, model->tDurationMaxMaxStone->getValue(), 0, 1000, tr("Длительность импульса ИМ для максимального камня (бута) в милисекундах"));
    connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    if(model->withMaxMaxStone->getValue()){
        ui.tMinMaxParams->showColumn(2);
    } else {
        ui.tMinMaxParams->hideColumn(2);
    }
    ui.tMinMaxParams->resizeColumnsToContents();
    ui.tMinMaxParams->resizeRowsToContents();
    ui.tMinMaxParams->setHorizontalHeaderLabels(hHeaderTitle);
    ui.tMinMaxParams->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

    ui.cbWithMaxMaxStone->setChecked(model->withMaxMaxStone->getValue());
    connect(ui.cbWithMaxMaxStone, SIGNAL(clicked(bool)), this, SLOT(viewChange(bool)));

    ui.leBlockParam->setValue(model->blockImsParam->getValue());
    connect(ui.leBlockParam, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    ui.leKoefA->setValue(model->kSpeedOreA->getValue());
    connect(ui.leKoefA, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    ui.leKoefB->setValue(model->kSpeedOreB->getValue());
    connect(ui.leKoefB, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    ui.leLimitAbrasion->setValue(model->limitAbrasion->getValue());
    connect(ui.leLimitAbrasion, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    widgetsShow();

    return model;

}

void SPRSettingsIMSWidget::repaintGraphicSetts(double)
{
    double grAdd = (model->timesMettering[model->timesMettering.size()-1]->getValue()/2) * 1.05;
    QVector<QPointF> grDataRed = {
        {0, (double)model->tDelayMinStone->getValue()},
        {(double)model->timesMettering[0]->getValue(), (double)model->tDelayMinStone->getValue()},
        {(double)model->timesMettering[model->timesMettering.size()-1]->getValue()/2, (double)model->tDelayMaxStone->getValue()},
        {grAdd, (double)model->tDelayMaxStone->getValue()}
    };

    QVector<QPointF> grDataGreen = {
        {0, (double)model->tDurationMinStone->getValue()},
        {(double)model->timesMettering[0]->getValue(),(double)model->tDurationMinStone->getValue()},
        {(double)model->timesMettering[model->timesMettering.size()-1]->getValue()/2, (double)model->tDurationMaxStone->getValue()},
        {grAdd, (double)model->tDurationMaxStone->getValue()}
    };

    curveRed->setSamples(grDataRed);
    curveGreen->setSamples(grDataGreen);
    ui.plotParams->setAxisScale(QwtPlot::xBottom, 0, grAdd);
    ui.plotParams->replot();


    QVector<QPointF> grDataYellow;
    for(int i=0; i < model->getIms()->getValue(); i++) {
        double r = ((rand() * 10) % model->limitAbrasion->getValue());
        qlonglong v = model->limitAbrasion->getValue();
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
        model->blockImsParam->setValue(ui.leBlockParam->value());
        return;
    }
    if(sender() == ui.leKoefA){
        model->kSpeedOreA->setValue(ui.leKoefA->value());
        return;
    }
    if(sender() == ui.leKoefB){
        model->kSpeedOreB->setValue(ui.leKoefB->value());
        return;
    }
    if(sender() == ui.leLimitAbrasion){
        model->limitAbrasion->setValue(ui.leLimitAbrasion->value());
        return;
    }
    QObject *tw = sender()->property("tw").value<QObject*>();
    int row = sender()->property("row").toInt();
    int col = sender()->property("col").toInt();

    if(tw == ui.tMinMaxParams){
        QLineEdit *le = (QLineEdit*)sender();
        if(row == 0){
            if(col == 0){
                model->tMeteringMinStone->setValue(le->text().toInt());
                if(model->tMeteringMinStone->getValue() > model->tMeteringMaxStone->getValue()){
                    model->tMeteringMaxStone->setValue(model->tMeteringMinStone->getValue());
                }
            }
            if(col == 1){
                model->tMeteringMaxStone->setValue(le->text().toInt());
                if(model->tMeteringMinStone->getValue() > model->tMeteringMaxStone->getValue()){
                    model->tDurationMinStone->setValue(model->tMeteringMinStone->getValue());
                }
            }
            if(col == 2){
                model->tMeteringMaxMaxStone->setValue(le->text().toInt());
            }
        }
        if(row == 1){
            if(col == 0){
                model->tDelayMinStone->setValue(le->text().toInt());
            }
            if(col == 1){
                model->tDelayMaxStone->setValue(le->text().toInt());
            }
            if(col == 2){
                model->tDelayMaxMaxStone->setValue(le->text().toInt());
            }
        }
        if(row == 2){
            if(col == 0){
                model->tDurationMinStone->setValue(le->text().toInt());
            }
            if(col == 1){
                model->tDurationMaxStone->setValue(le->text().toInt());
            }
            if(col == 2){
                model->tDurationMaxMaxStone->setValue(le->text().toInt());
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

