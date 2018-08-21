#ifndef SPRSETTINGSWIDGET_H
#define SPRSETTINGSWIDGET_H

#include <QDomDocument>
#include <QWidget>
#include "ui_sprsettingswidget.h"

#include "isprwidget.h"
#include "models/sprmainmodel.h"

#include "isprsettingswidget.h"

class SPRSettingsWidget : public QWidget, public ISPRWidget, ISPRSettingsWidget
{
    Q_OBJECT

    QDomDocument document;
    QDomDocument *doc;
    QString docFName;

    bool masterMode;

    SPRMainModel *model;
public:
    explicit SPRSettingsWidget(QWidget *parent = 0);
    explicit SPRSettingsWidget(QDomDocument *_doc, QString fName = "", QWidget *parent = 0);
    Ui::SPRSettingsWidget ui;

    void setDoc(QDomDocument *_doc);
    SPRMainModel *setModelData(SPRMainModel *_model);

signals:
    void doStore();
    void doShow();
    void changeFileSettinds(QString);
    void changeFileSpectrum(QString);

    // ISPRWidget interface
public:
    virtual ISPRModelData *getModelData(){return nullptr;}
    bool getMasterMode() const;
    virtual void setMasterMode(bool value);

public slots:
    virtual void viewChange(){}
    virtual void viewChange(int){}
    virtual void viewChange(QTableWidget *, int, int){}
    virtual void onModelChanget(IModelVariable *);
    virtual void widgetsShow();
    void onChangeFileSpectrum(QString fName);
    void onChangeFileSettinds(QString fName);

    // ISPRWidget interface
    void onCompliteButtomClick(bool);
    void onCancelButtomClick(bool);
protected:
};

#endif // SPRSETTINGSWIDGET_H
