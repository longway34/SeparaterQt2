#include "testtablewidget.h"
#include "models/sprspectrumlistitemsmodel.h"
#include <QFile>

testTableWidget::testTableWidget(QWidget *parent) :
    QWidget(parent), model(nullptr)
{
    ui.setupUi(this);

//    connect(ui.bAdd, SIGNAL(clicked(bool)), this, SLOT(onClickAdd(bool)));
    connect(ui.table, SIGNAL(rowSelectedChecked(QList<int>,int)), ui.graphic, SLOT(onChangeSelectedCheckedItems(QList<int>,int)));
    connect(ui.table, SIGNAL(modelChanged()), this, SLOT(onModelChanged()));

    connect(ui.bGetGist, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    connect(ui.bGetKSPK, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    connect(ui.bGETSepar, SIGNAL(clicked(bool)), this, SLOT(onGetButtomsClick(bool)));
    
//    connect(getGistogramm, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
//    connect(getKSpectrums, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
//    connect(getSeparate, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
}

void testTableWidget::onModelChanged(){
    ui.graphic->setModel(model);
    ui.table->setModel(model);
    ui.graphic->widgetsShow();
    ui.table->widgetsShow();
    
}

void testTableWidget::onGetButtomsClick(bool)
{
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
    
}

void testTableWidget::onCommandComplite(TCPCommand *_command)
{
    if(sender() == getGistogramm){
        return;
    }
    if(sender() == getSeparate){
        return;
    }
    if(sender() == getKSpectrums){
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
                model->addSpectrum(buf, DEF_SPECTRUM_DATA_BUF_LENGTH);
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
