#include "sprsettingsmainwidget.h"
#include "ipvalidator.h"

#include <QFileDialog>

ISPRModelData *SPRSettingsMainWidget::setModel(ISPRModelData *data)
{
    model = (SPRSettingsMainModel*)data;

    if(model){
        ui.leName->setText(model->name->getData());
        ui.leAddress->setText(model->ipAddress->getData());
        ui.lePort->setText(model->ipPort->toString());
        ui.leSpectrumsFName->setText(model->spectrumFileName->getData());

        const QString strs[] = {
            tr("1 ручей"), tr("2 ручья"), tr("3 ручья"), tr("4 ручья"),
            tr("5 ручьев"), tr("6 ручьев"), tr("7 ручьев"), tr("8 ручьев")
        };

        ui.cbThreads->blockSignals(true);
        ui.cbThreads->clear();
        for(int i=0, r=1; i<MAX_SPR_MAIN_THREADS; i++, r++){
            ui.cbThreads->addItem(strs[i], r);
        }
        int d = model->threads->getData();
        int cind = ui.cbThreads->findData(QVariant(model->threads->getData()));
        ui.cbThreads->setCurrentIndex(cind);
        ui.cbThreads->blockSignals(false);

        ui.cbRentgens->blockSignals(true);
        ui.cbRentgens->clear();
        const QString rstr[] = {tr(" рентген"), tr(" рентгена")};
        int maxRen = model->threads->getData() > 3 ? 2 : 1;
        for(int i=0, r=1; i<maxRen; i++, r++){
            ui.cbRentgens->addItem(QString::number(r) + rstr[i], r);
        }
        int ind = ui.cbRentgens->findData(QVariant(model->rentgens->getData()));
        if(ind < 0){
            ind = 0; model->rentgens->setData(DEF_SPR_MAIN_RENTGENS);
        }
        ui.cbRentgens->setCurrentIndex(ind);
        ui.cbRentgens->blockSignals(false);

        widgetsShow();
    }
    return model;
}

SPRSettingsMainWidget::SPRSettingsMainWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    model = nullptr;

    ui.leAddress->setValidator(new IpValidator());
    ui.lePort->setValidator(new QIntValidator(1024, 65535));

    ui.cbTypePRAM->clear();
    ui.cbTypePRAM->addItem(tr("ПРАМ-50"), pram50);
    ui.cbTypePRAM->addItem(tr("ПРАМ-50МС"), pram50mc);
    ui.cbTypePRAM->addItem(tr("Модуль-50"), module50);

    ui.cbTypeThermo->clear();
    ui.cbTypeThermo->addItem(tr("Нагреватель"), hotModule);
    ui.cbTypeThermo->addItem(tr("Термохолодильник"), coldModule);

    connect(ui.leName, SIGNAL(editingFinished()), SLOT(viewChange()));
    connect(ui.leAddress, SIGNAL(editingFinished()), SLOT(viewChange()));
    connect(ui.lePort, SIGNAL(editingFinished()), SLOT(viewChange()));

    connect(ui.cbTypePRAM, SIGNAL(currentIndexChanged(int)), SLOT(viewChange(int)));
    connect(ui.cbTypeThermo, SIGNAL(currentIndexChanged(int)), SLOT(viewChange(int)));
    connect(ui.cbRentgens,SIGNAL(currentIndexChanged(int)), SLOT(viewChange(int)));
    connect(ui.cbIMCount, SIGNAL(currentIndexChanged(int)), SLOT(viewChange(int)));
    connect(ui.cbThreads, SIGNAL(currentIndexChanged(int)), SLOT(viewChange(int)));

    connect(ui.bFNameSelect, SIGNAL(clicked(bool)), this, SLOT(viewChange(bool)));
    connect(ui.bSpectrumFNameSelect, SIGNAL(clicked(bool)), this, SLOT(viewChange(bool)));
}


