#ifndef _SCROLLZOOMER_H
#define _SCROLLZOOMER_H

#include <qglobal.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot.h>

#include "sprgraphitem.h"

class ScrollData;
class ScrollBar;

class ScrollZoomer: public QwtPlotZoomer
{
    Q_OBJECT
public:
    enum ScrollBarPosition
    {
        AttachedToScale,
        OppositeToScale
    };

    SPRGraphItem *current;
    QwtPlot *myPlot;


    ScrollZoomer( QWidget *, QwtPlot *plot=nullptr);
    virtual ~ScrollZoomer();

    ScrollBar *horizontalScrollBar() const;
    ScrollBar *verticalScrollBar() const;

    void setHScrollBarMode( Qt::ScrollBarPolicy );
    void setVScrollBarMode( Qt::ScrollBarPolicy );

    Qt::ScrollBarPolicy vScrollBarMode () const;
    Qt::ScrollBarPolicy hScrollBarMode () const;

    void setHScrollBarPosition( ScrollBarPosition );
    void setVScrollBarPosition( ScrollBarPosition );

    ScrollBarPosition hScrollBarPosition() const;
    ScrollBarPosition vScrollBarPosition() const;

    QWidget* cornerWidget() const;
    virtual void setCornerWidget( QWidget * );

    virtual bool eventFilter( QObject *, QEvent * );

    virtual void rescale();

    virtual QwtText trackerTextF( const QPointF &pos ) const
    {
        QColor bg( Qt::white );
        bg.setAlpha( 200 );
        QwtText nText;
        if(current){
            int x = pos.toPoint().rx(); int y=pos.toPoint().ry();
            size_t size = current->spect->dataSize();
            if(x>=0 && x<current->spect->dataSize()){
                nText = QString(tr("%1 (%2ch : %3)")).
                              arg(current->model->getSpectrumName()).
                              arg(QString::number(x)).
                              arg(QString::number(current->spect->sample(x).toPoint().ry()));
            }
        } else {
            nText = QwtPlotZoomer::trackerTextF( pos );
        }
//            QwtText text = QwtPlotZoomer::trackerTextF( pos );
//            text.setBackgroundBrush( QBrush( bg ) );
        nText.setBackgroundBrush(QBrush(bg));
        return nText;
    }
    void setCurrent(SPRGraphItem *value){current = value;}

protected:
    virtual ScrollBar *scrollBar( Qt::Orientation );
    virtual void updateScrollBars();
    virtual void layoutScrollBars( const QRect & );

private Q_SLOTS:
    void scrollBarMoved( Qt::Orientation o, double min, double max );

private:
    bool needScrollBar( Qt::Orientation ) const;
    int oppositeAxis( int ) const;

    QWidget *d_cornerWidget;

    ScrollData *d_hScrollData;
    ScrollData *d_vScrollData;

    bool d_inZoom;
    bool d_alignCanvasToScales[ QwtPlot::axisCnt ];
};

#endif
