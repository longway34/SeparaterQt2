#ifndef TESTTABLEWIDGET_H
#define TESTTABLEWIDGET_H

#include "ui_testtablewidget.h"
#include "models/sprmainmodel.h"
#include "models/sprseparatemodel.h"
#include "tcp/TCPCommand.h"
#include "tcp/tcpgetspectrumsgistogramms.h"

class testTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit testTableWidget(QWidget *parent = 0);

    SPRMainModel *mainModel;
    SPRSpectrumListItemsModel *spectrumModel;
    SPRSeparateModel *separateModel;

    TCPGetSpectrumsGistogramms *getGistogramm;
    TCPGetSpectrumsGistogramms *getKSpectrums;
    TCPCommand *getSeparate;
    TCPCommand *setSeparate;


//    ISPRModelData *setModel(SPRSpectrumListItemsModel *_model, SPRFormulaItemModel *_formulas){
//        if(_model){
////            if(model) delete model;
//            ui.table->setModel(_model->getSpectrumListItemsModel());
//        }
//        return model;
//    }
    ISPRModelData *setModel(SPRMainModel* _model);

    void widgetsShow();

    void addSpectrumsModel(QFile *inp);
public slots:
    void onClickAdd(bool);
    void onModelChanged();
    void onGetButtomsClick(bool);
    void onCommandComplite(TCPCommand *_command);
private:
    Ui::testTableWidget ui;
};

#endif // TESTTABLEWIDGET_H
