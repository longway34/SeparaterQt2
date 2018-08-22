#include "sprsettingsrentgenautosetdialog.h"
#include "qwt_plot_grid.h"


void SPRSettingsRentgenAutoSetDialog::onChangeSelectedItemValue(QwtPlotItem *item, double distance, MovedItemPosition position){
    if(item && item == peakCurve){
//        if(rentgenModel){
//            uint last = rentgenModel->peakPosition->getData();
//            rentgenModel->peakPosition->setData(last + distance);
//        }
        tempPeakPosition += distance;
    }
}

void SPRSettingsRentgenAutoSetDialog::onCusorOverSelectItem(QwtPlotItem *item, MovedItemPosition){
    if(item){
        if(item == peakCurve){
            peakCurve->setPen(selPen);
        }
    } else {
        peakCurve->setPen(defPen);
    }
}

void SPRSettingsRentgenAutoSetDialog::widgetsShow()
{
    while(ui.tDeuCP->rowCount()>0){
        ui.tDeuCP->removeRow(0);
    }
    while(ui.tPeakRS->rowCount()>0){
        ui.tPeakRS->removeRow(0);
    }

    foreach (int th, threads) {
        ui.tDeuCP->insertRow(ui.tDeuCP->rowCount());
        ui.tPeakRS->insertRow(ui.tPeakRS->rowCount());

        QColor _color = QColor(Qt::white);
//        QString _textTitle = "Ruchey";
        QString _textTitle = QString(tr("Ручей %1")).arg(QString::number(th + 1));
        bool _deleteVisible = false, _colorVisible = true, _selectVisible = false;
        if(spectrums){
            QList<SPRSpectrumItemModel*> lst = spectrums->getSpectrumsItemByThread(th);
            if(lst.size() > 0){
                SPRSpectrumItemModel *spect = lst.last();
                if(spect){
                    _color = spect->getSpectrumColor();
                    _textTitle = spect->getSpectrumName();
                }
            }

        }

        FirstCollumn2 *fc = new FirstCollumn2(_textTitle, _colorVisible, _color, _selectVisible, _deleteVisible, nullptr);
        ui.tDeuCP->setCellWidget(ui.tDeuCP->rowCount()-1, 0, fc);
//        fc->widgetsShow();

//        ui.tDeuCP->setItem(ui.tDeuCP->rowCount()-1, 0, new QTableWidgetItem(QString::number(th+1)));
        ui.tDeuCP->setItem(ui.tDeuCP->rowCount()-1, 1, new QTableWidgetItem(QString::number(tempDEU[th])));
        ui.tDeuCP->setItem(ui.tDeuCP->rowCount()-1, 2, new QTableWidgetItem(QString::number(tempCP[th])));

        fc = new FirstCollumn2(_textTitle, _colorVisible, _color, _selectVisible, _deleteVisible, nullptr);
        ui.tPeakRS->setCellWidget(ui.tPeakRS->rowCount()-1, 0, fc);
//        fc->widgetsShow();

//        ui.tPeakRS->setItem(ui.tPeakRS->rowCount()-1, 0, new QTableWidgetItem(QString::number(th+1)));
        QString rs = "0"; QString peak = "0";
        if(spectrums){
            QList<SPRSpectrumItemModel *> lst = spectrums->getSpectrumsItemByThread(th);
            if(lst.size() > 0){
                SPRSpectrumItemModel* spec = lst.last();
                rs = QString::number(*spec->getSpectrumData()->Rs, 'f', 0);
                peak = QString::number(*spec->getSpectrumData()->peak);
            }
        }
        ui.tPeakRS->setItem(ui.tPeakRS->rowCount()-1, 1, new QTableWidgetItem(peak));
        ui.tPeakRS->setItem(ui.tPeakRS->rowCount()-1, 2, new QTableWidgetItem(rs));
    }
    ui.tDeuCP->resizeColumnsToContents();
    ui.tDeuCP->resizeRowsToContents();
    ui.tPeakRS->resizeColumnsToContents();
    ui.tPeakRS->resizeRowsToContents();

    ui.tmContrilDEU->setValue(controlDEU);
    ui.tmContrilDEU->setToolTip(QString::number(controlDEU,'f',1) + "%");

    ui.tmContrilCP->setValue(controlCP);
    ui.tmContrilCP->setToolTip(QString::number(controlCP,'f',1) + "%");

    ui.tmContrilAll->setValue(controlCP);
    ui.tmContrilAll->setToolTip(QString::number(controlAll,'f',1) + "%");

    ui.graphic->hideGraphics(this->peakCurve);
    ui.graphic->getCanvas()->replot();

    peakData[1].setY(ui.graphic->getCanvas()->axisInterval(QwtPlot::yLeft).maxValue());
    peakCurve->setSamples(peakData);
    peakCurve->attach(ui.graphic->getCanvas());

    ui.graphic->getCanvas()->replot();

    setGraphicTitle();
    onDblClickMouseEvent();
}

