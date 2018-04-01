#ifndef SPRSPECTRUMTABLEADV_H
#define SPRSPECTRUMTABLEADV_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QMouseEvent>
#include <QDropEvent>

class SPRSpectrumTableAdv : public QTableWidget
{
private:

public:
    SPRSpectrumTableAdv(QWidget * parent = 0);

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void dropEvent(QDropEvent *event);
};

#endif // SPRSPECTRUMTABLEADV_H
