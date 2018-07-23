#include "sprseparategistogrammwidget.h"

#include <qwt_plot_renderer.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_multi_barchart.h>
#include <qwt_column_symbol.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>

SPRSeparateGistogrammWidget::SPRSeparateGistogrammWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    ui.qwtPlot->setAutoFillBackground( true );

    ui.qwtPlot->setPalette( Qt::white );
    ui.qwtPlot->canvas()->setPalette( QColor( Qt::lightGray ) );

    threadCurrent = 0;


//    ui.qwtPlot->setTitle(tr("Гистограмма (все каналы)"));

    ui.qwtPlot->setAxisTitle( QwtPlot::yLeft, tr("Кол-во камней") );
    ui.qwtPlot->setAxisTitle( QwtPlot::xBottom, tr("Диапазоны рабочего параметра") );

//    gistogramms = new QwtPlotMultiBarChart( "Bar Chart " );
//    gistogramms->setLayoutPolicy( QwtPlotMultiBarChart::AutoAdjustSamples );
//    gistogramms->setSpacing( 3 );
//    gistogramms->setMargin( 3 );

//    gistogramms->attach( ui.qwtPlot );

//    ui.qwtPlot->insertLegend( new QwtLegend() );

//    populate();
//    ui.qwtPlot->setOrientation( 0 );

    graphic = new QwtPlotCurve();
    graphic->attach(ui.qwtPlot);

    porogHX = new QwtPlotCurve(QString(tr("Действующий порог параметра")));
    porogHX->setPen(QColor(Qt::black), 2, Qt::DashLine);
//    porogHX->setStyle(QwtPlotCurve::Steps);
    porogHX->attach(ui.qwtPlot);

    porogHXCurrent = new QwtPlotCurve(QString(tr("Желательный порог параметра")));
    porogHXCurrent->setPen(QColor(Qt::red), 2, Qt::DashLine);
    porogHXCurrent->attach(ui.qwtPlot);

    ui.qwtPlot->setAutoReplot( true );

    pm = new SPRPorogsMoved(ui.qwtPlot, porogHXCurrent);
    connect(pm, SIGNAL(dblClickMouseEvent()), this, SLOT(onDblClickMouseEvent()));
    connect(pm, SIGNAL(changeArgumentValue(QwtPlotItem*,double,MovedItemPosition)), this, SLOT(onChangeArgument(QwtPlotItem*, double, MovedItemPosition)));
    connect(pm, SIGNAL(setSelectItem(QwtPlotItem*,MovedItemPosition)), this, SLOT(onSetSecectedItem(QwtPlotItem*, MovedItemPosition)));

    ui.qwtPlot->setAxisAutoScale(QwtPlot::xBottom);
    ui.qwtPlot->setAxisAutoScale(QwtPlot::yLeft);

    ui.qwtPlot->plotLayout()->setAlignCanvasToScale( QwtPlot::xBottom, true );
    ui.qwtPlot->plotLayout()->setAlignCanvasToScale( QwtPlot::yLeft, true );

    ui.qwtPlot->plotLayout()->setCanvasMargin( 0 );
    ui.qwtPlot->updateCanvasMargins();

    ui.bComplite->setEnabled(false);
    connect(ui.bComplite, SIGNAL(clicked(bool)), this, SLOT(onCompliteButtomClick(bool)));

}

void SPRSeparateGistogrammWidget::onSetSecectedItem(QwtPlotItem* item, MovedItemPosition){
    if(item){
        porogHXCurrent->setPen(QColor(Qt::red), 4, Qt::DashLine);
    } else {
        porogHXCurrent->setPen(QColor(Qt::red), 2, Qt::DashLine);
    }
}

void SPRSeparateGistogrammWidget::onCompliteButtomClick(bool){
    ui.bComplite->setEnabled(false);

    emit changePorogsCompite();
}


void SPRSeparateGistogrammWidget::onDblClickMouseEvent(){
    if(model){
        threadCurrent++; if(threadCurrent >= model->getThreads()->getData()) threadCurrent = -1;
        widgetsShow();
    }
}

