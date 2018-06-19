#ifndef SPRZONESRANGES_H
#define SPRZONESRANGES_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QMessageBox>
#include <QTableWidgetItem>

#include "models/sprspectrumzonesmodel.h"
#include "isprwidget.h"
#include "firstcollumn2.h"

class SPRZonesRanges : public QTableWidget, ISPRWidget
{
    Q_OBJECT

    SPRSpectrumZonesModel *zoneModel;
    bool firstCollumnVisible;
    bool hHeaderVisible;
    bool selectEnable;

    QObject *lastCurrent;
public:
    SPRZonesRanges(QWidget *parent=nullptr);

    void setModelData(SPRSpectrumZonesModel *_model){
        if(_model){
            if(zoneModel){
                disconnect(zoneModel, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
            }
            zoneModel = _model;
            connect(zoneModel, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        }
//        widgetsShow();
    }

    QSize getMySize(){

    }
    bool getFirstCollumnVisible() const
    {
        return firstCollumnVisible;
    }
    void setFirstCollumnVisible(bool value)
    {
        firstCollumnVisible = value;
    }

    bool getHHeaderVisible() const
    {
        return hHeaderVisible;
    }
    void setHHeaderVisible(bool value){
        hHeaderVisible = value;
    }

    bool getSelectEnable() const
    {
        return selectEnable;
    }
    void setSelectEnable(bool value)
    {
        selectEnable = value;
    }

    EnumElements getCurrentElement(bool *result = nullptr);
public slots:
    void widgetsShow();
    void onChangeValue();

    // ISPRWidget interface
    void onChangeColor(QColor color);
    void onDeleteElements(int);
    //    void onFocusCurrentCell(QModelIndex index);
    void onChangeSelectedRow(bool value, int row);
    virtual void onModelChanget(IModelVariable *);
public:
    virtual ISPRModelData *getModelData()
    {
        return zoneModel;
    }
    QSize sizeHint() const;

signals:
    void deleteElement(int thread, EnumElements el);
    void gotFocus(SPRZonesRanges *, QLineEdit*);
    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event);


    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event);

    // QWidget interface
    QObject *getLastCurrent() const;

    SPRSpectrumZonesModel *getZoneModel() const;

protected:
    //    virtual void focusInEvent(QFocusEvent *event);

    // ISPRWidget interface
protected:
};











#endif // SPRZONESRANGES_H
