#ifndef SPRZONESRANGESTABLEWIDGET_H
#define SPRZONESRANGESTABLEWIDGET_H

#include "ui_sprzonesrangestablewidget.h"
#include "models/sprspectrumzonestablemodel.h"
#include "isprwidget.h"
#include "sprzonesrangestablewidget.h"
#include "isprsettingswidget.h"

class SPRZonesRangesTableWidget : public QWidget, public ISPRWidget, public ISPRSettingsWidget
{
    Q_OBJECT


    SPRSpectrumZonesTableModel *model;
    bool editControlEnable;
    bool selectBottonRowVisible;
public:
    explicit SPRZonesRangesTableWidget(QWidget *parent = 0);

    ISPRModelData *setModelData(SPRSpectrumZonesTableModel *value);

    void setThreadsVisible(QList<int> visible);

    void setThreadsVisible(int thr);
public slots:
    void onAddClick(bool);
    void onEditClick(bool);
    void setControlEnabled(SPRZonesRanges *_thread, QLineEdit *_el);
    void onRoundClick(bool);
    virtual void onModelChanget(IModelVariable *);
    virtual void widgetsShow();
private:
    Ui::SPRZonesRangesTableWidget ui;

    // ISPRWidget interface
public:
    virtual ISPRModelData *getModelData();
    void setSelectBottonRowVisible(bool value);

protected:
    void showEdabled();

    // ISPRWidget interface
protected:

    // ISPRSettingsWidget interface
public:
    virtual void setMasterMode(bool value);
};









#endif // SPRZONESRANGESTABLEWIDGET_H
