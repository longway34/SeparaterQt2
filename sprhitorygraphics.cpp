#include "sprhitorygraphics.h"


/**
 * @brief SPRHitoryGraphics::getGraphics
 * @param thread
 * @return
 */
QList<QwtPlotItem *> SPRHitoryGraphics::getGraphics(int thread){
    QList<QwtPlotItem *> source;

    foreach(HistoryGraphic *gr, lstGraphics){
        if(gr->getThread() == thread || thread < 0)
        source << gr->getGraphic();
    }

    return source;
}
QList<QwtPlotItem *> SPRHitoryGraphics::getGraphics(HistoryGraphic *_current){
    QList<QwtPlotItem *> ret;

    foreach(HistoryGraphic *gr, lstGraphics){
        if(gr == _current || _current == nullptr)
        ret << gr->getGraphic();
    }

    return ret;
}

/**
 * @brief SPRHitoryGraphics::eventFilter
 * @param object
 * @param event
 * @return
 */
bool SPRHitoryGraphics::eventFilter(QObject *object, QEvent *event){
    switch( event->type() )
    {
    case QEvent::MouseButtonDblClick: {
        onDblClick();
    }
    }
    return QObject::eventFilter( object, event );
}
/**
 * @brief SPRHitoryGraphics::SPRHitoryGraphics
 * @param parent
 */
SPRHitoryGraphics::SPRHitoryGraphics(QWidget *parent):
    QwtPlot(parent), model(nullptr), grid(nullptr), legend(nullptr), timeDraw(nullptr)
{
}
/**
 * @brief SPRHitoryGraphics::~SPRHitoryGraphics
 */
SPRHitoryGraphics::~SPRHitoryGraphics(){
    foreach(HistoryGraphic *gr, lstGraphics){
        if(gr){
            QwtPlotCurve *item = gr->getGraphic();
            if(itemList().contains(item)){
                item->detach();
//                delete item;
            }
        }
    }
    qDeleteAll(lstGraphics);

    if(grid) delete grid;
    if(legend) delete legend;
//    if(timeDraw) delete timeDraw;
}

/**
 * @brief SPRHitoryGraphics::init
 * @param _thread
 * @param _model
 * @param _type
 * @param _titleFormat
 * @param _interval_in_sec
 * @param _size_parts_in_sec
 */

void SPRHitoryGraphics::init(SPRThreadList _thread, SPRHistoryModel *_model, SPRWorkSeparateTypeData _type, QString _titleFormat, qint64 _interval_axis_scale_in_sec/*, uint64_t _size_parts_in_sec*/)
{
    if((model != _model) && model){
        disconnect(model, SIGNAL(modelChange(IModelVariable*)), this, SLOT(onModelChange(IModelVariable*)));
        delete model;
    }
    model = _model;
    if(model)
        connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChange(IModelVariable*)));

    threads.clear();
    if(_thread.size() == 0){
        threads = model->getThreadList();
//        for(int i=0; i<MAX_SPR_MAIN_THREADS; i++){
//            threads << i;
//        }
    } else {
        threads = _thread;
    }

    type  = _type;

    if(_titleFormat.isEmpty()){
        titleFormat = "( %1 )";
    } else {
        titleFormat = _titleFormat;
    }

    this->setCanvasBackground(QBrush(Qt::lightGray));
    current = nullptr;

    interval_axis_scale_in_sec = _interval_axis_scale_in_sec;
//    size_parts_in_sec = _size_parts_in_sec;

    foreach(uint8_t th, threads){

        HistoryGraphic *graph = new HistoryGraphic(th, QString(tr("Ручей %1")).arg(th+1), defThreadsColor[th]);

//        curvesData << QVector<QPointF>();
        lstGraphics << graph;
    }

    setGraphicTitle();

    grid = new QwtPlotGrid();
    grid->setMajorPen(QColor(Qt::white), 1);

    grid->attach(this);

    legend = new QwtLegend();
    QFont sys = parentWidget()->font();
    sys.setPointSize(8);
    legend->setFont(sys);
    this->insertLegend(legend, QwtPlot::BottomLegend);

    timeDraw = new TimeScaleDraw();
    this->setAxisScaleDraw(QwtPlot::xBottom, timeDraw);
    this->setAxisAutoScale(QwtPlot::yLeft);
    qint64 dt = QDateTime::currentSecsSinceEpoch();
//    this->setAxisScale(QwtPlot::xBottom,(double)(dt - interval_axis_scale_in_sec), (double)dt, (double)size_parts_in_sec*60);
    this->setAxisScale(QwtPlot::xBottom,(double)(dt - 600), (double)dt, (double)1);
    this->setAxisMaxMinor(QwtPlot::xBottom, 3);



    onModelChange(nullptr);

}

void SPRHitoryGraphics::setThreadsVisible(HistoryGraphic *_current){
    QList<QwtPlotItem *> all = getGraphics(-1);
    foreach(QwtPlotItem *gr, all){
        if(itemList().contains(gr)){
            if( _current && (gr != (QwtPlotItem *)_current)){
                gr->detach();
            }
        } else {
            if(!_current || (gr == (QwtPlotItem *)_current)){
                gr->attach(this);
            }
        }
    }

//    QList<QwtPlotItem *> source = getGraphics(_current);
//    foreach(QwtPlotItem *gr, source){
//        gr->attach(this);
//    }

}

void SPRHitoryGraphics::setGraphicTitle(){
    QString titleText;
    if(!current){
        titleText = QString(titleFormat).arg(tr("все ручьи"));
//        setTitle();
    } else {
        titleText = QString(titleFormat).arg(current->getName());
//        setTitle();
    }
    setTitle(titleText);
    QwtText txt = QwtText(titleText);
    QFont system  = parentWidget()->font();
    system.setPointSize(10);
//    qDebug() << system;
    txt.setFont(system);
    setTitle(txt);
}

void SPRHitoryGraphics::onDblClick(){
    if(current == lstGraphics.last()){
        current = nullptr;
    } else {
        if(!current){
            current = lstGraphics.first();
            setTitle(QString(titleFormat).arg(current->getName()));
        } else {
            QList<HistoryGraphic*>::iterator it = lstGraphics.begin();
            while(*it != current && *it != lstGraphics.last()) it++;

            current = *(++it);
        }
    }

    setGraphicTitle();
    onModelChange(nullptr);
}
/**
 * @brief SPRHitoryGraphics::onModelChange
 */
void SPRHitoryGraphics::onModelChange(IModelVariable *){
    if(model){
        foreach(HistoryGraphic* gr, lstGraphics){

            QVector<QPointF> vect = model->getSamplesToGraphic(type, gr->getThread());
//            curvesData[index] = vect;
//            qDebug() << "TH: " << gr->getThread() << "; vect: " << vect;
            gr->setGraphicData(vect);
        }
    }
    qint64 dt = QDateTime::currentDateTime().toSecsSinceEpoch();
    this->setAxisScale(QwtPlot::xBottom,(dt - interval_axis_scale_in_sec), dt, /*static_cast<double>(size_parts_in_sec*15)*/ interval_axis_scale_in_sec / 10);
    setThreadsVisible(current);
    replot();
}
/**
 * @brief SPRHitoryGraphics::SPRHitoryGraphics
 * @param parent
 */


