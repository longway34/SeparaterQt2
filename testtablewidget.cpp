#include "testtablewidget.h"
#include "models/sprspectrumlistitemsmodel.h"
#include <QFile>
#include <QMessageBox>


testTableWidget::testTableWidget(QWidget *parent) :
    QWidget(parent), mainModel(nullptr), separateModel(nullptr)
{
    ui.setupUi(this);

//    connect(ui.bAdd, SIGNAL(clicked(bool)), this, SLOT(onClickAdd(bool)));
    connect(ui.baseTable, SIGNAL(rowSelectedChecked(QList<int>,int)), ui.baseGrapthics, SLOT(onChangeSelectedCheckedItems(QList<int>,int)));
    connect(ui.baseTable, SIGNAL(modelChanged()), this, SLOT(onModelChanged()));

    setSeparate = new TCPCommand(setsepar);
    getSeparate = new TCPCommand(getsepar);

    connect(ui.bGetGist, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    connect(ui.bGetKSPK, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    connect(ui.bGETSepar, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    connect(ui.bSetSepar,  SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));

    connect(ui.bStartSepar, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    connect(ui.bStopSepar, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    
    connect(getSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(setSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

    ui.towidget->setVisible(false);

    ui.baseGrapthics->getCanvas()->setAxisScale(QwtPlot::Axis::xBottom, 0, 256, 25);
    ui.kSpertGraphic->getCanvas()->setAxisScale(QwtPlot::Axis::xBottom, 0, 256, 25);

    startSeparate = new TCPTestStartSeparate(nullptr, ui.towidget, ui.logWidget);
    TCPCommand *getRen = startSeparate->findCommands(getren).last();
    connect(getRen, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(startSeparate, SIGNAL(errorCommand(TCPCommand*)), this, SLOT(onCommandError(TCPCommand*)));


    stopSeparate = new TCPTestStopSeparate(ui.towidget);
    connect(stopSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
}

ISPRModelData *testTableWidget::setModel(SPRMainModel *_model){
    if(_model){
        mainModel = _model;
        spectrumsBaseModel = new SPRSpectrumListItemsModel(_model->getSpectrumZonesTableModel(), _model->getSettingsFormulaModel());
        kSpectrumsModel = new SPRSpectrumListItemsModel(_model->getSpectrumZonesTableModel(), _model->getSettingsFormulaModel());
        separateModel = new SPRSeparateModel(mainModel->getDoc());
        separateModel->setMainModel(_model);

        ui.kspectTable->setModel(kSpectrumsModel);
        ui.kSpertGraphic->setModel(kSpectrumsModel);

        ui.baseTable->setModel(spectrumsBaseModel);
        ui.baseGrapthics->setModel(spectrumsBaseModel);

        getGistogramm = new TCPGetSpectrumsGistogramms(mainModel->getServer(), getgist);
        getKSpectrums = new TCPGetSpectrumsGistogramms(mainModel->getServer(), getkspk);

        connect(getGistogramm, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
        connect(getKSpectrums, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

        startSeparate->setSeparateModel(separateModel);
        connect(startSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
        connect(startSeparate, SIGNAL(errorCommand(TCPCommand*)), this, SLOT(onCommandError(TCPCommand*)));

        getBaseSpectrumCommand = startSeparate->getGetBaseSpectrumCommand();
        connect(getBaseSpectrumCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

        kspecCommand = startSeparate->getSeparateGoCommand()->getKspectCommand();
        connect(kspecCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

        hiskCommand = startSeparate->getSeparateGoCommand()->getHistCommand();
        connect(hiskCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

        getseparCommand = startSeparate->getSeparateGoCommand()->getGetseparCommand();
        connect(getseparCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

        QVector<TCPCommand*> vcomm = startSeparate->findCommands(setGetSpectrumsGistorfamms);
        for(int i=0; i<vcomm.size();i++){
            connect(((TCPGetSpectrumsGistogramms*)vcomm[i]), SIGNAL(commandComplite(TCPCommand*)), SLOT(onCommandComplite(TCPCommand*)));
        }

    }

    //        model->setModel(_model);
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
        ui.workSeparTable->setRowCount(separateModel->workSeparate.size() * MAX_SPR_MAIN_THREADS);
        qDebug() << "ws_model rows:" << separateModel->workSeparate.size() << " workspase table rows:" << ui.workSeparTable->rowCount();

        uint row = 0;
        for(int i=0; i<separateModel->workSeparate.size(); i++){

            for(uint th=0; th<mainModel->getSettingsMainModel()->getThreads()->getData(); th++){
                QVector<double> data = {
                    separateModel->workSeparate[i]->i_prd[th][0],separateModel->workSeparate[i]->i_prd[th][1],separateModel->workSeparate[i]->i_prd[th][2],separateModel->workSeparate[i]->i_prd[th][3],
                    separateModel->workSeparate[i]->p_prd[th][0],separateModel->workSeparate[i]->p_prd[th][1],separateModel->workSeparate[i]->p_prd[th][2],separateModel->workSeparate[i]->p_prd[th][3],

                    separateModel->workSeparate[i]->p_tk[th],
                    separateModel->workSeparate[i]->p_tkh1[th],
                    separateModel->workSeparate[i]->p_tkh2[th],
                    separateModel->workSeparate[i]->p_tkh3[th],

                    separateModel->workSeparate[i]->wcount[th],
                    separateModel->workSeparate[i]->s_rst[th][0],separateModel->workSeparate[i]->s_rst[th][1],separateModel->workSeparate[i]->s_rst[th][2],separateModel->workSeparate[i]->s_rst[th][3],separateModel->workSeparate[i]->s_rst[th][4],

                };
                data.push_back(separateModel->workSeparate[i]->error);

                for(int col=0; col<ui.workSeparTable->columnCount(); col++){
                    QLabel *lb = new QLabel(QString::number(data[col],'f', 3));
                        ui.workSeparTable->setCellWidget(row, col, lb);
                }
                row++;
            }
        }
    }
    ui.workSeparTable->resizeColumnsToContents();

    ui.kspectTable->widgetsShow();
    ui.kSpertGraphic->widgetsShow();
    ui.baseTable->widgetsShow();
    ui.baseGrapthics->widgetsShow();
}

void testTableWidget::onModelChanged(){
    ui.baseGrapthics->setModel(mainModel->getSpectrumListItemsModel());
    ui.baseTable->setModel(mainModel->getSpectrumListItemsModel());
    ui.baseGrapthics->widgetsShow();
    ui.baseTable->widgetsShow();
    
}

void testTableWidget::onGetButtomsClick(bool)
{
    if(mainModel){
        if(sender() == ui.bGetGist){
            getGistogramm->send(mainModel->getServer());
            return;
        }
        if(sender() == ui.bGetKSPK){
            getKSpectrums->send(mainModel->getServer());
            return;
        }
        if(sender() == ui.bGETSepar){
            getSeparate->send(mainModel->getServer());
            return;
        }
        if(sender() == ui.bSetSepar){
            setSeparate->setSendData(separateModel->toByteArray());
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
    QVector<TCPCommand*> vkspect = _command->findCommands(getkspk);
    for(int th=0; th<vkspect.size();th++){
        QByteArray res = _command->getKSpectrumData(th);
        uint8_t *spec = (uint8_t*)(res.left(DEF_SPECTRUM_DATA_LENGTH).constData());

        SPRSpectrumItemModel *item = kSpectrumsModel->addSpectrum(spec, DEF_SPECTRUM_DATA_LENGTH);
        uint32_t t = _command->getKSpectrumTime(th);
        item->setTimeScope(t);
        if(spectrumsBaseModel){
            SPRSpectrumItemModel *bItem = spectrumsBaseModel->getSpectrumItem((th % spectrumsBaseModel->getSpectrumsModel()->size()));
            if(bItem){
                double correl = item->getCorrel(bItem);
                qDebug() << "Correl :" << correl /*<< std::endl*/;
            }
        }
    }

}


void testTableWidget::onCommandComplite(TCPCommand *_command)
{

    if(sender() == getGistogramm){
        return;
    }
    if(sender() == getSeparate){
        QByteArray res = getSeparate->getReplayData();
        separateModel->addWorkSeparateData(res);
        widgetsShow();
        return;
    }
    if(sender() == getseparCommand){
        QByteArray res = getSeparate->getReplayData();
        separateModel->addWorkSeparateData(res);
        widgetsShow();
        return;
    }
    if(sender() == getKSpectrums){
        kSpectrumsModel->clearSpectrums();
//        for(uint th=0; th<MAX_SPR_MAIN_THREADS; th++){
//            QByteArray res = getKSpectrums->getKSpectrumData(th);
//            uint8_t *spec = (uint8_t*)(res.left(DEF_SPECTRUM_DATA_LENGTH).constData());

//            SPRSpectrumItemModel *item = kSpectrumsModel->addSpectrum(spec, DEF_SPECTRUM_DATA_LENGTH);
//            uint32_t t = getKSpectrums->getKSpectrumTime(th);
//            item->setTimeScope(t);

//            SPRSpectrumItemModel *bItem = spectrumsBaseModel->getSpectrumItem((th) % MAX_SPR_MAIN_THREADS);
//            if(bItem){
//                double correl = item->getCorrel(bItem);
//                qDebug() << "Correl :" << correl /*<< std::endl*/;
//            }

//            SPRSpectrumItemModel *mod = ui.kspectTable->addSpectrum(spec, DEF_SPECTRUM_DATA_LENGTH);
//            ui.kspectTable->addSpectrum(spec, DEF_SPECTRUM_DATA_LENGTH, th);

//            ui.kSpertGraphic->setModel(ui.kspectTable->getModels());
//        }


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
        spectrumsBaseModel->clearSpectrums();

//        for(uint th=0;th<MAX_SPR_MAIN_THREADS; th++){
        QVector<TCPCommand*> vspk = getBaseSpectrumCommand->findCommands(getspk);
        for(int i=0; i<vspk.size();i++){
            QByteArray spk = vspk[i]->getReplayData().right(DEF_SPECTRUM_DATA_LENGTH);
            SPRSpectrumItemModel *item = spectrumsBaseModel->addSpectrum(spk);
            item->setTimeScope(1);
        }
//            ui.baseTable->setModel(spectrumsBaseModel);
//        }

        ui.baseGrapthics->setVisibleAll();
        widgetsShow();
        return;
    }
    if(sender() == stopSeparate){
        ui.logWidget->onLogsCommand(startSeparate, "Сепаратор остановлен...");
        return;
    }
    if(_command){
        if(_command->getCommand() == getren){
            QByteArray res = _command->getReplayData();
            uint16_t mka, v;
            memcpy(&v, res.constData()+1, sizeof(v));
            memcpy(&mka, res.constData()+3, sizeof(mka));

            ui.logWidget->onLogsCommand(_command, "return: v="+QString::number(v, 16)+" mka="+QString::number(mka, 16));
            return;
        }
    }
}

void testTableWidget::onCommandError(TCPCommand *_command)
{
    if(sender() == startSeparate){
        uint16_t kV, mka;
        QByteArray res = _command->getReplayData();
        memcpy(&kV, res.constData()+1, sizeof(kV));
        memcpy(&mka, res.constData()+3, sizeof(mka));
        QString msg = QString(tr("Ошибка...\nРентген не вышел на рабочий режим kV=0x%1, mka=0x%2 < 0х600")).
                arg(QString::number(kV, 16)).arg(QString::number(mka, 16));
        ui.logWidget->onErrorLogsCommand(msg);
        return;
    }
}
void testTableWidget::addSpectrumsModel(QFile *inp)
{
    if(inp){
        bool res = true;
        if(!inp->isOpen()){
            res = inp->open(QIODevice::ReadOnly);
        }
        if(res){
            char begin[2];
            uint8_t *buf = (uint8_t*)malloc(DEF_SPECTRUM_DATA_BUF_LENGTH);
            inp->read(begin, 2);
            while(inp->read((char*)buf, DEF_SPECTRUM_DATA_BUF_LENGTH)){
                mainModel->getSpectrumListItemsModel()->addSpectrum(buf, DEF_SPECTRUM_DATA_BUF_LENGTH);
            }
            free(buf);
        }
    }
}

void testTableWidget::onClickAdd(bool)
{
    QString fName("F:\\Projects\\Separator\\Real spectors\\СРФ3.spc");
    QFile in(fName);
    if(in.open(QIODevice::ReadOnly)){
        addSpectrumsModel(&in);
        ui.baseTable->widgetsShow();
        ui.baseGrapthics->widgetsShow();
    }
}
