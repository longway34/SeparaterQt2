#include "maintabwidget.h"
#include "tcp/TCPCommand.h"
#include <QFileInfo>

MainTabWidget::MainTabWidget(QWidget *parent) :
    QTabWidget(parent), model(nullptr)
{
    ui.setupUi(this);
    setDoc(DEF_SPR_SETTINGS_FNAME);

    connect(ui.tabSettings, SIGNAL(changeFileSettinds(QString)), this, SLOT(onChangeFileSettings(QString)));
    connect(ui.tabSettings, SIGNAL(changeFileSpectrum(QString)), this, SLOT(onChangeFileSpectrum(QString)));

    connect(ui.tabSettings, SIGNAL(doShow()), this, SLOT(widgetsShow()));

}

MainTabWidget::MainTabWidget(QDomDocument *_doc, QWidget *parent): QTabWidget(parent), model(nullptr)
{
    ui.setupUi(this);
    setDoc(doc);
    setModel(new SPRMainModel(doc));

    connect(ui.tabSettings, SIGNAL(changeFileSettinds(QString)), this, SLOT(onChangeFileSettings(QString)));
    connect(ui.tabSettings, SIGNAL(changeFileSpectrum(QString)), this, SLOT(onChangeFileSpectrum(QString)));
//    connect(ui.bSetSeparate, SIGNAL(clicked(bool)), SLOT(onClickSetSeparateButton(bool)));
}

MainTabWidget::MainTabWidget(QString _fName, QWidget *parent): QTabWidget(parent), model(nullptr)
{
    ui.setupUi(this);
    bool createNewDoc = false;
    if(!QFile::exists(_fName)){
        _fName = ":/def/defSetting/xml";
        createNewDoc = true;

    }
    setDoc(_fName);
    setModel(new SPRMainModel(doc));

    if(createNewDoc){
        QFile out(model->getSettingsMainModel()->name->getData());
        if(out.open(QIODevice::WriteOnly)){}
        QTextStream stream( &out );
          stream << doc->toString();

          out.close();
    }

    connect(ui.tabSettings, SIGNAL(changeFileSettinds(QString)), this, SLOT(onChangeFileSettings(QString)));
    connect(ui.tabSettings, SIGNAL(changeFileSpectrum(QString)), this, SLOT(onChangeFileSpectrum(QString)));
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

ISPRModelData *MainTabWidget::setModel(SPRMainModel *_model)
{
    if(model) delete model; model = nullptr;
    model = _model;
    ui.tabSettings->setModel(model);
    QFile f(model->getSettingsMainModel()->spectrumFileName->getData());
    if(f.open(QIODevice::ReadOnly)){
        ui.tabSpectrum->setModel(model->getSpectrumListItemsModel(), &f);
        f.close();
    }
    ui.wTest->setModel(model);

    ui.tabTest->setModel(model);

    separateModel = new SPRSeparateModel(model->getDoc());

}

void MainTabWidget::onClickSetSeparateButton(bool value){
//    ui.teResult->clear();
    QByteArray result = separateModel->toByteArray();
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
    setModel(new SPRMainModel(doc));
    emit doShow();
}

void MainTabWidget::onChangeFileSpectrum(QString fName){
    QFile spc(fName);
    if(spc.open(QIODevice::ReadOnly)){
        //        ui.tabSpectrum->setModel(model->getSpectrumZonesTableModel(), &spc);
        ui.wTest->addSpectrumsModel(&spc);
        spc.close();
    }
}
