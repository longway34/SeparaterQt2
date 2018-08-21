#include "sprseparatecontrolwidget.h"
#include <QMessageBox>

void SPRSeparateControlWidget::setLogWidget(TCPLogsWigtets *value)
{
    logWidget = value;
    if(startSeparate){
        startSeparate->setLogWidget(logWidget);
    }
    if(stopSeparate){
        stopSeparate->setLogWidget(logWidget);
    }
}

SPRSeparateControlWidget::SPRSeparateControlWidget(QWidget *parent) :
    QWidget(parent), model(nullptr), kSpectrumModel(nullptr), startSeparate(nullptr), stopSeparate(nullptr), setSeparate(nullptr), toWidget(nullptr)
{
    ui.setupUi(this);

    connect(ui.bSeparateStart, SIGNAL(clicked(bool)), this, SLOT(onCommandButtomClick(bool)));
    connect(ui.bSeparateStop, SIGNAL(clicked(bool)), this, SLOT(onCommandButtomClick(bool)));
    connect(ui.bSeparateComplite, SIGNAL(clicked(bool)), this, SLOT(onCommandButtomClick(bool)));

    separateTimer.setInterval(1000);
    lastTimerUpdate = QDateTime::currentDateTime();
    connect(&separateTimer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

void SPRSeparateControlWidget::setModelData(SPRMainModel *_model, TCPTimeOutWigget *_toWidget, TCPLogsWigtets *_logWidget)
{
    model = _model;
    toWidget = _toWidget;
    logWidget = _logWidget;

    if(model){

        connect(model->getServer(), SIGNAL(serverStateChange(uint32_t)), this, SLOT(onServerStateChange(uint32_t)));

        kSpectrumModel = model->getKSpectrums();

        if(startSeparate) delete startSeparate;
        startSeparate = new TCPStartSeparate2(model, getTimeOutWidget(), getLogWidget());
        connect(startSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onTCPCommandComplite(TCPCommand*)));
        connect(startSeparate, SIGNAL(commandNotComplite(TCPCommand*)), this, SLOT(onTCPCommandNotComplite(TCPCommand*)));

        stopSeparate = new TCPTestStopSeparate(toWidget);
        connect(stopSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onTCPCommandComplite(TCPCommand*)));

        if(setSeparate) delete setSeparate;
        setSeparate = new TCPCommand(setsepar);
        connect(setSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onTCPCommandComplite(TCPCommand*)));
        int err;
        QByteArray settings = model->getSeparateModel()->toByteArray(&err);
        setSeparate->setSendData(settings);


    }

}

void SPRSeparateControlWidget::onCommandButtomClick(bool)
{
    if(model){
        if(sender() == ui.bSeparateStart){
            startSeparate->send(model->getServer());
            return;
        }
        if(sender() == ui.bSeparateStop){
            stopSeparate->send(model->getServer());
            return;
        }
        if(sender() == ui.bSeparateComplite){
            if(setSeparate){
                int err;
                QByteArray settings = model->getSeparateModel()->toByteArray(&err);
                setSeparate->setSendData(settings);
                setSeparate->send(model->getServer());
            }
        }
    }

}

void SPRSeparateControlWidget::onTimeOut()
{
    QDateTime diff = QDateTime::fromSecsSinceEpoch(QDateTime::currentDateTime().toSecsSinceEpoch() - lastTimerUpdate.toSecsSinceEpoch());
//    diff = diff.addSecs(-(7*60*60));
    if(diff.toSecsSinceEpoch() > 0){
        ui.leRudospustTime->setText(diff.toString("hh:mm:ss"));
    }


}

void SPRSeparateControlWidget::onSpectrumsReady(TCPGetSpectrumsGistogramms *_command, bool isKSpectrum, uint32_t _time_scope_in_msec){
    if(kSpectrumModel){
        blockSignals(true);
        QVector<TCPCommand*> vspect = isKSpectrum ? _command->findCommands(getkspk) : _command->findCommands(getspk);
        for(int th=0; th<vspect.size();th++){
            QByteArray res;
            if(isKSpectrum){
                res = _command->getKSpectrumData(th);
            } else {
                res = _command->getSpectrumData(th);
            }
            QByteArray spk = vspect[th]->getReplayData().right(DEF_SPECTRUM_DATA_LENGTH_BYTE);

//            uint8_t *spec = (uint8_t*)(res.left(DEF_SPECTRUM_DATA_LENGTH_BYTE).constData());

            uint32_t t;
            if(isKSpectrum)
                t = _command->getKSpectrumTime(th);
            else if(_time_scope_in_msec > 0){
                t = _time_scope_in_msec;
            } else {
                t = 5000;
            }

//            SPRSpectrumItemModel *item = kSpectrumModel->addSpectrum(spec, DEF_SPECTRUM_DATA_LENGTH_BYTE, t);
            SPRSpectrumItemModel *item = kSpectrumModel->setSpectrumData(th, spk, spectrumsOnly, t);
            item->setTimeScope(t);

            QList<SPRSpectrumItemModel *> lbase = kSpectrumModel->getSpectrumsItemByThread(th);
            if(lbase.size() > 0){
                EnumElements area = model->getSettingsControlModel()->controlArea->getData();
                double correl = item->getCorrel(lbase.first(), true, area);
                double critical = model->getSettingsControlModel()->correlSpectrumCritical->getData();
                if(correl < critical){
                    if(getLogWidget()){
                        getLogWidget()->onErrorLogsCommand(_command, QString(tr("Корреляция на ручье %1(%2) ниже критического %3 ")).
                                                           arg(th).
                                                           arg(QString::number(correl,'f',2)).arg(QString::number(critical, 'f', 2)));
                    }
                }
                qDebug() << "Correl :" << correl /*<< std::endl*/;
            }
        }
        blockSignals(false);
    }
}


