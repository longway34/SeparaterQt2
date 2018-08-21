#include "sprseparatehistogrammreport.h"

#include <qwt_plot_layout.h>
#include <qwt_date_scale_draw.h>

SPRSeparateHistogrammReport::SPRSeparateHistogrammReport(QWidget *parent):
    QwtPlot(parent), model(nullptr), mainModel(nullptr), drawTitle(nullptr), porogCurveCritical(nullptr), porogCurvePermitt(nullptr), chart(nullptr)
{

}

void SPRSeparateHistogrammReport::setModelData(SPRMainModel *_MainModel, SPRHistoryModel *_histModel, QString _title, QColor _barColor, SPRThreadList _threads, SPRWorkSeparateTypeData _type, bool _woPorogs)
{
    mainModel = _MainModel;
    withOutPorogs = _woPorogs;

    type = _type;
    if(type == percentConcentrate2Input){
        withOutPorogs = true;
    }
    model = _histModel;

    threads = _threads;

    if(mainModel){
        if(threads.size() == 0){
            threads = mainModel->getThreadsList();
        }
        if(model){
            connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));

            setAxisMaxMinor(QwtPlot::xBottom, 0);
            setAxisMaxMajor(QwtPlot::xBottom, threads.size());

            setAxisAutoScale(QwtPlot::yLeft);
            setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignCenter);

            chart = new QwtPlotMultiBarChart();
            chart->setAxes(QwtPlot::xBottom, QwtPlot::yLeft);
            chart->setStyle(QwtPlotMultiBarChart::Stacked);
            setAxisScaleDraw(QwtPlot::xBottom, new DrawTitle(threads.size() > 0));

            porogCurvePermitt = new QwtPlotMarker(tr("Допустимый порог загрузки"));
            porogCurveCritical = new QwtPlotMarker(tr("Критичный порог загрузки"));

            if(!withOutPorogs){
                QPen pen;

                QwtLegend *leg = new QwtLegend();
                this->insertLegend(leg, QwtPlot::BottomLegend);

                pen.setColor(Qt::red); pen.setStyle(Qt::DashLine); pen.setWidth(2);
                porogCurveCritical->setLinePen(pen);
                porogCurveCritical->setLineStyle( QwtPlotMarker::HLine );

                pen.setColor(Qt::blue);
                porogCurvePermitt->setLinePen(pen);
                porogCurvePermitt->setLineStyle( QwtPlotMarker::HLine );

                porogCurvePermitt->attach(this);
                porogCurveCritical->attach(this);

                chart->setBarTitles({QwtText(tr("нормально")), QwtText(tr("допустимо")), QwtText(tr("опасно"))});
                QColor col[3] = {Qt::gray, Qt::blue, Qt::red};
                if(_barColor.isValid()){
                    col[0] = _barColor;
                }

                for(int i=0; i<3; i++){

                    QwtColumnSymbol *symbol = new QwtColumnSymbol( QwtColumnSymbol::Box );
                    symbol->setLineWidth( 2 );
                    symbol->setFrameStyle( QwtColumnSymbol::Raised );
                    symbol->setPalette( QPalette( col[i] ) );

                    chart->setSymbol( i, symbol );
                }
            }
            title = QwtText(_title);
            QFont font = parentWidget()->font();
            font.setPointSize(10);
            title.setFont(font);
            setTitle(title);

            QwtText leftTitle;
            if(type == percentConcentrate2Input){
                leftTitle.setText("%");
                setAxisScale(QwtPlot::yLeft, 0, 100, 25);
            } else if(type == countInputInSec || type == countConcentrateInSec){
                leftTitle.setText(QString(tr("кусков/сек.")));
            } else {
                leftTitle.setText("");
            }
            setAxisTitle(QwtPlot::yLeft, leftTitle);

            plotLayout()->setAlignCanvasToScale( QwtPlot::yLeft, false );

            plotLayout()->setCanvasMargin( 0 );
            updateCanvasMargins();

            chart->attach(this);

            onModelChanget(nullptr);

//            replot();
        }
    }

}

void SPRSeparateHistogrammReport::onModelChanget(IModelVariable* send){
    if(send == model || !send){
        if(model){
            double porog1 = mainModel->getSettingsControlModel()->speedStreamPermiss->getData();
            double porog2 = mainModel->getSettingsControlModel()->speedStreamCritical->getData();

            if(!withOutPorogs){
                porogCurvePermitt->setValue(0, porog1);
                porogCurveCritical->setValue(0, porog2);
            }


            QVector<QVector<double>> series;
            if(threads.size() > 0){
                foreach(uint8_t th, threads){
                    QVector<double> for1Thread;

                    if(withOutPorogs){
                        for1Thread = model->getSamplesToHistogramms(type, th);
                    } else {
                        for1Thread = model->getSamplesToHistogramms(type, th, porog1, porog2);
                    }

                    if(for1Thread.isEmpty()){
                        for1Thread.fill(0, 3);
                    }

                    series.append(for1Thread);

                }
            }
            chart->setSamples(series);
            this->replot();
        }
        return;
    }
}

SPRSeparateHistogrammReport::DrawTitle::DrawTitle(bool _allThreads)
{
    withThreads = _allThreads;
}

QwtText SPRSeparateHistogrammReport::DrawTitle::label(double v) const
{
    int th = static_cast<int>( v );
    QwtText txt;
    if(withThreads){
        txt = QwtText(tr("Ручей %1").arg(th+1));
    } else {
        txt = QwtText(tr("все ручьи"));
    }
    QFont sys = QFont("Tahoma", 8);
    txt.setFont(sys);
    return txt;

}