void SPRSettingsMainWidget::widgetsShow()
{
    if(model){
//        QString name = model->name->getData();
//        if(!docFileName.isEmpty()){
//            name += QString("(")+docFileName+QString(")");
//        }
//        ui.leName->blockSignals(true);
//        ui.leName->setText(model->name->getData());
//        ui.leName->blockSignals(false);

//        ui.leAddress->blockSignals(true);
//        ui.leAddress->setText(model->ipAddress->getData());
//        ui.leAddress->blockSignals(false);

//        ui.lePort->blockSignals(true);
//        ui.lePort->setText(model->ipPort->toString());
//        ui.lePort->blockSignals(false);

//        ui.leSpectrumsFName->blockSignals(true);
//        ui.leSpectrumsFName->setText(model->spectrumFileName->getData());
//        ui.leSpectrumsFName->blockSignals(false);

//        ui.cbThreads->blockSignals(true);
//        int index = ui.cbThreads->findData(model->getThreads()->getData());
//        ui.cbThreads->setCurrentIndex(model->getThreads()->getData());

        ui.cbIMCount->blockSignals(true);
        ui.cbIMCount->clear();
        switch (model->threads->getData()) {
        case 1:
            ui.cbIMCount->addItem(tr("1 механизм"), 1);
            ui.cbIMCount->addItem(tr("2 механизма"), 2);
            break;
        case 2:
            ui.cbIMCount->addItem(tr("2 механизма"), 2);
            ui.cbIMCount->addItem(tr("4 механизма"), 4);
            break;
        case 3:
            ui.cbIMCount->addItem(tr("3 механизма"), 3);
            ui.cbIMCount->addItem(tr("6 механизмов"), 6);
            break;
        case 4:
            ui.cbIMCount->addItem(tr("4 механизма"), 4);
            ui.cbIMCount->addItem(tr("8 механизмов"), 8);
            break;
        }
        int ind = ui.cbIMCount->findData(QVariant(model->ims->getData()));
        if(ind < 0){
            ind = 0;
            ui.cbIMCount->setCurrentIndex(ind);
            model->ims->setData(ui.cbIMCount->itemData(ind).toUInt());
        } else {
            ui.cbIMCount->setCurrentIndex(ind);
        }
        ui.cbIMCount->blockSignals(false);

        ind = ui.cbTypePRAM->findData(QVariant(model->typePRAM->getData()));
        ui.cbTypePRAM->setCurrentIndex(ind);

        ind = ui.cbTypeThermo->findData(QVariant(model->typeThermo->getData()));
        ui.cbTypeThermo->setCurrentIndex(ind);
    }
}

ISPRModelData *SPRSettingsMainWidget::getModel()
{
    return model;
}

void SPRSettingsMainWidget::viewChange(int)
{
    if(model){
        if(sender() == ui.cbThreads){ // Изменилось кол-во ручьев
            int value = ui.cbThreads->currentData().toInt();
            model->threads->setData(value);
            widgetsShow();
            emit doShow();
            return;
        }
        if(sender() == ui.cbRentgens){ // изменилось количество рентренов
            int value = ui.cbRentgens->currentData().toInt();
            model->rentgens->setData(value);
            widgetsShow();
            emit doShow();
            return;
        }
        if(sender() == ui.cbIMCount){ // изменилось количество исполнительных механизмов
            qint16 value = ui.cbIMCount->currentData().toInt();
            model->ims->setData(value);
            emit doShow();
            return;
        }
        if(sender() == ui.cbTypePRAM){ // измениля тип ПРАМ
            TypePRAM value = ui.cbTypePRAM->currentData().value<TypePRAM>();
            model->typePRAM->setData(value);
            return;
        }
        if(sender() == ui.cbTypeThermo){ // измениля тип терморегулятора
            TypeThermo value = ui.cbTypeThermo->currentData().value<TypeThermo>();
            model->typeThermo->setData(value);
            return;
        }
    }
}

void SPRSettingsMainWidget::viewChange()
{
    if(model){
        if(sender() == ui.leName){ // изменилось название сепаратора
            model->name->setData(ui.leName->text());
            return;
        }
        if(sender() == ui.leAddress){ // изменился IP адрес сепаратора
            model->ipAddress->setData(ui.leAddress->text());
            return;
        }
        if(sender() == ui.lePort){ // изменился IP порт сепаратора
            model->ipPort->setData(QString(ui.lePort->text()).toInt());
            return;
        }
    }
}

void SPRSettingsMainWidget::viewChange(bool)
{
    if(sender() == ui.bFNameSelect){
        QString fName = model->getName()->getData()+".xml";
        QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл..."), "",
            tr("Файлы настроек (*.xml)"));
        if(!fileName.isEmpty()){
            QFile f(fileName);
            if(f.open(QIODevice::ReadWrite)){
                if(fName != fileName){
//                    model->getName()->setData(fileNa);
                    emit changeFileSettinds(fileName);
                }
            }
        }
        return;
    }
    if(sender() == ui.bSpectrumFNameSelect){
        QString fName = model->getSpectrumFileName()->getData()+".spc";
        QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл..."), "",
            tr("Файлы сохраненных спектров (*.spc)"));
        if(!fileName.isEmpty()){
            QFile f(fileName);
            if(f.open(QIODevice::ReadWrite)){
                if(fName != fileName){
                    model->getSpectrumFileName()->setData(fileName);
                    emit changeFileSpectrum(fileName);
                }
            }
        }
        return;
    }
}

