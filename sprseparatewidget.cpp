#include "sprseparatewidget.h"

SPRSeparateWidget::SPRSeparateWidget(QWidget *parent) :
    QWidget(parent), history(nullptr), wPorogs(nullptr), kSpectrumModel(nullptr)/*, startSeparate(nullptr), stopSeparate(nullptr), separatorOn(nullptr)*/
{
    ui.setupUi(this);

    toWidget = new TCPTimeOutWigget();
    toWidget->setVisible(false);

//    stopSeparate = new TCPTestStopSeparate(toWidget);
//    connect(stopSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

    connect(ui.bChangePorogs, SIGNAL(clicked(bool)), this, SLOT(onChangePororsCommand(bool)));

//    connect(ui.bSeparateStart, SIGNAL(clicked(bool)), this, SLOT(onCommandButtomClick(bool)));
//    connect(ui.bSeparateStop, SIGNAL(clicked(bool)), this, SLOT(onCommandButtomClick(bool)));
//    connect(ui.bSeparatorOn, SIGNAL(clicked(bool)), this, SLOT(onCommandButtomClick(bool)));
//    connect(ui.bSeparatorOff, SIGNAL(clicked(bool)), this, SLOT(onCommandButtomClick(bool)));

    ui.gKSpectrums->getCanvas()->setAxisScale(QwtPlot::Axis::xBottom, 0, 256, 25);
//    ui.gKSpectrums->getCanvas()->setAxisMaxMinor(QwtPlot::xBottom, 10);
    connect(ui.gKSpectrums->getPorogsMoved(), SIGNAL(dblClickMouseEvent()), this, SLOT(onDblClickMouseEvent()));
    ui.gKSpectrums->setWithLegend(true);

    ui.spLeftRigth->setStretchFactor(0,1);
    ui.spLeftRigth->setStretchFactor(1,1);
    ui.spLeftRigth->setSizes(QList<int>() << 600 << 500);

    ui.spGraphics->setStretchFactor(0,1);
    ui.spGraphics->setStretchFactor(1,1);
    ui.spGraphics->setSizes(QList<int>() << 6500 << 3500);

    ui.spMaintable->setStretchFactor(1,0);
    ui.spMaintable->setStretchFactor(1,1);
    ui.spMaintable->setSizes(QList<int>() << 6500 << 3500);

//    ui.spGraphic->setSizes(QList<int>()<<50<<20);

}

void SPRSeparateWidget::setGraphicTitle(){
    QString titlePref = QString(tr("Спектры (%1)"));
    int thr = ui.gKSpectrums->getCurrentThread();
    QString titleSuff = thr < 0 ? QString(tr("Все ручьи")) : QString(tr("Ручей %1")).arg(thr+1);
    QString title = titlePref.arg(titleSuff);

    ui.gKSpectrums->getCanvas()->setTitle(title);
}

void SPRSeparateWidget::onDblClickMouseEvent(){
    setGraphicTitle();
    int thr = ui.gKSpectrums->getCurrentThread();
    SPRGraphItem *gr = nullptr;
    if(thr >= 0){
        if(kSpectrumModel){
            QList<SPRSpectrumItemModel*> lst = kSpectrumModel->getSpectrumsItemByThread(thr, spectrumsOnly);
            if(lst.size() > 0){
                 gr = lst.last()->getGraphic();
            }
        }
    }
    ui.gKSpectrums->setCurrentItem(gr);

}

extern TCPCommand *restartsep;

void SPRSeparateWidget::onCommandButtomClick(bool){
    if(model){
    }

}

void SPRSeparateWidget::onServerStateChange(uint32_t){
    if(model){
        if(model->getServer()->isState(spr_state_separated))
        if(history){
            history->clear();
        }
    }
}

void SPRSeparateWidget::widgetsShow()
{
}

ISPRModelData *SPRSeparateWidget::setModelData(ISPRModelData *data)
{
    model = data->getMainModel();
    if(model){

        connect(model->getServer(), SIGNAL(serverStateChange(uint32_t)), this, SLOT(onServerStateChange(uint32_t)));

        ui.tSeparateDetail->setModelData(model);
        ui.tSeparateDetail->widgetsShow();

        ui.gGistogramm->setModelData(model);
        ui.gGistogramm->widgetsShow();

//        if(kSpectrumModel) delete kSpectrumModel;
//        kSpectrumModel = new SPRSpectrumListItemsModel(model, model->getSpectrumListItemsModel()->getSpectrumsModel(spectrumBase) ,true, nullptr);

        kSpectrumModel = model->getKSpectrums();

        ui.gKSpectrums->setModelData(kSpectrumModel, spectrumsOnly, false, true);
//        ui.gKSpectrums->setVisibleAll();
        ui.gKSpectrums->setAllCurrent(true);

        if(wPorogs) delete wPorogs;
        wPorogs = new SPRSettingsPorogsWidget();
        wPorogs->setModelData(model->getSettingsPorogsModel());


//        if(startSeparate) delete startSeparate;
//        startSeparate = new TCPStartSeparate2(model, toWidget, getLogWidget());

        ui.wControlPitatel->setModelData(model);
        ui.wSeparateControl->setModelData(model, toWidget, getLogWidget());
//        forKSpertCommands = startSeparate->getKSpectrunsCommand();
//        forHistCommands = startSeparate->getHistogrammCommand();


//        ui.wSeparateControl->setModelData(model, toWidget, getLogWidget());

//        connect(startSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onSeparateCommandComplite(TCPCommand*)));


//        if(separatorOn) delete separatorOn;
//        separatorOn = new TCPCommandSeparatorOnFull(model->getServer(), model, toWidget);
//        connect(separatorOn, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));


        history = new SPRHistoryModel(model, kSpectrumModel);
        ui.gHistoryIn->init({}, history, countInputInTime, tr("Подача руды ( %1 )"), /*60*60*2*/ 600);
        ui.gHistoryOut->init({}, history, percentConcentrate2Input, tr("Выход продукта,% ( %1 )"), /*60*60*2*/ 600);
        ui.gHistoryOut->setAxisScale(QwtPlot::xBottom, 0, 100, 20);

        SPRThreadList trs = model->getThreadsList();

        ui.gMainInput->setModelData(model, history, QString(tr("Рекомендуемый рудопоток (кусков/сек.)")), QColor(Qt::lightGray), trs, countInputInSec, false);
        ui.gMainOutput->setModelData(model, history, QString(tr("Выход продуктов сортировки")), QColor(Qt::lightGray), trs, percentConcentrate2Input, true);

        ui.tMainInfo->init(model, history, kSpectrumModel);

        ui.gGrandsostav->setModels(model, history, model->getSeparateModel());

        onServerStateChange(model->getServer()->getState());
    }
}

