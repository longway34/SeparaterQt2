#include "sprspectrumchoisetimedialog.h"

#include <QObjectList>
#include <QWidget>

SPRSpectrumChoiseTimeDialog::SPRSpectrumChoiseTimeDialog(QWidget *parent) :
    QDialog(parent), model(nullptr), threads(DEF_SPR_MAIN_THREADS), rentgenWidget(nullptr)
{
    ui.setupUi(this);

    connect(ui.bRentgentParamsShow, SIGNAL(clicked(bool)), this, SLOT(onShowRengenParams(bool)));
    connect(ui.gbThreads, SIGNAL(toggled(bool)), this, SLOT(onAllThreadClick(bool)));
}

SPRSpectrumChoiseTimeDialog::SPRSpectrumChoiseTimeDialog(SPRMainModel *_model, QWidget *parent):
    QDialog(parent), model(nullptr), threads(DEF_SPR_MAIN_THREADS), rentgenWidget(nullptr)
{
    ui.setupUi(this);

    QList<QRadioButton*> times = ui.gbTimes->findChildren<QRadioButton*>();
    foreach(QRadioButton *time, times){
        if(time){
            ui.gbTimes->layout()->removeWidget(time);
            delete time;
        }
    }
    ui.gbTimes->layout()->removeItem(ui.timesSpaces);
    struct a
    {
        QString title; double value;
    };
    QVector<struct a> timesSpec = {
        {tr("0.2 сек."), 0.2},
        {tr("1 сек."), 1},
        {tr("3 сек."), 3},
        {tr("5 сек."), 5},
        {tr("10 сек."), 10},
        {tr("15 сек."), 15}
    };

    for(int i=0; i<timesSpec.size(); i++){
        QRadioButton *rb = new QRadioButton(timesSpec[i].title);
        rb->setProperty("value", QVariant(timesSpec[i].value));
        if(i == 3){
            rb->setChecked(true);
        }
        ui.gbTimes->layout()->addWidget(rb);
    }
    ui.gbTimes->layout()->addItem(ui.timesSpaces);

    setModelData(_model);
    connect(ui.bRentgentParamsShow, SIGNAL(clicked(bool)), this, SLOT(onShowRengenParams(bool)));
}

void SPRSpectrumChoiseTimeDialog::onShowRengenParams(bool){
    if(rentgenWidget){
        rentgenWidget->show();
    }
}

void SPRSpectrumChoiseTimeDialog::onAllThreadClick(bool value)
{
    QList<QCheckBox*> threads = ui.gbThreads->findChildren<QCheckBox*>();
    ui.gbThreads->setEnabled(!value);
    if(!value){
        foreach(QCheckBox *cb, threads){
            cb->setChecked(true);
        }
    }
}

void SPRSpectrumChoiseTimeDialog::setModelData(SPRMainModel *_model)
{
    model = _model;
    if(model){
        threads = model->getSettingsMainModel()->getThreads()->getData();

        if(!rentgenWidget){
            rentgenWidget = new SPRSettingsRentgenWidget();
            rentgenWidget->setModelData(model->getSettingsRentgenModel());
            int index = rentgenWidget->ui.tabWidget->indexOf(rentgenWidget->ui.tabAutoSettings);
            if(index >= 0){
                rentgenWidget->ui.tabWidget->removeTab(index);
            }
            rentgenWidget->setWindowModality(Qt::ApplicationModal);
        }
        QList<QCheckBox*> cbThreadsList = ui.gbThreads->findChildren<QCheckBox*>();
        foreach(QCheckBox *th, cbThreadsList){
            ui.gbThreads->layout()->removeWidget(th);
            delete th;
        }
        ui.gbThreads->layout()->removeItem(ui.thrSpacer);
        for(uint8_t i=0; i<threads; i++){
           QCheckBox *th = new QCheckBox(QString(tr("Ручей %1")).arg(QString::number(i+1)));
           th->setObjectName(QString("cb_%1").arg(QString::number(i+1)));
           th->setProperty("index", QVariant(i));
           th->setChecked(true);
//           connect(ui.gbThreads, SIGNAL(toggled(bool)), th, SLOT(setChecked(bool)));
           ui.gbThreads->layout()->addWidget(th);
        }
        ui.gbThreads->layout()->addItem(ui.thrSpacer);
        ui.gbThreads->setChecked(true);
    }
}

QList<uint8_t> SPRSpectrumChoiseTimeDialog::getThreads(){
    QList<uint8_t> res;
    QGroupBox *gbThreads = ui.gbThreads;
    QList<QCheckBox*> childrens = gbThreads->findChildren<QCheckBox*>();
    foreach(QCheckBox *child, childrens){
            bool cbChecket = child->isChecked();
            if(cbChecket){
                res.push_back(child->property("index").value<uint8_t>());
            }
    }
    return res;
}

double SPRSpectrumChoiseTimeDialog::getTime()
{
    double res = 0;
    QList<QRadioButton*> times = ui.gbTimes->findChildren<QRadioButton*>();
    foreach(QRadioButton *rb, times){
        if(rb && rb->isChecked()){
            res = rb->property("value").value<double>();
            break;
        }
    }
    return res;
}

bool SPRSpectrumChoiseTimeDialog::isRentgenOn(){
    return ui.cbRentgenOn->isChecked();
}

bool SPRSpectrumChoiseTimeDialog::isRentgenOff(){
    return ui.cbReangenOff->isChecked();
}

bool SPRSpectrumChoiseTimeDialog::isRGUDown(){
    return ui.cbRGUDown->isChecked();
}

QString SPRSpectrumChoiseTimeDialog::getPrefix()
{
    return ui.leNamePrefix->text();
}
