#include "sprsettingswidget.h"
//#include "sprspectrumranges.h"
//#include "models/sprsettingsmainmodel.h"
#include <QDir>

SPRSettingsWidget::SPRSettingsWidget(QWidget *parent):
    QWidget(parent), model(nullptr)
{
    ui.setupUi(this);
}

SPRSettingsWidget::SPRSettingsWidget(QDomDocument *_doc, QString fName, QWidget *parent):
    QWidget(parent), model(nullptr), docFName(fName)
{
    ui.setupUi(this);
    setDoc(_doc);
}

void SPRSettingsWidget::setDoc(QDomDocument *_doc)
{
    doc = _doc;

    if(model) {
        delete model;
        model = nullptr;
    }
    model = new SPRMainModel(doc, nullptr);

    setModel(model);
}

void SPRSettingsWidget::onChangeFileSettinds(QString fName){
    emit changeFileSettinds(fName);
}
void SPRSettingsWidget::onChangeFileSpectrum(QString fName)
{
    emit changeFileSpectrum(fName);
}

SPRMainModel *SPRSettingsWidget::setModel(SPRMainModel *_model)
{
    ISPRModelData *imodel;
    model = _model;


    imodel = ui.wSettingsMainWidget->setModel(model->getSettingsMainModel());
    connect(this,SIGNAL(doShow()), ui.wSettingsMainWidget, SLOT(widgetsShow()));
    connect(ui.wSettingsMainWidget,SIGNAL(doShow()), this, SLOT(widgetsShow()));
    connect(ui.wSettingsMainWidget,SIGNAL(changeFileSettinds(QString)), this, SLOT(onChangeFileSettinds(QString)));
    connect(ui.wSettingsMainWidget,SIGNAL(changeFileSpectrum(QString)), this, SLOT(onChangeFileSpectrum(QString)));

    imodel = ui.wSettingsIMSWidget->setModel(model->getSettingsIMSModel());
    connect(this, SIGNAL(doShow()), ui.wSettingsIMSWidget, SLOT(widgetsShow()));
    connect(ui.wSettingsIMSWidget, SIGNAL(doShow()), this, SLOT(widgetsShow()));

    model->getSettingsPorogsModel()->setTMeteringMinStone(model->getSettingsIMSModel()->getTMeteringMinStone());
    model->getSettingsPorogsModel()->setTMeteringMaxStone(model->getSettingsIMSModel()->getTMeteringMaxStone());
    imodel = ui.wSettingsPorogsWidget->setModel(model->getSettingsPorogsModel());
    connect(this,SIGNAL(doShow()), ui.wSettingsPorogsWidget, SLOT(widgetsShow()));
    connect(ui.wSettingsPorogsWidget, SIGNAL(doShow()), this, SLOT(widgetsShow()));

    imodel = ui.wSettingsFormulaWidget->setModel(model->getSettingsFormulaModel());
    connect(this,SIGNAL(doShow()), ui.wSettingsFormulaWidget, SLOT(widgetsShow()));
    connect(ui.wSettingsFormulaWidget,SIGNAL(doShow()), this, SLOT(widgetsShow()));

    imodel = ui.wSettingsRentgen->setModel(model->getSettingsRentgenModel());
    connect(this,SIGNAL(doShow()), ui.wSettingsRentgen, SLOT(widgetsShow()));


    imodel = ui.wSettingsControl->setModel(model->getSettingsControlModel());
    connect(this, SIGNAL(doShow()), ui.wSettingsControl, SLOT(widgetsShow()));

    imodel = ui.wSpectrumZonesWidget->setModel(model->getSpectrumZonesTableModel());
    connect(this, SIGNAL(doShow()), ui.wSpectrumZonesWidget, SLOT(widgetsShow()));

    emit doShow();

    return model;
}

