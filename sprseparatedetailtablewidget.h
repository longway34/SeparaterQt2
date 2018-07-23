#ifndef SPRSEPARATEDETAILTABLEWIDGET_H
#define SPRSEPARATEDETAILTABLEWIDGET_H

#include "ui_sprseparatedetailtablewidget.h"

#include "models/sprmainmodel.h"
#include "isprwidget.h"

class SPRSeparateDetailTableWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRMainModel *model;
public:
    SPRSeparateDetailTableWidget(QWidget *parent = 0);

private:
    Ui::SPRSeparateDetailTableWidget ui;

    // ISPRWidget interface
public:
    virtual ISPRModelData *setModelData(ISPRModelData *data);
    virtual ISPRModelData *getModelData();

protected:

    QList<int> getVisbleThreads();
public slots:
    virtual void onModelChanget(IModelVariable *variable);
    virtual void widgetsShow();

    void onChangeThreadList(bool value);
    void onClickButtomCommand(bool);
    void onChangeIntValue(int);
    void onChandeServerState(uint32_t);
};

#endif // SPRSEPARATEDETAILTABLEWIDGET_H
