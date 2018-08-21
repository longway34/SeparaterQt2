#include "sprstonesizeschart.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QPieSeries>
#include "_types.h"

SPRStoneSizesChart::SPRStoneSizesChart(QWidget *parent) :
    QWidget(parent), allThreadsSeries(nullptr), mainModel(nullptr), historyModel(nullptr), separateModel(nullptr)
{
            chart = new StoneChart();

            chart->setTheme(QChart::ChartThemeLight);
            chart->setAnimationOptions(QChart::AllAnimations);
            chart->legend()->setVisible(true);
            chart->legend()->setAlignment(Qt::AlignRight);
            chart->legend()->setShowToolTips(true);

            allThreadsSeries = new QPieSeries();
            allThreadsSeries->setName(tr("Все ручьи"));


            QChartView *chartView = new QChartView(chart);
            chartView->setLineWidth(0);
            chartView->setContentsMargins(0,0,0,0);
//            chartView->layout()->setContentsMargins(0,0,0,0);
//            chartView->layout()->setSpacing(0);
            chartView->setRenderHint(QPainter::Antialiasing);
            setLayout(new QBoxLayout(QBoxLayout::TopToBottom));

            setSizePolicy(*new QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

            layout()->setMargin(0);
            layout()->setSpacing(0);
            layout()->addWidget(chartView);


//            QPieSeries *yearSeries = new QPieSeries(this);
//            yearSeries->setName("Sales by year - All");

//            const QStringList months = {
//                "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
//            };
//            const QStringList names = {
//                "Jane", "John", "Axel", "Mary", "Susan", "Bob"
//            };

//            for (const QString &name : names) {
//                QPieSeries *series = new QPieSeries(this);
//                series->setName("Sales by month - " + name);

//                for (const QString &month : months)
//                    *series << new PercentStoneSizesSlice(qrand() % 1000, month, yearSeries);

//                QObject::connect(series, &QPieSeries::clicked, chart, &StoneChart::handleSliceClicked);

//                *yearSeries << new PercentStoneSizesSlice(series->sum(), name, series);
//            }

//            QObject::connect(yearSeries, &QPieSeries::clicked, chart, &StoneChart::handleSliceClicked);

//            chart->changeSeries(yearSeries);

}

void SPRStoneSizesChart::initSeries(){
    if(!allThreadsSeries){
        allThreadsSeries = new QPieSeries(this);
        allThreadsSeries->setObjectName(QString("Series_all"));
    }

    QList<int> groups = {0, 1, 2, 3, 4};

    allThreadsSeries->clear();
    foreach(uint8_t th, threads){
        QPieSeries *thSeries = new QPieSeries(this);
        thSeries->setObjectName(QString("thSeries_th_%1").arg(th));
        groupSeries << thSeries;
        thSeries->setName(titlesThreadsSlides[th].title);
        foreach(int gr, groups){

            PercentStoneSizesSlice *slice = new PercentStoneSizesSlice(1/* / groups.size()*/, titlesStonesSlides[gr].title, allThreadsSeries);
            slice->setColor(titlesStonesSlides[gr].color);
            slice->setObjectName(QString("slice_th_%1_gr_%2").arg(th).arg(gr));

            *thSeries << slice;
        }

        QObject::connect(thSeries, &QPieSeries::clicked, chart, &StoneChart::handleSliceClicked);
        PercentStoneSizesSlice *slice = new PercentStoneSizesSlice(1/* / threads.size()*/, titlesThreadsSlides[th].title, thSeries);
        slice->setColor(titlesThreadsSlides[th].color);
        slice->setObjectName(QString("slice_th_%1").arg(th));

        *allThreadsSeries << slice;
    }
    QObject::connect(allThreadsSeries, &QPieSeries::clicked, chart, &StoneChart::handleSliceClicked);

    chart->changeSeries(allThreadsSeries);
}

void SPRStoneSizesChart::setSeriesData()
{
    QAbstractSeries *_current = chart->currentSeries();
    QVector<double> toAllThs;
    foreach(uint8_t th, threads){
        if(historyModel->getLast()){
        toAllThs.append(historyModel->getLast()->source.i_prd[th][SPR_SEPARATE_OUT_INDEX_ALL]);
        } else {
            toAllThs.append(0);
        }
    }

//    QVector<QVector<double>> toAllThs = historyModel->getMTableCounts(threads);
    QVector<QVector<double>> stonesCounts = historyModel->getMTableSizesStones(threads);
    uint8_t countTh = 0;
    foreach(QPieSlice *oneChannelSlice, allThreadsSeries->slices()){
//        oneChannelSlice->setColor(titlesThreadsSlides[countTh].color);
        oneChannelSlice->setValue(toAllThs[countTh]);

        PercentStoneSizesSlice *ps = static_cast<PercentStoneSizesSlice*>(oneChannelSlice);
        if(ps){
            ps->updateLabel();
        }

        countTh++;
    }
    countTh = 0;
    foreach(QPieSeries *oneChannelSeries, groupSeries){
//        oneChannelSeries->setName(titlesThreadsSlides[countTh].title);
        int groupNum = 0;
        foreach(QPieSlice *oneGroup, oneChannelSeries->slices()){

//            oneGroup->setColor(titlesStonesSlides[groupNum].color);
//            oneGroup->setLabel(titlesStonesSlides[groupNum].title);
            oneGroup->setValue(stonesCounts[countTh][groupNum]);

            PercentStoneSizesSlice *ps = static_cast<PercentStoneSizesSlice*>(oneGroup);
            if(ps){
                ps->updateLabel();
            }
            groupNum++;
        }
    }


    chart->changeSeries(_current);
}

void SPRStoneSizesChart::setTitles(){

    titlesThreadsSlides.clear();
    foreach(uint8_t th, threads){
        titlesThreadsSlides.append(title_slides_data({th, QString(tr("Руч.%1\n")).arg(th+1), defThreadsColor[th]}));
    }

    titlesStonesSlides[0] = title_slides_data({0, QString(tr("Ост.\n")), defThreadsColor[0]});

    for(uint8_t i=1; i<DEF_SPR_IMS_PARTS_SIZE+1;i++){
        if(separateModel->isSeparateEmpty()){
            titlesStonesSlides[i] = title_slides_data({i, QString("%1-%2\n").
                arg(mainModel->getSettingsIMSModel()->timesMettering[i-1]->getData()).
                arg(mainModel->getSettingsIMSModel()->timesMettering[i]->getData()),
                                    defThreadsColor[i]});
        } else {
            titlesStonesSlides[i] = title_slides_data({i, QString("%1-%2\n").
                arg(mainModel->getSeparateModel()->getSettingsSeparate()->tiz[i-1]).
                arg(mainModel->getSeparateModel()->getSettingsSeparate()->tiz[i]),
                                    defThreadsColor[i]});
        }
    }
}

void SPRStoneSizesChart::onModelChandet(IModelVariable *send){
    if(send == historyModel || sender() == historyModel){
//        chart->changeSeries(allThreadsSeries);
        setSeriesData();
    }
}

void SPRStoneSizesChart::setModels(SPRMainModel *_mainModel, SPRHistoryModel *_historyModel, SPRSeparateModel *_separateModel)
{
    mainModel = _mainModel;
    if(mainModel){
        threads = mainModel->getThreadsList();
    } else {
        threads = getAllThreadsListDefault();
    }
    if(_historyModel){
        historyModel = _historyModel;
        connect(historyModel, &SPRHistoryModel::modelChanget, this, &SPRStoneSizesChart::onModelChandet);
    }
    if(!_separateModel){
        if(mainModel){
            separateModel = mainModel->getSeparateModel();
        }
    } else {
        separateModel = _separateModel;
    }

    setTitles();
    initSeries();

    chart->changeSeries(allThreadsSeries);

}
