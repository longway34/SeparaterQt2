#ifndef SPRSPECTRUMRANGES_H
#define SPRSPECTRUMRANGES_H

//#include "ui_sprspectrumranges.h"
#include <QTableWidget>
#include <QPoint>

#include "models/sprspectrumzonesmodel.h"
#include "isprwidget.h"

class SPRSpectrumRanges : public QTableWidget, ISPRWidget
{
    Q_OBJECT

    SPRSpectrumZonesModel *model;
    bool firstStateView;
public:
    explicit SPRSpectrumRanges(QWidget *parent = 0);

    virtual SPRSpectrumZonesModel *getModel();
    QSize sizeHint() const;
//    QSize actualSize;
private:

    // ISPRWidget interface
public:
    virtual void widgetsShow();
    void setModel(SPRSpectrumZonesModel *value);
    QPoint getSelectedPosition();
    EnumElements getSelectedElement();

    void setFirstStateView(bool value);

signals:
    void tableChange(QTableWidget*, int row, int col);
    void changeColor(EnumElements el, QColor color);

    // ISPRWidget interface
protected slots:
    virtual void viewChange();
    void viewChange(QColor color);
};

#endif // SPRSPECTRUMRANGES_H
