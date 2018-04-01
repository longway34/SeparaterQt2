#include "sprtestimswidget.h"
#include "tcp/TCPTimeOutCommand.h"
#include <QMessageBox>
#include <QDataStream>

SPRTestIMSWidget::SPRTestIMSWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    towidget = new TCPTimeOutWigget();

// connect command buttons and change values
// *****************************************
    connect(ui.bSeparatorOn, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));
    connect(ui.bSeparatorOff, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));

    connect(ui.bRentgenOn, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));
    connect(ui.bRentgenOff, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));

    connect(ui.bRGUReadPosition, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));
    connect(ui.bRGUUp, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));
    connect(ui.bRGUDown, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));

    connect(ui.bThermoGet, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));
    connect(ui.bThermoSet, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));

    connect(ui.bGetSpectrum, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));

    connect(ui.cbZonesShow, SIGNAL(toggled(bool)), this, SLOT(onChangeValue(bool)));
    ui.wSpectrumWidget->setAllCurrent(true);

    connect(ui.bIMSStart, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));
    connect(ui.bIMSStop, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));

    connect(ui.lePitatelPercents, SIGNAL(valueChanged(double)), this, SLOT(onChangeValue(double)));
    connect(ui.bPitatelStart, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));
    connect(ui.bPitatelStop, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));

    connect(ui.bRaskladStart, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));
    connect(ui.bRaskladStop, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));

    connect(ui.bRudospuskStart, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));
    connect(ui.bRudospustStop, SIGNAL(clicked(bool)), this, SLOT(onCommand(bool)));

    vectorIms = {ui.cbIMSThread0, ui.cbIMSThread1, ui.cbIMSThread2, ui.cbIMSThread3, ui.cbIMSThread4, ui.cbIMSThread5, ui.cbIMSThread6, ui.cbIMSThread7};

    vectorSeparatorOnEnabled = {ui.bIMSStart,
                                ui.bPitatelStart,
                                ui.bRentgenOn,
                                ui.bRGUDown, ui.bRGUUp,
                                ui.bRudospuskStart,
                                ui.bThermoGet,
                                ui.bThermoSet,
                                ui.bRaskladStart,
                                ui.bGetSpectrum};

