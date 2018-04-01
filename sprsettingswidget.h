#ifndef SPRSETTINGSWIDGET_H
#define SPRSETTINGSWIDGET_H

#include <QDomDocument>
#include <QWidget>
#include "ui_sprsettingswidget.h"

#include "isprwidget.h"
#include "models/sprmainmodel.h"

class SPRSettingsWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    QDomDocument document;
    QDomDocument *doc;
    QString docFName;

    SPRMainModel *model;
public:
    explicit SPRSettingsWidget(QWidget *parent = 0);
    explicit SPRSettingsWidget(QDomDocument *_doc, QString fName = "", QWidget *parent = 0);
    Ui::SPRSettingsWidget ui;

    void setDoc(QDomDocument *_doc);
    SPRMainModel *setModel(SPRMainModel *_model);

signals:
    void doStore();
    void doShow();
    void changeFileSettinds(QString);
    void changeFileSpectrum(QString);

    // ISPRWidget interface
public:
    virtual ISPRModelData *getModel(){return nullptr;}
public slots:
    virtual void viewChange(){}
    virtual void viewChange(int){}
    virtual void viewChange(QTableWidget *, int, int){}
    virtual void widgetsShow(){
        emit doShow();
    }
    void onChangeFileSpectrum(QString fName);
    void onChangeFileSettinds(QString fName);
};

#endif // SPRSETTINGSWIDGET_H
