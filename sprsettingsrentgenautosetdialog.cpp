#include "sprsettingsrentgenautosetdialog.h"
#include "qwt_plot_grid.h"


void SPRSettingsRentgenAutoSetDialog::onChangeSelectedItemValue(QwtPlotItem *item, double distance, MovedItemPosition position){
    if(item && item == peakCurve){
//        if(item->rtti() == QwtPlotItem::Rtti_PlotCurve){
//            QVector<QPointF> points;
//            for(int sam=0; sam<peakCurve->data()->size();sam++){
//                QPointF point = peakCurve->sample(sam);
//                point.setX(point.rx() + distance);
//                points.push_back(point);
//            }
//            peakCurve->setSamples(points);
//        }
        if(rentgenModel){
            uint last = rentgenModel->peakPosition->getData();
            rentgenModel->peakPosition->setData(last + distance);
        }
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

                    if(autoSetCommand) delete autoSetCommand;
                    autoSetCommand = new TCPAutoSetRentgen(mainModel, new TCPTimeOutWigget(nullptr));
                    connect(autoSetCommand, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(onCommandComplite(TCPCommand*)));

                    if(spectrums) delete spectrums;
                    spectrums = new SPRSpectrumListItemsModel(mainModel->getSpectrumZonesTableModel(), mainModel->getSettingsFormulaModel(), mainModel->getThreads(), nullptr, mainModel->getSettingsControlModel()->controlArea, nullptr);

                    ui.graphic->setModelData(spectrums, spectrumsOnly, false, true);
                    ui.graphic->setAllCurrent(true);
                }
            }
            for(int th=0; th<mainModel->getThreads()->getData(); th++){
                tempDEU[th] = rentgenModel->codeBegin->getData();
                tempCP[th] = 128;
            }

            controlCP = 0;
            controlDEU = 0;

            qreal xmin = 0, xmax = 100;
            peakData = {QPointF(rentgenModel->peakPosition->getData(), xmin), QPointF(rentgenModel->peakPosition->getData(), xmax)};
            peakCurve->setSamples(peakData);

            widgetsShow();
        }
    }
    return value;
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
            QVector<SPRSpectrumItemModel*> vspect = spectrums->getSpectrumsItemByThread(th);
            if(vspect.size() > 0){
                SPRSpectrumItemModel *spect = vspect.last();
                if(spect){
                    _color = spect->getSpectrumColor();
                    _textTitle = spect->getSpectrumName();
                }
            }

        }

        FirstCollumn2 *fc = new FirstCollumn2(_textTitle, _colorVisible, _color, _selectVisible, _deleteVisible, nullptr);
//        fc->setSelectVisible(false); fc->setDeleteVisible(false);
//        if(spectrums){
//            QVector<SPRSpectrumItemModel*> vspect = spectrums->getSpectrumsItemByThread(th);
//            if(vspect.size() > 0){
//                SPRSpectrumItemModel *spect = vspect.last();
//                fc->setColor(spect->getSpectrumColor());
//                QString ttt = spect->getSpectrumName();
//                fc->setText(ttt);
//            } else {
//                fc->setColor(Qt::white);
//                QString ttt = QString(tr("Ручей %1")).arg(QString::number(th + 1));
//                fc->setText(ttt);
//            }
//        } else {
//            fc->setColor(Qt::white);
//            QString ttt = QString(tr("Ручей %1")).arg(QString::number(th + 1));
//            fc->setText(ttt);
//        }
        ui.tDeuCP->setCellWidget(ui.tDeuCP->rowCount()-1, 0, fc);
//        fc->widgetsShow();

//        ui.tDeuCP->setItem(ui.tDeuCP->rowCount()-1, 0, new QTableWidgetItem(QString::number(th+1)));
        ui.tDeuCP->setItem(ui.tDeuCP->rowCount()-1, 1, new QTableWidgetItem(QString::number(tempDEU[th])));
        ui.tDeuCP->setItem(ui.tDeuCP->rowCount()-1, 2, new QTableWidgetItem(QString::number(tempCP[th])));

