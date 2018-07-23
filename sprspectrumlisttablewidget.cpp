#include "sprspectrumlisttablewidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include "tcp/tcpexpositionoff.h"

SPRViewGraphicsMode SPRSpectrumListTableWidget::getGraphicsMode() const
{
    SPRViewGraphicsMode val = viewModeAsIs;
    if(ui.gbViewAsIs->isChecked()){
        if(ui.rbAsOneSecond){
            val = viewModeOneSecond;
        }
        if(ui.rbScales){
            val = viewModeScales;
        }
    }
//    if(val != graphicsMode){
//        graphicsMode = val;
//    }
    return val;
}

//void SPRSpectrumListTableWidget::setGraphicsMode(const SPRViewGraphicsMode &value)
//{
//    graphicsMode = value;
//    ui.graphic->setGraphicsMode(value);
//}

SPRSpectrumListTableWidget::SPRSpectrumListTableWidget(QWidget *parent) :
    QWidget(parent), ISPRWidget(),
    model(nullptr),
    spectrums(nullptr),
    choiseTimeDialog(nullptr),
    gettingSpectrumsCommand(nullptr),
    gettingBaseSpectrumsCommand(nullptr),
    separateOffCommand(nullptr),
    rguUpDownCommand(nullptr),
    rentgenOffCommand(nullptr),
    commands(nullptr),
    toWidget(nullptr)/*,
    graphicsMode(viewModeAsIs)*/
{
    ui.setupUi(this);
//    ui.graphic->setCanvasBackground(QBrush(Qt::black));


    connect(ui.tListSpectrumItem, SIGNAL(cellClicked(int,int)), this, SLOT(onSpectSpectrumTableClick(int,int)));
    connect(ui.tListSpectrumItem, SIGNAL(rowSelectedChecked2(int, SPRSpectrumListTable*)), this, SLOT(onSpectSpectrumTableClick2(int, SPRSpectrumListTable*)));

    connect(ui.tListBasedSpectrumItem, SIGNAL(cellClicked(int,int)), this, SLOT(onSpectSpectrumTableClick(int,int)));
    connect(ui.tListBasedSpectrumItem, SIGNAL(rowSelectedChecked(QList<int>,int)), this, SLOT(onChangeCheckedSpectrum(QList<int>,int)));

//    connect(ui.tRangesChannel, SIGNAL(changeColor(EnumElements,QColor)), this, SLOT(onChangeZoneColor(EnumElements,QColor)));
//    connect(ui.tRangesChannel, SIGNAL(tableChange(EnumElements,int,int)), this, SLOT(onChangeZoneRange(EnumElements,int,int)));

    connect(ui.bGetSpectrums, SIGNAL(clicked(bool)), this, SLOT(onGetSpectrums(bool)));
    connect(ui.bGetBaseSpectrum, SIGNAL(clicked(bool)), this, SLOT(onGetSpectrums(bool)));

    connect(ui.bSeparatorOff, SIGNAL(clicked(bool)), this, SLOT(onClickedBooton(bool)));
    connect(ui.bRecompliteSpectrums, SIGNAL(clicked(bool)), this, SLOT(onClickedBooton(bool)));
    connect(ui.bOpenSpectrums, SIGNAL(clicked(bool)), this, SLOT(onClickedBooton(bool)));
    connect(ui.bSaveSpectrums, SIGNAL(clicked(bool)), this, SLOT(onClickedBooton(bool)));
    connect(ui.bSpectrumFNameSelect, SIGNAL(clicked(bool)), this, SLOT(onClickedBooton(bool)));

    connect(ui.gbViewAsIs, SIGNAL(toggled(bool)), this, SLOT(onClickedBooton(bool)));
    connect(ui.rbAsOneSecond, SIGNAL(toggled(bool)), this, SLOT(onClickedBooton(bool)));
    connect(ui.rbScales, SIGNAL(toggled(bool)), this, SLOT(onClickedBooton(bool)));

    connect(ui.leSpectrumsFName, SIGNAL(editingFinished()), this, SLOT(onChangeSpectrumsFileName()));

    ui.gbBasetSpectrums->setVisible(ui.cbBasetSpectrumVisible->isChecked());

    ui.tRangesChannel->setSelectBottonRowVisible(false);
}

