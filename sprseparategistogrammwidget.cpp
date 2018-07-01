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

    ui.qwtPlot->setTitle(tr("Гистограмма (все каналы)"));

    ui.qwtPlot->setAxisTitle( QwtPlot::yLeft, tr("Кол-во камней") );
    ui.qwtPlot->setAxisTitle( QwtPlot::xBottom, tr("Диапазоны рабочего параметра") );

    gistogramms = new QwtPlotMultiBarChart( "Bar Chart " );
    gistogramms->setLayoutPolicy( QwtPlotMultiBarChart::AutoAdjustSamples );
    gistogramms->setSpacing( 3 );
    gistogramms->setMargin( 3 );

    gistogramms->attach( ui.qwtPlot );

    ui.qwtPlot->insertLegend( new QwtLegend() );

//    populate();
//    ui.qwtPlot->setOrientation( 0 );

    ui.qwtPlot->setAutoReplot( true );

    QList<QwtText> titles;
    for ( int i = 0; i < MAX_SPR_MAIN_THREADS; i++ )
    {
        QString title(tr("Ручей %1"));
        titles += title.arg( i );
    }
    gistogramms->setBarTitles( titles );
    gistogramms->setLegendIconSize( QSize( 10, 14 ) );

    QVector< QVector<double> > series;
    for ( int i = 0; i < MAX_SPR_MAIN_THREADS; i++ )
    {
        QVector<double> values;
        for ( int j = 0; j < 10; j++ )
            values += 0;

        series += values;
    }

    gistogramms->setSamples( series );
    gistogramms->setStyle( QwtPlotMultiBarChart::Grouped );

    ui.qwtPlot->setAxisScale( QwtPlot::yLeft, 0, 100 );


    ui.qwtPlot->plotLayout()->setAlignCanvasToScale( QwtPlot::xBottom, true );

    ui.qwtPlot->plotLayout()->setCanvasMargin( 0 );
    ui.qwtPlot->updateCanvasMargins();

}

SPRSeparateGistogrammWidget::~SPRSeparateGistogrammWidget()
{
//    for(int i=0; i< gistogramms.size(); i++){
//        delete gistogramms[i];
//    }
}


void SPRSeparateGistogrammWidget::widgetsShow()
{
    if(model){
        QList<QwtText> titles;
        QVector<QVector<QwtIntervalSample>> data = model->getGistogrammContentData(-1, 10);
        QVector<QColor> col = {Qt::red, Qt::green, Qt::blue, Qt::cyan};

        if(data.size() > 0){
            for ( int i = 0; i < data.size(); i++ )
            {
                QString title(tr("Ручей %1"));
                titles += title.arg( i );
                QwtColumnSymbol *symbol = new QwtColumnSymbol( QwtColumnSymbol::Box );
                QList<QwtText> xTitles;

//                if(data[0].size() <= 10){
                    QString btitle("%1-%2");
                    xTitles += btitle.arg(QString::number(data[0][i].interval.minValue(),'f',0))
                            .arg(QString::number(data[0][i].interval.maxValue(),'f',0));
                    symbol->setLineWidth( 2 );
                    symbol->setFrameStyle( QwtColumnSymbol::Raised );
//                } else {
//                    QString title("%1");
//                    xTitles += title.arg(QString::number(data[0][i].interval.maxValue(),'f',0));
//                    symbol->setLineWidth( 1 );
//                    symbol->setFrameStyle( QwtColumnSymbol::Plain );
//                }

                symbol->setPalette( QPalette( col[i % col.size()] ) );

                gistogramms->setSymbol( i, symbol );
            }
            gistogramms->setBarTitles( titles );

            gistogramms;

            int samplesNumber = data[0].size();
            for(int i = 0; i < data.size(); i++){
            }


//            QVector<QVector<double>> samples;

            QVector<QwtSetSample> samples;

            for(int i=0; i<samplesNumber; i++){
//                QVector<double> vth;
                QwtSetSample vth;
                vth.value = (data[0][i].interval.maxValue() - data[0][i].interval.minValue()) / 2 + data[0][i].interval.minValue();
                for(int th=0; th < data.size(); th++){
//                   vth += data[th][i].value;
                   vth.set += data[th][i].value;
    //            if(gistogramms.size() <= th){
    //                QwtPlotHistogram *plGist = new QwtPlotHistogram(QString(tr("Ручей %1")).arg(th));
    //                gistogramms.push_back(plGist);
    //                gistogramms[th]->setPen(col[th % col.size()]);
    //                gistogramms[th]->attach(ui.qwtPlot);
    //            }
                }
                samples += vth;
            }
            gistogramms->setSamples(samples);

            ui.qwtPlot->setAxisAutoScale(QwtPlot::xBottom);
            ui.qwtPlot->setAxisAutoScale(QwtPlot::yLeft);

            ui.qwtPlot->plotLayout()->setAlignCanvasToScale( QwtPlot::xBottom, true );
            ui.qwtPlot->plotLayout()->setAlignCanvasToScale( QwtPlot::yLeft, true );

            ui.qwtPlot->plotLayout()->setCanvasMargin( 0 );
            ui.qwtPlot->updateCanvasMargins();

            ui.qwtPlot->replot();
        }
    }
}

ISPRModelData *SPRSeparateGistogrammWidget::setModelData(ISPRModelData *data)
{
    model = (SPRSeparateModel*)data;
    if(model){
        connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
    }
}

ISPRModelData *SPRSeparateGistogrammWidget::getModelData()
{
    return model;
}

void SPRSeparateGistogrammWidget::onModelChanget(IModelVariable *)
{
}
