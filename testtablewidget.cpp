#include "testtablewidget.h"
#include "models/sprspectrumlistitemsmodel.h"
#include <QFile>
#include <QMessageBox>
#include <QList>

testTableWidget::testTableWidget(QWidget *parent) :
    QWidget(parent), mainModel(nullptr), separateModel(nullptr), separate_error(SPR_SEPARATE_STATE_OK)
{
    ui.setupUi(this);

    towidget = new TCPTimeOutWigget();

//    connect(ui.bAdd, SIGNAL(clicked(bool)), this, SLOT(onClickAdd(bool)));
//    connect(ui.baseTable, SIGNAL(rowSelectedChecked(QList<int>,int)), ui.baseGrapthics, SLOT(onChangeSelectedCheckedItems(QList<int>,int)));

//    connect(ui.kspectTable, SIGNAL(rowSelectedChecked(QList<int>,int)), ui.kSpertGraphic, SLOT(onChangeSelectedCheckedItems(QList<int>,int)));

    setSeparate = new TCPCommand(setsepar);
    getSeparate = new TCPCommand(getsepar);

    connect(ui.bSeparatorOn, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    connect(ui.bGetBaseSpectrum, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    connect(ui.bSeparatorOff, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    connect(ui.bSetSepar,  SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));

    connect(ui.bStartSepar, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    connect(ui.bStopSepar, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    
    connect(getSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(setSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

    towidget->setVisible(false);

//    ui.baseGrapthics->getCanvas()->setAxisScale(QwtPlot::Axis::xBottom, 0, 256, 25);
    ui.kSpertGraphic->getCanvas()->setAxisScale(QwtPlot::Axis::xBottom, 0, 256, 25);

//    startSeparate = new TCPTestStartSeparate(nullptr, ui.towidget, ui.logWidget);
//    TCPCommand *getRen = startSeparate->findCommands(getren).last();
//    connect(getRen, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
//    connect(startSeparate, SIGNAL(errorCommand(TCPCommand*)), this, SLOT(onCommandError(TCPCommand*)));


    stopSeparate = new TCPTestStopSeparate(towidget);
    connect(stopSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
}

ISPRModelData *testTableWidget::setModelData(SPRMainModel *_model){
    if(_model){
        mainModel = _model;

        ui.separateDetailsTable->setModelData(mainModel);
        ui.separateDetailsTable->getMyModel()->setVisibleThreads(QList<int>({0,1,2,3}));
        ui.separateDetailsTable->getMyModel()->setScopeData(0);

//        spectrumsBaseModel = new SPRSpectrumListItemsModel(_model->getSpectrumZonesTableModel(), _model->getSettingsFormulaModel(),_model->getSettingsMainModel()->getThreads(), _model->getSettingsMainModel()->getSpectrumFileName());
//        kSpectrumsModel = new SPRSpectrumListItemsModel(_model->getSpectrumZonesTableModel(), _model->getSettingsFormulaModel(),_model->getSettingsMainModel()->getThreads(), _model->getSettingsMainModel()->getSpectrumFileName(), _model->getSettingsControlModel()->controlArea);
        kSpectrumsModel = new SPRSpectrumListItemsModel(_model->getSpectrumZonesTableModel(), _model->getSettingsFormulaModel(),_model->getSettingsMainModel()->getThreads(), nullptr, _model->getSettingsControlModel()->controlArea);
        connect(kSpectrumsModel, SIGNAL(modelChanget(IModelVariable*)), ui.kSpertGraphic, SLOT(onModelChanged(IModelVariable*)));
//        separateModel = new SPRSeparateModel(mainModel->getDoc());
//        separateModel->setModelData(_model);
        separateModel = mainModel->getSeparateModel();

        startSeparate = new TCPStartSeparate2(mainModel, towidget, getLogWidget());
//        startSeparate = new TCPTestStartSeparate(nullptr, mainModel, towidget, getLogWidget());
//        getRen = startSeparate->findCommands(getren).last();
//        connect(getRen, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
        connect(startSeparate, SIGNAL(errorCommand(TCPCommand*)), this, SLOT(onCommandError(TCPCommand*)));

        stopSeparate->setModelData(mainModel);

        ui.kspectTable->setModelData(kSpectrumsModel, spectrumsOnly);
        ui.kSpertGraphic->setModelData(kSpectrumsModel, spectrumsOnly, true);

        ui.separateDetailsTable->setModelData(mainModel);
//        ui.baseGrapthics->setModelData(kSpectrumsModel, spectrumBase, true);
        ui.testGistogramm->setModelData(mainModel);




        rentgenOnFull = new TCPCommandSeparatorOnFull(mainModel->getServer(), mainModel, towidget);
        rentgenOnFull->setModelData(mainModel);
        connect(rentgenOnFull, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

//        getSpectrumsSetCommand = new TCPCommandGetSpectrums(mainModel->getServer(), ui.towidget, mainModel, 5);
//        connect(getSpectrumsSetCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
//        connect(getSpectrumsSetCommand, SIGNAL(rentgenNotReady(TCPCommand*)), this, SLOT(onCommandError(TCPCommand*)));
        QList<uint8_t> lth;
        for(uint8_t th=0; th<mainModel->getThreads()->getData(); th++) lth << th;


        getGistogramm = new TCPGetSpectrumsGistogramms(mainModel->getServer(), getgist, mainModel, 5, lth, towidget, getLogWidget());
        getKSpectrums = new TCPGetSpectrumsGistogramms(mainModel->getServer(), getkspk, mainModel, 5, lth, towidget, getLogWidget());


        connect(getGistogramm, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
        connect(getKSpectrums, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

//        startSeparate->setModeDatal(separateModel);
//        connect(startSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
//        connect(startSeparate, SIGNAL(errorCommand(TCPCommand*)), this, SLOT(onCommandError(TCPCommand*)));

          getBaseSpectrumCommand = new TCPGetSpectrumsGistogramms(mainModel->getServer(), getspk, mainModel, 5, lth, towidget, getLogWidget());
          getBaseSpectrumCommand->setThreadTimer(5, lth);

//        getBaseSpectrumCommand = startSeparate->getGetBaseSpectrumCommand();
        connect(getBaseSpectrumCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

//        kspecCommand = startSeparate->getSeparateGoCommand()->getKspectCommand();
//        connect(kspecCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

//        hiskCommand = startSeparate->getSeparateGoCommand()->getHistCommand();
//        connect(hiskCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

//        getseparCommand = startSeparate->getSeparateGoCommand()->getGetseparCommand();
//        connect(getseparCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

//        QVector<TCPCommand*> vcomm = startSeparate->findCommands(setGetSpectrumsGistorfamms);
//        for(int i=0; i<vcomm.size();i++){
//            connect(((TCPGetSpectrumsGistogramms*)vcomm[i]), SIGNAL(commandComplite(TCPCommand*)), SLOT(onCommandComplite(TCPCommand*)));
//        }
          connect(startSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onStartSepareteCommandComplite(TCPCommand*)));
    }

    //        model->setModel(_model);
    widgetsShow();
    return mainModel;
}

void testTableWidget::widgetsShow(){

    while(ui.workSeparTable->rowCount() > 0){
        ui.workSeparTable->removeRow(0);
    }

    if(separateModel){
        while(ui.workSeparTable->rowCount()>0){
            ui.workSeparTable->removeRow(0);
        }
        ui.workSeparTable->setRowCount(separateModel->workSeparateRows.size());
        qDebug() << "ws_model rows:" << separateModel->workSeparateRows.size() << " workspase table rows:" << ui.workSeparTable->rowCount();

        uint row = 0;
        for(int i=0; i<separateModel->workSeparateRows.size(); i++){

                QVector<double> data = {
                    separateModel->workSeparateRows[i]->i_prd[0],separateModel->workSeparateRows[i]->i_prd[1],separateModel->workSeparateRows[i]->i_prd[2],separateModel->workSeparateRows[i]->i_prd[3],
                    separateModel->workSeparateRows[i]->p_prd[0],separateModel->workSeparateRows[i]->p_prd[1],separateModel->workSeparateRows[i]->p_prd[2],separateModel->workSeparateRows[i]->p_prd[3],

                    separateModel->workSeparateRows[i]->p_tk,
                    separateModel->workSeparateRows[i]->p_tkh1,
                    separateModel->workSeparateRows[i]->p_tkh2,
                    separateModel->workSeparateRows[i]->p_tkh3,

                    separateModel->workSeparateRows[i]->wcount,
                    separateModel->workSeparateRows[i]->s_rst[0],separateModel->workSeparateRows[i]->s_rst[1],separateModel->workSeparateRows[i]->s_rst[2],separateModel->workSeparateRows[i]->s_rst[3],separateModel->workSeparateRows[i]->s_rst[4],
                };
//                data.push_back(separateModel->workSeparateCurrent[i].error);

                QLabel *lb = new QLabel(QString::number(separateModel->workSeparateRows[i]->number));
                ui.workSeparTable->setCellWidget(i, 0, lb);

                lb = new QLabel(separateModel->workSeparateRows[i]->dt.toString("dd.MM hh:mm:ss"));
                ui.workSeparTable->setCellWidget(i, 1, lb);

                lb = new QLabel(QString::number(separateModel->workSeparateRows[i]->thread));
                ui.workSeparTable->setCellWidget(i, 2, lb);


                for(int col=3, vec = 0; col<ui.workSeparTable->columnCount()-1 && vec < data.size(); col++, vec++){
                    lb = new QLabel(QString::number(data[vec],'f', 3));
                        ui.workSeparTable->setCellWidget(i, col, lb);
                }
                lb = new QLabel(QString::number(round(separateModel->workSeparateCurrent.error)));
        }
        while (ui.workGistorrammTable->rowCount() > 0) {
            ui.workGistorrammTable->removeRow(0);
        }
        ui.workGistorrammTable->setRowCount(separateModel->workGistogrammRows.size());
        for(int row=0; row < separateModel->workGistogrammRows.size(); row++){
            ui.workGistorrammTable->setCellWidget(row, 0, new QLabel(
                                        QString::number(row)));
            ui.workGistorrammTable->setCellWidget(row, 1, new QLabel(
                                        QString::number(separateModel->workGistogrammRows[row].thread)));
            ui.workGistorrammTable->setCellWidget(row, 2, new QLabel(
                                        QString(separateModel->workGistogrammRows[row].dt.toString("dd.MM hh:mm:ss"))));
            ui.workGistorrammTable->setCellWidget(row, 3, new QLabel(
                                        QString(separateModel->workGistogrammRows[row].toString())));
        }

    }
    ui.workGistorrammTable->resizeColumnsToContents();
    ui.workSeparTable->resizeColumnsToContents();

    ui.kspectTable->widgetsShow();
    ui.kSpertGraphic->widgetsShow();
//    ui.baseTable->widgetsShow();
    ui.testGistogramm->widgetsShow();
//    ui.baseGrapthics->widgetsShow();
}

void testTableWidget::onModelChanged(IModelVariable*){
//    ui.baseGrapthics->setModelData(mainModel->getSpectrumListItemsModel(), spectrumBase, true);
//    ui.baseTable->setModelData(mainModel->getSpectrumListItemsModel(), spectrumBase);
//    ui.baseGrapthics->widgetsShow();
//    ui.baseTable->widgetsShow();
    
}

void testTableWidget::onStartSepareteCommandComplite(TCPCommand *command){
    if(sender() == startSeparate){
        if(command->getCommand() == getsepar){
            QByteArray res = command->getReplayData();
            int s = res.size();
            if(s > 1){
                separateModel->setWorkSeparateData(res.right(s-1));
                widgetsShow();
            }
        } else {
            EnumCommands ccc = command->getCommand();
            bool res = command->isCommandSet();
            QString n = command->getName();
            if(command->isCommandSet() && command->getName() == "setGetSpectrumsGistorfamms"){
                TCPGetSpectrumsGistogramms *comm = (TCPGetSpectrumsGistogramms*)command;
                if(comm->getDataType() == getkspk){
                    blockSignals(true);
                    kSpectrumsModel->clearSpectrums();
                    onKSpectrumReady(comm);
                    ui.kSpertGraphic->setVisibleAll();
                    widgetsShow();
                    blockSignals(false);

                }
                if(comm->getDataType() == getspk){
                    QVector<TCPCommand*> vspk = comm->findCommands(getspk);
                    for(int i=0; i<vspk.size();i++){
                        QByteArray spk = vspk[i]->getReplayData().right(DEF_SPECTRUM_DATA_LENGTH_BYTE);
                        SPRSpectrumItemModel *item = kSpectrumsModel->setSpectrumData(i, spk);
                        item->setTimeScope(1);
                    }
//                    ui.baseGrapthics->setVisibleAll();
                    widgetsShow();
                }
                if(comm->getDataType() == getgist){
                    QVector<TCPCommand*> vgist = comm->findCommands(getgist);
                    for(int i=0; i<vgist.size();i++){
                        QByteArray gist = vgist[i]->getReplayData();
                        int s = gist.size();
                        if(s > 1){
                            separateModel->setWorkGistogrammData(gist.right(s-1), i);
                        }
                    }
                }
            }
        }
    }
}


void testTableWidget::onGetButtomsClick(bool)
{
    if(mainModel){
        if(sender() == ui.bSeparatorOn){
//            getGistogramm->send(mainModel->getServer());

            rentgenOnFull->send(mainModel->getServer());
            return;
        }
        if(sender() == ui.bGetBaseSpectrum){
            getBaseSpectrumCommand->send(mainModel->getServer());
            return;
        }
//        if(sender() == ui.bSeparatorOff){
//            getSeparate->send(mainModel->getServer());
//            return;
//        }
        if(sender() == ui.bSetSepar){
            setSeparate->addSendData(separateModel->toByteArray(mainModel, &separate_error));
            setSeparate->send(mainModel->getServer());
        }
        if(sender() == ui.bStartSepar){
            startSeparate->send(mainModel->getServer());
        }
        if(sender() == ui.bStopSepar){
            stopSeparate->send(mainModel->getServer());
        }
    }
}

void testTableWidget::onKSpectrumReady(TCPGetSpectrumsGistogramms *_command){
    blockSignals(true);
    QVector<TCPCommand*> vkspect = _command->findCommands(getkspk);
    for(int th=0; th<vkspect.size();th++){
        QByteArray res = _command->getKSpectrumData(th);
        uint8_t *spec = (uint8_t*)(res.left(DEF_SPECTRUM_DATA_LENGTH_BYTE).constData());

        SPRSpectrumItemModel *item = kSpectrumsModel->addSpectrum(spec, DEF_SPECTRUM_DATA_LENGTH_BYTE,_command->getKSpectrumTime(th), th, QString("kspect %1"));
        uint32_t t = _command->getKSpectrumTime(th);
        item->setTimeScope(t);
        if(kSpectrumsModel){
            SPRSpectrumItemModel *bItem = kSpectrumsModel->getSpectrumBaseItem((th % kSpectrumsModel->getSpectrumsModelBase()->size()));
            if(bItem){
                double correl = item->getCorrel(bItem);
                qDebug() << "Correl :" << correl /*<< std::endl*/;
            }
        }
    }
    blockSignals(false);

}


void testTableWidget::onCommandComplite(TCPCommand *_command)
{

    if(sender() == getGistogramm){
        return;
    }
    if(sender() == getSeparate){
        QByteArray res = getSeparate->getReplayData();
        separateModel->setWorkSeparateData(res);
//        widgetsShow();
        return;
    }
    if(sender() == getseparCommand){
        QByteArray res = getseparCommand->getReplayData();
        separateModel->setWorkSeparateData(res);
//        widgetsShow();
        return;
    }
    if(sender() == getKSpectrums){
        kSpectrumsModel->clearSpectrums();


        onKSpectrumReady(getKSpectrums);
        ui.kSpertGraphic->setVisibleAll();
        widgetsShow();
        return;
    }
    if(sender() == kspecCommand){

//        TCPGetSpectrumsGistogramms *kspk = startSeparate->findCommands(setGetRentgenParams).first();
        kSpectrumsModel->clearSpectrums();
        onKSpectrumReady((TCPGetSpectrumsGistogramms*)kspecCommand);
        ui.kSpertGraphic->setVisibleAll();
        widgetsShow();
        return;
    }
    if(sender() == setSeparate){
        return;
    }
    if(sender() == getBaseSpectrumCommand){
//        spectrumsBaseModel->clearSpectrums();

//        for(uint th=0;th<MAX_SPR_MAIN_THREADS; th++){
        QVector<TCPCommand*> vspk = getBaseSpectrumCommand->findCommands(getspk);
        for(int i=0; i<vspk.size();i++){
            QByteArray spk = vspk[i]->getReplayData().right(DEF_SPECTRUM_DATA_LENGTH_BYTE);
            SPRSpectrumItemModel *item = kSpectrumsModel->setSpectrumData(i, spk);
            item->setTimeScope(1);
        }
//            ui.baseTable->setModel(spectrumsBaseModel);
//        }

//        ui.baseGrapthics->setVisibleAll();
        widgetsShow();
        return;
    }
    if(sender() == stopSeparate){
        if(getLogWidget())
            getLogWidget()->onLogsCommand(startSeparate, "Сепаратор остановлен...");
        return;
    }
    if(sender() == rentgenOnFull){
        QMessageBox::information(nullptr, QString(tr("Рентген включен...")), QString(tr("Рентген включен...")), QMessageBox::Button::Ok);
    }
    //    if(sender() == getSpectrumsSetCommand){
    //        TCPCommand *ren = _command->findCommands(getren).last();
    //        QByteArray res = ren->getReplayData();
    //        uint16_t mka, v;
    //        memcpy(&v, res.constData()+1, sizeof(v));
    //        memcpy(&mka, res.constData()+3, sizeof(mka));

    //        if(getLogWidget()){
    //            getLogWidget()->onLogsCommand(_command, "return: v="+QString::number(v, 16)+" mka="+QString::number(mka, 16));
    //        }
    //        QVector<TCPCommand*> vspk = getSpectrumsSetCommand->findCommands(getspk);
    //        for(int i=0; i<vspk.size();i++){
    //            QByteArray spk = vspk[i]->getReplayData().right(DEF_SPECTRUM_DATA_LENGTH_BYTE);
    //            SPRSpectrumItemModel *item = kSpectrumsModel->setSpectrumData(i, spk);
    //            item->setTimeScope(5);
    //            widgetsShow();
    //        }
    //    }
//    if(_command){
//        if(_command->getCommand() == getren){
//            uint16_t kV, mka;
//            QByteArray res = _command->getReplayData();
//            memcpy(&kV, res.constData()+1, sizeof(kV));
//            memcpy(&mka, res.constData()+3, sizeof(mka));
//            QString msg = QString(tr("Рентген вышел на рабочий режим kV=0x%1, mka=0x%2 > 0х600")).
//                    arg(QString::number(kV, 16)).arg(QString::number(mka, 16));
//            if(getLogWidget())
//                getLogWidget()->onErrorLogsCommand(nullptr, msg);
//            return;
//        }
//    }
}

void testTableWidget::onCommandError(TCPCommand *_command)
{
    if(sender() == startSeparate || sender() == getBaseSpectrumCommand){
        uint16_t kV, mka;
        QByteArray res = _command->getReplayData();
        memcpy(&kV, res.constData()+1, sizeof(kV));
        memcpy(&mka, res.constData()+3, sizeof(mka));
        QString msg = QString(tr("Ошибка...\nРентген не вышел на рабочий режим kV=0x%1, mka=0x%2 < 0х600")).
                arg(QString::number(kV, 16)).arg(QString::number(mka, 16));
        if(getLogWidget())
            getLogWidget()->onErrorLogsCommand(nullptr, msg);
        return;
    }
}
//void testTableWidget::addSpectrumsModel(QFile *inp)
//{
//    if(inp){
//        bool res = true;
//        if(!inp->isOpen()){
//            res = inp->open(QIODevice::ReadOnly);
//        }
//        if(res){
//            char begin[2];
//            uint8_t *buf = (uint8_t*)malloc(DEF_SPECTRUM_DATA_BUF_LENGTH);
//            inp->read(begin, 2);
//            while(inp->read((char*)buf, DEF_SPECTRUM_DATA_BUF_LENGTH)){
//                mainModel->getSpectrumListItemsModel()->addSpectrum(buf, DEF_SPECTRUM_DATA_BUF_LENGTH);
//            }
//            free(buf);
//        }
//    }
//}

//void testTableWidget::onClickAdd(bool)
//{
//    QString fName("F:\\Projects\\Separator\\Real spectors\\СРФ3.spc");
//    QFile in(fName);
//    if(in.open(QIODevice::ReadOnly)){
//        addSpectrumsModel(&in);
//        ui.baseTable->widgetsShow();
//        ui.baseGrapthics->widgetsShow();
//    }
//}


ISPRModelData *testTableWidget::getModelData()
{
}

void testTableWidget::setLogWidget(TCPLogsWigtets *value)
{
    ISPRWidget::setLogWidget(value);

    getBaseSpectrumCommand->setLogWidget(value);
    getGistogramm->setLogWidget(value);
    getKSpectrums->setLogWidget(value);

    startSeparate->setLogWidget(value);
}

void testTableWidget::onModelChanget(IModelVariable *)
{
}
