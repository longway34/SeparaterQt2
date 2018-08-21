#include "sprspectrumlisttablewidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include "tcp/tcpexpositiononoff.h"

#include <qwt_plot_grid.h>

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
    connect(ui.tListSpectrumItem, SIGNAL(rowSelectedChecked3(QList<SPRSpectrumItemModel*>,SPRSpectrumItemModel*)), this, SLOT(onSpectSpectrumTableClick(QList<SPRSpectrumItemModel*>,SPRSpectrumItemModel*)));
    connect(ui.tListSpectrumItem, SIGNAL(itemChangeColor(SPRSpectrumItemModel*,QColor)), ui.graphic, SLOT(onChangeGraphicItemsColor(SPRSpectrumItemModel*,QColor)));

    connect(ui.tListBasedSpectrumItem, SIGNAL(cellClicked(int,int)), this, SLOT(onSpectSpectrumTableClick(int,int)));
    connect(ui.tListBasedSpectrumItem, SIGNAL(rowSelectedChecked3(QList<SPRSpectrumItemModel*>,SPRSpectrumItemModel*)), this, SLOT(onSpectSpectrumTableClick(QList<SPRSpectrumItemModel*>,SPRSpectrumItemModel*)));

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
    ui.graphic->setWithLegend(true);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setMajorPen(QColor(Qt::white), 1);
    grid->attach(ui.graphic->getCanvas());
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
    }

//    if(model){
//        QList<SPRSpectrumItemModel*> *spects = spectrums->getSpectrumsModelAll();
//        spectrums->setV
//    }
    spectrums->setGraphicMode(mode);

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

                gettingSpectrumsCommand->setThreadTimer(time, lth);
                gettingSpectrumsCommand->setWithRGU(choiseTimeDialog->isRGUDown());
                gettingSpectrumsCommand->setWithOffExp(choiseTimeDialog->isRentgenOff());
                gettingSpectrumsCommand->send(model->getServer());
            }
        }
    }
    if(sender() == ui.bGetBaseSpectrum){

        QList<uint8_t> lthreads;
        for(int th=0; th<model->getThreads()->getData(); th++){
            lthreads << th;
        }
        gettingBaseSpectrumsCommand->setThreadTimer(30, lthreads);
        gettingBaseSpectrumsCommand->setWithRGU(true, false);
        gettingBaseSpectrumsCommand->setWithOffExp(true);

        gettingBaseSpectrumsCommand->send(model->getServer());
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
            item->setTimeScope(choiseTimeDialog->getTime() * 1000);
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
            gettingSpectrumsCommand = new TCPGetSpectrumsGistogramms(model->getServer(), getspk, model, 5, {}, toWidget, getLogWidget());
            connect(gettingSpectrumsCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCompliteCommand(TCPCommand*)));
            connect(gettingSpectrumsCommand, SIGNAL(commandNotComplite(TCPCommand*)), this, SLOT(onErrorsCommand(TCPCommand*)));
        }
        gettingSpectrumsCommand->setModelData(model);

        if(!gettingBaseSpectrumsCommand){
            gettingBaseSpectrumsCommand = new TCPGetSpectrumsGistogramms(model->getServer(), getspk, model, 30, {}, toWidget, getLogWidget());
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

//void SPRSpectrumListTableWidget::onSpectSpectrumTableClick2(QVector<SPRSpectrumItemModel*>, SPRSpectrumListTable *_sender){
//    blockSignals(true);
//    SPRSpectrumItemModel* toGraphCurrent = nullptr;
//    if(_sender)
//        toGraphCurrent = model->getSpectrumListItemsModel()->getSpectrumItem(row, _sender->getTypeData());

//    QList<SPRSpectrumItemModel*> toGraphSelected;
//    toGraphSelected = ui.tListBasedSpectrumItem->getSelectedItems() + ui.tListSpectrumItem->getSelectedItems();

//    ui.graphic->onChangeSelectedCheckedItems(toGraphSelected, toGraphCurrent);

//    blockSignals(false);
//}
void SPRSpectrumListTableWidget::onSpectSpectrumTableClick(int row, int){
    blockSignals(true);
    SPRSpectrumItemModel *current = nullptr;
    if(sender() == ui.tListSpectrumItem){
        current = ui.tListSpectrumItem->getModelData(row);
    }
    if(sender() == ui.tListBasedSpectrumItem){
        current = ui.tListBasedSpectrumItem->getModelData(row);
    }

    onSpectSpectrumTableClick({}, current);
    blockSignals(false);
}

void SPRSpectrumListTableWidget::onSpectSpectrumTableClick(QList<SPRSpectrumItemModel*>, SPRSpectrumItemModel *_current){
    blockSignals(true);
//    SPRSpectrumItemModel *toGraphCurrent = nullptr;
//    if(sender() && (sender() == ui.tListSpectrumItem || sender() == ui.tListBasedSpectrumItem)){
        if(_current){
            int thr = _current->getThread();
//            int thr = ui.tListSpectrumItem->getThread(row);

            ui.tRangesChannel->setThreadsVisible(thr);
//            toGraphCurrent = ui.tListSpectrumItem->getModelData(row);
        }
//    } else if(sender() && sender() == ){
//        if(row >=0 && row<ui.tListBasedSpectrumItem->rowCount()){
//            int thr = ui.tListBasedSpectrumItem->getThread(row);

//            ui.tRangesChannel->setThreadsVisible(thr);
//            toGraphCurrent = ui.tListBasedSpectrumItem->getModelData(row);
//        }

//    }
    QList<SPRSpectrumItemModel*> toGraphSelected;
    foreach(SPRSpectrumItemModel* model, ui.tListBasedSpectrumItem->getSelectedItems()){
        if(model){
            toGraphSelected.push_back(model);
        }
    }
    foreach(SPRSpectrumItemModel* model, ui.tListSpectrumItem->getSelectedItems()){
        if(model){
            toGraphSelected.push_back(model);
        }
    }

    ui.graphic->onChangeSelectedCheckedItems(toGraphSelected, _current);
    ui.graphic->widgetsShow();
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


void SPRSpectrumListTableWidget::onModelChanget(IModelVariable *variable)
{
    if((sender() == model && variable != spectrums) || (!sender())){
        widgetsShow();
    }
}


void SPRSpectrumListTableWidget::setLogWidget(TCPLogsWigtets *value)
{
    ISPRWidget::setLogWidget(value);
    gettingSpectrumsCommand->setLogWidget(value);
    gettingBaseSpectrumsCommand->setLogWidget(value);
    rguUpDownCommand->setLogWidget(value);
    separateOffCommand->setLogWidget(value);
}
