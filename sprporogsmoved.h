#ifndef SPRPOROGSMOVED_H
#define SPRPOROGSMOVED_H

#include <QObject>
#include <QWidget>

#include <qwt_plot_item.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_histogram.h>

typedef enum _movedItemPosition{
    movedLeftSide, movedRigthSize, movedAllSides, noMovedSides
} MovedItemPosition;

typedef struct minDistance{
    QwtPlotItem *item;
    MovedItemPosition position;
    int distance;
    minDistance(): item(nullptr), position(noMovedSides), distance(-1){
    }
    
    minDistance(const minDistance &copy){
        item = copy.item; position = copy.position; distance = copy.distance;
    }
    
    minDistance &operator =(const minDistance copy){
        item = copy.item; position = copy.position; distance = copy.distance;
        return *this;
    }
} MinDistance;


class SPRPorogsMoved : public QObject
{
    Q_OBJECT
public:

    SPRPorogsMoved(QwtPlot *plot, QList<QwtPlotItem *> _items = {} );
    SPRPorogsMoved( QwtPlot *plot, QwtPlotItem*_item);

    virtual bool eventFilter(QObject *object, QEvent * event);

    QList<QwtPlotItem *> getMovedItems() const;
    void addMovedItems(QList<QwtPlotItem *> &value);
    void addMovedItems(QwtPlotItem *value);
    void remoteItem(QwtPlotItem *value);

private:
    //    virtual void select( const QPoint & );
    //    virtual void move( const QPoint & );


    void init(QwtPlot *plot, QList<QwtPlotItem*> _items);

    QList<QwtPlotItem*> movedItems;
    QPointF selectedPoint;
    QwtPlotItem *currentMovedItem;
    MovedItemPosition currentMovedItemSide;

    QCursor defCursor;
    QCursor resizeCursor;
    QCursor movedCursor;

    bool moved_begin;
    double new_argument;
    QPoint lastMovedPosition;
    int movedDistance;
    double dMovedDistance;
signals:
    void changeArgumentValue(QwtPlotItem *item, double distance, MovedItemPosition);
//    void movedArgumentValue(QwtPlotItem *item, double distance);
    void setSelectItem(QwtPlotItem *item, MovedItemPosition);
    void dblClickMouseEvent(void);
public slots:
protected:
    const QwtPlot *plot() const;
    QwtPlot *plot();
    void setCursor(QCursor cursor);
    MinDistance minCurveHorisontalDistance(QwtPlot *plot, QPoint pos);
    virtual void toMouseDragMoved(QwtPlot *plot, QPoint pos, double distance, QwtPlotItem *item, MovedItemPosition position = movedAllSides);
    virtual void toMovedMouse(QPoint pos);
};

#endif // SPRPOROGSMOVED_H
