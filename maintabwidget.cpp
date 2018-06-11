#include "maintabwidget.h"
#include "tcp/TCPCommand.h"
#include <QFileInfo>

MainTabWidget::MainTabWidget(QWidget *parent) :
    QTabWidget(parent), model(nullptr)
{
    ui.setupUi(this);
    setDoc(DEF_SPR_MAIN_SETTINGS_FNAME+DEF_SPR_MAIN_SETTINGS_FNAME_SUFFIX);

    connect(ui.tabSettings, SIGNAL(changeFileSettinds(QString)), this, SLOT(onChangeFileSettings(QString)));
    connect(ui.tabSettings, SIGNAL(changeFileSpectrum(QString)), this, SLOT(onChangeFileSpectrum(QString)));

    connect(ui.tabSettings, SIGNAL(doShow()), this, SLOT(widgetsShow()));

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(widgetsShow()));
}

MainTabWidget::MainTabWidget(QDomDocument *_doc, QWidget *parent): QTabWidget(parent), model(nullptr)
{
    ui.setupUi(this);
    setDoc(doc);
    setModelData(new SPRMainModel(doc));

    connect(ui.tabSettings, SIGNAL(changeFileSettinds(QString)), this, SLOT(onChangeFileSettings(QString)));
    connect(ui.tabSettings, SIGNAL(changeFileSpectrum(QString)), this, SLOT(onChangeFileSpectrum(QString)));

//    connect(this, SIGNAL(doShow()), ui.tabSeparate, SLOT(widgetsShow()));

    connect(this, SIGNAL(doShow()), ui.tabSettings, SLOT(widgetsShow()));
    connect(this, SIGNAL(doShow()), ui.tabSpectrum, SLOT(widgetsShow()));
    connect(this, SIGNAL(doShow()), ui.tabTest, SLOT(widgetsShow()));

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(widgetsShow()));
//    connect(ui.bSetSeparate, SIGNAL(clicked(bool)), SLOT(onClickSetSeparateButton(bool)));
}

MainTabWidget::MainTabWidget(QString _fName, QWidget *parent): QTabWidget(parent), model(nullptr)
{
    ui.setupUi(this);
    bool createNewDoc = false;
    if(!QFile::exists(_fName)){
        _fName = ":/def/defSettings.xml";
        createNewDoc = true;

    }
    setDoc(_fName);
    setModelData(new SPRMainModel(doc));

    if(createNewDoc){
//        QString fName = model->getSettingsMainModel()->name->toString();
//        QString fName = DEF_SPR_MAIN_SETTINGS_FNAME+DEF_SPR_MAIN_SETTINGS_FNAME_SUFFIX;
        model->setFName(docFileName);
        QFile out(docFileName);
        if(out.open(QIODevice::WriteOnly)){
        QTextStream stream( &out );
          stream << doc->toString();

          out.close();
        }
    } else {
        if(model)
            model->setFName(docFileName);
    }

    connect(ui.tabSettings, SIGNAL(changeFileSettinds(QString)), this, SLOT(onChangeFileSettings(QString)));
    connect(ui.tabSettings, SIGNAL(changeFileSpectrum(QString)), this, SLOT(onChangeFileSpectrum(QString)));
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(widgetsShow()));
//    connect(ui.bSetSeparate, SIGNAL(clicked(bool)), this, SLOT(onClickSetSeparateButton(bool)));
}

//void MainTabWidget::setDoc(QString _fName)
//{
//    QFile in(_fName);
//    if(in.open(QIODevice::ReadOnly)){
//        if(document.setContent(&in)){
//            QFileInfo fi(_fName);
//            docFileName = fi.absoluteFilePath();
//            docFilePath = fi.absolutePath();
//            doc = &document;
//            setDoc(doc);
//        }
//    }
//}

//void MainTabWidget::setDoc(QDomDocument *_doc)
//{
//    doc = _doc;
//    ui.tabSettings->setModel(new SPRMainModel(doc));
//}

ISPRModelData *MainTabWidget::setModelData(SPRMainModel *_model)
{
    if(model){
        disconnect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
    }
    model = _model;
    connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
    ui.tabSettings->setModelData(model);
//    QFile f(model->getSettingsMainModel()->spectrumFileName->getData());
//    if(f.open(QIODevice::ReadOnly)){
//        f.close();
//    }
    ui.tabSpectrum->setModelData(model);
    ui.wTest->setModelData(model);

    ui.tabTest->setModelData(model);

    separateModel = new SPRSeparateModel(model->getDoc());

}

void MainTabWidget::onClickSetSeparateButton(bool value){
//    ui.teResult->clear();
    QByteArray result = separateModel->toByteArray(model);
    QString str;
    for(int ch=0; ch<result.size(); ch++){
        char buf[20];
        sprintf(buf, " 0x%0X", ((uchar)result.at(ch)));
        str += buf;
    }
//    ui.teResult->appendPlainText(str);

    TCPCommand *setsep = new TCPCommand(setsepar);
    setsep->setSendData(result);
    setsep->send(model->getServer());

}

void MainTabWidget::onChangeFileSettings(QString fName)
{
    setDoc(fName);
    setModelData(new SPRMainModel(doc));
    emit doShow();
}

void MainTabWidget::onChangeFileSpectrum(QString fName){
    QFile spc(fName);
    if(spc.open(QIODevice::ReadOnly)){
        //        ui.tabSpectrum->setModel(model->getSpectrumZonesTableModel(), &spc);
//        ui.wTest->addSpectrumsModel(&spc);
        spc.close();
    }
}


void MainTabWidget::onModelChanget(IModelVariable *)
{
    widgetsShow();
}
