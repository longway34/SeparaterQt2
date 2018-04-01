#ifndef MAINTABWIDGET_H
#define MAINTABWIDGET_H

#include "ui_maintabwidget.h"
#include "isprwidget.h"
#include "models/sprseparatemodel.h"

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
    virtual ISPRModelData *getModel(){return model;}
public slots:
    virtual ISPRModelData *setModel(SPRMainModel *_model);
    virtual void viewChange(){}
    virtual void viewChange(int){}
    virtual void viewChange(QTableWidget *, int, int){}
    virtual void widgetsShow(){
        emit doShow();
    }

    void onChangeFileSettings(QString fName);
    void onChangeFileSpectrum(QString fName);
    void onClickSetSeparateButton(bool value);
};

#endif // MAINTABWIDGET_H
