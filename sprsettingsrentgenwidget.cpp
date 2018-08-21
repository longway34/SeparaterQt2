#include "sprsettingsrentgenwidget.h"

SPRSettingsRentgenWidget::SPRSettingsRentgenWidget(QWidget *parent) :
    QWidget(parent), model(nullptr)
{
    ui.setupUi(this);

    masterMode = false;
    connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(widgetsShow()));
}


void SPRSettingsRentgenWidget::widgetsShow()
{
    setMasterMode(masterMode);
    ui.wSettingsRentgenAutoWidget->widgetsShow();
    ui.wSettingsRentgenControlWidget->widgetsShow();
    ui.wSettingsRentgenMainWidget->widgetsShow();
}

ISPRModelData *SPRSettingsRentgenWidget::setModelData(SPRSettingsRentgenModel *value){
    if(value){
        if(model){
            disconnect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        }
        model = value;
        connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        connect(this, SIGNAL(doStore()), model, SLOT(store()));

        ui.wSettingsRentgenMainWidget->setModelData(model);
        connect(this, SIGNAL(doShow()), ui.wSettingsRentgenMainWidget, SLOT(widgetsShow()));

        ui.wSettingsRentgenControlWidget->setModelData(model);
        connect(this, SIGNAL(doShow()), ui.wSettingsRentgenControlWidget, SLOT(widgetsShow()));

        ui.wSettingsRentgenAutoWidget->setModelData(model);
        connect(this, SIGNAL(doShow()), ui.wSettingsRentgenAutoWidget, SLOT(widgetsShow()));
    }
}

void SPRSettingsRentgenWidget::viewChange(QTableWidget *, int, int)
{
}


void SPRSettingsRentgenWidget::onModelChanget(IModelVariable*)
{
    widgetsShow();
}


void SPRSettingsRentgenWidget::setMasterMode(bool value)
{
    masterMode = value;
    QList<ISPRSettingsWidget *> lst = {ui.wSettingsRentgenAutoWidget,
                                       ui.wSettingsRentgenControlWidget,
                                       ui.wSettingsRentgenMainWidget};
    foreach(ISPRSettingsWidget *w, lst){
        w->setMasterMode(value);
    }
//    this->setEnabled(value);
}