void SPRSpectrumListTableWidget::onChangeSpectrumsFileName(){
    if(model){
        blockSignals(true);
        if(sender() == ui.leSpectrumsFName){
            model->getSettingsMainModel()->getSpectrumFileName()->setData(ui.leSpectrumsFName->text());
        }
        blockSignals(false);
    }
}

void SPRSpectrumListTableWidget::changeGraphicViewMode(){
    SPRViewGraphicsMode mode = viewModeAsIs;
    if(ui.gbViewAsIs->isChecked()){
        if(ui.rbAsOneSecond->isChecked()){
            mode = viewModeOneSecond;
        }
        if(ui.rbScales->isChecked()){
            mode = viewModeScales;
        }
    } else {

    }

    ui.graphic->setGraphicsMode(mode);

    switch(mode){
    case viewModeAsIs:
        ui.gbViewAsIs->setTitle(tr("Отображение (как есть)"));
        break;
    case viewModeOneSecond:
        ui.gbViewAsIs->setTitle(tr("Отображение (1 секунда)"));
        break;
    case viewModeScales:
        ui.gbViewAsIs->setTitle(tr("Отображение (масштаб)"));
        break;
    }
}

void SPRSpectrumListTableWidget::onClickedBooton(bool value){
    if(model){
        if(sender() == ui.gbViewAsIs){
//            if(!value)
            changeGraphicViewMode();
        }
        if(sender() == ui.rbAsOneSecond){
            if(value)
                changeGraphicViewMode();
        }
        if(sender() == ui.rbScales){
            if(value)
                changeGraphicViewMode();
        }
        if(sender() == ui.bSeparatorOff){
            if(separateOffCommand){
                separateOffCommand->send(model->getServer());
            }
        }
        if(sender() == ui.bOpenSpectrums){
            QString fname = QFileDialog::getOpenFileName(this, tr("Выберите файл с готовыми спекрами..."), QDir::currentPath(), tr("файлы спектров (*.spc)"));

            this->blockSignals(true);
            model->getSpectrumListItemsModel()->addSpectrums(fname);
            this->blockSignals(false);
        }
        if(sender() == ui.bRecompliteSpectrums){
            uint32_t res =
            QMessageBox::warning(nullptr, tr("Необходимо подтверждение"),
                                 tr("Вы действительно хотите пересчитать спектры?"),
                                 QMessageBox::Yes, QMessageBox::No);

            if(res == QMessageBox::Yes){
                this->blockSignals(true);
    //            QVector<SPRSpectrumItemModel*> *items = model->getSpectrumListItemsModel()->getSpectrumsModel(spectrumBase);
    //            for(int i = 0; i<items->size(); i++){
    //                SPRSpectrumItemModel *it = items->at(i);
    //                if(it){
    //                    model->getSpectrumListItemsModel()->recomplite(it, spectrumBase);
    //                }
    //            }
    //            items = model->getSpectrumListItemsModel()->getSpectrumsModel(spectrumsOnly);
    //            for(int i=0; i < items->size(); i++){
    //                SPRSpectrumItemModel *it = items->at(i);
    //                if(it){
    //                    model->getSpectrumListItemsModel()->recomplite(it, spectrumsOnly);
    //                }
    //            }

                model->getSpectrumListItemsModel()->recomplite();

                blockSignals(false);
                ui.tListSpectrumItem->widgetsShow();
                ui.tListBasedSpectrumItem->widgetsShow();
            }
        }
        if(sender() == ui.bSpectrumFNameSelect){
            QString fName = model->getSettingsMainModel()->getSpectrumFileName()->getData();
            QString nfName = QFileDialog::getSaveFileName(nullptr,
                                QString(tr("Выберите файл для записи спектров")),
                                QDir::currentPath(),
                                QString(tr("файлы спектров (*.spc)")));

            if(fName != nfName){
                 model->getSettingsMainModel()->getSpectrumFileName()->setData(nfName);
           }
        }
        if(sender() == ui.bSaveSpectrums){
            model->getSpectrumListItemsModel()->saveAs(model->getSettingsMainModel()->getSpectrumFileName()->getData());
        }
    }
}

