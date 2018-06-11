#include "sprsettingsrentgenwidget.h"

SPRSettingsRentgenWidget::SPRSettingsRentgenWidget(QWidget *parent) :
    QWidget(parent), model(nullptr)
{
    ui.setupUi(this);

    connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(widgetsShow()));
}


void SPRSettingsRentgenWidget::widgetsShow()
{
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