void SPRSettingsRentgenAutoSetDialog::setGraphicTitle(){
    QString titlePref = QString(tr("Спектры (%1)"));
    int thr = ui.graphic->getCurrentThread();
    QString titleSuff = thr < 0 ? QString(tr("Все ручьи")) : QString(tr("Ручей %1")).arg(thr+1);
    QString title = titlePref.arg(titleSuff);

    ui.graphic->getCanvas()->setTitle(title);
}

SPRThreadList SPRSettingsRentgenAutoSetDialog::getThreads()
{
    return threads;
}

double SPRSettingsRentgenAutoSetDialog::prDeuCpOKAll(QList<bool> *_src){
    QList<bool> src = _src == nullptr ? deuOK + cpOK : *_src;
    double countOk = 0, size = src.size();
    foreach(bool b, src){
        if(b){
            countOk++;
        }
    }
    return (countOk / size) * 100.;
}

bool SPRSettingsRentgenAutoSetDialog::isDeuCpAllOK(uint8_t th, QList<bool> *_src)
{
    QList<bool> src; bool res = true;
    if(th == 255){
        if(_src){
            src = *_src;
        } else {
            src = deuOK + cpOK;
        }
    } else {
        if(_src){
            src = QList<bool>({(*_src)[th]});
        } else {
            if(th < deuOK.size() && th < cpOK.size()){
                src =  QList<bool>({deuOK[th] + cpOK[th]});
            } else {
                return false;
            }
        }
    }

//    QList<uint8_t> lst = th < 0 ? threads : QList<uint8_t>({th});
    foreach(bool b, src){
        res &= b;
    }
    return res;
}

void SPRSettingsRentgenAutoSetDialog::setThreads(SPRThreadList &value)
{
    threads = value;
    ui.graphic->setVisibleThreads(threads);

    deuOK.clear();
    cpOK.clear();
    if(mainModel){
        for(int i=0; i< MAX_SPR_MAIN_THREADS; i++){
            tempDEU[i] = mainModel->getSettingsRentgenModel()->deuCodes[i]->getData();
            tempCP[i] = mainModel->getSettingsRentgenModel()->cpCodes[i]->getData();
        }
        foreach(uint8_t i, threads){
            deuOK << false;
            cpOK << false;
            tempCP[i] = 128;
            tempDEU[i] = mainModel->getSettingsRentgenModel()->codeBegin->getData();
        }
    }
}

TCPTimeOutWigget *SPRSettingsRentgenAutoSetDialog::getToWidget() const
{
    return toWidget;
}

void SPRSettingsRentgenAutoSetDialog::setToWidget(TCPTimeOutWigget *value)
{
    toWidget = value;
}

