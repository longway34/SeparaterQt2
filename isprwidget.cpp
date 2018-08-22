#include <QLineEdit>
#include <QTableWidget>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QResource>
#include <QIODevice>
#include <QDir>
#include <QByteArray>
#include <QtXml/QDomDocument>
#include <QTextCodec>

#include "isprwidget.h"


TCPLogsWigtets *ISPRWidget::getLogWidget() const
{
    return logWidget;
}

void ISPRWidget::setLogWidget(TCPLogsWigtets *value)
{
    logWidget = value;
}

ISPRWidget::ISPRWidget():logWidget(nullptr)
{
}

void ISPRWidget::setDoc(QString _fName)
{
    doc = nullptr;
    QFile in(_fName);
    if(in.open(QIODevice::ReadOnly | QIODevice::Text)){

//        qint64 fSize = in.size();

        QTextStream ins(&in);
        QByteArray ba = in.readAll();
        in.seek(0);
//        QString mText = ins.readAll().trimmed();
        QList<QByteArray> listCodecs = {QByteArray("UTF-8"), QByteArray("Windows-1251"), QByteArray("KOI8-R"), QByteArray("UTF-8")};
        int index = 0; bool find = false;
        QString mtUtf8;
        while(index < listCodecs.size() && !find){
            QTextCodec *codec = QTextCodec::codecForName(listCodecs[index].constData());
            struct QTextCodec::ConverterState state;
            mtUtf8 = codec->toUnicode(ba, ba.length(), &state);
            if(state.invalidChars == 0){
                find = true;
                break;
            }
            index++;
        }

        if(!find){
            qDebug() << "Warnind! Invalid converted file to unucode: " << _fName;
        }
//        qDebug() << mText;
        QString err = "no errors"; int ln, col;
//        bool res = document.setContent(mtUtf8.toLocal8Bit(), &err, &ln, &col);
        bool res = document.setContent(mtUtf8);
//        QDomElement root = document.documentElement();
        if(res){
            doc = &document;
            setDoc(doc);
            if(_fName.at(0) != ':'){ // resource
                QFileInfo fi(_fName);
                docFileName = fi.absoluteFilePath();
                docFilePath = fi.absolutePath();
            } else {
                docFilePath = QDir::currentPath();
                docFileName = docFilePath + QDir::separator()+(DEF_SPR_MAIN_SETTINGS_FNAME)+(DEF_SPR_MAIN_SETTINGS_FNAME_SUFFIX);
            }

        }
        in.close();
    }
}

void ISPRWidget::setDoc(QDomDocument *_doc)
{
    doc = _doc;
}

ISPRModelData *ISPRWidget::setModelData(ISPRModelData *data)
{
    return data;
}

QLineEdit *ISPRWidget::setNumberCell(QTableWidget *table, int row, int col, int value, int min, int max, const QString toolType)
{
   QLineEdit *leNum = new QLineEdit(QString::number(value), table);
   leNum->setFrame(false); leNum->setAlignment(Qt::AlignCenter);
   leNum->setValidator(new QIntValidator(min, max, leNum/*table->item(row, col)->*/));
   table->setCellWidget(row, col, leNum);
   leNum->setProperty("row", row); leNum->setProperty("col", col);

   QVariant qv_tw;
   qv_tw.setValue<QTableWidget*>(table);
   leNum->setProperty("tw", qv_tw);

   leNum->setObjectName(QString("leTable_%1_%2").arg(QString::number(row)).arg(QString::number(col)));
   leNum->setToolTip(toolType);
   return leNum;
}

