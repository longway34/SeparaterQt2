#ifndef SPRPITATELCONTROLWIDGET_H
#define SPRPITATELCONTROLWIDGET_H

#include "ui_sprpitatelcontrolwidget.h"
#include "models/sprmainmodel.h"
#include "tcp/tcpcommandstartpitatel.h"
#include "tcp/tcpcommandstoppitatel.h"
#include "tcp/tcpcommandseparatoronfull.h"
#include "tcp/tcpteststopseparate.h"
#include "isprwidget.h"
#include "models/sprsettingscontrolmodel.h"

#include <QAccessibleStateChangeEvent>
#include <QSpinBox>

class SPRPitatelControlWidget : public QWidget, public ISPRWidget
{
    Q_OBJECT

    SPRMainModel *mainModel;

    TCPCommandStartPitatel *startPitatelCommand;
    TCPCommandStopPitatel *stopPitatelCommand;
    TCPCommand *setPitatelCommand;

    TCPCommandSeparatorOnFull *separatorOn;
    TCPTestStopSeparate *separatorOff;
    TCPTimeOutWigget *toWidget;

    bool fullMode;

public:
    explicit SPRPitatelControlWidget(QWidget *parent = nullptr) :
        QWidget(parent), mainModel(nullptr),
        startPitatelCommand(nullptr),
        stopPitatelCommand(nullptr),
        setPitatelCommand(nullptr),
        separatorOn(nullptr),
        separatorOff(nullptr),
        toWidget(nullptr),
        fullMode(true)

    {
        ui.setupUi(this);
        toWidget = new TCPTimeOutWigget(nullptr);

        ui.bPitatelPause->setEnabled(false);

        QObject::connect(ui.bPitatelMinus, &QAbstractButton::clicked, this, &SPRPitatelControlWidget::onCommandButtomClickBool);
        QObject::connect(ui.bPitatelPlus, &QAbstractButton::clicked, this, &SPRPitatelControlWidget::onCommandButtomClickBool);
        QObject::connect(ui.bPitatelPause, &QAbstractButton::clicked, this, &SPRPitatelControlWidget::onCommandButtomClickBool);
        QObject::connect(ui.bSeparatorOn, &QAbstractButton::clicked, this, &SPRPitatelControlWidget::onCommandButtomClickBool);
        QObject::connect(ui.bSeparatorOff, &QAbstractButton::clicked, this, &SPRPitatelControlWidget::onCommandButtomClickBool);

        connect(ui.spPitatelPercents, SIGNAL(valueChanged(int)), this, SLOT(onCommandButtomClickInt(int)));

    }
    virtual ~SPRPitatelControlWidget(){

    }

    virtual ISPRModelData *setModelData(ISPRModelData *_mainModel)
    {
        mainModel = _mainModel->getMainModel();
        if(mainModel){

            connect(mainModel->getServer(), &ServerConnect::serverStateChange, this, &SPRPitatelControlWidget::onServerStateChange);

            if(startPitatelCommand) delete startPitatelCommand;
            startPitatelCommand = new TCPCommandStartPitatel(mainModel->getServer(), toWidget);
            startPitatelCommand->setModelVariable(mainModel->getSettingsControlModel()->VEMSBeginCode);
            startPitatelCommand->setLogWidget(this->getLogWidget());

            if(stopPitatelCommand) delete stopPitatelCommand;
            stopPitatelCommand = new TCPCommandStopPitatel(mainModel->getServer(), toWidget);
            stopPitatelCommand->setLogWidget(getLogWidget());

            if(setPitatelCommand) delete setPitatelCommand;
            setPitatelCommand = new TCPCommand(setpuw);

            if(separatorOn) delete separatorOn;
            separatorOn = new TCPCommandSeparatorOnFull(mainModel->getServer(), mainModel, toWidget, fullMode);

            if(separatorOff) delete separatorOff;
            separatorOff = new TCPTestStopSeparate(toWidget, mainModel);
            blockSignals(true);
            uint16_t val = mainModel->getSettingsControlModel()->VEMSBeginCode->getData();
            QString tt = tr("значение производительности питателя %1%").arg(val);
            ui.thPitatelPercents->setValue(val); ui.thPitatelPercents->setToolTip(tt);
            ui.spPitatelPercents->setValue(val); ui.spPitatelPercents->setToolTip(tt);

            ui.bPitatelPause->setCheckable(true);

//            ui.bPitatelPause->installEventFilter(this);

            connect(mainModel, SIGNAL(modelChanget(IModelVariable *)), this, SLOT(onModelChanget(IModelVariable *)));
            onServerStateChange(0);
            blockSignals(false);
        }
        return mainModel;
    }
private:
    Ui::SPRPitatelControlWidget ui;