//        fc = new FirstCollumn2();
//        fc->setSelectVisible(false); fc->setDeleteVisible(false);
//        if(spectrums){
//            QVector<SPRSpectrumItemModel*> vspect = spectrums->getSpectrumsItemByThread(th);
//            if(vspect.size() > 0){
//                SPRSpectrumItemModel *spect = vspect.last();
//                fc->setColor(spect->getSpectrumColor());
//                QString ttt = spect->getSpectrumName();
//                fc->setText(ttt);
//            } else {
//                fc->setColor(Qt::white);
//                QString ttt = QString(tr("Ручей %1")).arg(QString::number(th + 1));
//                fc->setText(ttt);
//            }
//        } else {
//            fc->setColor(Qt::white);
//            QString ttt = QString(tr("Ручей %1")).arg(QString::number(th + 1));
//            fc->setText(ttt);
//        }
        fc = new FirstCollumn2(_textTitle, _colorVisible, _color, _selectVisible, _deleteVisible, nullptr);
        ui.tPeakRS->setCellWidget(ui.tPeakRS->rowCount()-1, 0, fc);
//        fc->widgetsShow();

//        ui.tPeakRS->setItem(ui.tPeakRS->rowCount()-1, 0, new QTableWidgetItem(QString::number(th+1)));
        QString rs = "0"; QString peak = "0";
        if(spectrums){
            QVector<SPRSpectrumItemModel *> specs = spectrums->getSpectrumsItemByThread(th);
            if(specs.size() > 0){
                SPRSpectrumItemModel* spec = specs.last();
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

    ui.graphic->getCanvas()->replot();

//    QList<QwtLegendData> legendValues = peakCurve->legendData();
//    foreach (QwtLegendData data, legendValues) {
//        qDebug() << "legend title: "<< data.title().text();
//        qDebug() << "data: "<< data.values();
//    }
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

QList<int> SPRSettingsRentgenAutoSetDialog::getThreads()
{
    return threads;
}

void SPRSettingsRentgenAutoSetDialog::setThreads(QList<int> &value)
{
    threads = value;
    ui.graphic->setVisibleThreads(threads);
}

SPRSettingsRentgenAutoSetDialog::SPRSettingsRentgenAutoSetDialog(QWidget *parent) :
    QDialog(parent), rentgenModel(nullptr), mainModel(nullptr), autoSetCommand(nullptr), peakCurve(nullptr), spectrums(nullptr)

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

    connect(ui.bStart, SIGNAL(clicked(bool)), this, SLOT(onStartButton(bool)));

    for(int th=0; th<MAX_SPR_MAIN_THREADS; th++){
        threads.push_back(th);
        ui.graphic->setVisibleThreads(threads);
    }
    ui.graphic->setEnableChangeTypeSet(true);
//    ui.graphic->setWithLegend(true);

    widgetsShow();
}

void SPRSettingsRentgenAutoSetDialog::onDblClickMouseEvent(){
    setGraphicTitle();
    int thr = ui.graphic->getCurrentThread();
    SPRGraphItem *gr = nullptr;
    if(thr >= 0){
        if(spectrums){
            QVector<SPRSpectrumItemModel*> vmodels = spectrums->getSpectrumsItemByThread(thr, spectrumsOnly);
            if(vmodels.size() > 0){
                 gr = ui.graphic->findGraphItemByModel(vmodels.last());
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

void SPRSettingsRentgenAutoSetDialog::onStartButton(bool){
    if(mainModel){
        ui.bSuspend->setEnabled(true);

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

}


void SPRSettingsRentgenAutoSetDialog::onCommandComplite(TCPCommand* _command){
    if(_command == autoSetCommand){
        QVector<TCPCommand*> vspc = autoSetCommand->findCommands(getspk);
        for(int i=0; i<vspc.size(); i++){
            QByteArray rep = vspc[i]->getReplayData();
            QByteArray spc = rep.right(DEF_SPECTRUM_DATA_LENGTH_BYTE);

            SPRSpectrumItemModel *mod = nullptr;
            mod = spectrums->setSpectrumData(threads[i], spc, spectrumsOnly, 30000, QString(tr("Ручей %1")));
            mod->setTimeScope(10000);
//            mod->setSpectrumName(QString(tr("Ручей %1")).arg(threads[i]+1));

        }
        ui.graphic->getCanvas()->replot();
        double h = ui.graphic->getCanvas()->axisScaleDiv(QwtPlot::yLeft).upperBound();
        peakData[0].setY(0); peakData[1].setY(h);
        peakCurve->setSamples(peakData);

        widgetsShow();
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