// init commands and connects
// *********************************************************************

    getSpectrumsCommand = new TCPCommandGetSpectrums(nullptr, towidget, model);
    connect(getSpectrumsCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(getSpectrumsCommand, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    rentgenOnCommand = new TCPCommandRentgerOn(nullptr, towidget);
    connect(rentgenOnCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(rentgenOnCommand, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    rentgenOffCommand = new TCPCommandSet(nullptr, towidget, QVector<TCPCommand*>{
            new TCPCommand(expoff),
            new TCPCommand(offren)
                                          });
    rentgenOffCommand->findCommands(expoff).first()->setSendData(QByteArray::fromRawData("\x00", 1));

    connect(rentgenOffCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(rentgenOffCommand, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    separatorOnCommand = new TCPCommandSeparatorOn(nullptr, towidget);
    connect(separatorOnCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(separatorOnCommand, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    separatorOffCommand = new TCPCommandSet(nullptr, towidget, {
        new TCPCommand(getstate),
        new TCPCommand(offren),
        new TCPCommand(offosw),
        new TCPCommand(offsep),
        new TCPCommand(getstate)
                                           });
    connect(separatorOffCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(separatorOffCommand, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    rguReadStateCommand = new TCPCommandSet(nullptr, towidget,
        {new TCPCommand(getrgu2),
         new TCPCommand(getstate)
                                            });
    connect(rguReadStateCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(rguReadStateCommand, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    rguUpCommand = new TCPCommandRGUUpDown(nullptr, towidget, true);
    connect(rguUpCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(rguUpCommand, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    rguDownCommand = new TCPCommandRGUUpDown(nullptr, towidget, false);
    connect(rguDownCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(rguDownCommand, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    thermoReadStateCommand = new TCPCommandSet(nullptr, towidget, {});
    QByteArray ct0 = QByteArray::fromRawData("\x00\x00\x00\x00", 4);
    TCPCommand *stemp = new TCPCommand(settemp); stemp->setSendData(ct0);
    thermoReadStateCommand->addCommand(stemp);
    thermoReadStateCommand->addCommand(new TCPTimeOutCommand(timeoutcommand, 2000));
    thermoReadStateCommand->addCommand(new TCPCommand(gettemp));
    connect(thermoReadStateCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(thermoReadStateCommand, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    thermoWriteStateCommand = new TCPCommandSet(nullptr, towidget, {});
    QByteArray term;
    uint16_t min, max;
    min = ui.slThermoMin->value(); max = ui.slThermoMax->value();
    QDataStream ds(&term, QIODevice::WriteOnly);
    ds <<  min << max;
    TCPCommand *sthermo = new TCPCommand(settemp); sthermo->setSendData(term);
    thermoWriteStateCommand->addCommand(sthermo)->addCommand(new TCPTimeOutCommand(timeoutcommand, 2000))->addCommand(new TCPCommand(gettemp))->addCommand(new TCPCommand(getstate));
    connect(thermoWriteStateCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(thermoWriteStateCommand, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    startTestImsCommand = new TCPStartSopTestIMSCommand(nullptr, towidget);
    connect(startTestImsCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(startTestImsCommand, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    commandStartPitatel = new TCPCommandStartPitatel(nullptr);
    connect(commandStartPitatel, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(commandStartPitatel, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    commandStopPitatel = new TCPCommandStopPitatel(nullptr);
    connect(commandStopPitatel, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(commandStopPitatel, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    commandChangePersentPitatel = new TCPCommand(setpuw);
    uint16_t persent = 880;
    commandChangePersentPitatel->setSendData(QByteArray::fromRawData((char*)&persent, sizeof(persent)));
    connect(commandChangePersentPitatel, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(commandChangePersentPitatel, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    commandStartRasklad = new TCPCommand(oniw);
    connect(commandStartRasklad, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(commandStartRasklad, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    commandStopRasklad = new TCPCommand(offiw);
    connect(commandStopRasklad, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(commandStopRasklad, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    commandStartRudostusk = new TCPStartRudospuskCommand(nullptr, nullptr);
    connect(commandStartRudostusk, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(commandStartRudostusk, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

    commandStopRudospusk = new TCPStopRudostuskCommand(nullptr, nullptr);
    connect(commandStopRudospusk, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(commandStopRudospusk, SIGNAL(commandComplite(TCPCommand*)), ui.logsWidget, SLOT(onLogsCommand(TCPCommand*)));

}

void SPRTestIMSWidget::onChangeValue(bool _val){
    if(sender() == ui.cbZonesShow){
        ui.wSpectrumWidget->setZonesShow(_val);
    }
}

void SPRTestIMSWidget::onChangeValue(double _val){
    if(sender() == ui.lePitatelPercents){
        if(model->getServer()->isState(spr_state_pitatel_on)){
            uint16_t percent = _val;
            commandChangePersentPitatel->setSendData(QByteArray::fromRawData((char*)&percent, sizeof(percent)));
            commandChangePersentPitatel->send(model->getServer());
        }
    }
}

void SPRTestIMSWidget::onCommand(bool){

    if(sender() == ui.bRudospuskStart){
        commandStartRudostusk->send(model->getServer());
        return;
    }
    if(sender() == ui.bRudospustStop){
        commandStopRudospusk->send(model->getServer());
        return;
    }

    if(sender() == ui.bRaskladStart){
        commandStartRasklad->send(model->getServer());
        return;
    }

    if(sender() == ui.bRaskladStop){
        commandStopRasklad->send(model->getServer());
    }

    if(sender() == ui.bRGUReadPosition){
        rguReadStateCommand->send(model->getServer());
        return;
    }
    if(sender() == ui.bRGUUp){
        rguUpCommand->send(model->getServer());
        return;
    }
    if(sender() == ui.bRGUDown){
        rguDownCommand->send(model->getServer());
        return;
    }
    if(sender() == ui.bSeparatorOn){
        separatorOnCommand->send(model->getServer());
    }
    if(sender() == ui.bSeparatorOff){
        separatorOffCommand->send(model->getServer());
        return;
    }
    if(sender() == ui.bThermoGet){
        thermoReadStateCommand->send(model->getServer());
        return;
    }
    if(sender() == ui.bThermoSet){
        setThermo();
        return;
    }
    if(sender() == ui.bIMSStart){
        QVector<uint8_t> _vch;
//        QVector<QCheckBox*> _cbCh= {ui.cbIMSThread0, ui.cbIMSThread1, ui.cbIMSThread2, ui.cbIMSThread3, ui.cbIMSThread4, ui.cbIMSThread5, ui.cbIMSThread6, ui.cbIMSThread7};
        for(uint i=0; i < model->getSettingsMainModel()->getIms()->getData(); i++){
            if(ui.cbIMSAllThreads->isChecked() || vectorIms[i]->isChecked()){
                _vch.push_back(i);
            }
        }
        startTestImsCommand->setParams(_vch, ui.slIMSDelay->value(), ui.slIMSTime->value(), ui.slIMSFrequency->value());
        startTestImsCommand->send(model->getServer());
        return;
    }
    if(sender() == ui.bIMSStop){
        startTestImsCommand->stopTest();
        return;
    }
    if(sender() == ui.bPitatelStart){
        commandStartPitatel->send(model->getServer());
        return;
    }
    if(sender() == ui.bPitatelStop){
//        commandStartPitatel->setPitatelStopFlag();
        commandStopPitatel->send(model->getServer());
        return;
    }
    if(sender() == ui.bRentgenOn){
        rentgenOnCommand->send(model->getServer());
        return;
    }
    if(sender() == ui.bRentgenOff){
        rentgenOffCommand->send(model->getServer());
        return;
    }
    if(sender() == ui.bGetSpectrum){
        getSpectrumsCommand->send(model->getServer());
        return;
    }
}

void SPRTestIMSWidget::onCommandComplite(TCPCommand *_comm){
    QString msgTitle;
    QString msgText;

    if(sender() == commandStopRasklad){
        if(!_comm->noErrors()){
            int err = _comm->getErrors();
            msgTitle = tr("Команда выключить раскладчик");
            msgText = tr("Выполнено с ошибкой %1").arg(QString::number(err));
            QMessageBox::information(this, msgTitle, msgText);
        }
        return;
    }

    if(sender() == commandStartRasklad){
        if(!_comm->noErrors()){
            int err = _comm->getErrors();
            msgTitle = tr("Команда включить раскладчик");
            msgText = tr("Выполнено с ошибкой %1").arg(QString::number(err));
            QMessageBox::information(this, msgTitle, msgText);
        }
        return;
    }

    if(sender() == commandStopPitatel){
        if(_comm->noErrors()){
            model->getServer()->clearState(spr_state_pitatel_on);
        } else {
            int err = _comm->getErrors();
            msgTitle = tr("Команда выключить питатель");
            msgText = tr("Выполнено с ошибкой %1").arg(QString::number(err));
            QMessageBox::information(this, msgTitle, msgText);
        }
        return;
    }

    if(sender() == commandStartPitatel){
        if(_comm->noErrors()){
            model->getServer()->addState(spr_state_pitatel_on);
        } else {
            int err = _comm->getErrors();
            msgTitle = tr("Команда включить питатель");
            msgText = tr("Выполнено с ошибкой %1").arg(QString::number(err));
            QMessageBox::information(this, msgTitle, msgText);
        }
        return;
    }

    if(sender() == separatorOnCommand){
        int err = separatorOnCommand->getErrors();
        msgTitle = tr("Команда включить сепаратор");
        if(err == 0){
            msgText = tr("Выплнена успешно.");
            model->getServer()->addState(spr_state_separator_on);
        } else {
            msgText = tr("Выполнено с ошибкой %1").arg(QString::number(err));
        }
        QMessageBox::information(this, msgTitle, msgText);
        return;
    }
    if(sender() == separatorOffCommand){
        uint8_t err = separatorOffCommand->getErrors();
        msgTitle = tr("Команда выключить сепаратор");
        if(err == 0){
            msgText = tr("Выплнена успешно.");
            model->getServer()->clearState(spr_state_separator_on);
            model->getServer()->clearState(spr_state_rentgen_on);
        } else {
            msgText = tr("Выполнено с ошибкой %1 state %2").arg(QString::number(err));
        }
        QMessageBox::information(this, msgTitle, msgText);
        return;
    }
    if(sender() == rguReadStateCommand){

        QByteArray result = rguReadStateCommand->getResult(getrgu2);
        if(result.size() > 0){
            switch (result.at(result.size()-1)) {
            case 0:
                ui.lRGUPosition->setText(tr("Положение не определено"));
                break;
            case 1:
                ui.lRGUPosition->setText(tr("Верхнее положение"));
                break;
            case 2:
                ui.lRGUPosition->setText(tr("Нижнее положение"));
                break;
            }
        }
        return;
    }

    if(sender() == rguUpCommand){
        msgTitle = tr("Команда поднять РГУ");
        int state = rguUpCommand->getState();
        QByteArray result = rguUpCommand->getResult(getrgu2);
        if(state == 0 && result.size() > 0 && result[0] == 1){
           msgText = tr("Команда поднять РГУ\подана успешно");
        } else {
            msgText = QString("Команда поднять РГУ\подана с ошибкой %1 state %2").arg(QString::number(result[0]), QString::number(state));
        }
        QMessageBox::information(this, tr("Команда поднять РГУ"), tr("Команда поднять РГУ\подана успешно"));
        rguReadStateCommand->send(model->getServer());
    }
    if(sender() == rguDownCommand){
        msgTitle = tr("Команда опустить РГУ");
        int state = rguDownCommand->getState();
        QByteArray result = rguDownCommand->getResult(getrgu2);
        if(state == 0 && result.size() > 0 && result[0] == 2){
           msgText = tr("Команда опустить РГУ\подана успешно");
        } else {
           msgText = QString("Команда опусеить РГУ\опущена с ошибкой %1 state %2").arg(QString::number(result[0]), QString::number(state));
        }
        QMessageBox::information(this, msgTitle, msgText);
        rguReadStateCommand->send(model->getServer());
    }
    if(sender() == thermoReadStateCommand){
        QByteArray res = thermoReadStateCommand->getResult(gettemp);
        if(res.size() >= sizeof(double) * 3){
            double terms[3];
            memcpy(terms, res.constData(), sizeof(double) * 3);

            ui.lThermoCurrent->setText(QString::number(terms[2]));
            ui.slThermoMin->setValue(terms[0]);
            ui.slThermoMax->setValue(terms[1]);
        }
    }
    if(sender() == thermoWriteStateCommand){
        int err = separatorOnCommand->getErrors();
        if(err != 0){
            msgTitle = tr("Команда установки темпиратурного регулятора");
            msgText = tr("Выполнено с ошибкой %1").arg(QString::number(err));
        }
        QMessageBox::information(this, msgTitle, msgText);
        return;

    }

    if(sender() == rentgenOnCommand){
        QByteArray res = rentgenOnCommand->getReplayData();
        if(res.size() > 0 && res[0] == 0){
            model->getServer()->addState(spr_state_rentgen_on);
            QMessageBox::information(this, tr("Включение рентгена"), tr("Рентген включен"));
        } else {
            QMessageBox::information(this, tr("Включение рентгена"), tr("Рентген не включен\n или включен с ошибкой..."));
        }
        return;
    }
    if(sender() == rentgenOffCommand){
        QByteArray res = rentgenOffCommand->getReplayData();
        if(res.size() > 0 && res[0] == 0){
            model->getServer()->clearState(spr_state_rentgen_on);
            QMessageBox::information(this, tr("Выключение рентгена"), tr("Рентген выключен"));
        } else {
            QMessageBox::information(this, tr("Выключение рентгена"), tr("Рентген не выключен\n или выключен с ошибкой..."));
        }
        return;

    }
    if(sender() == getSpectrumsCommand){
        QVector<QColor> colors = {Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::cyan, Qt::magenta};
        QByteArray res = getSpectrumsCommand->getReplayData();
        if(res.size() > 0 && res[0] == 0){
            QMessageBox::information(this, tr("Получение спектров"), tr("Спектры получены"));
            ui.wSpectrumWidget->getModel()->clearGraphicsItemModel();
            for(int ch=0; ch<getSpectrumsCommand->getChannels(); ch++){
                QByteArray sp = getSpectrumsCommand->getSpectrum(ch);
                ui.wSpectrumWidget->getModel()->addSpectrum(sp);
                QColor col = colors[ch % colors.size()];
                ui.wSpectrumWidget->getModel()->getSpectrumItem(ch)->setSpectrumColor(col);
            }

            ui.wSpectrumWidget->setVisibleAll();
            ui.wSpectrumWidget->widgetsShow();
        } else {
            QMessageBox::information(this, tr("Получение спектров"), tr("Спектры не получены\n или получены с ошибкой..."));
        }
        return;

    }
    if(sender() == startTestImsCommand){
        if(_comm == startTestImsCommand->getStopTestCommand()){
            QMessageBox::information(this, tr("Останов теста"), tr("Тест остановлен пользователем"));

        }
    }
}

void SPRTestIMSWidget::onServerConnectError(ITCPCommand* _comm){
    ui.logsWidget->onLogsCommand((TCPCommand*)_comm, tr("Ошибка присоединения"));
}

void SPRTestIMSWidget::onServerReadWriteError(ITCPCommand* _comm){
    ui.logsWidget->onLogsCommand((TCPCommand*)_comm, tr("Ошибка чтения/записи команды"));
}

void SPRTestIMSWidget::onServerConnectChangeState(uint32_t _state){
    if(_state & spr_state_test_ims){
        ui.bIMSStart->setEnabled(false);
    } else {
        ui.bIMSStart->setEnabled(true);
    }
    if(_state & spr_state_separator_on){
        for(int i=0; i<vectorSeparatorOnEnabled.size(); i++){
            vectorSeparatorOnEnabled[i]->setEnabled(true);
        }
        if(_state & spr_state_rentgen_on){
            ui.bGetSpectrum->setEnabled(true);
        } else {
            ui.bGetSpectrum->setEnabled(false);
        }
    } else {
        for(int i=0; i<vectorSeparatorOnEnabled.size(); i++){
            vectorSeparatorOnEnabled[i]->setEnabled(false);
        }
    }
}

ISPRModelData *SPRTestIMSWidget::setModel(SPRMainModel *_model)
{
    model = _model;

    connect(model->getServer(), SIGNAL(serverConnectTimeOutError(ITCPCommand*)), this, SLOT(onServerConnectError(ITCPCommand*)));
    connect(model->getServer(), SIGNAL(serverReadWriteTimeOutError(ITCPCommand*)), this, SLOT(onServerReadWriteError(ITCPCommand*)));
    connect(model->getServer(), SIGNAL(serverStateChange(uint32_t)), this, SLOT(onServerConnectChangeState(uint32_t)));

    ui.wSpectrumWidget->setModel(new SPRSpectrumListItemsModel(model->getSpectrumZonesTableModel(), model->getSettingsFormulaModel()));

    getSpectrumsCommand->setModel(model);
    separatorOnCommand->setModel(model->getSettingsRentgenModel());
    rentgenOnCommand->setModel(model->getSettingsRentgenModel());

//    commandRaskladStart

    widgetsShow();

    return model;
}


void SPRTestIMSWidget::widgetsShow()
{
    if(model){
        for(int i=0; i<model->getSettingsMainModel()->getIms()->getData(); i++){
            vectorIms[i]->setVisible(true);
        }
        for(int i=model->getSettingsMainModel()->getIms()->getData(); i<vectorIms.size(); i++){
            vectorIms[i]->setVisible(false);
        }
        onServerConnectChangeState(model->getServer()->getState());
    }

}

ISPRModelData *SPRTestIMSWidget::getModel()
{
}
