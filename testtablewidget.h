#ifndef TESTTABLEWIDGET_H
#define TESTTABLEWIDGET_H

#include "ui_testtablewidget.h"
#include "models/sprmainmodel.h"
#include "models/sprseparatemodel.h"
#include "tcp/TCPCommand.h"
#include "tcp/tcpgetspectrumsgistogramms.h"
#include "tcp/tcpteststartseparate.h"

#include "tcp/tcpteststopseparate.h"

#include "tcp/tcptimeoutwigget.h"
#include "tcp/tcplogswigtets.h"
#include "tcp/tcpseparatego.h"
#include "isprwidget.h"

#include "tcp/tcpcommandseparatoronfull.h"
#include "tcp/tcpcommandgetspectrums.h"

#include "tcp/tcpstartseparate2.h"

class testTableWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

public:
    Ui::testTableWidget ui;
    explicit testTableWidget(QWidget *parent = 0);

    TCPTimeOutWigget *towidget;

    SPRMainModel *mainModel;

    int separate_error;
//    SPRSpectrumListItemsModel *spectrumsBaseModel;
    SPRSpectrumListItemsModel *kSpectrumsModel;
    SPRSeparateModel *separateModel;
    SPRSeparateModel *separateModelLast;

    TCPGetSpectrumsGistogramms *getGistogramm;
    TCPGetSpectrumsGistogramms *getKSpectrums;
    TCPCommand *getSeparate;
    TCPCommand *setSeparate;

    TCPCommandSeparatorOnFull *rentgenOnFull;
//    TCPGetSpectrumsGistogramms *getSpectrumsSetCommand;


    TCPStartSeparate2 *startSeparate;

    TCPTestStopSeparate *stopSeparate;

    TCPSeparateGo *separateGo;

    TCPCommand *getRen;

    TCPGetSpectrumsGistogramms *getBaseSpectrumCommand;
    TCPGetSpectrumsGistogramms *kspecCommand;
    TCPGetSpectrumsGistogramms *hiskCommand;
    TCPCommand *getseparCommand;

//    TCPTimeOutWigget *towidget;
//    TCPLogsWigtets *logs;

//    ISPRModelData *setModel(SPRSpectrumListItemsModel *_model, SPRFormulaItemModel *_formulas){
//        if(_model){
////            if(model) delete model;
//            ui.table->setModel(_model->getSpectrumListItemsModel());
//        }
//        return model;
//    }
    ISPRModelData *setModelData(SPRMainModel* _model);

    void widgetsShow();

//    void addSpectrumsModel(QFile *inp);
    void onKSpectrumReady(TCPGetSpectrumsGistogramms *_command);
public slots:
//    void onClickAdd(bool);
    void onModelChanged(IModelVariable *);
    void onGetButtomsClick(bool);
    void onCommandComplite(TCPCommand *_command);
    void onCommandError(TCPCommand *_command);
    void onStartSepareteCommandComplite(TCPCommand *command);
private:

    // ISPRWidget interface
public:
    virtual ISPRModelData *getModelData();
    virtual void setLogWidget(TCPLogsWigtets *value);

protected:
    virtual void onModelChanget(IModelVariable *);
};

#endif // TESTTABLEWIDGET_H
