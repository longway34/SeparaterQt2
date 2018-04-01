#ifndef ISEPARATORMC_H
#define ISEPARATORMC_H

#include <QObject>
#include <QWidget>
#include <QVector>
#include <QVariant>
#include <QTableWidget>

#include "_types.h"
#include "models/isprmodeldata.h"
#include "isprwidget.h"

class ISPRController : public QObject, public ISPRWidget
{
    Q_OBJECT
private:
    viewChangeData vcData;
public:
    explicit ISPRController(QObject *parent = nullptr);

    virtual QWidget *addWidgetModel(WidgetsEnum widgetId, QWidget *widget, ModelsEnum modelId, ISPRModelData *model);

signals:
    void doStore();
    void doShow();
public slots:
//    virtual void widgetsShow()=0;
    virtual void viewChange(QObject *source, viewChangeData newData)=0;
    virtual void viewChange(){
        viewChange(sender(), vcData);
    }
    virtual void viewChange(int data){
        vcData.data_int16 = data;
        viewChange(sender(), vcData);
    }
    virtual void viewChange(double data){
        vcData.data_double = data;
        viewChange(sender(), vcData);
    }
    virtual void viewChange(bool data){
        vcData.data_bool = data;
        viewChange(sender(), vcData);
    }
    virtual void viewChange(int row, int col){
        vcData.pointInt.row = row;
        vcData.pointInt.col = col;
        viewChange(sender(), vcData);
    }
    virtual void viewChange(QTableWidget *table, int row, int col){
        vcData.pointInt.row = row;
        vcData.pointInt.col = col;
        viewChange(table, vcData);
    }

protected:
    QMap<WidgetsEnum, QWidget *> widgets;
    QMap<ModelsEnum, ISPRModelData *> models;

};

#endif // ISEPARATORMC_H