void SPRSeparateGistogrammWidget::onChangeArgument(QwtPlotItem*, double value, MovedItemPosition){
    if(model){
        if(threadCurrent >=0 && threadCurrent < model->getThreads()->getData()){
            SPRVariable<double> *porog = model->getSettingsPorogsModel()->getPorog(threadCurrent, model->getSettingsFormulaModel()->getConditions()->getIndex());
            if(porog){
                double old = porog->getData();
                porog->setData(old + value);
                ui.bComplite->setEnabled(true);
            }
        }
    }
}

SPRSeparateGistogrammWidget::~SPRSeparateGistogrammWidget()
{
}


void SPRSeparateGistogrammWidget::widgetsShow()
{
    if(separateModel){
        QList<QwtText> titles;
//        QVector<QVector<QwtIntervalSample>> data = model->getGistogrammContentData(-1, 10);
        QVector<QPointF> data = separateModel->getGistogrammContentData2(threadCurrent);
        QVector<QColor> col = {Qt::gray, Qt::red, Qt::green, Qt::blue, Qt::cyan};

        if(data.size() > 0){
            double maxValue = 0;
            double minValue = 0;
            for ( int samples = 0; samples < data.size(); samples++ )
            {
                if(data[samples].ry() > maxValue) maxValue = data[samples].ry();
            }

            QString title;
            if(threadCurrent < 0) {
                title = QString(tr("Гистограмма (все каналы)"));
            } else {
                title = QString(tr("Гистограмма (Ручей %1)")).arg( threadCurrent + 1 );
//                titles += title;
            }
            ui.qwtPlot->setTitle(title);

                graphic->setPen(col[(threadCurrent+1)  % col.size()], 2, Qt::SolidLine);
                graphic->setSamples(data);

                if(threadCurrent < 0){
                    porogHX->setVisible(false);
                    porogHXCurrent->setVisible(false);
                } else {
                    if(maxValue < 1){
                        maxValue = 1;
                    }
                    int indHX = model == nullptr ? static_cast<int>(DEF_SPR_FORMULA_CONDITION) :
                                                    model->getSettingsFormulaModel()->getConditions()->getIndex();
                    double porog = model == nullptr ? 1 : model->getSettingsPorogsModel()->getPorog(threadCurrent, indHX)->getData();

                    QVector<QPointF> dataHX = {
                        QPointF(separateModel->getSettingsSeparate()->prg[threadCurrent][indHX], minValue),
                        QPointF(separateModel->getSettingsSeparate()->prg[threadCurrent][indHX], maxValue)

                    };
                    QVector<QPointF> dataHXCurrent = {
                        QPointF(porog, minValue),
                        QPointF(porog, maxValue)
                    };

                    porogHX->setSamples(dataHX);
                    porogHX->setVisible(true);

                    porogHXCurrent->setSamples(dataHXCurrent);
                    porogHXCurrent->setVisible(true);
                }


            ui.qwtPlot->replot();
        }
    }
}

ISPRModelData *SPRSeparateGistogrammWidget::setModelData(ISPRModelData *data)
{
    model = (SPRMainModel*)data;
    if(model){
        connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        separateModel = model->getSeparateModel();

//        rentgenOn = new TCPCommandSeparatorOnFull(model->getServer(), model, toWidget);
        if(separateModel){
            connect(separateModel, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        }
    }
}

ISPRModelData *SPRSeparateGistogrammWidget::getModelData()
{
    return separateModel;
}

void SPRSeparateGistogrammWidget::onModelChanget(IModelVariable *source)
{
    if(model){
        if(sender() == separateModel){
            widgetsShow();
            return;
        }
        if(source == model->getThreads()){
            widgetsShow();
            return;
        }
        QList<IModelVariable *> lporogs = model->getSettingsPorogsModel()->getPorogs()->getAllPorogs();
        if(lporogs.contains(source)){
            widgetsShow();
            return;
        }
    }
}
