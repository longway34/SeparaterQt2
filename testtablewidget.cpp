#include "testtablewidget.h"
#include "models/sprspectrumlistitemsmodel.h"
#include <QFile>

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
    
    connect(getSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(setSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

    towidget = new TCPTimeOutWigget(this);

    ui.baseGrapthics->getCanvas()->setAxisScale(QwtPlot::Axis::xBottom, 0, 256, 25);
    ui.kSpertGraphic->getCanvas()->setAxisScale(QwtPlot::Axis::xBottom, 0, 256, 25);

    startSeparate = new TCPTestStartSeparate(nullptr, towidget);
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
    }

    //        model->setModel(_model);
    return mainModel;
}

void testTableWidget::widgetsShow(){

    while(ui.workSeparTable->rowCount() > 0){
        ui.workSeparTable->removeRow(0);
    }

    if(separateModel){
        ui.workSeparTable->setRowCount(separateModel->workSeparate.size() * MAX_SPR_MAIN_THREADS);
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
    if(sender() == getKSpectrums){
        kSpectrumsModel->clearSpectrums();
        for(uint th=0; th<MAX_SPR_MAIN_THREADS; th++){
            QByteArray res = getKSpectrums->getKSpectrumData(th);
            uint8_t *spec = (uint8_t*)(res.left(DEF_SPECTRUM_DATA_LENGTH).constData());

            SPRSpectrumItemModel *item = kSpectrumsModel->addSpectrum(spec, DEF_SPECTRUM_DATA_LENGTH);
            uint32_t t = getKSpectrums->getKSpectrumTime(th);
            item->setTimeScope(t);

//            SPRSpectrumItemModel *mod = ui.kspectTable->addSpectrum(spec, DEF_SPECTRUM_DATA_LENGTH);
//            ui.kspectTable->addSpectrum(spec, DEF_SPECTRUM_DATA_LENGTH, th);

//            ui.kSpertGraphic->setModel(ui.kspectTable->getModels());
        }
        ui.kSpertGraphic->setVisibleAll();
        widgetsShow();
        return;
    }
    if(sender() == setSeparate){
        return;
    }
    if(sender() == startSeparate){
        spectrumsBaseModel->clearSpectrums();
//        for(uint th=0;th<MAX_SPR_MAIN_THREADS; th++){
        QVector<TCPCommand*> vspk = startSeparate->findCommands(getspk);
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
