#include "testtablewidget.h"
#include "models/sprspectrumlistitemsmodel.h"
#include <QFile>

testTableWidget::testTableWidget(QWidget *parent) :
    QWidget(parent), mainModel(nullptr), separateModel(nullptr)
{
    ui.setupUi(this);

//    connect(ui.bAdd, SIGNAL(clicked(bool)), this, SLOT(onClickAdd(bool)));
    connect(ui.table, SIGNAL(rowSelectedChecked(QList<int>,int)), ui.graphic, SLOT(onChangeSelectedCheckedItems(QList<int>,int)));
    connect(ui.table, SIGNAL(modelChanged()), this, SLOT(onModelChanged()));

    setSeparate = new TCPCommand(setsepar);
    getSeparate = new TCPCommand(getsepar);

    connect(ui.bGetGist, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    connect(ui.bGetKSPK, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    connect(ui.bGETSepar, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    connect(ui.bSetSepar,  SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    
    connect(getSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    connect(setSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
}

ISPRModelData *testTableWidget::setModel(SPRMainModel *_model){
    if(_model){
        mainModel = _model;
        spectrumModel = new SPRSpectrumListItemsModel(_model->getSpectrumZonesTableModel(), _model->getSettingsFormulaModel());
        separateModel = new SPRSeparateModel(mainModel->getDoc());
        separateModel->setMainModel(_model);

        ui.kspectTable->setModel(spectrumModel);

        ui.table->setModel(spectrumModel);
        ui.graphic->setModel(spectrumModel);

        getGistogramm = new TCPGetSpectrumsGistogramms(mainModel->getServer(), getgist);
        getKSpectrums = new TCPGetSpectrumsGistogramms(mainModel->getServer(), getkspk);

        connect(getGistogramm, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
        connect(getKSpectrums, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
    }

    //        model->setModel(_model);
    return mainModel;
}

void testTableWidget::widgetsShow(){
    ui.table->widgetsShow();


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
}

void testTableWidget::onModelChanged(){
    ui.graphic->setModel(mainModel->getSpectrumListItemsModel());
    ui.table->setModel(mainModel->getSpectrumListItemsModel());
    ui.graphic->widgetsShow();
    ui.table->widgetsShow();
    
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
        for(uint th=0; th<MAX_SPR_MAIN_THREADS; th++){
            QByteArray res = getKSpectrums->getKSpectrumData(th);
            uint8_t *spec = (uint8_t*)(res.left(DEF_SPECTRUM_DATA_LENGTH).constData());
            ui.kspectTable->addSpectrum(spec, DEF_SPECTRUM_DATA_LENGTH, th);
        }
        widgetsShow();
        return;
    }
    if(sender() == setSeparate){
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
        ui.table->widgetsShow();
        ui.graphic->widgetsShow();
    }
}