void SPRSeparateControlWidget::onTCPCommandComplite(TCPCommand *command)
{
    if(model){
        if(sender() == startSeparate){
            if(command->getCommand() == getsepar){
                QByteArray res = command->getReplayData();
                int s = res.size();
                if(s > 1){
                    model->getSeparateModel()->setWorkSeparateData(res.right(s-1));
                    model->getSeparateOutputModel()->addWorkSeparateData(res.right(s-1));
//                    widgetsShow();
                }
                return;
            }
            TCPGetSpectrumsGistogramms *gsg = dynamic_cast<TCPGetSpectrumsGistogramms *>(command);
            if(gsg){
                if(gsg->getDataType() == getgist){
//                if(command == startSeparate->getHistogrammCommand()){
                    QVector<TCPCommand*> vgist = command->findCommands(getgist);
                    for(int i=0; i<vgist.size();i++){
                        QByteArray gist = vgist[i]->getReplayData();
                        int s = gist.size();
                        if(s > 1){
                            model->getSeparateModel()->setWorkGistogrammData(gist.right(s-1), i);
                        }
                    }
                    return;
                }
                if(gsg->getDataType() == getkspk){
//                if(command == startSeparate->getKSpectrunsCommand()){
//                    onSpectrumsReady(dynamic_cast<TCPGetSpectrumsGistogramms*>(command), true);
                    onSpectrumsReady(gsg, true);
                    return;
                }
                if(gsg->getDataType() == getspk){
//                if(command == startSeparate->getGetBaseSpectrum()){
//                    onSpectrumsReady(dynamic_cast<TCPGetSpectrumsGistogramms*>(command), false);
                    onSpectrumsReady(gsg, false);
                    return;
                }
            }
        }
        if(sender() == setSeparate){
            if(getLogWidget()){
                getLogWidget()->onLogsCommand(tr("Применены новые настройки сепарирования..."));
            }
            QMessageBox::information(nullptr, tr("Настройки сепаратора"), tr("Применены новые настройки сепарирования..."));
        }
    }

}

void SPRSeparateControlWidget::onTCPCommandNotComplite(TCPCommand *command)
{
    if(sender() == startSeparate){
        if(command->getCommand() == getren){
            uint16_t mka, mkv; uint8_t err;
            startSeparate->isRentgenReady(command->getReplayData(), &mkv, &mka, &err);
            QMessageBox::warning(nullptr, tr("Ошибка включения рентгена"),
                                 tr("Рентген не вышел на рабочий режим (%1, %2 < 0x600)\n"
                                    "Попробуйте выключить рентген\n"
                                    "И запустить сепарацию еще раз...").
                                 arg(mkv, 0, 16).arg(mka, 0, 16));

        }
    }
}


void SPRSeparateControlWidget::onServerStateChange(uint32_t)
{
    if(model){
        if(model->getServer()->isState(spr_state_separator_on)){
            ui.bSeparateStart->setEnabled(true);
        } else {
            ui.bSeparateStart->setEnabled(false);
        }
        if(model->getServer()->isState(spr_state_separated)){
            lastTimerUpdate = QDateTime::currentDateTime();
            separateTimer.start();
        } else {
            separateTimer.stop();
        }
    }

}

void SPRSeparateControlWidget::onModelChanget(IModelVariable *)
{

}


SPRSpectrumListItemsModel *SPRSeparateControlWidget::getKSpectrumModel() const
{
    return kSpectrumModel;
}

void SPRSeparateControlWidget::setKSpectrumModel(SPRSpectrumListItemsModel *value)
{
    kSpectrumModel = value;
}

TCPLogsWigtets *SPRSeparateControlWidget::getLogWidget() const
{
    return logWidget;
}

TCPTimeOutWigget *SPRSeparateControlWidget::getTimeOutWidget() const
{
    return toWidget;
}

