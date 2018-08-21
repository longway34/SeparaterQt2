#include "sprporogsmoved.h"

#include <qwt_plot_canvas.h>
#include <QEvent>
#include <QMouseEvent>

SPRPorogsMoved::SPRPorogsMoved(QwtPlot *plot, QList<QwtPlotItem *> _items) : QObject(plot)
{
    init(plot, _items);
}

SPRPorogsMoved::SPRPorogsMoved(QwtPlot *plot, QwtPlotItem *_item) : QObject(plot)
{
    init(plot, QList<QwtPlotItem *>({_item}));
}

void SPRPorogsMoved::init(QwtPlot *plot, QList<QwtPlotItem *> _items)
{
    movedItems.clear();
    addMovedItems(_items);
    currentMovedItem = nullptr;


    QwtPlotCanvas *canvas = qobject_cast<QwtPlotCanvas *>( plot->canvas() );
    canvas->setMouseTracking(true);
    canvas->installEventFilter( this );

    defCursor = canvas->cursor();
    resizeCursor.setShape(Qt::SizeHorCursor);
    movedCursor.setShape(Qt::SizeAllCursor);

    moved_begin = false;

    // We want the focus, but no focus rect. The
    // selected point will be highlighted instead.

    canvas->setFocusPolicy( Qt::StrongFocus );
}

QList<QwtPlotItem *> SPRPorogsMoved::getMovedItems() const
{
    return movedItems;
}

void SPRPorogsMoved::addMovedItems(QList<QwtPlotItem *> &value)
{
    movedItems << value;
}

void SPRPorogsMoved::addMovedItems(QwtPlotItem *value)
{
    if(!movedItems.contains(value)){
        movedItems.push_back(value);
    }
}

void SPRPorogsMoved::remoteItem(QwtPlotItem *value)
{
    if(movedItems.contains(value)){
        foreach(QwtPlotItem* item, movedItems){
            if(item == value){
                movedItems.removeOne(item);
                break;
            }
        }
    }
}


QwtPlot *SPRPorogsMoved::plot()
{
    return qobject_cast<QwtPlot *>( parent() );
}

const QwtPlot *SPRPorogsMoved::plot() const
{
    return qobject_cast<const QwtPlot *>( parent() );
}

bool SPRPorogsMoved::eventFilter(QObject *object, QEvent *event)
{
    QwtPlot *_plot = plot();
    QWidget *_canvas = nullptr;

    if(_plot){
         _canvas = plot()->canvas();
    }

    if ( plot() == NULL || object != plot()->canvas() )
        return false;


    switch( event->type() )
    {
        case QEvent::MouseMove: {
            bool isMoved = false;
            const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>( event );
            if(lastMovedPosition.rx() < 0) lastMovedPosition = mouseEvent->pos();
            if(mouseEvent->buttons() == Qt::LeftButton){
                if(currentMovedItem){
                    isMoved = true;
                    if(movedDistance < 0) movedDistance = 0;
                    double last = plot()->invTransform(QwtPlot::xBottom, lastMovedPosition.rx());
                    double dpos = plot()->invTransform(QwtPlot::xBottom, mouseEvent->pos().rx());
                    movedDistance += lastMovedPosition.rx() - mouseEvent->pos().rx();
                    double diff = dpos - last;
                    dMovedDistance += diff;
                    toMouseDragMoved(plot(), mouseEvent->pos(), diff, currentMovedItem, currentMovedItemSide);
                    qDebug() << " pos:" << mouseEvent->pos() << "; last: " << last << "; dpos: " << dpos << "; diff: " << diff << "; dist: " << dMovedDistance;
//                    lastMovedPosition = mouseEvent->pos();
                }
            }
            toMovedMouse(mouseEvent->pos());
            lastMovedPosition = mouseEvent->pos();
            if(isMoved){
                return true;
            } else {
                return false;
            }
        }
        break;
        case QEvent::MouseButtonDblClick: {
            emit dblClickMouseEvent();
        }
        break;
    case QEvent::MouseButtonPress:{
        const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>( event );
        if(mouseEvent->button() == Qt::LeftButton){
            dMovedDistance = 0;
        }
    }
        case QEvent::MouseButtonRelease: {
            if(moved_begin){
                lastMovedPosition = QPoint(-1, -1);

//                if(currentMovedItemSide == movedAllSides){
                    emit changeArgumentValue(currentMovedItem, dMovedDistance, currentMovedItemSide);
//                } else {
//                    emit changeArgumentValue(currentMovedItem, new_argument, currentMovedItemSide);
//                }
            }
            moved_begin =false;
            return true;
        }
    }
    return QObject::eventFilter( object, event );
}

void SPRPorogsMoved::setCursor(QCursor cursor){
    QwtPlotCanvas *canvas = qobject_cast<QwtPlotCanvas *>( plot()->canvas() );
    canvas->setCursor(cursor);
}

