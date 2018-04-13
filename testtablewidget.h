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

class testTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit testTableWidget(QWidget *parent = 0);

    SPRMainModel *mainModel;
//    SPRSpectrumListItemsModel *spectrumsBaseModel;
    SPRSpectrumListItemsModel *kSpectrumsModel;
    SPRSeparateModel *separateModel;

    TCPGetSpectrumsGistogramms *getGistogramm;
    TCPGetSpectrumsGistogramms *getKSpectrums;
    TCPCommand *getSeparate;
    TCPCommand *setSeparate;

    TCPTestStartSeparate *startSeparate;

    TCPTestStopSeparate *stopSeparate;

    TCPSeparateGo *separateGo;

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
    ISPRModelData *setModel(SPRMainModel* _model);

    void widgetsShow();

//    void addSpectrumsModel(QFile *inp);
    void onKSpectrumReady(TCPGetSpectrumsGistogramms *_command);
public slots:
//    void onClickAdd(bool);
    void onModelChanged();
    void onGetButtomsClick(bool);
    void onCommandComplite(TCPCommand *_command);
    void onCommandError(TCPCommand *_command);
private:
    Ui::testTableWidget ui;
};

#endif // TESTTABLEWIDGET_H
