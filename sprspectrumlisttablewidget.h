#ifndef SPRSPECTRUMLISTTABLEWIDGET_H
#define SPRSPECTRUMLISTTABLEWIDGET_H

#include "ui_sprspectrumlisttablewidget.h"
#include "models/sprgrspectrumitemmodel.h"
#include "models/sprmainmodel.h"
#include "isprwidget.h"
#include "sprspectrumchoisetimedialog.h"

//#include "tcp/tcpcommandgetspectrums.h"
#include "tcp/tcpgetspectrumsgistogramms.h"
//#include "tcp/tcpcommandseparatoron.h"

#include "tcp/tcpcommandrentgenonfull.h"

#include "tcp/tcpcommandrentgeron.h"
#include "tcp/TCPCommandSet.h"
#include "tcp/tcpcommandrguupdown.h"

#include "qwt_plot_curve.h"
#include "qwt_plot_histogram.h"
#include "qwt_plot_grid.h"



class SPRSpectrumListTableWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRMainModel *model;
    SPRSpectrumListItemsModel *spectrums;

    SPRSpectrumChoiseTimeDialog *choiseTimeDialog;

    TCPGetSpectrumsGistogramms *gettingSpectrumsCommand;
    TCPCommandSeparatorOnFull *rentgenOnCommand;
    TCPCommandRGUUpDown *rguUpDownCommand;
    TCPCommand *rentgenOffCommand;
    TCPCommandSet *commands;

    TCPTimeOutWigget *toWidget;


public:
    explicit SPRSpectrumListTableWidget(QWidget *parent = 0);

    virtual ISPRModelData *setModelData(SPRMainModel *mainModel);
    virtual ISPRModelData *getModelData();

    void setSpectrumsData(QFile *inp);

public slots:
    void widgetsShow();
    void onChangeSpectColor(int row);
    void onChangeCheckedSpectrum(QList<int> checked, int current){
        if(sender() == ui.tListSpectrumItem){
            if((current >= 0 && current < ui.tListSpectrumItem->rowCount()) || checked.size() > 0)
                onSpectSpectrumTableClick(current, 0, ui.tListSpectrumItem);
            return;
        } else if(sender() == ui.tListBasedSpectrumItem){
            if((current >= 0 && current < ui.tListBasedSpectrumItem->rowCount()) || checked.size() > 0)
                onSpectSpectrumTableClick(current, 0, ui.tListBasedSpectrumItem);
            return;
        }
    }

    void onGetSpectrums(bool);
    void onCompliteCommand(TCPCommand *command);
    void onErrorsCommand(TCPCommand *command);
    void onSpectSpectrumTableClick(int row, int col, SPRSpectrumListTable *_sender=nullptr);
private:
    Ui::SPRSpectrumListTableWidget ui;

    // ISPRWidget interface
public:
signals:
    void doShow();

    // ISPRWidget interface
protected slots:
    virtual void viewChange(int row, int col);
    virtual void viewChange(int row);
    virtual void viewChange(bool value);
    void onChangeZoneRange(EnumElements el, int thread, int col);
    virtual void onModelChanget(IModelVariable *);


    // ISPRWidget interface
protected:
};

#endif // SPRSPECTRUMLISTTABLEWIDGET_H