void SPRSpectrumListTableWidget::onGetSpectrums(bool){
    if(sender() == ui.bGetSpectrums){
        if(choiseTimeDialog){
            choiseTimeDialog->setModelData(model);

            int res = choiseTimeDialog->exec();

            if(res == QDialog::Accepted){
                double time = choiseTimeDialog->getTime();
                QList<uint8_t> lth = choiseTimeDialog->getThreads();

                gettingSpectrumsCommand->setThreadTimer(model->getSettingsMainModel()->getThreads()->getData(), time, lth);

                if(!commands){
                    commands = new TCPCommandSet(model->getServer(), toWidget, {});
                }

                commands->clear();
                if(choiseTimeDialog->isRGUDown()){
                    commands->addCommand(rguUpDownCommand);
                }
    //            if(choiseTimeDialog->isRentgenOn()){
    //                commands->addCommand(rentgenOnCommand);
    //            }

                commands->addCommand(gettingSpectrumsCommand);

                if(choiseTimeDialog->isRentgenOff()){
                    commands->addCommand(new TCPExpositionOff(getLogWidget()));
                }

                commands->setLogWidget(getLogWidget());
                commands->send(model->getServer());
            }
        }
    }
    if(sender() == ui.bGetBaseSpectrum){
        if(!commands){
            commands = new TCPCommandSet(model->getServer(), toWidget, {});
        }
        commands->clear();
        gettingBaseSpectrumsCommand->setThreadTimer(model->getThreads()->getData(), 30);
        commands->addCommand(gettingBaseSpectrumsCommand);
        if(choiseTimeDialog){
            if(choiseTimeDialog->isRentgenOff()){
                commands->addCommand(new TCPExpositionOff(getLogWidget()));
                commands->addCommand(offren);
                commands->addCommand(offosw);
            }
        }
        commands->setLogWidget(getLogWidget());

        commands->send(model->getServer());
//        gettingBaseSpectrumsCommand->send(model->getServer());
    }
}

void SPRSpectrumListTableWidget::onCompliteCommand(TCPCommand *command)
{

    if(command == gettingSpectrumsCommand){
        QMessageBox::information(this, QString(tr("Все готово")), QString(tr("Все готово")), QMessageBox::Ok);

        QList<uint8_t> lth = choiseTimeDialog->getThreads();

        for(uint8_t th=0; th<lth.size(); th++) {
            SPRSpectrumItemModel *item =
            model->getSpectrumListItemsModel()->addSpectrum(gettingSpectrumsCommand->getSpectrumData(th), choiseTimeDialog->getTime(), lth[th], choiseTimeDialog->getPrefix());
            item->setTimeScope(choiseTimeDialog->getTime());
        }
        widgetsShow();
    }
    if(command == gettingBaseSpectrumsCommand){
        QVector<TCPCommand*> vec = gettingBaseSpectrumsCommand->findCommands(getspk);

        for(uint8_t th=0; th<vec.size(); th++){
            QByteArray buf = gettingBaseSpectrumsCommand->getSpectrumData(th);
            model->getSpectrumListItemsModel()->setSpectrumData(th, buf, spectrumBase);
        }
        widgetsShow();
    }
    ui.graphic->setGraphicsMode(getGraphicsMode());
}