SPRSettingsRentgenAutoSetDialog::SPRSettingsRentgenAutoSetDialog(QWidget *parent) :
    QDialog(parent),
    rentgenModel(nullptr),
    mainModel(nullptr),
    autoSetCommand(nullptr),
    autoSetGo(nullptr),
    toWidget(nullptr),
    peakCurve(nullptr),
    spectrums(nullptr)

{
    ui.setupUi(this);

    currentViewThread = -1;

    grid = new QwtPlotGrid();
    grid->enableXMin(true);
    grid->setMajorPen(Qt::white, 1);
    grid->setMinorPen(Qt::gray, 1, Qt::DotLine);

    grid->attach(ui.graphic->getCanvas());

    peakCurve = new QwtPlotCurve(QString(tr("Пиковое значение")));
    defPen = QPen(Qt::red, 2);
    selPen = QPen(Qt::red, 4, Qt::DotLine);

    peakCurve->setPen(defPen);
    peakData = {QPointF(DEF_SPR_AUTOSETTINGS_PEAC_POSITION, 0), QPointF(DEF_SPR_AUTOSETTINGS_PEAC_POSITION,100)};

    peakCurve->setSamples(peakData);
    peakCurve->attach(ui.graphic->getCanvas());

    ui.graphic->getPorogsMoved()->addMovedItems(peakCurve);

    ui.graphic->setEnableChangeTypeSet(true);
    ui.graphic->setWithLegend(true);

    ui.graphic->getCanvas()->replot();

    connect(ui.graphic->getPorogsMoved(), SIGNAL(setSelectItem(QwtPlotItem*,MovedItemPosition)), this, SLOT(onCusorOverSelectItem(QwtPlotItem*,MovedItemPosition)));
    connect(ui.graphic->getPorogsMoved(), SIGNAL(changeArgumentValue(QwtPlotItem*,double,MovedItemPosition)), this, SLOT(onChangeSelectedItemValue(QwtPlotItem*,double,MovedItemPosition)));
    connect(ui.graphic->getPorogsMoved(), SIGNAL(dblClickMouseEvent()), this, SLOT(onDblClickMouseEvent()));

    connect(ui.bStart, SIGNAL(clicked(bool)), this, SLOT(onClickButtons(bool)));
    connect(ui.bSuspend, SIGNAL(clicked(bool)), this, SLOT(onClickButtons(bool)));
    connect(ui.bExit, SIGNAL(clicked(bool)), this, SLOT(onClickButtons(bool)));
    connect(ui.bComplite, SIGNAL(clicked(bool)), this, SLOT(onClickButtons(bool)));

    tempCP.resize(MAX_SPR_MAIN_THREADS);
    tempDEU.resize(MAX_SPR_MAIN_THREADS);

    for(int th=0; th<MAX_SPR_MAIN_THREADS; th++){
        threads.push_back(th);
        ui.graphic->setVisibleThreads(threads);
    }
    setThreads(threads);

    ui.graphic->setEnableChangeTypeSet(true);
//    ui.graphic->setWithLegend(true);

    widgetsShow();
}