    // ISPRWidget interface
public:
    virtual ISPRModelData *getModelData();

protected slots:
    virtual void onModelChanget(IModelVariable *send)
    {
        if(send == mainModel->getSettingsControlModel()->VEMSMaxCode){
            double val = static_cast<double>(mainModel->getSettingsControlModel()->VEMSMaxCode->getData());
            ui.thPitatelPercents->setScale(0, val);
            ui.spPitatelPercents->setMaximum(val);
            return;
        }
        if(send == mainModel->getSettingsControlModel()->VEMSLevelLess){
            double val = static_cast<double>(mainModel->getSettingsControlModel()->VEMSLevelLess->getData());
            ui.thPitatelPercents->setScaleStepSize(val);
            return;
        }
        if(send ==mainModel->getSettingsControlModel()->VEMSBeginCode){
            blockSignals(true);
            uint16_t val = mainModel->getSettingsControlModel()->VEMSBeginCode->getData();
            QString tt = tr("Значение производительности питателя %1").arg(val);
            ui.spPitatelPercents->setValue(val); ui.spPitatelPercents->setToolTip(tt);
            ui.thPitatelPercents->setValue(val); ui.thPitatelPercents->setToolTip(tt);
            blockSignals(false);
        }
    }
    virtual void widgetsShow();

    void onCommandComplite(TCPCommand *){

    }
    void onCommandNotComplite(TCPCommand *){

    }
    void onServerStateChange(uint32_t){
        if(mainModel){
            ServerConnect *server = mainModel->getServer();
            blockSignals(true);
            if(server->isState(spr_state_separator_on)){
                ui.bPitatelPause->setEnabled(true);
                ui.spPitatelPercents->setEnabled(true);
                ui.bPitatelMinus->setEnabled(true);
                ui.bPitatelPlus->setEnabled(true);
                ui.bSeparatorOn->setEnabled(false);
                ui.bSeparatorOff->setEnabled(true);
            } else {
                ui.bPitatelPause->setEnabled(false);
                ui.spPitatelPercents->setEnabled(false);
                ui.bPitatelMinus->setEnabled(false);
                ui.bPitatelPlus->setEnabled(false);
                ui.bSeparatorOn->setEnabled(true);
                ui.bSeparatorOff->setEnabled(false);
            }
            if(server->isState(spr_state_pitatel_on)){
                ui.bPitatelPause->setChecked(false);
                ui.bPitatelPause->setText(tr("Пауза"));
            } else {
                ui.bPitatelPause->setChecked(true);
                ui.bPitatelPause->setText(tr("Старт"));
            }
            blockSignals(false);
        }
    }
    void onCommandButtomClickBool(bool){
        if(mainModel){
            ServerConnect *server = mainModel->getServer();
            if(sender() == ui.bPitatelPause){
                if(!ui.bPitatelPause->isChecked()){
                    startPitatelCommand->send(server);
                } else {
                    stopPitatelCommand->send(server);
                }
            }
            if(sender() == ui.bSeparatorOn){
                separatorOn->send(server);
                return;
            }
            if(sender() == ui.bSeparatorOff){
                separatorOff->send(server);
            }
            if(sender() == ui.bPitatelPlus || sender() == ui.bPitatelMinus){
                blockSignals(true);
                double val = mainModel->getSettingsControlModel()->VEMSBeginCode->getData();
                if(sender() == ui.bPitatelPlus){
                    val += 1;
                    val = val > mainModel->getSettingsControlModel()->VEMSMaxCode->getData() ? mainModel->getSettingsControlModel()->VEMSMaxCode->getData() : val;
                } else {
                    val -= 1;
                    val = val < 0 ? 0 : val;
                }
                ui.thPitatelPercents->setValue(val);
                ui.spPitatelPercents->setValue(val);
                mainModel->getSettingsControlModel()->VEMSBeginCode->setData(val);
                startPitatelCommand->send(server);
                blockSignals(false);
                return;
            }
        }
    }

    void onCommandButtomClickInt(int){
        if(mainModel){
            ServerConnect *server = mainModel->getServer();
            blockSignals(true);
            uint16_t val = ui.spPitatelPercents->value() > mainModel->getSettingsControlModel()->VEMSMaxCode->getData() ?
                        mainModel->getSettingsControlModel()->VEMSMaxCode->getData() :
                                       ui.spPitatelPercents->value();
            ui.thPitatelPercents->setValue(val);
            mainModel->getSettingsControlModel()->VEMSBeginCode->setData(val);
            if(server->isState(spr_state_pitatel_on)){
                startPitatelCommand->send(server);
            }
            blockSignals(false);
        }
    }

    // ISPRWidget interface
public:
    virtual void setLogWidget(TCPLogsWigtets *value){
        ISPRWidget::setLogWidget(value);
        if(separatorOn){
            separatorOn->setLogWidget(value);
        }
        if(separatorOff){
            separatorOff->setLogWidget(value);
        }
    }

    // QObject interface
public:
//    virtual bool eventFilter(QObject *watched, QEvent *event){


//        if(watched == ui.bPitatelPause){
//            if (event->type() == QEvent::MouseButtonRelease)
//                if(mainModel->getServer()->isState(spr_state_separator_on)){
//                    return QObject::event(event);
//                } else {
//                    return true;
//                }
//            qDebug() << event->type();

//        }
//        return QObject::event(event);
//    }
    void setFullMode(bool value);
};

#endif // SPRPITATELCONTROLWIDGET_H