void SPRSpectrumListTableWidget::onErrorsCommand(TCPCommand *command)
{
    if(command->getCommand() == getren){
        QMessageBox::information(this, QString(tr("Рентген не готов")), QString(tr("Рентген не готов")), QMessageBox::Ok);
    }
    if(command->getCommand() == getstate){
        QMessageBox::information(this, QString(tr("Сепаратор не готов")), QString(tr("Сепаратор не готов")), QMessageBox::Ok);
    }
}


ISPRModelData *SPRSpectrumListTableWidget::setModelData(SPRMainModel *mainModel){
    if(mainModel){
        if(model){
            disconnect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        }
        model = mainModel;
        connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));

        spectrums = mainModel->getSpectrumListItemsModel();
//        connect(spectrums, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(widgetsShow()));

        ui.graphic->setModelData(spectrums, spectrumsAll, true);

        ui.tListBasedSpectrumItem->setModelData(spectrums, spectrumBase);
        ui.tListSpectrumItem->setModelData(spectrums, spectrumsOnly);

        ui.tRangesChannel->setModelData(mainModel->getSpectrumZonesTableModel());
        ui.tRangesChannel->setThreadsVisible(0);

        if(choiseTimeDialog){
            delete choiseTimeDialog;
        }

        choiseTimeDialog = new SPRSpectrumChoiseTimeDialog(model, this);

        if(!toWidget){
            toWidget = new TCPTimeOutWigget();
        }

        if(!gettingSpectrumsCommand){
            gettingSpectrumsCommand = new TCPGetSpectrumsGistogramms(model->getServer(), getspk, model, toWidget);
            connect(gettingSpectrumsCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCompliteCommand(TCPCommand*)));
            connect(gettingSpectrumsCommand, SIGNAL(commandNotComplite(TCPCommand*)), this, SLOT(onErrorsCommand(TCPCommand*)));
        }
        gettingSpectrumsCommand->setModelData(model);

        if(!gettingBaseSpectrumsCommand){
            gettingBaseSpectrumsCommand = new TCPGetSpectrumsGistogramms(model->getServer(), getspk, model, toWidget);
            connect(gettingBaseSpectrumsCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCompliteCommand(TCPCommand*)));
            connect(gettingBaseSpectrumsCommand, SIGNAL(commandNotComplite(TCPCommand*)), this, SLOT(onErrorsCommand(TCPCommand*)));
        }
        gettingBaseSpectrumsCommand->setModelData(model);


        if(!separateOffCommand){
            separateOffCommand = new TCPTestStopSeparate(toWidget);
            connect(separateOffCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCompliteCommand(TCPCommand*)));
            connect(separateOffCommand, SIGNAL(commandNotComplite(TCPCommand*)), this, SLOT(onErrorsCommand(TCPCommand*)));
        }
//        if(!rentgenOnCommand){
//            rentgenOnCommand = new TCPCommandSeparatorOnFull(model->getServer(), model, toWidget);
//            connect(rentgenOnCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCompliteCommand(TCPCommand*)));
//            connect(rentgenOnCommand, SIGNAL(errorsCommand(TCPCommand*)), this, SLOT(onErrorsCommand(TCPCommand*)));
//        }
        if(!rguUpDownCommand){
            rguUpDownCommand = new TCPCommandRGUUpDown(model->getServer(), toWidget, false);
            connect(rguUpDownCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCompliteCommand(TCPCommand*)));
        }
//        if(!rentgenOffCommand){
//            rentgenOffCommand = new TCPCommand(expoff);
//            connect(rentgenOffCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCompliteCommand(TCPCommand*)));
//        }
    }

    widgetsShow();
    return model;
}

void SPRSpectrumListTableWidget::widgetsShow()
{

    ui.tListBasedSpectrumItem->widgetsShow();
    ui.tListSpectrumItem->widgetsShow();
    ui.tRangesChannel->widgetsShow();
    ui.graphic->widgetsShow();
    if(model){
        blockSignals(true);
        ui.leSpectrumsFName->setText(model->getSettingsMainModel()->getSpectrumFileName()->getData());
        blockSignals(false);
    }
}

