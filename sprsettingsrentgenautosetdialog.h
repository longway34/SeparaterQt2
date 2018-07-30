#ifndef SPRSETTINGSRENTGENAUTOSETDIALOG_H
#define SPRSETTINGSRENTGENAUTOSETDIALOG_H

#include "ui_sprsettingrentgenautosetdialog.h"

#include "models/sprmainmodel.h"
#include "tcp/tcpautosetrentgen.h"
#include "firstcollumn2.h"

#include "isprwidget.h"

#include "qwt_legend.h"

class SPRSettingsRentgenAutoSetDialog : public QDialog, public ISPRWidget
{
    Q_OBJECT

    SPRSettingsRentgenModel *rentgenModel;
    SPRMainModel *mainModel;

    QList<int> threads;
//    QVector<uint16_t> codesDEU, codesCP;

    bool useGRU;

    TCPAutoSetRentgen *autoSetCommand;


    QwtPlotGrid *grid;
    QwtLegend *legend;

    SPRSpectrumListItemsModel *spectrums;

    uint16_t tempDEU[MAX_SPR_MAIN_THREADS];
    uint16_t tempCP[MAX_SPR_MAIN_THREADS];

    double controlDEU, controlCP;
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

    QList<int> getThreads();
    void setThreads(QList<int> &value);
    void setGraphicTitle();


public slots:
    virtual void widgetsShow();

    void onCommandComplite(TCPCommand *_command);
    void onStartButton(bool);
    void onCusorOverSelectItem(QwtPlotItem *item, MovedItemPosition);
    void onChangeSelectedItemValue(QwtPlotItem *item, double distance, MovedItemPosition position);
private:

    QByteArray codesToByteArray(uint16_t *table);
protected slots:
    virtual void onModelChanget(IModelVariable *source);
    void onDblClickMouseEvent();
};

#endif // SPRSETTINGSRENTGENAUTOSETDIALOG_H