ISPRModelData *SPRSettingsRentgenAutoSetDialog::setModelData(ISPRModelData *value)
{
    if(value){
        rentgenModel = (SPRSettingsRentgenModel*)value;
        if(rentgenModel){
            IModelVariable *_mod = rentgenModel->getMainModel();
            if(_mod && _mod->objectName() == "MainModel"){
                mainModel = (SPRMainModel*)_mod;
                if(mainModel){
                    ui.bStart->setEnabled(true);

                    for(int th=0; th<mainModel->getThreads()->getData(); th++){
                        tempDEU[th] = rentgenModel->codeBegin->getData();
                        tempCP[th] = 128;
                    }

                    settingBACodes();

                    if(autoSetCommand) delete autoSetCommand;
                    if(!toWidget) toWidget = new TCPTimeOutWigget(nullptr);
                    autoSetCommand = new TCPAutoSetRentgen(mainModel, toWidget);
                    connect(autoSetCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));
                    connect(autoSetCommand, SIGNAL(commandNotComplite(TCPCommand*)), this, SLOT(onCommandNotComplite(TCPCommand*)));

                    if(autoSetGo) delete autoSetGo;
                    autoSetGo = new TCPAutosetCommandGo(mainModel->getServer(), tempDEU2Command, tempCP2Command, threads, toWidget, nullptr);
                    connect(autoSetGo, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

                    if(spectrums) delete spectrums;
                    spectrums = new SPRSpectrumListItemsModel(mainModel, nullptr, true, nullptr);

                    ui.graphic->setModelData(spectrums, spectrumsOnly, false, true);
                    ui.graphic->setAllCurrent(true);

                    tempPeakPosition = mainModel->getSettingsRentgenModel()->peakPosition->getData();
                }
            }

            controlCP = 0;
            controlDEU = 0;
            controlAll = 0;

            qreal xmin = 0, xmax = 100;
            peakData = {QPointF(tempPeakPosition, xmin), QPointF(tempPeakPosition, xmax)};
            peakCurve->setSamples(peakData);

            widgetsShow();
        }
    }
    return value;
}

void SPRSettingsRentgenAutoSetDialog::onDblClickMouseEvent(){
    setGraphicTitle();
    int thr = ui.graphic->getCurrentThread();
    SPRGraphItem *gr = nullptr;
    if(thr >= 0){
        if(spectrums){
            QList<SPRSpectrumItemModel*> lst = spectrums->getSpectrumsItemByThread(thr, spectrumsOnly);
            if(lst.size() > 0){
                 gr = lst.last()->getGraphic();
            }
        }
    }
    ui.graphic->setCurrentItem(gr);
}

SPRSettingsRentgenAutoSetDialog::~SPRSettingsRentgenAutoSetDialog()
{
    delete autoSetCommand;
}

QByteArray SPRSettingsRentgenAutoSetDialog::codesToByteArray(uint16_t *table){
    QByteArray res;
    char ren = 0;
    res.append(&ren, 1);
    for(int th=0; th<MAX_SPR_MAIN_THREADS; th++){
        res.append((char*)(table + th), sizeof(uint16_t));
    }
    return res;
}

TCPCommand *restartsep = nullptr;

void SPRSettingsRentgenAutoSetDialog::onClickButtons(bool){
    if(mainModel){
        if(sender() == ui.bStart){

//            if(!restartsep){
//                restartsep = new TCPCommand(clearrs);
//            }
//            restartsep->send(mainModel->getServer());

            ui.bSuspend->setEnabled(true);
            ui.bComplite->setEnabled(false);

            finish = false;

            QByteArray codeDeu, codeCP;
    //        char ren = 0;
    //        codeDeu.append(&ren, 1); codeCP.append(&ren, 1);
            for(int th=0; th<MAX_SPR_MAIN_THREADS; th++){
                uint16_t deu = rentgenModel->deuCodes[th]->getData();
                uint16_t cp = rentgenModel->cpCodes[th]->getData();
                codeDeu.append((char*)(&deu), sizeof(deu));
                codeCP.append((char*)(&cp), sizeof(cp));
            }
            autoSetCommand->setCodesDEU(codeDeu);
            autoSetCommand->setCodesCP(codeCP);
            autoSetCommand->setThreads(threads);

            autoSetCommand->send(mainModel->getServer());
        }
        if(sender() == ui.bSuspend){
            ui.bComplite->setEnabled(false);
            ui.bSuspend->setEnabled(false);
            finish = true;
        }
        if(sender() == ui.bComplite){
            if(mainModel){
                foreach(uint8_t th, threads){
                    mainModel->getSettingsRentgenModel()->deuCodes[th]->setData(tempDEU[th]);
                    mainModel->getSettingsRentgenModel()->cpCodes[th]->setData(tempCP[th]);
                }
                mainModel->getSettingsRentgenModel()->peakPosition->setData(tempPeakPosition);
            }
        }
    }
    if(sender() == ui.bExit){
        finish = true;
        this->accept();
    }

}

