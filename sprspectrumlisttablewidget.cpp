#include "sprspectrumlisttablewidget.h"
#include <QHeaderView>
#include <QMessageBox>


SPRSpectrumListTableWidget::SPRSpectrumListTableWidget(QWidget *parent) :
    QWidget(parent), ISPRWidget(),
    model(nullptr),
    spectrums(nullptr),
    choiseTimeDialog(nullptr),
    gettingSpectrumsCommand(nullptr),
    rentgenOnCommand(nullptr),
    rguUpDownCommand(nullptr),
    rentgenOffCommand(nullptr),
    commands(nullptr),
    toWidget(nullptr)
{
    ui.setupUi(this);
//    ui.graphic->setCanvasBackground(QBrush(Qt::black));


    connect(ui.tListSpectrumItem, SIGNAL(cellClicked(int,int)), this, SLOT(onSpectSpectrumTableClick(int,int)));
    connect(ui.tListSpectrumItem, SIGNAL(rowSelectedChecked(QList<int>,int)), this, SLOT(onChangeCheckedSpectrum(QList<int>,int)));

    connect(ui.tListBasedSpectrumItem, SIGNAL(cellClicked(int,int)), this, SLOT(onSpectSpectrumTableClick(int,int)));
    connect(ui.tListBasedSpectrumItem, SIGNAL(rowSelectedChecked(QList<int>,int)), this, SLOT(onChangeCheckedSpectrum(QList<int>,int)));

//    connect(ui.tRangesChannel, SIGNAL(changeColor(EnumElements,QColor)), this, SLOT(onChangeZoneColor(EnumElements,QColor)));
//    connect(ui.tRangesChannel, SIGNAL(tableChange(EnumElements,int,int)), this, SLOT(onChangeZoneRange(EnumElements,int,int)));

    connect(ui.bGetSpectrums, SIGNAL(clicked(bool)), this, SLOT(onGetSpectrums(bool)));

    ui.gbBasetSpectrums->setVisible(ui.cbBasetSpectrumVisible->isChecked());

    ui.tRangesChannel->setSelectBottonRowVisible(false);

}

void SPRSpectrumListTableWidget::onGetSpectrums(bool){
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
            if(choiseTimeDialog->isRentgenOn()){
                commands->addCommand(rentgenOnCommand);
            }

            commands->addCommand(gettingSpectrumsCommand);

            if(choiseTimeDialog->isRentgenOff()){
                commands->addCommand(rentgenOffCommand);
            }

            commands->send(model->getServer());
        }
    }
}

void SPRSpectrumListTableWidget::onCompliteCommand(TCPCommand *command)
{

    if(command == gettingSpectrumsCommand){
        QMessageBox::information(this, QString(tr("Все готово")), QString(tr("Все готово")), QMessageBox::Ok);

        QList<uint8_t> lth = choiseTimeDialog->getThreads();

        for(uint8_t th=0; th<lth.size(); th++) {
            model->getSpectrumListItemsModel()->addSpectrum(gettingSpectrumsCommand->getSpectrumData(th), lth[th], choiseTimeDialog->getPrefix());
        }
        widgetsShow();
    }
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
        connect(spectrums, SIGNAL(modelChanget()), this, SLOT(widgetsShow()));

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
            gettingSpectrumsCommand = new TCPGetSpectrumsGistogramms(model->getServer(), getspk, toWidget);
            connect(gettingSpectrumsCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCompliteCommand(TCPCommand*)));
        }
        if(!rentgenOnCommand){
            rentgenOnCommand = new TCPCommandSeparatorOnFull(model->getServer(), model, toWidget);
            connect(rentgenOnCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCompliteCommand(TCPCommand*)));
            connect(rentgenOnCommand, SIGNAL(errorsCommand(TCPCommand*)), this, SLOT(onErrorsCommand(TCPCommand*)));
        }
        if(!rguUpDownCommand){
            rguUpDownCommand = new TCPCommandRGUUpDown(model->getServer(), toWidget, false);
            connect(rguUpDownCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCompliteCommand(TCPCommand*)));
        }
        if(!rentgenOffCommand){
            rentgenOffCommand = new TCPCommand(expoff);
            connect(rentgenOffCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCompliteCommand(TCPCommand*)));
        }
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
    foreach(int num, ui.tListBasedSpectrumItem->getSelectedItems()){
        toGraphSelected.push_back(num);
    }
    foreach(int num, ui.tListSpectrumItem->getSelectedItems()){
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
