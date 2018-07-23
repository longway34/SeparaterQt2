#include "sprseparatewidget.h"

SPRSeparateWidget::SPRSeparateWidget(QWidget *parent) :
    QWidget(parent), wPorogs(nullptr), kSpectrumModel(nullptr), startSeparate(nullptr), stopSeparate(nullptr), separatorOn(nullptr)
{
    ui.setupUi(this);

    toWidget = new TCPTimeOutWigget();
    toWidget->setVisible(false);

    stopSeparate = new TCPTestStopSeparate(toWidget);
    connect(stopSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

    connect(ui.bChangePorogs, SIGNAL(clicked(bool)), this, SLOT(onChangePororsCommand(bool)));

    connect(ui.bSeparateStart, SIGNAL(clicked(bool)), this, SLOT(onCommandButtomClick(bool)));
    connect(ui.bSeparateStop, SIGNAL(clicked(bool)), this, SLOT(onCommandButtomClick(bool)));
    connect(ui.bSeparatorOn, SIGNAL(clicked(bool)), this, SLOT(onCommandButtomClick(bool)));
    connect(ui.bSeparatorOff, SIGNAL(clicked(bool)), this, SLOT(onCommandButtomClick(bool)));

}

void SPRSeparateWidget::onCommandButtomClick(bool){
    if(model){
        if(sender() == ui.bSeparateStart){
            startSeparate->send(model->getServer());
            return;
        }
        if(sender() == ui.bSeparatorOn){
            separatorOn->send(model->getServer());
            return;
        }
        if(sender() == ui.bSeparateStop){
            stopSeparate->send(model->getServer());
            return;
        }
        if(sender() == ui.bSeparatorOff){
            stopSeparate->send(model->getServer());
        }
    }

}

void SPRSeparateWidget::onServerStateChange(uint32_t){
    if(model){
        if(model->getServer()->isState(spr_state_separator_on)){
            ui.bSeparateStart->setEnabled(true);
        } else {
            ui.bSeparateStart->setEnabled(false);
        }
    }
}

void SPRSeparateWidget::widgetsShow()
{
}

ISPRModelData *SPRSeparateWidget::setModelData(ISPRModelData *data)
{
    model = (SPRMainModel*)data;
    if(model){

        connect(model->getServer(), SIGNAL(serverStateChange(uint32_t)), this, SLOT(onServerStateChange(uint32_t)));

        ui.tSeparateDetail->setModelData(model);
        ui.tSeparateDetail->widgetsShow();

        ui.gGistogramm->setModelData(model);
        ui.gGistogramm->widgetsShow();

        if(kSpectrumModel) delete kSpectrumModel;
        kSpectrumModel = new SPRSpectrumListItemsModel(model->getSpectrumZonesTableModel(), model->getSettingsFormulaModel(),model->getSettingsMainModel()->getThreads(), nullptr, model->getSettingsControlModel()->controlArea);

        ui.gKSpectrums->setModelData(kSpectrumModel, spectrumsOnly, false);
        ui.gKSpectrums->setVisibleAll();

        if(wPorogs) delete wPorogs;
        wPorogs = new SPRSettingsPorogsWidget();
        wPorogs->setModelData(model->getSettingsPorogsModel());


        if(startSeparate) delete startSeparate;
        startSeparate = new TCPStartSeparate2(model, toWidget, getLogWidget());
        connect(startSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onSeparateCommandComplite(TCPCommand*)));

        if(separatorOn) delete separatorOn;
        separatorOn = new TCPCommandSeparatorOnFull(model->getServer(), model, toWidget);
        connect(separatorOn, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

    }
}

void SPRSeparateWidget::onCommandComplite(TCPCommand *command){

}

void SPRSeparateWidget::onKSpectrumReady(TCPGetSpectrumsGistogramms *_command){
    if(kSpectrumModel){
        blockSignals(true);
        QVector<TCPCommand*> vkspect = _command->findCommands(getkspk);
        for(int th=0; th<vkspect.size();th++){
            QByteArray res = _command->getKSpectrumData(th);
            uint8_t *spec = (uint8_t*)(res.left(DEF_SPECTRUM_DATA_LENGTH_BYTE).constData());

            SPRSpectrumItemModel *item = kSpectrumModel->addSpectrum(spec, DEF_SPECTRUM_DATA_LENGTH_BYTE, _command->getKSpectrumTime(th));
            uint32_t t = _command->getKSpectrumTime(th);
            item->setTimeScope(t);

            SPRSpectrumItemModel *bItem = kSpectrumModel->getSpectrumBaseItem((th % kSpectrumModel->getSpectrumsModelBase()->size()));
            if(bItem){
                double correl = item->getCorrel(bItem);
                qDebug() << "Correl :" << correl /*<< std::endl*/;
            }
        }
        blockSignals(false);
    }
}

void SPRSeparateWidget::onSeparateCommandComplite(TCPCommand* command){
    if(model){
        if(sender() == startSeparate){
            if(command->getCommand() == getsepar){
                QByteArray res = command->getReplayData();
                int s = res.size();
                if(s > 1){
                    model->getSeparateModel()->setWorkSeparateData(res.right(s-1));
//                    widgetsShow();
                }
            }
            if(command->isCommandSet() && command->getName() == "setGetSpectrumsGistorfamms"){
                TCPGetSpectrumsGistogramms *comm = (TCPGetSpectrumsGistogramms*)command;
                if(comm->getDataType() == getkspk){
                    blockSignals(true);
                    kSpectrumModel->clearSpectrums();
                    onKSpectrumReady(comm);
                    ui.gKSpectrums->setVisibleAll();
                    ui.gKSpectrums->widgetsShow();
                    blockSignals(false);

                }
                if(comm->getDataType() == getspk){
                    QVector<TCPCommand*> vspk = comm->findCommands(getspk);
                    for(int i=0; i<vspk.size();i++){
                        QByteArray spk = vspk[i]->getReplayData().right(DEF_SPECTRUM_DATA_LENGTH_BYTE);
                        SPRSpectrumItemModel *item = kSpectrumModel->setSpectrumData(i, spk);
                        item->setTimeScope(1);
                    }
//                    ui.baseGrapthics->setVisibleAll();
//                    widgetsShow();
                }
                if(comm->getDataType() == getgist){
                    QVector<TCPCommand*> vgist = comm->findCommands(getgist);
                    for(int i=0; i<vgist.size();i++){
                        QByteArray gist = vgist[i]->getReplayData();
                        int s = gist.size();
                        if(s > 1){
                            model->getSeparateModel()->setWorkGistogrammData(gist.right(s-1), i);
                        }
                    }
                }
            }
        }
    }
}

void SPRSeparateWidget::onChangePororsCommand(bool){
    if(wPorogs){
        wPorogs->setWindowModality(Qt::WindowModal);
        wPorogs->show();
    }
}


ISPRModelData *SPRSeparateWidget::getModelData()
{
    return model;
}

void SPRSeparateWidget::onModelChanget(IModelVariable *)
{
}


void SPRSeparateWidget::setLogWidget(TCPLogsWigtets *value)
{
    ISPRWidget::setLogWidget(value);
    startSeparate->setLogWidget(value);

}
