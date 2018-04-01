#ifndef TESTTABLEWIDGET_H
#define TESTTABLEWIDGET_H

#include "ui_testtablewidget.h"
#include "models/sprspectrumlistitemsmodel.h"
#include "models/sprspectrumzonestablemodel.h"
#include "models/sprmainmodel.h"
#include "models/sprseparatemodel.h"
#include "tcp/TCPCommand.h"
#include "tcp/tcpgetkspectrums.h"
#include "tcp/tcpgetgistogramms.h"


class testTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit testTableWidget(QWidget *parent = 0);

    SPRMainModel *mainModel;
    SPRSpectrumListItemsModel *model;
    SPRSeparateModel *separateModel;

    TCPGetGistogramms *getGistogramm;
    TCPGetKSpectrums *getKSpectrums;
    TCPCommand *getSeparate;

    ISPRModelData *setModel(SPRSpectrumListItemsModel *_model, SPRFormulaItemModel *_formulas){
        if(_model){
            if(model) delete model;
            model = _model;
            ui.table->setModel(model);
        }
        return model;
    }
    ISPRModelData *setModel(SPRMainModel* _model){
        if(_model){
            mainModel = _model;
            model = new SPRSpectrumListItemsModel(_model->getSpectrumZonesTableModel(), _model->getSettingsFormulaModel());
            separateModel = new SPRSeparateModel(mainModel->getDoc());
        }
        
//        model->setModel(_model);
        ui.table->setModel(model);
        ui.graphic->setModel(model);
        return model;
    }

    void widgetsShow(){
        ui.table->widgetsShow();
    }

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
