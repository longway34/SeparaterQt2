#ifndef SPRSPECTRUMZONESTABLEWIDGET_H
#define SPRSPECTRUMZONESTABLEWIDGET_H

#include "ui_sprspectrumzonestablewidget.h"
#include "isprwidget.h"

class SPRSpectrumZonesTableWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

public:
    explicit SPRSpectrumZonesTableWidget(QWidget *parent = 0);

private:
    Ui::SPRSpectrumZonesTableWidget ui;

    // ISPRWidget interface
public:
    virtual ISPRModelData *getModel();

    SPRSpectrumZonesTableModel *setModel(SPRSpectrumZonesTableModel *data);
    void setThreads(SPRVariable<uint> *threads){ui.wSpectrumTable->model->setThreads(threads);}
public slots:
    virtual void widgetsShow();
    virtual void viewChange(QTableWidget *, int, int);
    virtual void viewChange(bool);
    virtual void onChangeColor(EnumElements el, QColor color){
        emit changeColor(el, color);
    }
signals:
    void doShow();
    void changeColor(EnumElements, QColor);

    // ISPRWidget interface
protected:
};

#endif // SPRSPECTRUMZONESTABLEWIDGET_H