void SPRSettingsRentgenAutoSetDialog::onCommandNotComplite(TCPCommand*){
    if(sender() == autoSetCommand){
        qDebug() << "ERRORS WORKING Rentgen.... ********************";
    }
}

void SPRSettingsRentgenAutoSetDialog::onCommandComplite(TCPCommand* _command){
    if(spectrums && mainModel){
        if((_command == autoSetCommand || _command == autoSetGo) && !finish){

            QVector<TCPCommand*> vspc = _command->findCommands(getspk);
            for(int i=0; i<vspc.size(); i++){
                QByteArray rep = vspc[i]->getReplayData();
                QByteArray spc = rep.right(DEF_SPECTRUM_DATA_LENGTH_BYTE);

                SPRSpectrumItemModel *mod = nullptr;
                mod = spectrums->setSpectrumData(threads[i], spc, spectrumsOnly, 10000, QString(tr("Ручей %1")));
                mod->setTimeScope(10000);
    //            mod->setSpectrumName(QString(tr("Ручей %1")).arg(threads[i]+1));

            }
            ui.graphic->getCanvas()->replot();
            double h = ui.graphic->getCanvas()->axisScaleDiv(QwtPlot::yLeft).upperBound();
            peakData[0].setY(0); peakData[1].setY(h);
            peakCurve->setSamples(peakData);

            controlCP = prDeuCpOKAll(&cpOK);
            controlDEU = prDeuCpOKAll(&deuOK);
            controlAll = prDeuCpOKAll();

            widgetsShow();

            uint peakFinish = tempPeakPosition;
            uint deuStep = mainModel->getSettingsRentgenModel()->codeStep->getData();

            int cnt = 0;
            foreach(uint8_t th, threads){
                QList<SPRSpectrumItemModel*> lst = spectrums->getSpectrumsItemByThread(th, spectrumsOnly);
                SPRSpectrumItemModel *spect = nullptr;
                if(lst.size() > 0){
                    spect = lst.last();
                    if(spect){
                        if(*spect->getSpectrumData()->peak == peakFinish){
                            deuOK[cnt] = true;
                            cpOK[cnt] = true;
                        } else if (*spect->getSpectrumData()->peak > peakFinish) {
                            deuOK[cnt] = true;
                            cpOK[cnt] = false;
                        } else {
                            deuOK[cnt] = false;
                            cpOK[cnt] = false;
                        }
                    }
                }
                if(spect){
                    if(!isDeuCpAllOK(255, &deuOK)){
                        if(!deuOK[cnt]){
                            tempDEU[cnt] += deuStep;
                        }
                    } else {
                        if(!cpOK[cnt]){
                            tempCP[cnt] -= 1;
                        }
                    }
qDebug() << "ch: " << threads[cnt] << "(" << deuOK[cnt] << "); deu: " << tempDEU[cnt] << "; cp: " << tempCP[cnt] << " (" << cpOK[cnt] << ")";
                }
                cnt++;
            }
            if(isDeuCpAllOK()){
                finish = true;
            } else {
                settingBACodes();
                if(!isDeuCpAllOK(255, &deuOK)){
                    autoSetGo->setDeuCpCodes(tempDEU2Command, {});
                } else {
                    autoSetGo->setDeuCpCodes({}, tempCP2Command);
                }
                autoSetGo->send(mainModel->getServer());
            }
        }
    }

}

ISPRModelData *SPRSettingsRentgenAutoSetDialog::getModelData()
{
    return rentgenModel;
}

void SPRSettingsRentgenAutoSetDialog::onModelChanget(IModelVariable *source)
{
    if(source == spectrums){
        ui.graphic->widgetsShow();
    }
}

