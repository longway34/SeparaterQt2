#include "sprsettingsporogswidget.h"
#include <QObjectUserData>

ISPRModelData *SPRSettingsPorogsWidget::setModel(ISPRModelData *value)
{
    model = (SPRSettingsPorogsModel*)value;
    ui.wPorogs->setModel(model->getPorogs());
    ui.wPorogs->setConditions(model->getConditions());
    ui.wPorogs->setThreads(model->getThreads());
    widgetsShow();
    return model;
}

void SPRSettingsPorogsWidget::repaintGraphic(double)
{
    QVector<QPointF> grData = {
        {0, ui.leMinStone->value() * 10},
        {model->getTMeteringMinStone()->getData(), ui.leMinStone->value() * 10},
        {model->getTMeteringMaxStone()->getData(), ui.leMaxStone->value() * 10},
        {model->getTMeteringMaxStone()->getData() * 1.05, ui.leMaxStone->value() * 10}
    };

//    double x[] = {0, 15, 100, 115};
//    double y[] = {ui.leMinStone->value() * 10,
//                 ui.leMinStone->value() * 10,
//                 ui.leMaxStone->value() * 10,
//                 ui.leMaxStone->value() * 10};


    curve->setSamples(grData);
    ui.plotKoeffLengths->setAxisScale(QwtPlot::xBottom, 0, model->getTMeteringMaxStone()->getData() * 1.05);
    ui.plotKoeffLengths->replot();
}

SPRSettingsPorogsWidget::SPRSettingsPorogsWidget(QWidget *parent) :
    QWidget(parent), ISPRWidget()
{
    ui.setupUi(this);
    ui.wPorogs->ui.gbPorogs->setTitle("");

    ui.rbTail->setProperty("selections", QVariant(OnTail));
    ui.rbConcentrat->setProperty("selections", QVariant(OnConsentrate));
    bgTypeSelection = new QButtonGroup(this);
    bgTypeSelection->addButton(ui.rbTail, 0);
    bgTypeSelection->addButton(ui.rbConcentrat, 1);
    connect(bgTypeSelection, SIGNAL(buttonClicked(QAbstractButton*)), SLOT(viewChange(QAbstractButton*)));

    ui.rbH1Resume->setProperty("conditions", QVariant(H1));
    ui.rbH2Resume->setProperty("conditions", QVariant(H2));
    ui.rbH3Resume->setProperty("conditions", QVariant(H3));
    bgTypeConditions = new QButtonGroup(this);
    bgTypeConditions->addButton(ui.rbH1Resume, 0);
    bgTypeConditions->addButton(ui.rbH2Resume, 1);
    bgTypeConditions->addButton(ui.rbH3Resume, 2);
    connect(bgTypeConditions, SIGNAL(buttonClicked(QAbstractButton*)), SLOT(viewChange(QAbstractButton*)));

    curve = new QwtPlotCurve(tr("Коэффициеты"));
    curve->setPen(QColor(Qt::red), 2, Qt::SolidLine);
    curve->attach(ui.plotKoeffLengths);

    grid = new QwtPlotGrid();
    grid->enableXMin(true);
    grid->setMajorPen(QColor(Qt::black), 0.1, Qt::DotLine);
    grid->setMinorPen(QColor(Qt::gray), 0.1, Qt::DotLine);
    grid->attach(ui.plotKoeffLengths);

    pickter = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPicker::CrossRubberBand, QwtPicker::AlwaysOn, ui.plotKoeffLengths->canvas());

    connect(ui.leMaxStone, SIGNAL(valueChanged(double)), SLOT(repaintGraphic(double)));
    connect(ui.leMaxStone, SIGNAL(valueChanged(double)), SLOT(viewChange(double)));
    connect(ui.leMinStone, SIGNAL(valueChanged(double)), SLOT(repaintGraphic(double)));
    connect(ui.leMinStone, SIGNAL(valueChanged(double)), SLOT(viewChange(double)));

    connect(bgTypeSelection, SIGNAL(buttonClicked(int)), SLOT(viewChange(int)));
    connect(ui.leXRayCorrection, SIGNAL(valueChanged(double)), SLOT(viewChange(double)));

    connect(ui.cbInvert, SIGNAL(toggled(bool)), SLOT(viewChange(bool)));

}

void SPRSettingsPorogsWidget::widgetsShow()
{
    ui.leMinStone->setValue(model->forMinStone->getData());
    ui.leMaxStone->setValue(model->forMaxStone->getData());
    ui.leXRayCorrection->setValue(model->xRayCorrection->getData());

    TypeSelection type = model->typeSelection->getData();
    if(type == OnConsentrate){
        ui.rbConcentrat->setChecked(true);
    } else if(type == OnTail){
        ui.rbTail->setChecked(true);
    }

    TypeConditions cond = model->getConditions()->getData();
    switch (cond) {
    case H1:
        ui.rbH1Resume->setChecked(true);
        break;
    case H2:
        ui.rbH2Resume->setChecked(true);
        break;
    case H3:
        ui.rbH3Resume->setChecked(true);
        break;
    default:
        break;
    }

    repaintGraphic(0);
    ui.wPorogs->widgetsShow();
}

ISPRModelData *SPRSettingsPorogsWidget::getModel()
{
    return model;
}

void SPRSettingsPorogsWidget::viewChange(int data)
{
}

void SPRSettingsPorogsWidget::viewChange(bool data)
{
    if(sender() == ui.cbInvert){ // изменили инверсию отбора
        model->invertSelection->setData(data);
        return;
    }
}

void SPRSettingsPorogsWidget::viewChange(double data)
{
    if(sender() == ui.leMaxStone){ // изменили коэффициент для максимального камня
       double k = ui.leMaxStone->value();
       model->forMaxStone->setData(k);
    }
    if(sender() == ui.leMinStone){ // изменили коэффициент для минимального камня
        double k = ui.leMinStone->value();
        model->forMinStone->setData(k);
        return;
    }
    if(sender() == ui.leXRayCorrection){ // изменили коэффициент коррекции X-Ray
        double k = ui.leXRayCorrection->value();
        model->xRayCorrection->setData(k);
        return;
    }
}

void SPRSettingsPorogsWidget::viewChange(QAbstractButton *btn)
{
    if(sender() == bgTypeConditions){ // изменили состояние выводов по сепарации
        TypeConditions val = btn->property("conditions").value<TypeConditions>();
        model->getConditions()->setData(val);
        emit doShow();
    }
    if(sender() == bgTypeSelection){ // изменили состояние сортировки хвост/концентрат
        TypeSelection val = btn->property("selections").value<TypeSelection>();;
        model->typeSelection->setData(val);
        return;
    }
}


//void SPRSettingsPorogsWidget::viewChange(QTableWidget *, int, int)
//{
//}