MinDistance SPRPorogsMoved::minCurveHorisontalDistance(QwtPlot *plot, QPoint pos){
    MinDistance res;
    if(plot){
        foreach(QwtPlotItem *item, movedItems){
            MinDistance _res;
            if(item->rtti() == QwtPlotItem::Rtti_PlotCurve){
                QwtPlotCurve *curve = (QwtPlotCurve*)item;
                for(int sample=0; sample < curve->data()->size(); sample++){
                    int x = plot->transform(QwtPlot::xBottom, curve->data()->sample(sample).rx());
                    int d = abs(x - pos.rx());
                    if(_res.distance < 0 || d < _res.distance){
                        _res.distance = d;
                        _res.position = movedLeftSide;
                        _res.item = item;
                    }
                }
            }
            if(item->rtti() == QwtPlotItem::Rtti_PlotHistogram){
                QwtPlotHistogram *hist = (QwtPlotHistogram*)item;

                for(int sam=0; sam<hist->data()->size(); sam++){
                    QwtIntervalSample interval = hist->sample(sam);
                    int xmin = plot->transform(QwtPlot::xBottom, interval.interval.minValue());
                    int xmax = plot->transform(QwtPlot::xBottom, interval.interval.maxValue());
                    int ymax = plot->transform(QwtPlot::yLeft, interval.value);
                    if(ymax < pos.ry()){
                        if(pos.rx() > xmin && pos.rx() < xmax){
                            _res.item = item;
                            _res.position = movedAllSides;
                            _res.distance = abs(xmax - xmin);
                        }
                        int d = abs(pos.rx() - xmin);
                        if(d <= 3){
                            _res.item = item;
                            _res.distance = d;
                            _res.position = movedLeftSide;
                        }
                        d = abs(pos.rx() - xmax);
                        if(d <= 3){
                            _res.item = item;
                            _res.distance = d;
                            _res.position = movedRigthSize;
                        }
                    }
                }
            }
            if((_res.distance >= 0 && _res.distance < res.distance) ||
                    (_res.distance >=0 && res.distance < 0)){
                res = _res;
            }
        }
    }
    return res;
}

void SPRPorogsMoved::toMouseDragMoved(QwtPlot *plot, QPoint pos, double distance, QwtPlotItem* item, MovedItemPosition position){

    if(plot){
        new_argument = plot->invTransform(QwtPlot::xBottom, pos.rx());
        moved_begin = true;
        if(item->rtti() == QwtPlotItem::Rtti_PlotCurve){
//            if(item){
                qDebug() << "moved to: "<< pos << "; dist:" << distance;
                QwtPlotCurve *curve = (QwtPlotCurve*)item;
                QVector<QPointF> vp;
                for(int sample=0; sample<curve->data()->size(); sample++){
                    QPointF f = curve->data()->sample(sample);
                    f.setX(f.rx() + distance);
                    vp.push_back(f);
                }
                curve->setSamples(vp);
                plot->replot();
//                emit changeArgumentValue(item, new_argument);
//            }

        }
        if(item->rtti() == QwtPlotItem::Rtti_PlotHistogram){
            QwtPlotHistogram *hist = (QwtPlotHistogram*)item;
            QVector<QwtIntervalSample> vi;
            for(int sam = 0; sam<hist->data()->size(); sam++){
                QwtIntervalSample interval = hist->sample(sam);
                if(position == movedLeftSide){
                    interval.interval.setMinValue(interval.interval.minValue() + distance);
                } else if(position == movedRigthSize){
                    interval.interval.setMaxValue(interval.interval.maxValue() + distance);
                } else if(position == movedAllSides){
                    interval.interval.setMinValue(interval.interval.minValue() + distance);
                    interval.interval.setMaxValue(interval.interval.maxValue() + distance);
//                    int d = plot->invTransform(QwtPlot::xBottom, distance);
//                    int minValue = interval.interval.minValue() - d;
//                    int maxValue = interval.interval.maxValue() - d;
//                    interval.interval.setInterval(minValue, maxValue);
                }
                vi.push_back(interval);
            }
            hist->setSamples(vi);
            plot->replot();
        }
    }
}

void SPRPorogsMoved::toMovedMouse(QPoint pos)
{
    MinDistance distance;
    int ind = -1;
    MinDistance d;
    double y = -1;

    distance = minCurveHorisontalDistance(plot(), pos);
//    if(movedItems.size() > 0){
//        for(int i=0; i<movedItems.size();i++){
//            if(movedItems[i]->rtti() == QwtPlotItem::Rtti_PlotCurve){

                y = plot()->invTransform(QwtPlot::xBottom, pos.rx());
//                ((QwtPlotCurve*)(movedItems[i]))->closestPoint(pos, &d);
//                if(abs(distance.distance) > abs(d.distance)){
//                    distance = d;
//                    distance.item = movedItems[i];
//                    distance.position = movedLeftSide;
//                    ind = i;
//                }
//            }
//            if(movedItems[i]->rtti() == QwtPlotItem::Rtti_PlotHistogram){
//                y = plot()->invTransform(QwtPlot::xBottom, pos.rx());
                
//            }
            if(distance.item){
                if(abs(distance.distance) <= 3 || distance.position == movedAllSides){
                    currentMovedItem = distance.item;
                    currentMovedItemSide = distance.position;
                    if(distance.position == movedLeftSide || distance.position == movedRigthSize){
                        setCursor(resizeCursor);
                    } else if(distance.position == movedAllSides){
                        setCursor(movedCursor);
                    }
                } else {
                    currentMovedItem = nullptr;
                    setCursor(defCursor);
                    currentMovedItemSide = noMovedSides;
                }
            } else {
                currentMovedItem = nullptr;
                setCursor(defCursor);
                currentMovedItemSide = noMovedSides;
            }
            emit setSelectItem(currentMovedItem, currentMovedItemSide);
//        }
//        qDebug() << "pos_x: " << pos.rx() << "; trans: " << d << "; dist: " << distance;
//    }
}

