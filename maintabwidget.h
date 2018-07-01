#ifndef MAINTABWIDGET_H
#define MAINTABWIDGET_H

#include "ui_maintabwidget.h"
#include "isprwidget.h"
#include "models/sprseparatemodel.h"
#include "tcp/tcplogswigtets.h"

class MainTabWidget : public QTabWidget, public ISPRWidget
{
    Q_OBJECT

public:
    explicit MainTabWidget(QWidget *parent = 0);
    MainTabWidget(QDomDocument *_doc, QWidget *parent = 0);
    MainTabWidget(QString _fName, QWidget *parent = 0);

    SPRMainModel *model;
    Ui::MainTabWidget ui;

    SPRSeparateModel *separateModel;

signals:
    void doStore();
    void doShow();

    // ISPRWidget interface
public:
    virtual ISPRModelData *getModelData(){return model;}
public slots:
    virtual ISPRModelData *setModelData(SPRMainModel *_model);
    virtual void viewChange(){}
    virtual void viewChange(int){}
    virtual void viewChange(QTableWidget *, int, int){}
    virtual void widgetsShow(){
//        emit doShow();
        ui.tabSettings->widgetsShow();
        ui.tabSpectrum->widgetsShow();
//        ui.tabSeparate->
//        ui.tabHistory->wid
        ui.tabTest->widgetsShow();
        ui.tabTestSeparateDetail->widgetsShow();
    }

    void onChangeFileSettings(QString fName);
    void onChangeFileSpectrum(QString fName);
    void onClickSetSeparateButton(bool value);
    virtual void onModelChanget(IModelVariable *);

    // ISPRWidget interface

    // ISPRWidget interface
protected:

    // ISPRWidget interface
public:
    virtual void setDoc(QString _fName);
    void setLogWidget(TCPLogsWigtets *value);
};



#endif // MAINTABWIDGET_H