void SPRSeparateWidget::onCommandComplite(TCPCommand *command){

}

void SPRSeparateWidget::onSpectrumReady(TCPGetSpectrumsGistogramms *_command){
//    if(kSpectrumModel){
//        blockSignals(true);
//        QVector<TCPCommand*> vkspect = _command->findCommands(getkspk);
//        for(int th=0; th<vkspect.size();th++){
//            QByteArray res = _command->getKSpectrumData(th);
//            uint8_t *spec = (uint8_t*)(res.left(DEF_SPECTRUM_DATA_LENGTH_BYTE).constData());

//            SPRSpectrumItemModel *item = kSpectrumModel->addSpectrum(spec, DEF_SPECTRUM_DATA_LENGTH_BYTE, _command->getKSpectrumTime(th));
//            uint32_t t = _command->getKSpectrumTime(th);
//            item->setTimeScope(t);

//            SPRSpectrumItemModel *bItem = kSpectrumModel->getSpectrumBaseItem((th % kSpectrumModel->getSpectrumsModelBase()->size()));
//            if(bItem){
//                double correl = item->getCorrel(bItem);
//                qDebug() << "Correl :" << correl /*<< std::endl*/;
//            }
//        }
//        blockSignals(false);
//    }
}

void SPRSeparateWidget::onSeparateCommandComplite(TCPCommand* command){
    if(model){
//        if(sender() == startSeparate){
//            if(command->getCommand() == getsepar){
//                QByteArray res = command->getReplayData();
//                int s = res.size();
//                if(s > 1){
//                    model->getSeparateModel()->setWorkSeparateData(res.right(s-1));
//                    model->getSeparateOutputModel()->addWorkSeparateData(res.right(s-1));
////                    widgetsShow();
//                }
//                return;
//            }
//            if(command == startSeparate->getHistogrammCommand()){
//                QVector<TCPCommand*> vgist = command->findCommands(getgist);
//                for(int i=0; i<vgist.size();i++){
//                    QByteArray gist = vgist[i]->getReplayData();
//                    int s = gist.size();
//                    if(s > 1){
//                        model->getSeparateModel()->setWorkGistogrammData(gist.right(s-1), i);
//                    }
//                }
//                return;
//            }
//            if(command == startSeparate->getKSpectrunsCommand()){
//                QVector<TCPCommand*> vspk = command->findCommands(getkspk);
//                for(int i=0; i<vspk.size(); i++){
//                    QByteArray spk = vspk[i]->getReplayData().right(DEF_SPECTRUM_DATA_LENGTH_BYTE);
//                    uint64_t ms = abs(lastKSpect.msecsTo(QDateTime::currentDateTime()));

//                    SPRSpectrumItemModel *item = kSpectrumModel->setSpectrumData(i, spk, spectrumsOnly, ms);
//                    lastKSpect = QDateTime::currentDateTime();

//                    QList<SPRSpectrumItemModel *> lbase = kSpectrumModel->getSpectrumsItemByThread(i);
//                    if(lbase.size() > 0){
//                        EnumElements area = model->getSettingsControlModel()->controlArea->getData();
//                        double corr = item->getCorrel(lbase.first(), true, area);
//                        double critical = model->getSettingsControlModel()->correlSpectrumCritical->getData();
//                        if(corr < critical){
//                            if(getLogWidget()){
//                                getLogWidget()->onErrorLogsCommand(command, QString(tr("Корреляция на ручье %1(%2) ниже критического %3 ")).
//                                                                   arg(i).
//                                                                   arg(QString::number(corr,'f',2)).arg(QString::number(critical, 'f', 2)));
//                            }
//                        }
//                    }
//                }
//                return;
//            }
//            if(command == startSeparate->getGetBaseSpectrum()){
//                return;
//            }
//        }
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
    ui.wSeparateControl->setLogWidget(value);
//    startSeparate->setLogWidget(value);

}
