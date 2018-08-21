#ifndef SPRSETTINGSRENTGENAUTOSETDIALOG_H
#define SPRSETTINGSRENTGENAUTOSETDIALOG_H

#include "ui_sprsettingrentgenautosetdialog.h"

#include "models/sprmainmodel.h"
#include "tcp/tcpautosetrentgen.h"
#include "tcp/tcpautosetcommandgo.h"
#include "firstcollumn2.h"

#include "isprwidget.h"

#include "qwt_legend.h"

class SPRSettingsRentgenAutoSetDialog : public QDialog, public ISPRWidget
{
    Q_OBJECT

    SPRSettingsRentgenModel *rentgenModel;
    SPRMainModel *mainModel;

    SPRThreadList threads;
//    QVector<uint16_t> codesDEU, codesCP;

    bool useGRU;

    TCPAutoSetRentgen *autoSetCommand;
    TCPAutosetCommandGo *autoSetGo;

    uint tempPeakPosition;

    TCPTimeOutWigget *toWidget;

    QwtPlotGrid *grid;
    QwtLegend *legend;

    SPRSpectrumListItemsModel *spectrums;

    QVector<uint16_t> tempDEU;
    QVector<uint16_t> tempCP;

    QByteArray tempDEU2Command;
    QByteArray tempCP2Command;

    void settingBACodes(){
        tempDEU2Command.clear();
        tempCP2Command.clear();

        for(int i=0; i<MAX_SPR_MAIN_THREADS; i++){
            tempDEU2Command.append((char*)&tempDEU[i], sizeof(uint16_t));
            tempCP2Command.append((char*)&tempCP[i], sizeof(uint16_t));
        }
    }

    QList<bool> deuOK;
    QList<bool> cpOK;

    double controlDEU, controlCP, controlAll;
public:
    QVector<QPointF> peakData;
    QwtPlotCurve *peakCurve;

    explicit SPRSettingsRentgenAutoSetDialog(QWidget *parent = 0);
    Ui::SPRSettingRentgenAutoSetDialog ui;

    virtual ~SPRSettingsRentgenAutoSetDialog();

    virtual ISPRModelData *getModelData();
    virtual ISPRModelData *setModelData(ISPRModelData *value);

    int currentViewThread;

    QPen defPen, selPen;

    SPRThreadList getThreads();
    void setThreads(SPRThreadList &value);
    void setGraphicTitle();

    bool finish;

    TCPTimeOutWigget *getToWidget() const;
    void setToWidget(TCPTimeOutWigget *value);

public slots:
    virtual void widgetsShow();

    void onCommandComplite(TCPCommand *_command);
    void onClickButtons(bool);
    void onCusorOverSelectItem(QwtPlotItem *item, MovedItemPosition);
    void onChangeSelectedItemValue(QwtPlotItem *item, double distance, MovedItemPosition position);
protected:
    bool isDeuCpAllOK(uint8_t th = 255, QList<bool> *_src = nullptr);
    double prDeuCpOKAll(QList<bool> *_src = nullptr);
private:

    QByteArray codesToByteArray(uint16_t *table);
protected slots:
    virtual void onModelChanget(IModelVariable *source);
    void onDblClickMouseEvent();

};

#endif // SPRSETTINGSRENTGENAUTOSETDIALOG_H