void SPRSpectrumListTableWidget::onChangeSpectColor(int row)
{
    QColor color(ui.tListSpectrumItem->getColorSpectrum(row));
//    grItems[row].spect->setPen(color);
    widgetsShow();
}

ISPRModelData *SPRSpectrumListTableWidget::getModelData()
{
    return model;
}

void SPRSpectrumListTableWidget::viewChange(int row, int col)
{
    blockSignals(true);
    int thr = ui.tListSpectrumItem->getThread(row);

    ui.tRangesChannel->setThreadsVisible(thr);

    QSize s = ui.tRangesChannel->sizeHint();
    s = ui.tRangesChannel->sizeHint();
    ui.gbSpectrumElements->setMaximumWidth(ui.tRangesChannel->sizeHint().width());
    widgetsShow();
    blockSignals(false);
}

void SPRSpectrumListTableWidget::onSpectSpectrumTableClick2(int row, SPRSpectrumListTable *_sender){
    blockSignals(true);
    SPRSpectrumItemModel* toGraphCurrent = nullptr;
    if(_sender)
        toGraphCurrent = model->getSpectrumListItemsModel()->getSpectrumItem(row, _sender->getTypeData());

    QList<SPRSpectrumItemModel*> toGraphSelected;
    toGraphSelected = ui.tListBasedSpectrumItem->getSelectedItems() + ui.tListSpectrumItem->getSelectedItems();

    ui.graphic->onChangeSelectedCheckedItems(toGraphSelected, toGraphCurrent);

    blockSignals(false);
}


void SPRSpectrumListTableWidget::onSpectSpectrumTableClick(int row, int col, SPRSpectrumListTable *_sender){
    blockSignals(true);
    int toGraphCurrent = -1;
    if(_sender == ui.tListSpectrumItem || (sender() && sender() == ui.tListSpectrumItem)){
        if(row >=0 && row<ui.tListSpectrumItem->rowCount()){
            int thr = ui.tListSpectrumItem->getThread(row);

            ui.tRangesChannel->setThreadsVisible(thr);
            toGraphCurrent = ui.tListBasedSpectrumItem->rowCount() + row;
        }
    } else if(_sender == ui.tListBasedSpectrumItem || (sender() && sender() == ui.tListBasedSpectrumItem)){
        if(row >=0 && row<ui.tListBasedSpectrumItem->rowCount()){
            int thr = ui.tListBasedSpectrumItem->getThread(row);

            ui.tRangesChannel->setThreadsVisible(thr);
            toGraphCurrent = row;
        }

    }
    QList<int> toGraphSelected;
    foreach(int num, ui.tListBasedSpectrumItem->getSelectedItemsNumbers()){
        toGraphSelected.push_back(num);
    }
    foreach(int num, ui.tListSpectrumItem->getSelectedItemsNumbers()){
        toGraphSelected.push_back(ui.tListBasedSpectrumItem->rowCount() + num);
    }

    ui.graphic->onChangeSelectedCheckedItems(toGraphSelected, toGraphCurrent);
    blockSignals(false);

}

void SPRSpectrumListTableWidget::viewChange(int row)
{
    widgetsShow();
}

void SPRSpectrumListTableWidget::viewChange(bool value)
{
    if(sender() == ui.cbBasetSpectrumVisible){
        ui.gbBasetSpectrums->setVisible(value);
    }
}

void SPRSpectrumListTableWidget::onChangeZoneRange(EnumElements el, int thread, int col)
{
    widgetsShow();
}


void SPRSpectrumListTableWidget::onModelChanget(IModelVariable *)
{
    widgetsShow();
}


void SPRSpectrumListTableWidget::setLogWidget(TCPLogsWigtets *value)
{
    ISPRWidget::setLogWidget(value);
    gettingSpectrumsCommand->setLogWidget(value);
    gettingBaseSpectrumsCommand->setLogWidget(value);
    rguUpDownCommand->setLogWidget(value);
    separateOffCommand->setLogWidget(value);
}
