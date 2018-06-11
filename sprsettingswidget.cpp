#include "sprsettingswidget.h"
//#include "sprspectrumranges.h"
//#include "models/sprsettingsmainmodel.h"
#include <QDir>

SPRSettingsWidget::SPRSettingsWidget(QWidget *parent):
    QWidget(parent), model(nullptr)
{
    ui.setupUi(this);

    connect(ui.tabSettingsWidget, SIGNAL(currentChanged(int)), this, SLOT(widgetsShow()));
}

SPRSettingsWidget::SPRSettingsWidget(QDomDocument *_doc, QString fName, QWidget *parent):
    QWidget(parent), model(nullptr), docFName(fName)
{
    ui.setupUi(this);
    setDoc(_doc);
    connect(ui.tabSettingsWidget, SIGNAL(currentChanged(int)), this, SLOT(widgetsShow()));
}

void SPRSettingsWidget::setDoc(QDomDocument *_doc)
{
    doc = _doc;

    if(model) {
        delete model;
        model = nullptr;
    }
    model = new SPRMainModel(doc, nullptr);

    setModelData(model);
}

void SPRSettingsWidget::onChangeFileSettinds(QString fName){
    emit changeFileSettinds(fName);
}
void SPRSettingsWidget::onChangeFileSpectrum(QString fName)
{
    emit changeFileSpectrum(fName);
}

SPRMainModel *SPRSettingsWidget::setModelData(SPRMainModel *_model)
{
    ISPRModelData *imodel;
    if(_model){
        if(model){
            disconnect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        }
        model = _model;
        connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));

        imodel = ui.wSettingsMainWidget->setModelData(model->getSettingsMainModel());
        connect(this,SIGNAL(doShow()), ui.wSettingsMainWidget, SLOT(widgetsShow()));
        connect(ui.wSettingsMainWidget,SIGNAL(doShow()), this, SLOT(widgetsShow()));
        connect(ui.wSettingsMainWidget,SIGNAL(changeFileSettinds(QString)), this, SLOT(onChangeFileSettinds(QString)));
        connect(ui.wSettingsMainWidget,SIGNAL(changeFileSpectrum(QString)), this, SLOT(onChangeFileSpectrum(QString)));

        imodel = ui.wSettingsIMSWidget->setModelData(model->getSettingsIMSModel());
        connect(this, SIGNAL(doShow()), ui.wSettingsIMSWidget, SLOT(widgetsShow()));
        connect(ui.wSettingsIMSWidget, SIGNAL(doShow()), this, SLOT(widgetsShow()));

        model->getSettingsPorogsModel()->setTMeteringMinStone(model->getSettingsIMSModel()->getTMeteringMinStone());
        model->getSettingsPorogsModel()->setTMeteringMaxStone(model->getSettingsIMSModel()->getTMeteringMaxStone());
        imodel = ui.wSettingsPorogsWidget->setModelData(model->getSettingsPorogsModel());
        connect(this,SIGNAL(doShow()), ui.wSettingsPorogsWidget, SLOT(widgetsShow()));
        connect(ui.wSettingsPorogsWidget, SIGNAL(doShow()), this, SLOT(widgetsShow()));

        imodel = ui.wSettingsFormulaWidget->setModelData(model->getSettingsFormulaModel());
        connect(this,SIGNAL(doShow()), ui.wSettingsFormulaWidget, SLOT(widgetsShow()));
        connect(ui.wSettingsFormulaWidget,SIGNAL(doShow()), this, SLOT(widgetsShow()));

        imodel = ui.wSettingsRentgen->setModelData(model->getSettingsRentgenModel());
        connect(this,SIGNAL(doShow()), ui.wSettingsRentgen, SLOT(widgetsShow()));


        imodel = ui.wSettingsControl->setModelData(model->getSettingsControlModel());
        connect(this, SIGNAL(doShow()), ui.wSettingsControl, SLOT(widgetsShow()));

        imodel = ui.wSpectrumZonesWidget->setModelData(model->getSpectrumZonesTableModel());
        connect(this, SIGNAL(doShow()), ui.wSpectrumZonesWidget, SLOT(widgetsShow()));

        connect(ui.bComplite, SIGNAL(clicked(bool)), this, SLOT(onCompliteButtomClick(bool)));
        connect(ui.bCancel, SIGNAL(clicked(bool)), this, SLOT(onCancelButtomClick(bool)));
//    emit doShow();
    }
    return model;
}

void SPRSettingsWidget::onCompliteButtomClick(bool){
    model->store();
    model->saveAs();
}

void SPRSettingsWidget::onCancelButtomClick(bool){
    blockSignals(true);
    model->restore();
    widgetsShow();
    blockSignals(false);
}
void SPRSettingsWidget::onModelChanget(IModelVariable*)
{
    widgetsShow();
}
